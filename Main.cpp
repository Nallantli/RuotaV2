#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <boost/dll/runtime_symbol_info.hpp>
#include "Ruota/Ruota.h"
#include "Ruota/Console.h"
#include "Ruota/FILE_IO/FileIO.h"

void setForeColor(int k)
{
	printf("\033[38;5;%dm", k);
}
void setBackColor(int k)
{
	printf("\033[48;5;%dm", k);
}

std::vector<DataPoint> __fore_color(std::vector<DataPoint> args)
{
	setForeColor(args[0].getInt());
	return {};
}

std::vector<DataPoint> __back_color(std::vector<DataPoint> args)
{
	setBackColor(args[0].getInt());
	return {};
}

int main(int argc, char *argv[])
{
	srand(time(0));
	RuotaWrapper::register_command("console_print", &console_print);
	RuotaWrapper::register_command("console_input_str", &__input_str);
	RuotaWrapper::register_command("console_to_string", &__to_string);
	RuotaWrapper::register_command("console_to_number", &__to_number);
	RuotaWrapper::register_command("console_rand", &__rand);
	RuotaWrapper::register_command("console_fore_color", &__fore_color);
	RuotaWrapper::register_command("console_back_color", &__back_color);

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

	std::string current_dir = boost::dll::program_location().string();
	while (current_dir.back() != '\\' && current_dir.back() != '/')
		current_dir.pop_back();

	RuotaWrapper *wrapper = new RuotaWrapper(current_dir);

	try
	{
		if (argc == 1)
		{
			std::cout << "Ruota RW - " << RUOTA_VERSION << "\n\n";
			std::string line;
			do
			{
				setForeColor(1);
				std::cout << "> ";
				setForeColor(7);
				std::getline(std::cin, line);
				DataPoint dp = wrapper->evaluate(line);
				int i = 0;
				setForeColor(4);
				for (auto &d : dp.getVector())
				{
					std::cout << "\t" << i++ << ")\t" << d.getDebug() << "\n";
				}
			} while (line != "");
		}
		else
		{
			DataPoint dp = wrapper->evaluate("load \"" + std::string(argv[1]) + "\"");
		}
	}
	catch (std::runtime_error &e)
	{
		setForeColor(1);
		std::cout << e.what() << std::endl;
	}

	//setColor(8);
	//std::cout << wrapper->MAIN->getDebug();

	delete wrapper;

	setForeColor(7);
	return 0;
}