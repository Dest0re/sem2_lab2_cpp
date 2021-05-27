#pragma once

#include "array.h"
#include "Output.h"
#include "Input.h"
#include <Windows.h>

enum console_color
{
	black = 0,
	blue = 1,
	green = 2,
	cyan = 3,
	red = 4,
	magenta = 5,
	brown = 6,
	light_gray = 7,
	dark_gray = 8,
	light_blue = 9,
	light_green = 10,
	light_cyan = 11,
	light_red = 12,
	light_magenta = 13,
	yellow = 14,
	white = 15
};


namespace program
{
	const std::string greeting_string =
		"Starikov Danil, 404 gr. variation 3, task 2.\n\n"
		"The problem is: \n"
		"Write a program for rearranging the elements of an array so that all even elements are on the left side of the array, and all odd ones are on the right.\n";


	// Приветствие
	void greeting() {
		std::cout << greeting_string << std::endl;
	}

	// Открыть файл для вывода
	FileOutput open_file_for_output(std::string filepath) {
		while (true) {
			try {
				return FileOutput(filepath, std::ofstream::trunc);
			}
			catch (FileAlreadyExistError&) {
				std::cout << "Filepath: ";
			}
			catch (FileNotOpenError&) {
				std::cout << "File cannot exist. Filepath: ";
			}
			getline(std::cin, filepath);
		}
	}

	FileInput open_file_for_input(std::string filepath) {
		while (true) {
			try {
				return FileInput(filepath, std::ifstream::in);
			}
			catch (FileNotExistError&) {
				std::cout << "File does not exist. Filepath: ";
				getline(std::cin, filepath);
			}
			catch (FileNotOpenError&) {
				std::cout << "File does not exist. Filepath: ";
				getline(std::cin, filepath);
			}
		}
	}

	// Прочитать массив из файла
	bool read_array_from_file(array<int>& arr, FileInput& input) {
		try {
			const int length = input.get_num(1, INT_MAX);

			arr = *input.get_array<int>(length);
		}
		catch (ReadNumError&) {
			return false;
		}
		catch (EOFError&) {
			return false;
		}
		catch (std::invalid_argument&) {
			return false;
		}
		catch (FileNotOpenError&) {
			return false;
		}
		catch (std::range_error&) {
			return false;
		}

		return true;
	}

	bool read_array_from_console(array<int>& arr) {
		ConsoleInput input{};

		while (true)
		{
			std::cout << "Enter the array size: ";
			int length;
			try
			{
				length = input.get_num<int>(1, INT_MAX);
			}
			catch (std::range_error&)
			{
				continue;
			}
			catch (ReadNumError&)
			{
				continue;
			}

			arr = array<int>(length);
			for (int i = 0; i < length; i++)
			{
				std::cout << "Enter the number #" << i + 1 << " / " << length << ": ";
				arr[i] = input.get_num<int>();
			}

			return true;
		}
	}


	int sort_array(array<int>& arr)
	{
		int counter = 0;

		for (int shift = 0; shift < arr.get_length() - 1; shift++)
		{
			for (int i = shift; i < arr.get_length() - shift - 1; i++)
			{
				if (arr[i] % 2 != 0 && arr[i + 1] % 2 == 0)
				{
					const auto temp = arr[i];

					arr[i] = arr[i + 1];
					arr[i + 1] = temp;
					counter++;
				}
			}
		}
		return counter;
	}

	void ask_for_input_save(array<int>& arr)
	{
		std::cout << "Do you want to save input to file? [ENTER] for \"no\", filepath to save: ";

		std::string choice;
		getline(std::cin, choice);

		if (!choice.empty())
		{
			FileOutput file = open_file_for_output(choice);

			file.print(arr.get_length());
			file.print_array(arr);

			std::cout << "Successful save." << std::endl;
		}
	}

	void print_array_to_file(array<int>& arr, FileOutput& file)
	{
		for (int i = 0; i < arr.get_length(); i++)
		{
			file.print("arr[ ", "");
			file.print(i, "");
			file.print(" ] = ", "");
			file.print(arr[i]);
		}
	}

	// Сохранить вывод программы
	void save_output(const std::string& filepath, array<int>& input_arr, array<int>& output_arr) {
		FileOutput file = open_file_for_output(filepath);

		file.print("Input and output data of the program \"Odd Sorting\"", "\n\n\n");

		file.print("Input array: \n");

		print_array_to_file(input_arr, file);

		file.print("\nOutput array: \n");

		print_array_to_file(output_arr, file);

		std::cout << "Successful save." << std::endl;
	}

	// Спросить, нужно ли сохранить вывод программы в файл, и сохранить, если нужно
	void ask_for_output_save(array<int>& input_arr, array<int>& output_arr) {
		std::string filepath;

		std::cout << "Do you want to save output? Filepath to save, [ENTER] to continue: ";

		getline(std::cin, filepath);

		if (!filepath.empty()) save_output(filepath, input_arr, output_arr);
	}

	void print_arrays_to_console(array<int>& input_arr, array<int>& output_arr)
	{
		ConsoleOutput out{};
		const HANDLE h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);

		std::cout << "Input array: " << std::endl;
		out.print_array(input_arr, ", ");
		std::cout << std::endl;

		SetConsoleTextAttribute(h_std_out, static_cast<WORD>(black << 4 | cyan));

		std::cout << "Output array: " << std::endl;
		out.print_array(output_arr, ", ");
		std::cout << std::endl << std::endl;

		SetConsoleTextAttribute(h_std_out, static_cast<WORD>(black << 4 | white));
	}

	void fill_arr_with_random_nums(array<int>& arr)
	{
		ConsoleInput input{};
		std::cout << "Enter the array length: ";
		const int length = input.get_num(1, INT_MAX);
		std::cout << "Enter the minimum number: ";
		const int min = input.get_num(-1000, 1000);
		std::cout << "Enter the maximum number: ";
		const int max = input.get_num(min, 1000);

		auto new_arr = array<int>(length);

		for (int i = 0; i < length; i++)
		{
			new_arr[i] = min + rand() % (max - min + 1);
		}

		arr = new_arr;
	}


	void loop()
	{
		array<int> input_arr{};

		std::cout << "Choose input method. [ENTER] for console input, filepath for input from file: ";

		std::string choice;
		getline(std::cin, choice);

		std::cout << std::endl;

		if (choice.empty())
		{
			std::cout << "Any char for random input, [ENTER] for manual input: ";

			getline(std::cin, choice);

			if (choice.empty())
			{
				read_array_from_console(input_arr);
			}
			else
			{
				fill_arr_with_random_nums(input_arr);
			}

			array<int>* output_arr = input_arr.copy();

			ask_for_input_save(input_arr);

			sort_array(*output_arr);

			print_arrays_to_console(input_arr, *output_arr);

			ask_for_output_save(input_arr, *output_arr);

			delete output_arr;
		}
		else
		{
			if (FileInput input = open_file_for_input(choice); !read_array_from_file(input_arr, input)) {
				std::cout << "Invalid file. Please choose another one." << std::endl;
				return;
			}
			auto output_arr = input_arr.copy();
			sort_array(*output_arr);

			print_arrays_to_console(input_arr, *output_arr);

			ask_for_output_save(input_arr, *output_arr);

			delete output_arr;
		}
	}


	void run() {
		greeting();
		//Program::ask_for_tests();

		std::string choice;
		do {
			std::cout << std::endl;
			loop();
			std::cout << "[ENTER] to exit, any key to repeat;";
			getline(std::cin, choice);
		} while (!choice.empty());
		std::cout << "Bye!" << std::endl;
	}
}