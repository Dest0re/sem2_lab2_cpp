#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <Windows.h>

#include "Input.h"

enum class console_color
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

inline void change_color(const console_color foreground = console_color::white, const console_color background = console_color::black)
{
	const HANDLE h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleTextAttribute(h_std_out, static_cast<WORD>(static_cast<int>(background) << 4 | static_cast<int>(foreground)));
}

// Ошибка при записи
struct WriteError : std::exception {
private:
	std::string _error_text;

public:
	WriteError(std::string error_text) : _error_text{ error_text } {}

	const char* what() {
		return _error_text.c_str();
	}
};

// Ошибка - файл уже существует
struct FileAlreadyExistError : std::exception {
private:
	std::string _error_text;

public:
	FileAlreadyExistError(std::string error_text) : _error_text{ error_text } {}

	const char* what() {
		return _error_text.c_str();
	}
};

// Абстрактный класс вывода
class IOutput {
protected:
	// Печать значения T
	template <typename T>
	bool _print(T value, const std::string end = "\n") {
		
		if (!(*stream << value)) throw WriteError("Error while writing data.");

		*stream << end;

		if (stream->fail()) {
			stream->clear();

			throw WriteError("Error while writing data");
		}
		
		return true;
	}


public:
	template <typename T>
	void print_array(array<T>& arr, std::string end = "\n")
	{
		for (int i = 0; i < arr.get_length(); i++)
		{
			_print(arr[i], end);
		}
	}
	// Поток вывода
	std::ostream* stream = nullptr;
};

// Поток консольного вывода
class ConsoleOutput : public IOutput {
public:
	// Конструктор
	ConsoleOutput() {
		stream = &std::cout;
	}

	// Печать данных в консоль
	template <typename T>
	bool print(T value, std::string end = "\n") {
		try {
			return _print(value, end);
		}
		catch (WriteError) {
			return false;
		}
	}
};

// Класс вывода в файл
class FileOutput : public IOutput {
private:
	// Путь к файлу
	std::string _filepath;
public:

	// Существует ли файл
	static bool is_file_exist(const std::string filepath) {

		std::ifstream file(filepath, std::ifstream::in);
		bool result = file.good();
		file.close();
		return result;
	}

	// Пробовать перезаписать файл
	static bool try_to_overwrite(const std::string filepath) {
		std::error_code ec;
		if (std::filesystem::exists(filepath, ec)) {
			std::string user_choice;
			std::cout << "File already exists. [ENTER] to overwrite, any symbol to return:" << std::endl;
			getline(std::cin, user_choice);
			return user_choice.empty();
		}
		else if (ec.value() == 87) {
			throw FileNotOpenError("File cannot be open");
		}
		return true;
	}

	// Запись данных в файл
	template <typename T>
	bool print(T value, std::string end = "\n") {
		try {
			return _print(value, end);
		}
		catch (WriteError) {
			return true;
		}
	}

	// Конструктор
	FileOutput(const std::string filepath, std::ios_base::openmode mode) : _filepath{ filepath } {

		if (!try_to_overwrite(filepath)) {
			throw FileAlreadyExistError("File already exist.");
		}

		stream = new std::ofstream(filepath, mode);

		if (!dynamic_cast<std::ofstream*>(stream)->is_open()) {
			throw FileNotOpenError("File cannot be open");
		}
	}

	// Деструктор
	~FileOutput() {
		dynamic_cast<std::ofstream*>(stream)->close();
		delete stream;
	}
};

// Открыть файл для вывода
inline FileOutput open_file_for_output(std::string filepath) {
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
