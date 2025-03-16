#ifndef DATABASE
#define DATABASE

#include <map>
#include <string>
#include <vector>

#include <database/data.hpp>
#include <database/database_accessor.hpp>
#include <module.hpp>

class Database : public Module {
public:
	enum Type {
		INT,
		DOUBLE,
		TEXT
	};

public:
	virtual void Open(std::string database) = 0;
	virtual void Close()                    = 0;

	// Create
	virtual void AddTable(std::string name, std::string pKey, Type pType)  = 0;
	virtual void AddColumn(std::string name, std::string cKey, Type cType) = 0;
	virtual void AddRow(std::string name, std::vector<double> values)      = 0;

	// Read

	// Update
	virtual void UpdateColumn(std::string name, std::string cKey, std::string cKeyNew) = 0;

	// Drop
	virtual void DropTable(std::string name)                    = 0;
	virtual void DropColumn(std::string name, std::string cKey) = 0;
	// virtual void DropRow(std::string name)                      = 0;

	std::shared_ptr<Database> GetSharedPtr();

	virtual ~Database() {}

protected:
	const std::map<Type, std::string> Types{
			{INT, "INT"},
			{DOUBLE, "DOUBLE"},
			{TEXT, "TEXT"}};

private:
	virtual void Query(std::string query) = 0;

	static std::shared_ptr<Database> instance;

	friend Data DatabaseAccessor::Query(std::string query);
};

#endif /* DATABASE */
