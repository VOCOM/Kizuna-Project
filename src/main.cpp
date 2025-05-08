#include <harmony.hpp>
#include <kizuna.hpp>
#include <sqlite_database.hpp>
#include <webserver.hpp>

#include "cuda_runtime.h"

int main(int argc, char** argv) {
	Kizuna kizuna;
	kizuna.Initialize();

	kizuna.LoadModule(std::make_shared<WebServer>());
	kizuna.LoadModule(std::make_shared<SQLiteDB>());
	kizuna.LoadModule(std::make_shared<Harmony>());

	kizuna.Access();
}
