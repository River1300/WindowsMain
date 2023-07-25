#pragma once

#include <exception>
#include <Windows.h>
#include <stdio.h>
#include <string>

class com_exception : public std::exception
{
	HRESULT result;
	std::string description;

public:
	com_exception(HRESULT hr) : result(hr), description{} {}
	com_exception(HRESULT hr, std::string msg) : result(hr), description{ msg }{}

	virtual const char* what() const override
	{
		static char str[1024] = {};

		if (description.length() == 0)
		{
			sprintf_s(
				str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(result)
			);
		}
		else
		{
			sprintf_s(
				str, "Failure with HRESULT of %08X\n\n%s",
				static_cast<unsigned int>(result), description.c_str()
			);
		}
		return str;
	}
};

inline void ThrowIfFailed(HRESULT hr, std::string msg = "")
{
	if (FAILED(hr)) throw com_exception(hr, msg);
}