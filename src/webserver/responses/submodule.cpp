#include <webserver.hpp>

#include <sstream>

#include <kizuna.hpp>

namespace Responses {

void Status(std::stringstream& ss) {
	ss << "<!DOCTYPE html>";
	for (auto& module : Module::GetModules()) {
		ss << "<tr>"
			 << "<td>" << module->Name() << "</td>"
			 << "<td>" << module->Status() << "</td>"
			 << "</tr>";
	}
}

} // namespace Responses
