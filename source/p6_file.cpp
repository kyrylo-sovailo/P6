/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_file.hpp"

#ifdef P6_FILE_WXWIDGETS

	p6::InputFile::InputFile(const String filepath) : _file(filepath, wxFile::OpenMode::read)
	{
	}

	bool p6::InputFile::ok() const noexcept
	{
		return _file.IsOpened();
	}

	void p6::InputFile::read(void *data, uint size)
	{
		_file.Read(data, size);
	}

	p6::OutputFile::OutputFile(const String filepath) : _file(filepath, wxFile::OpenMode::write)
	{
	}

	bool p6::OutputFile::ok() const noexcept
	{
		return _file.IsOpened();
	}

	void p6::OutputFile::write(const void *data, uint size)
	{
		_file.Write(data, size);
	}

#else

	p6::InputFile::InputFile(const String filepath) : _file(filepath)
	{
	}

	bool p6::InputFile::ok() const noexcept
	{
		return _file.is_open();
	}

	void p6::InputFile::read(void *data, uint size)
	{
		_file.read((char*)data, size);
	}

	p6::OutputFile::OutputFile(const String filepath) : _file(filepath)
	{
	}

	bool p6::OutputFile::ok() const noexcept
	{
		return _file.is_open();
	}

	void p6::OutputFile::write(const void *data, uint size)
	{
		_file.write((const char*)data, size);
	}

#endif