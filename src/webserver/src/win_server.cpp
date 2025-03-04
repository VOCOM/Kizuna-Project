#include <win_server.hpp>

#include <WS2tcpip.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include <kizuna.hpp>

using namespace std::chrono_literals;

// Public Methods
void WebServer::Start() {
	listening = listen(listenSocket, SOMAXCONN) != SOCKET_ERROR;

	if (listening) {
		server = std::thread(&WebServer::StartServer, this);
	} else {
		std::cout << "Failed to start listening.\n";
		closesocket(listenSocket);
		WSACleanup();
	}
}

void WebServer::Restart() {
	CloseServer();
	Configuration::LoadConfig();
	Start();
}

WebServer::WebServer() {
	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		std::cout << "Error initializing WS2_32.dll.\n";
		return;
	}

	// Load configuration
	auto config = Configuration::Config["webserver"];
	port        = config["port"];
	nodename    = config["nodename"];

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags    = AI_PASSIVE;

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
	std::cout << "Webserver initialized on port " << port << "\n";
}

WebServer::~WebServer() {
	std::cout << "Shutting down webserver...\n";
	listening = false;
	closesocket(clientSocket);
	WSACleanup();
	if (server.joinable()) server.join();
}

// Private Methods
void WebServer::StartServer() {
	int bytesReceived;
	while (listening) {

		AcceptConnection();

		// Read data
		char buffer[BUFFER_SIZE];
		bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

		// Error Case
		if (bytesReceived < 0) {
			listening = false;
			closesocket(clientSocket);
			WSACleanup();
			continue;
		}

		// Closed Connection
		if (bytesReceived == 0) continue;

		// Build Response
		std::string response = BuildResponse();

		// Send data
		SendResponse(response.c_str(), response.size());
	}
}

void WebServer::CloseServer() {
	if (shutdown(clientSocket, SD_SEND) != SOCKET_ERROR) return;
	std::cout << "Shutdown Failed.\n";
	listening = false;
	closesocket(clientSocket);
	WSACleanup();
	server.join();
}

void WebServer::AcceptConnection() {
	clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket != INVALID_SOCKET) return;
	closesocket(listenSocket);
	WSACleanup();
}

std::string WebServer::BuildResponse() {
	std::ifstream fs("index.html");
	std::stringstream ss;
	ss << fs.rdbuf();

	std::stringstream response;
	char protocol[] = "HTTP/3 ";
	char status[]   = "200 OK\n";
	char headers[]  = "Content-Type: text/html; charset=UTF-8\n";
	char length[]   = "Content-Length: ";
	response << protocol << status << headers << length << ss.str().size() << ss.str();
	return response.str();
}

void WebServer::SendResponse(const char buffer[], int bytesToSend) {
	if (send(clientSocket, buffer, bytesToSend, 0) == SOCKET_ERROR) {
		std::cout << "Failed to send data.\n";
		listening = false;
		closesocket(clientSocket);
		WSACleanup();
	}
}
