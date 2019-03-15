#include "Console.h"

std::vector<DataPoint> console_print(std::vector<DataPoint> args)
{
    for (auto &arg : args)
    {
        for (auto &dp : arg.getVector())
        {
            std::cout << std::string(1, dp.getChar());
        }
    }
    return {};
}

void printToCoordinates(int x, int y, const std::string &text)
{
    printf("\033[%d;%dH%s\n", x, y, text.c_str());
}

std::pair<int, int> getConsoleSize()
{
    int columns, rows;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    columns = w.ws_col;
    rows = w.ws_row;
#endif
    return {columns, rows};
}

std::vector<DataPoint> __input_str(std::vector<DataPoint> args)
{
    std::string d;
    std::getline(std::cin, d);
    std::vector<DataPoint> dp;
    for (auto &c : d)
        dp.push_back(DataPoint(c));
    return dp;
}

std::vector<DataPoint> __to_string(std::vector<DataPoint> args)
{
    std::string d;
    switch (args[0].getLong())
    {
    case 0:
        d = std::to_string(args[1].getInt());
        break;
    case 1:
        d = std::to_string(args[1].getLong());
        break;
    case 2:
        d = std::to_string(args[1].getShort());
        break;
    case 3:
        d = std::to_string(args[1].getDouble());
        break;
    case 4:
        d = std::to_string(args[1].getFloat());
        break;
    case 5:
        d = std::to_string(args[1].getChar());
        break;
    case 6:
        d = std::to_string(args[1].getBool());
        break;
    }
    std::vector<DataPoint> dp;
    for (auto &c : d)
        dp.push_back(DataPoint(c));
    return dp;
}

std::vector<DataPoint> __to_number(std::vector<DataPoint> args)
{
    std::string d;
    for (auto &dp : args[1].getVector())
    {
        d += std::string(1, dp.getChar());
    }
    std::vector<DataPoint> ret;
    switch (args[0].getLong())
    {
    case 0:
        ret.push_back(DataPoint(std::stoi(d)));
        break;
    case 1:
        ret.push_back(DataPoint(std::stol(d)));
        break;
    case 3:
        ret.push_back(DataPoint(std::stod(d)));
        break;
    default:
        throw std::runtime_error("Error: " + args[0].getLong());
    }
    return ret;
}

std::vector<DataPoint> __rand(std::vector<DataPoint> args)
{
    std::vector<DataPoint> ret;
    ret.push_back(rand());
    return ret;
}

std::vector<DataPoint> __console_size(std::vector<DataPoint> args)
{
    auto size = getConsoleSize();
    return {DataPoint((long)size.first), DataPoint((long)size.second)};
}

std::vector<DataPoint> __filesystem_listdir(std::vector<DataPoint> args)
{
    std::vector<DataPoint> list;
    std::string path = RuotaWrapper::path.substr(1) + data_to_string(args[0]);
    for (auto &p : boost::filesystem::directory_iterator(path))
    {
        std::string file = p.path().string().substr(path.length());
        while (file[0] == '\\' || file[0] == '/')
            file = file.substr(1);
        list.push_back(string_to_data(file));
    }
    return list;
}

std::vector<DataPoint> __filesystem_mkdir(std::vector<DataPoint> args)
{
    boost::filesystem::create_directory(RuotaWrapper::path.substr(1) + data_to_string(args[0]));
    return {};
}

std::vector<DataPoint> __filesystem_exists(std::vector<DataPoint> args)
{
    return {DataPoint((bool)boost::filesystem::exists(RuotaWrapper::path.substr(1) + data_to_string(args[0])))};
}

std::vector<DataPoint> __filesystem_copy(std::vector<DataPoint> args)
{
    std::string origin = RuotaWrapper::path.substr(1) + data_to_string(args[0]);
    std::string path = RuotaWrapper::path.substr(1) + data_to_string(args[1]);
    boost::filesystem::copy(origin, path);
    return {};
}

std::vector<DataPoint> __filesystem_rename(std::vector<DataPoint> args)
{
    std::string origin = RuotaWrapper::path.substr(1) + data_to_string(args[0]);
    std::string path = RuotaWrapper::path.substr(1) + data_to_string(args[1]);
    boost::filesystem::rename(origin, path);
    return {};
}

std::vector<DataPoint> __filesystem_size(std::vector<DataPoint> args)
{
    return {DataPoint((int)boost::filesystem::file_size(RuotaWrapper::path.substr(1) + data_to_string(args[0])))};
}

std::vector<DataPoint> __filesystem_remove(std::vector<DataPoint> args)
{
    boost::filesystem::remove(RuotaWrapper::path.substr(1) + data_to_string(args[0]));
    return {};
}