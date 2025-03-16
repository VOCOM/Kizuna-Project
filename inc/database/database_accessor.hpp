#ifndef DATABASE_ACCESSOR
#define DATABASE_ACCESSOR

#include <database/data.hpp>

class DatabaseAccessor {
public:
	Data Query(std::string query);
};

#endif /* DATABASE_ACCESSOR */
