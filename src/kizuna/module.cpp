#include <module.hpp>

#include <typeindex>

std::vector<std::shared_ptr<Module>> Module::modules;

void Module::RegisterModule(std::shared_ptr<Module> module) {
	modules.push_back(module);
}

bool Module::IsOnline(std::string moduleName) {
	for (auto& module : modules)
		if (module->Name() == moduleName) return true;
	return false;
}

std::shared_ptr<Module> Module::GetModule(const std::string& moduleType) {
	for (auto& module : modules) {
		if (module->Name() == moduleType)
			return module;
	}
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
