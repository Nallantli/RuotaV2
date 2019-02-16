#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Ruota/Ruota.h"
#include "Ruota/Console.h"
#include "Ruota/FILE_IO/FileIO.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#pragma comment(lib, "User32.lib")
void setColor(int k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}
#else
void setColor(int k)
{
    printf("\033[3%d;40mH", k);
}
#endif

std::vector<DataPoint> __color(std::vector<DataPoint> args)
{
    setColor(args[0].getInt());
    return {};
}

int main(int argc, char * argv[])
{
	srand(time(0));
	RuotaWrapper::register_command("console_print", &console_print);
	RuotaWrapper::register_command("console_input_str", &__input_str);
	RuotaWrapper::register_command("console_to_string", &__to_string);
	RuotaWrapper::register_command("console_to_number", &__to_number);
	RuotaWrapper::register_command("console_rand", &__rand);
	RuotaWrapper::register_command("console_color", &__color);

	RuotaWrapper::register_command("system_dir", &__filesystem_listdir);
	RuotaWrapper::register_command("system_mkdir", &__filesystem_mkdir);
	RuotaWrapper::register_command("system_exists", &__filesystem_exists);
	RuotaWrapper::register_command("system_copy", &__filesystem_copy);
	RuotaWrapper::register_command("system_rename", &__filesystem_rename);
	RuotaWrapper::register_command("system_size", &__filesystem_size);
	RuotaWrapper::register_command("system_remove", &__filesystem_remove);

	RuotaWrapper::register_command("io_open", &__file_open);
	RuotaWrapper::register_command("io_close", &__file_close);
	RuotaWrapper::register_command("io_read_line", &__file_read_line);

	RuotaWrapper::register_command("io_open_w", &__filew_open);
	RuotaWrapper::register_command("io_close_w", &__filew_close);
	RuotaWrapper::register_command("io_write", &__filew_write);

	/*std::string code = "";
	std::string line;
	std::ifstream myfile("test.ruo");
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			code += line + "\n";
		}
		myfile.close();
	}*/

	RuotaWrapper *wrapper = new RuotaWrapper(argv[0]);

	try
	{
		std::string line;
		do {
			setColor(12);
			std::cout << "> ";
			setColor(7);
			std::getline(std::cin, line);
			DataPoint dp = wrapper->evaluate(line);
			int i = 0;
			setColor(1);
			for (auto &d : dp.getVector()) {
			std::cout << "\t" << i++ << ")\t" << d.getDebug() << "\n";
			}
		} while (line != "");
	}
	catch (std::runtime_error &e)
	{
		setColor(12);
		std::cout << e.what() << std::endl;
	}

	//setColor(8);
	//std::cout << wrapper->MAIN->getDebug();

	delete wrapper;

	setColor(7);
	return 0;
}