#include <webserver/webserver.hpp>

#include <sstream>

#include <kizuna/kizuna.hpp>

namespace Responses {

void Status(std::stringstream& ss) {
	ss << "<!DOCTYPE html>";
	for (auto& submodule : Kizuna::SubmoduleList) {
		ss << "<tr>"
			 << "<td>" << submodule->Name() << "</td>"
			 << "<td>" << submodule->Status() << "</td>"
			 << "</tr>";
	}
}

} // namespace Responses
