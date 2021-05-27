#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <utility>


// Ошибка при неудачном считывании числа
struct ReadNumError : std::exception {
private:
	std::string error_text_;

public:
	ReadNumError(std::string error_text) : error_text_{std::move(error_text)} {};

	const char* what() const override
	{
		return error_text_.c_str();
	}
};

// Ошибка при попытке открыть несуществующий файл
struct FileNotExistError : std::exception {
private:
	std::string error_text_;

public:
	FileNotExistError(std::string error_text) : error_text_{std::move(error_text)} {};

	const char* what() const override
	{
		return error_text_.c_str();
	}
};

// Ошибка при попытке прочитать файл
struct FileReadingError : std::exception {
private:
	std::string error_text_;

public:
	FileReadingError(std::string error_text) : error_text_{std::move(error_text)} {};

	const char* what() const override
	{
		return error_text_.c_str();
	}
};

// Ошибка при попытке открыть файл
struct FileNotOpenError : std::exception {
private:
	std::string error_text_;

public:
	FileNotOpenError(std::string error_text) : error_text_{std::move(error_text)} {};

	const char* what() const override
	{
		return error_text_.c_str();
	}
};

// Ошибка - конец файла
struct EOFError : std::exception {
private:
	std::string error_text_;

public:
	EOFError(std::string error_text) : error_text_(std::move(error_text)) {};

	const char* what() const override
	{
		return error_text_.c_str();
	}
};

// Абстрактный класс ввода
class IInput {
public:
	virtual ~IInput() = default;
	// Поток ввода
	std::istream* stream = 0;
	
	// Абстрактный метод получения строки
	virtual std::string get_string() = 0;


	// Получение строки.
	std::string _get_string() const
	{
		std::string str;
		std::getline(*stream, str);

		return (!str.empty() ? str : "\n");
	}

	// Получение числа типа T, ReadNumException при неудаче.
	template <typename T>
	T _get_num() {
		T number;
		*stream >> number;

		if (stream->fail()) {
			stream->clear();
			stream->ignore(INT_MAX, '\n');

			throw ReadNumError("Error while reading a number");
		}

		stream->ignore(INT_MAX, '\n');

		return number;
	}
};


// Класс консольного ввода
class ConsoleInput : IInput {

private:

	// Получение числа типа double в диапазоне от min до max, range_error при неудаче.
	template <typename T>
	T _get_num(T min, T max) {
		T number = get_num<T>();

		if (number >= min && number <= max) {
			return number;
		}
		else {
			throw std::range_error("Number must be in provided range");
		}
	}

public:
	// Получение числа
	template <typename T>
	T get_num() {
		while (true) {
			try {
				return IInput::_get_num<T>();
			}
			catch (ReadNumError e) {
				std::cout << "Wrong number! Try again:" << std::endl;
			}
		}
	}

	// Конструктор
	ConsoleInput() {
		this->stream = &std::cin;
	}

	// Получение строки
	std::string get_string() override {
		return this->_get_string();
	}

	// Получение строки в диапазоне от min до max
	template <typename T>
	T get_num(T min, T max) {
		while (true) {
			try {
				return _get_num<T>(min, max);
			}
			catch (std::range_error e) {
				std::cout << "Number not in range! Please enter the num between " << min << " and " << max << ":" << std::endl;
			}
		}
	}

	template <typename T>
	array<T>* get_array(int size)
	{
		array<T>* new_array = new array<T>(size);

		for (int i = 0; i < size; i++)
		{
			(*new_array)[i] = get_num<T>();
		}

		return new_array;
	}

};

// Класс файлового ввода
class FileInput : IInput {
private:
	// Путь к файлу
	std::string _filepath;

	// Вывод исключения при достижении конца файла
	void _eof_exc(std::string text = "") {
		throw EOFError("End of a file! " + text);
	}

	// Проверка, доступен ли файл
	static bool _is_regular_file(std::string filepath) {
		std::error_code ec;
		return std::filesystem::is_regular_file(filepath, ec);
	}

	// Получение числа типа double в диапазоне от min до max, range_error при неудаче.
	template <typename T>
	T _get_num(T min, T max) {
		T number = get_num<T>();

		if (number >= min && number <= max) {
			return number;
		}
		else {
			throw std::range_error("Number must be in provided range");
		}
	}

public:
	// Проверка, существует ли файл
	static bool is_file_exist(std::string filepath) {
		std::ifstream file(filepath);
		bool result = file.good();
		file.close();
		return result;
	}

	// Конструктор
	FileInput(std::string filepath, std::ios_base::openmode mode) : _filepath{ filepath } {
		this->stream = new std::ifstream(filepath, mode);

		if (!is_file_exist(filepath)) {
			throw FileNotExistError("File does not exist.");
		}

		if (!dynamic_cast<std::ifstream*>(stream)->is_open()) {
			dynamic_cast<std::ifstream*>(stream)->close();
			throw FileNotOpenError("Error while opening file.");
		}

		if (!_is_regular_file(filepath)) {
			dynamic_cast<std::ifstream*>(stream)->close();
			throw FileNotOpenError("Invalid filepath.");
		}
	}

	// Получение строки
	std::string get_string() {
		if (!dynamic_cast<std::ifstream*>(stream)->eof()) {
			return _get_string();
		}
		else {
			_eof_exc("Error while getting a string.");
			return "";
		}
	}

	// Получение числа
	template <typename T>
	T get_num() {
		if (!static_cast<std::ifstream*>(stream)->eof()) {
			return IInput::_get_num<T>();
		}
		else {
			_eof_exc("Error while getting a num.");
			return 0;
		}
	}

	// Получение числа в диапазоне от min до max
	template <typename T>
	T get_num(T min, T max) {
		if (!static_cast<std::ifstream*>(stream)->eof()) {
			return _get_num<T>(min, max);
		}
		else {
			_eof_exc("Error while getting a num in range.");
			return 0;
		}
	}

	template <typename T>
	array<T>* get_array(int size)
	{
		auto* new_array = new array<T>(size);

		for (int i = 0; i < size; i++)
		{
			(*new_array)[i] = get_num<T>();
		}

		return new_array;
	}

	// Деструктор
	~FileInput() {
		if (static_cast<std::ifstream*>(stream)->is_open()) {
			static_cast<std::ifstream*>(stream)->close();
		}
		if (this->stream) delete static_cast<std::ifstream*>(stream);
	}
};