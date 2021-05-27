#pragma once
#include <iostream>
#include <cassert>


class invalid_array_size final : public std::exception
{
private:
	std::string error_text_;

public:
	explicit invalid_array_size(std::string error_text) : error_text_{ std::move(error_text) } {}

	[[nodiscard]] const char* what() const override
	{
		return this->error_text_.c_str();
	}
};


template <typename T>
class array
{
private:
	T* data_;
	int length_{};

public:
	explicit array()
	{
		length_ = 0;
		data_ = nullptr;
	}
	
	explicit array(const int length, const bool clear = true)
	{
		if (length < 0) 
		{
			throw invalid_array_size("Please provide correct array length");
		}

		length_ = length;
		data_ = new T[length];

		if (clear) 
		{
			for (int i = 0; i < length; i++) 
			{
				data_[i] = 0;
			}
		}
	}

	explicit array(std::initializer_list<T> list)
	{
		length_ = list.size();
		data_ = new T[length_];

		for (int i = 0; T e : list)
		{
			data_[i] = e;
			i++;
		}
	}

	~array()
	{
		//delete[] data_;
	}

	void erase()
	{
		delete[] data_;
		data_ = nullptr;
		length_ = 0;
	}

	array<T>* copy()
	{
		auto new_arr = new array<T>(this->get_length());

		for (int i = 0; i < this->get_length(); i++)
		{
			(*new_arr)[i] = (*this)[i];
		}

		return new_arr;
	}

	T& operator[](const int index)
	{
		assert(index >= 0 && index < length_);
		return (this->data_[index]);
	}

	const T& operator[](const int index) const
	{
		assert(index >= 0 && index < length_);
		return (this->data_[index]);
	}

	[[nodiscard]] int get_length() const
	{
		return length_;
	}

	void print(std::ostream& stream = std::cout, std::string sep = " ", std::string end = "")
	{
		for (int i = 0; i < length_-1; i++) {
			stream << this->data_[i] << sep;
		}
		stream << this->data_[length_ - 1] << end;
	}

	array<T>& operator=(const array<T> other)
	{
		this->length_ = other.get_length();

		this->data_ = new T[length_];

		for (int i = 0; i < other.get_length(); i++)
		{
			this->data_[i] = other.data_[i];
		}

		return *this;
	}
};
