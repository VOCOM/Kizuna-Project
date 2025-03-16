#include <database.hpp>

Data DatabaseAccessor::Query(std::string query) {
	Database::instance->Query(query);
	return Data();
}
