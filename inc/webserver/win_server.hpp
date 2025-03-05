#ifndef TCP
#define TCP

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <string>
#include <thread>

#include <kizuna/kizuna.hpp>

class WebServer : public Submodule {
public:
	// Submodule Interface
	virtual void Info();
	virtual void Start();
	virtual void Stop();
	virtual void Restart();
	virtual void LoadConfiguration();

	WebServer();
	virtual ~WebServer();

private:
	void StartServer();
	void CloseServer();
	void AcceptConnection();
	std::string BuildResponse();
	void SendResponse(const char buffer[], int bytesToSend);

private:
	static const uint64_t BUFFER_SIZE = 0xFFFF;
	const char* Name                  = "Webserver";
	WSADATA wsaData;

	bool listening = false;
	std::string port;
	std::string nodename;
	std::thread server;

	struct addrinfo hints;
	struct addrinfo* result = nullptr;

	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;
};

#endif /* TCP */
