/*--------------------------------------------------------------------------*/ 
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#include <fstream> 
#include <iostream> 
#include "Diary.hxx"
#include "getFullFilename.hxx"
#include "getDiaryDate.hxx"
extern "C"
{
#include "charEncoding.h"
#include "MALLOC.h"
}
/*--------------------------------------------------------------------------*/ 
Diary::Diary(std::wstring _wfilename,int _mode,int ID, bool autorename)
{
	std::ios::openmode wofstream_mode = std::ios::trunc | std::ios::binary;

	std::wstring fullfilename = getUniqueFilename(_wfilename);
	if (autorename)
	{
		fullfilename = getUniqueFilename(_wfilename);
	}
	else
	{
		fullfilename = getFullFilename(_wfilename);
	}


	suspendwrite = false;

	PrefixTimeFormat = PREFIX_TIME_FORMAT_UNIX_EPOCH;
	IoModeFilter = DIARY_FILTER_INPUT_AND_OUTPUT; // default command & input
	PrefixIoModeFilter = PREFIX_FILTER_NONE; // no prefix

	if (_mode == 0)
	{
		wofstream_mode = std::ios::trunc | std::ios::binary ;
	}
	else
	{
		wofstream_mode = std::ios::app | std::ios::binary ;
	}

#ifdef _MSC_VER
	std::wofstream fileDiary(fullfilename.c_str(),wofstream_mode);
#else
	wchar_t *wcfile = (wchar_t*)fullfilename.c_str();
	char *filename = wide_string_to_UTF8(wcfile);

	std::ofstream fileDiary(filename, wofstream_mode);

	if (filename) {FREE(filename); filename = NULL;}
#endif

	if ( fileDiary.bad())
	{
		wfilename = std::wstring(L"");
		fileAttribMode = -1;
		setID(-1);
	}
	else
	{
		wfilename = fullfilename;
		fileAttribMode = wofstream_mode;
		setID(ID);
	}
	fileDiary.close();
}
/*--------------------------------------------------------------------------*/ 
Diary::~Diary()
{
	wfilename = std::wstring(L"");
	fileAttribMode = -1;
	setID(-1);
}
/*--------------------------------------------------------------------------*/ 
std::wstring Diary::getFilename(void)
{
	return wfilename;
}
/*--------------------------------------------------------------------------*/ 
void Diary::write(std::wstring _wstr, bool bInput)
{
	if (!suspendwrite)
	{
		std::ios::openmode wofstream_mode = std::ios::app | std::ios::binary;

#ifdef _MSC_VER
		std::wofstream fileDiary(wfilename.c_str(), wofstream_mode );
#else
		wchar_t *wcfile = (wchar_t*)wfilename.c_str();
		char *filename = wide_string_to_UTF8(wcfile);
		std::ofstream fileDiary(filename, wofstream_mode);
		if (filename) {FREE(filename); filename = NULL;}
#endif

		if (fileDiary.good())
		{
			char *line = NULL;

#ifdef _MSC_VER
			/* carriage return for Windows */
			_wstr = replace(_wstr, std::wstring(L"\n"), std::wstring(L"\r\n"));
			_wstr = replace(_wstr, std::wstring(L"\r\r"), std::wstring(L"\r"));
#endif
			line = wide_string_to_UTF8((wchar_t*)_wstr.c_str());

			if (bInput) // input
			{
				if ( (IoModeFilter == DIARY_FILTER_INPUT_AND_OUTPUT) || (IoModeFilter == DIARY_FILTER_ONLY_INPUT) )
				{
					if ( (PrefixIoModeFilter == PREFIX_FILTER_INPUT_AND_OUTPUT) || (PrefixIoModeFilter == PREFIX_FILTER_ONLY_INPUT) )
					{
						char *timeInfo = wide_string_to_UTF8((wchar_t*)getDiaryDate(PrefixTimeFormat).c_str());
						if (timeInfo) 
						{
							fileDiary << timeInfo << " ";
							FREE(timeInfo); timeInfo = NULL;
						}
					}
					if (line) fileDiary << line;
				}
			}
			else // output
			{
				if ( (IoModeFilter == DIARY_FILTER_INPUT_AND_OUTPUT) || (IoModeFilter == DIARY_FILTER_ONLY_OUTPUT) )
				{
					if ( (PrefixIoModeFilter == PREFIX_FILTER_INPUT_AND_OUTPUT) || (PrefixIoModeFilter == PREFIX_FILTER_ONLY_OUTPUT) )
					{
						char *timeInfo = wide_string_to_UTF8((wchar_t*)getDiaryDate(PrefixTimeFormat).c_str());
						if (timeInfo) 
						{
							fileDiary << timeInfo << " ";
							FREE(timeInfo); timeInfo = NULL;
						}
					}
					if (line) fileDiary << line;
				}
			}

			if (line) {FREE(line); line = NULL;}
		}
		fileDiary.close();
	}
}
/*--------------------------------------------------------------------------*/ 
void Diary::writeln(std::wstring _wstr, bool bInput)
{
	#define ENDLINE L"\n"
	write(_wstr.append(ENDLINE),bInput);
}
/*--------------------------------------------------------------------------*/ 
int Diary::getID(void)
{
	return ID_foutstream;
}
/*--------------------------------------------------------------------------*/ 
void Diary::setID(int _ID)
{
	ID_foutstream = _ID;
}
/*--------------------------------------------------------------------------*/ 
bool Diary::setSuspendWrite(bool bWith)
{
	bool previous =  suspendwrite;
	suspendwrite = bWith;
	return previous;
}
/*--------------------------------------------------------------------------*/ 
bool Diary::getSuspendWrite(void)
{
	return suspendwrite;
}
/*--------------------------------------------------------------------------*/ 
std::wstring Diary::replace(std::wstring text, std::wstring s, std::wstring replacement)
{
	std::wstring::size_type pos = 0;

	while (pos != std::wstring::npos) 
	{
		pos = text.find(s, pos);
		if (pos == std::wstring::npos)
			// no more 's' in '*this'
			break;

		text.replace(pos, s.length(), replacement);
		pos += replacement.length();
	} 
	return text;
}
/*--------------------------------------------------------------------------*/ 
diary_filter Diary::getIOMode(void)
{
	return IoModeFilter;
}
/*--------------------------------------------------------------------------*/ 
void Diary::setIOMode(diary_filter _mode)
{
	IoModeFilter = _mode;
}
/*--------------------------------------------------------------------------*/ 
void Diary::setPrefixMode(diary_prefix_time_format iPrefixTimeFormat)
{
	PrefixTimeFormat = iPrefixTimeFormat;
}
/*--------------------------------------------------------------------------*/ 
void Diary::setPrefixIoModeFilter(diary_prefix_time_filter mode)
{
	PrefixIoModeFilter = mode;
}
/*--------------------------------------------------------------------------*/ 
diary_prefix_time_format Diary::getPrefixMode(void)
{
	return PrefixTimeFormat;
}
/*--------------------------------------------------------------------------*/ 
diary_prefix_time_filter Diary::getPrefixIoModeFilter(void)
{
	return PrefixIoModeFilter;
}
/*--------------------------------------------------------------------------*/ 
bool compareDiary(Diary first,Diary second)
{
	if (first.getID() < second.getID()) return true;
	else return false;
}
/*--------------------------------------------------------------------------*/ 
