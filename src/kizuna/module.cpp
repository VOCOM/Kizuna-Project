#include <module.hpp>

std::vector<std::shared_ptr<Module>> Module::modules;

bool Module::IsOnline(std::string moduleName) {
	for (auto& module : modules)
		if (module->Name() == moduleName) return true;
	return false;
}

std::shared_ptr<Module> Module::GetModule(std::string moduleName) {
	std::transform(moduleName.begin(), moduleName.end(), moduleName.begin(), std::tolower);
	for (auto& module : modules)
		if (module->Name() == moduleName) return module;
	return nullptr;
}

std::vector<std::shared_ptr<Module>>& Module::GetModules() {
	return modules;
}

void Module::Shutdown() {
	for (auto& module : modules) {
		module->Stop();
		module->~Module();
	}
}
