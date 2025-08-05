#include <kizuna.hpp>

int main() {
	Kizuna kizuna;
	kizuna.Run();

	// kizuna.LoadModule(std::make_shared<WebServer>());
	// kizuna.LoadModule(std::make_shared<SQLiteDB>());
	// kizuna.LoadModule(std::make_shared<Harmony>());
}
