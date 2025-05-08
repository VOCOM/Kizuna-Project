#include <sqlite_database.hpp>

#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#include <configuration.hpp>
#include <utility/utils.hpp>

// Submodule Interface
void SQLiteDB::Info() {
	std::cout << "\n"
						<< std::setw(10) << std::left << "Module" << Name() << "\n"
						<< std::setw(10) << std::left << "Status" << ToString(status) << "\n"
						<< std::setw(10) << std::left << "Database" << dbName << "\n";
}
void SQLiteDB::Start() {
	status = Online;
	std::cout << "SQLiteDB Online\n";
}
void SQLiteDB::Stop() {
	Close();
	status = Offline;
	std::cout << "SQLiteDB terminating...\n";
}
void SQLiteDB::Restart() {
	Stop();
	Start();
}
void SQLiteDB::LoadConfiguration() {
	std::string param;
	auto& config = Configuration::Config["sqlite"];
	Open(config["database"]);
}

// Shell Interface
void SQLiteDB::Access() {
	std::string command;

	while (true) {
		// Input
		std::cout << "SQLiteDB: ";
		std::cin >> buffer;
		buffer >> command;

		// Shell Commands
		if (command == "open" && !buffer.empty()) Open(buffer.pop());
		// if (command == "query") Query(buffer.substr(buffer.find(' ') + 1, buffer.size()));

		if (command == "add") {
			if (buffer.size() < 3) continue;
			buffer >> command;
			std::string tableName = buffer.pop();
			std::string keyName   = buffer.pop();

			if (command == "table") AddTable(tableName, keyName);
			if (command == "col") AddColumn(tableName, keyName, Database::DOUBLE);
			if (command == "row") {
				std::vector<double> values;
				while (!buffer.empty()) {
					values.push_back(std::stod(buffer.pop()));
				}
				AddRow(tableName, values);
			}
		}
		if (command == "drop") {}

		if (command == "clear") Clear();
		if (command == "help") CommandList();
		if (command == "exit") return;
	}
}
void SQLiteDB::CommandList() {
	std::cout << "\nDatabase Commands\n"
						<< std::setw(10) << std::left << "OPEN" << "Loads a SQL database file.\n"
						<< std::setw(10) << std::left << "QUERY" << "Issues a SQL query.\n"
						<< std::setw(10) << std::left << "ADD" << "Add a row|col|table.\n"
						<< std::setw(10) << std::left << "DROP" << "Drops a table.\n"
						<< "\nShell Commands\n"
						<< std::setw(10) << std::left << "CLEAR" << "Clear terminal screen.\n"
						<< std::setw(10) << std::left << "HELP" << "List commands.\n"
						<< std::setw(10) << std::left << "EXIT" << "Exit submodule shell.\n"
						<< "\n";
}

void SQLiteDB::Open(std::string database) {
	if (database.empty()) return;
	Close();

	database += ext;
	if (sqlite3_open(database.c_str(), &db) != SQLITE_OK) {
		std::stringstream ss;
		ss << "Failed to open database" << sqlite3_errmsg(db);
		Raise(ss.str());
		return;
	}

	dbName = database;
}
void SQLiteDB::Close() {
	if (!db) return;

	sqlite3_close(db);
	dbName.clear();
}

void SQLiteDB::Query(std::string query) {
	char* errMsg = nullptr;
	if (sqlite3_exec(db, query.c_str(), 0, 0, &errMsg) == SQLITE_OK) return;
	sqlite3_free(errMsg);
	Raise("Error executing SQL Statement");
}

// Create
void SQLiteDB::AddTable(std::string name, std::string pKey, Type pType) {
	std::stringstream ss;
	ss << "CREATE TABLE " << name
		 << "(" << pKey << " " << Types.at(pType) << " PRIMARY KEY NOT NULL);";
	Query(ss.str());
}
void SQLiteDB::AddColumn(std::string name, std::string cKey, Type cType) {
	std::stringstream ss;
	ss << "ALTER TABLE " << name << "\n";
	ss << "ADD " << cKey << " " << Types.at(cType);
	Query(ss.str());
}
void SQLiteDB::AddRow(std::string name, std::vector<double> values) {
	std::stringstream ss;
	ss << "INSERT INTO " << name << " VALUES(";
	for (int i = 0; i < values.size(); i++) {
		ss << "\'" << values[i] << "\'";
		if (i < values.size() - 1) ss << ',';
	}
	ss << ");";
	Query(ss.str());
}

// Read

// Update
void SQLiteDB::UpdateColumn(std::string name, std::string cKey, std::string cKeyNew) {
	std::stringstream ss;
	ss << "ALTER TABLE " << name << "\n"
		 << "RENAME COLUMN " << cKey << " to " << cKeyNew;
	Query(ss.str());
}

// Drop
void SQLiteDB::DropTable(std::string name) {
	std::stringstream ss;
	ss << "DROP TABLE " << name << ";";
	Query(ss.str());
}
void SQLiteDB::DropColumn(std::string name, std::string cKey) {
	std::stringstream ss;
	ss << "ALTER TABLE " << name << "\n"
		 << "DROP COLUMN " << cKey;
	Query(ss.str());
}

SQLiteDB::SQLiteDB() : Database("SQLiteDB"), status(), db(0), dbName() {}
SQLiteDB::~SQLiteDB() { Close(); }
