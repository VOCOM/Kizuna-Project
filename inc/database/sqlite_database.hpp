#ifndef SQLITE_DATABASE
#define SQLITE_DATABASE

#include <map>

#include <sqlite/sqlite3.h>

#include <database.hpp>
#include <error_emitter.hpp>

class SQLiteDB : public Database, public Emitter {
public: // Module Interface
	virtual std::string Name() { return "SQLiteDB"; }
	virtual std::string Status() { return ToString(status); }
	virtual void Info();
	virtual void Start();
	virtual void Stop();
	virtual void Restart();
	virtual void LoadConfiguration();

public: // Shell Interface
	virtual void Access();

public:
	virtual void Open(std::string database);
	virtual void Close();

	virtual void Query(std::string query);

	// Create
	virtual void AddTable(std::string name, std::string pKey, Type pType = INT);
	virtual void AddColumn(std::string name, std::string cKey, Type cType);
	virtual void AddRow(std::string name, std::vector<double> values);

	// Read

	// Update
	virtual void UpdateColumn(std::string name, std::string cKey, std::string cKeyNew);

	// Drop
	virtual void DropTable(std::string name);
	virtual void DropColumn(std::string name, std::string cKey);

	SQLiteDB();
	virtual ~SQLiteDB();

private:
	void Execute(std::string sql_stmt);

private:
	StatusCode status;
	sqlite3* db;
	std::string dbName;

	const char* ext = ".sqlite";
};

#endif /* SQLITE_DATABASE */
