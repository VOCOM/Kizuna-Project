#include <win_server.hpp>

#include <WS2tcpip.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <kizuna/core.hpp>
#include <responses.hpp>
#include <utility/utils.hpp>

// Public Methods
void WebServer::Info() {
	std::cout << "Submodule " << Name() << "\n";
	std::cout << "Address " << nodename << ':' << port << "\n";
	std::cout << "Status " << Status() << "\n\n";
}
void WebServer::Start() {
	if (status == Online) return;

	// Resolve local IP Address and Port for server
	if (GetAddrInfo(nodename.c_str(), port.c_str(), &hints, &result)) {
		std::cout << "Error starting webserver.\n";
		WSACleanup();
		return;
	}

	// Create web socket
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		std::cout << "Error creating socket.\n";
		freeaddrinfo(result);
		return;
	}

	// Setup web socket with TCP
	if (bind(listenSocket, result->ai_addr, (int)result->ai_addrlen)) {
		std::cout << "Error establishing TCP socket.\n";
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	// Cleanup
	freeaddrinfo(result);

	// Begin Listening
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Failed to start listening.\n";
		closesocket(listenSocket);
		WSACleanup();
	}

	status = Online;
	server = std::thread(&WebServer::Loop, this);
	std::cout << "Webserver initialized " << nodename << ":" << port << "\n";
}
void WebServer::Stop() {
	if (status == Offline) return;
	std::cout << "Webserver terminating...\n";
	status = Offline;
	closesocket(clientSocket);
	WSACleanup();
	if (server.joinable()) server.join();
}
void WebServer::Restart() {
	Stop();
	LoadConfiguration();
	Start();
}
void WebServer::LoadConfiguration() {
	// Load configuration
	auto config = Configuration::Config["webserver"];
	port        = config["port"];
	nodename    = config["nodename"];
}

void WebServer::Shell(std::string command, std::queue<std::string> params) {
}

// Constructors
WebServer::WebServer() : Submodule("Webserver") {
	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		std::cout << "Error initializing WS2_32.dll.\n";
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags    = AI_PASSIVE;
}
WebServer::~WebServer() {}

// Private Methods
void WebServer::Loop() {
	try {
		while (status != Offline) {
			AcceptConnection();

			// Read data
			requestBytes = recv(clientSocket, buffer, BUFFER_SIZE, 0);

			// Error Case
			if (requestBytes < 0) {
				status = Offline;
				closesocket(clientSocket);
				WSACleanup();
				continue;
			}

			// Closed Connection
			if (requestBytes == 0) continue;

			ProcessRequest();
			ProcessResponse();
		}
		if (shutdown(clientSocket, SD_SEND) != SOCKET_ERROR) return;
	} catch (Error e) {
		status = Offline;
		Kizuna::ErrorQueue.push(e);
	}
}
void WebServer::AcceptConnection() {
	clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket != INVALID_SOCKET) return;
	closesocket(listenSocket);
	WSACleanup();
}

void WebServer::ProcessRequest() {
	std::string request;
	request.insert(0, buffer, requestBytes);
	std::stringstream ss(request);

	// Process HTML method
	ss >> method >> param >> protocol;
	contentType.clear();
}
void WebServer::ProcessResponse() {
	std::string content = BuildContent();
	std::stringstream responseStream;
	responseStream << "HTTP/1.1 200 OK\n"
								 << "content-type: " << contentType << "\n"
								 << "content-length: " << content.size() << "\n"
								 << "\n"
								 << content << "\n";

	std::string response(responseStream.str());
	if (send(clientSocket, response.c_str(), response.size(), 0) != SOCKET_ERROR) return;

	std::cout << "Failed to send data.\n";
	status = Offline;
	closesocket(clientSocket);
	WSACleanup();
}
std::string WebServer::BuildContent() {
	std::stringstream contentStream;

	param.erase(0, 1); // Remove starting '/'
	if (param == "") param = "index.html";

	int ext = param.find('.');
	if (ext != std::string::npos) { // File request
		std::string fileType(param.substr(ext, param.size() - ext));
		if (fileType == ".html") contentType = "text/html";
		if (fileType == ".css") contentType = "text/css";
		if (fileType == ".svg") contentType = "image/svg+xml";

		std::ifstream fileStream(param);
		contentStream << fileStream.rdbuf();
	} else { // Server command
		auto paramList = Split(param, '/');
		if (paramList.front() == "submodule") Responses::Status(contentStream);
	}

	return contentStream.str();
}
