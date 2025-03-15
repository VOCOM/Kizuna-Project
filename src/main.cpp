#include <core.hpp>
#include <harmony.hpp>
#include <webserver.hpp>

int main(int argc, char** argv) {
	Kizuna kizuna;
	kizuna.Initialize();

	kizuna.LoadModule(std::make_shared<WebServer>());
	kizuna.LoadModule(std::make_shared<Harmony>());

	kizuna.Access();
}