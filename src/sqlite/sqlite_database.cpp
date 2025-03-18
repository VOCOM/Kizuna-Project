#include <sqlite_database.hpp>

#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#include <utility/utils.hpp>

// Submodule Interface
void SQLiteDB::Info() {
	std::cout << Name() << "\n"
						<< "Status " << ToString(status) << "\n"
						<< "Database: " << dbName << "\n\n";
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
}

// Shell Interface
void SQLiteDB::Access() {
	std::string buffer, command;
	std::queue<std::string> params;

	while (true) {
		// Input
		std::cout << "SQLiteDB: ";
		std::getline(std::cin, buffer);
		ReplaceToken(buffer, ',');
		params  = Enqueue(buffer);
		command = params.front();
		params.pop();

		// Kernel Commands
		if (command == "exit") return;

		if (command == "open") {
			if (params.empty()) continue;
			Open(params.front());
		}
		if (command == "query") Query(buffer.substr(buffer.find(' ') + 1, buffer.size()));
		if (command == "add") {
			if (params.size() < 3) continue;
			command = params.front();
			params.pop();
			auto tableName = params.front();
			params.pop();
			auto keyName = params.front();

			if (command == "table") AddTable(tableName, keyName);
			if (command == "col") AddColumn(tableName, keyName, Database::DOUBLE);
			if (command == "row") {
				std::vector<double> values;
				while (!params.empty()) {
					values.push_back(std::stod(params.front()));
					params.pop();
				}
				AddRow(tableName, values);
			}
		}
		if (command == "drop") {}
	}
}

void SQLiteDB::Open(std::string database) {
	if (database.empty()) return;
	Close();

	database += ext;
	if (sqlite3_open(database.c_str(), &db) != SQLITE_OK) {
		std::stringstream ss;
		ss << "Failed to open database" << sqlite3_errmsg(db);
		Raise(ss.str());
	}

	dbName = database;
}
void SQLiteDB::Close() {
	if (db) {
		sqlite3_close(db);
		dbName.clear();
	}
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

SQLiteDB::SQLiteDB() : Database("SQLiteDB"), db(0) {}
SQLiteDB::~SQLiteDB() { Close(); }
