#ifndef CONSOLE_H
#define CONSOLE_H

#include "Ruota.h"
#include <vector>

std::vector<DataPoint> console_print(std::vector<DataPoint>);
std::vector<DataPoint> __color(std::vector<DataPoint> args);
std::vector<DataPoint> __input_str(std::vector<DataPoint> args);
std::vector<DataPoint> __to_string(std::vector<DataPoint> args);
std::vector<DataPoint> __to_number(std::vector<DataPoint> args);
std::vector<DataPoint> __rand(std::vector<DataPoint> args);
std::vector<DataPoint> __console_size(std::vector<DataPoint> args);
std::vector<DataPoint> __filesystem_listdir(std::vector<DataPoint> args);
std::vector<DataPoint> __filesystem_mkdir(std::vector<DataPoint> args);
std::vector<DataPoint> __filesystem_exists(std::vector<DataPoint> args);
std::vector<DataPoint> __filesystem_copy(std::vector<DataPoint> args);
std::vector<DataPoint> __filesystem_rename(std::vector<DataPoint> args);
std::vector<DataPoint> __filesystem_size(std::vector<DataPoint> args);
std::vector<DataPoint> __filesystem_remove(std::vector<DataPoint> args);

#endif