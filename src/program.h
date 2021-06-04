#pragma once

#include "array.h"
#include "Output.h"


namespace program
{
	// Приветствие
	void greeting();

	// Прочитать массив из файла
	bool read_array_from_file(array<int>& arr, FileInput& input);

	bool read_array_from_console(array<int>& arr);

	int sort_array(array<int>& arr);

	void ask_for_input_save(array<int>& arr);

	void print_array_to_file(array<int>& arr, FileOutput& file);

	// Сохранить вывод программы
	void save_output(const std::string& filepath, array<int>& input_arr, array<int>& output_arr);

	// Спросить, нужно ли сохранить вывод программы в файл, и сохранить, если нужно
	void ask_for_output_save(array<int>& input_arr, array<int>& output_arr);

	void print_arrays_to_console(array<int>& input_arr, array<int>& output_arr);

	void fill_arr_with_random_nums(array<int>& arr);

	void ask_for_tests();

	void loop();

	void run();
}
