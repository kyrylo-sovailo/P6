/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_FILE
#define P6_FILE

#include "p6_common.hpp"
#ifdef P6_FILE_WXWIDGETS
	#include <wx/file.h>
#else
	#include <fstream>
#endif

namespace p6
{
	///File for reading binary data
	class InputFile
	{
	private:
		#ifdef P6_FILE_WXWIDGETS
			wxFile _file;						///<wxWidgets's file
		#else
			std::ifstream _file;				///<C++ input file stream
		#endif
		
	public:
		InputFile(const String filepath);		///<Opens file for reading
		bool ok() const noexcept;				///<Gets if file if ok
		void read(void *data, uint size);		///<Reads data drom file
	};
	
	///File for writing binary data
	class OutputFile
	{
	private:
		#ifdef P6_FILE_WXWIDGETS
			wxFile _file;						///<wxWidgets's file
		#else
			std::ofstream _file;				///<C++ input file stream
		#endif
		
	public:
		OutputFile(const String filepath);		///<Opens file for writing
		bool ok() const noexcept;				///<Gets if file if ok
		void write(const void *data, uint size);///<Writes data to file
	};
}

#endif