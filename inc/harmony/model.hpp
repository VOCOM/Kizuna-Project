#ifndef MODEL
#define MODEL

#include <iostream>
#include <thread>

#include <data.hpp>
#include <results.hpp>

class Model {
public:
	std::string Name() const { return name; }
	void Load(DataTable& newData) { data = DataTable(newData); }
	void Load(std::string filepath, bool hasHeaders = true) {
		while (lock) std::this_thread::yield();
		data.LoadCSV(filepath, hasHeaders);
	}
	void Drop() {
		while (lock) std::this_thread::yield();
		data.DropData();
	}
	void Info(int count = -1) {
		while (lock) std::this_thread::yield();
		data.Info(count);
	}
	int DataCount() const { return data.Count(); }

	virtual void Execute() = 0;

	Model() = delete;
	Model(std::string name) : name(name) {}
	virtual ~Model() {}

protected:
	std::atomic_bool lock = false;
	std::string name;
	DataTable data;
	Results result;
};

#endif /* MODEL */
