#ifndef WIN_SERVER
#define WIN_SERVER

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <string>
#include <thread>

#include <kizuna.hpp>

class WebServer : public Module {
public:
	// Module Interface
	virtual std::string Name() { return "Webserver"; }
	virtual std::string Status() { return ToString(status); }
	virtual void Info();
	virtual void Start();
	virtual void Stop();
	virtual void Restart();
	virtual void LoadConfiguration();

	// Shell Interface
	virtual void Access();

	WebServer();
	virtual ~WebServer();

private:
	void Loop();
	void AcceptConnection();
	void ProcessRequest();
	void ProcessResponse();
	std::string BuildContent();

private:
	StatusCode status;

	static const uint64_t BUFFER_SIZE = 0xFFFF;
	WSADATA wsaData;

	std::string port;
	std::string nodename;
	std::thread server;

	// Requests
	int requestBytes;
	std::string method;
	std::string param;
	std::string protocol;
	std::string contentType;
	char buffer[BUFFER_SIZE];

	struct addrinfo hints;
	struct addrinfo* result = nullptr;

	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;
};

#endif /* WIN_SERVER */
