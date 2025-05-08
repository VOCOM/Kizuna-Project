#include <win_server.hpp>

#include <WS2tcpip.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <kizuna/configuration.hpp>
#include <responses.hpp>
#include <utility/utils.hpp>

// Submodule Interface
void WebServer::Info() {
	std::cout << "\n"
						<< std::setw(10) << std::left << "Module" << Name() << "\n"
						<< std::setw(10) << std::left << "Status" << Status() << "\n"
						<< std::setw(10) << std::left << "Address" << nodename << ':' << port << "\n";
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
	auto& config = Configuration::Config["webserver"];

	if (config.find("port") == config.end()) config["port"] = "8080";
	port = config["port"];

	if (config.find("nodename") == config.end()) config["nodename"] = "localhost";
	nodename = config["nodename"];
}

// Shell Interface
void WebServer::Access() {
	std::string command;

	while (true) {
		std::cout << "Webserver: ";
		std::cin >> Shell::buffer;
		Shell::buffer >> command;

		// Shell Commands
		if (command == "help") CommandList();
		if (command == "exit") return;
	}
}
void WebServer::CommandList() {
	std::cout << "\nShell Commands\n"
						<< std::setw(10) << std::left << "HELP" << "List commands.\n"
						<< std::setw(10) << std::left << "EXIT" << "Exit submodule shell.\n"
						<< "\n";
}

// Constructors
WebServer::WebServer() : Module("Webserver") {
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
	ss >> method >> resource >> protocol;
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
	auto resourceTree = Split(resource, '/');

	for (size_t i = 0; i < resourceTree.size(); i++) {
		if (resourceTree[i].empty()) contentStream << std::ifstream("html/index.html").rdbuf();
		if (resourceTree[i] == "dark") contentStream << std::ifstream("styles/darktheme.css").rdbuf();
		if (resourceTree[i] == "htmx") contentStream << std::ifstream("scripts/htmx.min.js").rdbuf();
		if (resourceTree[i] == "status") {
			if (i + 1 >= resourceTree.size()) continue;
			auto& mod = Module::GetModule(resourceTree[i + 1]);
			contentStream << (mod ? mod->Status() : "Offline");
		}
	}

	return contentStream.str();
}
