#ifndef TCP
#define TCP

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <string>
#include <thread>

class WebServer {
public:
	void Start();

	WebServer(std::string port = "27015");
	~WebServer();

private:
	void StartServer();
	void CloseServer();
	void AcceptConnection();
	std::string BuildResponse();
	void SendResponse(const char buffer[], int bytesToSend);

private:
	static const uint64_t BUFFER_SIZE = 0xFFFF;
	WSADATA wsaData;

	bool listening = false;
	std::string port;
	std::thread server;

	struct addrinfo hints;
	struct addrinfo* result = nullptr;

	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;
};

#endif /* TCP */
