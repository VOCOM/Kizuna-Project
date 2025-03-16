#include "database.hpp"

std::shared_ptr<Database> Database::GetSharedPtr() {
	return shared_from_this();
}
