#include "FileIO.h"

std::vector<DataPoint> __file_open(std::vector<DataPoint> args) {
	std::string fname = RuotaWrapper::path.substr(1) + data_to_string(args[0]);
	while (fname[0] == '\\') fname = fname.substr(1);
	std::ifstream *file = new std::ifstream(fname);
	if (!file->is_open()){
		throw std::runtime_error("Error: cannot open file " + fname + "!");
	}
	return { DataPoint((void*)file) };
}

std::vector<DataPoint> __filew_open(std::vector<DataPoint> args) {
	std::string fname = RuotaWrapper::path.substr(1) + data_to_string(args[0]);
	while (fname[0] == '\\') fname = fname.substr(1);
	std::ofstream *file = new std::ofstream(fname, args[1].getChar() == 'a' ? std::ofstream::app : std::ofstream::out);
	if (!file->is_open()){
		throw std::runtime_error("Error: cannot open file " + fname + "!");
	}
	return { DataPoint((void*)file) };
}

std::vector<DataPoint> __file_close(std::vector<DataPoint> args) {
	std::ifstream *file = (std::ifstream*)args[0].getPointer();
	file->close();
	delete file;
	return {};
}

std::vector<DataPoint> __filew_close(std::vector<DataPoint> args) {
	std::ofstream *file = (std::ofstream*)args[0].getPointer();
	file->close();
	delete file;
	return {};
}

std::vector<DataPoint> __filew_write(std::vector<DataPoint> args) {
	std::ofstream *file = (std::ofstream*)args[0].getPointer();
	*file << data_to_string(args[1]);
	return {};
}

std::vector<DataPoint> __file_read_line(std::vector<DataPoint> args) {
	std::ifstream *file = (std::ifstream*)args[0].getPointer();
	std::string line;
	if (std::getline(*file, line)) {
		return { DataPoint(true), string_to_data(line) };
	}
	else {
		return { DataPoint(false), string_to_data("") };
	}
}

std::vector<DataPoint> __file_read(std::vector<DataPoint> args) {
	std::ifstream *file = (std::ifstream*)args[0].getPointer();
	char * buffer = new char[(int)args[1].getLong()];
	if (file->read(buffer, args[1].getLong()))
		return string_to_data(std::string(buffer, args[1].getLong())).getVector();
	else
		return {};
}