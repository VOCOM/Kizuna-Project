#include <webserver/webserver.hpp>

#include <sstream>

#include <kizuna/kizuna.hpp>

namespace Responses {

void Status(std::stringstream& ss) {
	ss << "<!DOCTYPE html>";
	for (auto& module : Kizuna::ModuleList) {
		ss << "<tr>"
			 << "<td>" << module->Name() << "</td>"
			 << "<td>" << module->Status() << "</td>"
			 << "</tr>";
	}
}

} // namespace Responses
