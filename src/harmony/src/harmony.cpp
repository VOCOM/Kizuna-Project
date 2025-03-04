#include "harmony.hpp"

#include <iostream>

#include <kizuna.hpp>

bool Harmony::running;
std::thread Harmony::mainThread;

int Harmony::cpuThreadMax;
int Harmony::gpuThreadMax;

void Harmony::Start() {
	running    = true;
	mainThread = std::thread(&Harmony::Loop);
}

Harmony::Harmony() {
	// Load Configuration
	auto& config = Configuration::Config["harmony"];
	auto param   = config["max_cpu_threads"];
	if (param == "auto") {
		cpuThreadMax = 16;
	} else {
		cpuThreadMax = std::stoi(param);
	}
	param = config["max_gpu_threads"];
	if (param == "auto") {
		gpuThreadMax = 16;
	} else {
		gpuThreadMax = std::stoi(param);
	}
}

Harmony::~Harmony() {
	running = false;
	if (mainThread.joinable()) mainThread.join();
}

void Harmony::Loop() {
	while (running) {}
	std::cout << "Harmony terminating...\n";
}
