#ifndef FILE_IO_H
#define FILE_IO_H

#include "../Ruota.h"

std::vector<DataPoint> __file_open(std::vector<DataPoint> args);
std::vector<DataPoint> __filew_open(std::vector<DataPoint> args);
std::vector<DataPoint> __file_close(std::vector<DataPoint> args);
std::vector<DataPoint> __filew_close(std::vector<DataPoint> args);
std::vector<DataPoint> __filew_write(std::vector<DataPoint> args);
std::vector<DataPoint> __file_read_line(std::vector<DataPoint> args);
std::vector<DataPoint> __file_read(std::vector<DataPoint> args);

#endif