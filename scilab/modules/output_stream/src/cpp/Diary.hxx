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
#ifndef __DIARY_HXX__
#define __DIARY_HXX__
/*--------------------------------------------------------------------------*/ 
#include <string>
#include "DiaryModesEnum.hxx"
/*--------------------------------------------------------------------------*/ 
class Diary
{
private:
	std::wstring wfilename;
	int ID_foutstream;

	int fileAttribMode;

	void setID(int _ID);

	/* resume mode */
	bool suspendwrite;

	/* see DiaryModesEnum.hxx */
	diary_prefix_time_format PrefixTimeFormat;

	/* see DiaryModesEnum.hxx */
	diary_prefix_time_filter PrefixIoModeFilter;

	/* see DiaryModesEnum.hxx */
	diary_filter IoModeFilter;

	/*
	* used on Windows to replace carriage return
	*/
	std::wstring replace(std::wstring text, std::wstring s, std::wstring replacement);

public:
	/*
	* constructor
	*/
	Diary(std::wstring _wfilename,int _mode,int ID, bool autorename);

	/*
	* destructor
	*/
	~Diary();

	/*
	* get filename of this diary
	*/
	std::wstring getFilename(void);

	/*
	* get & set IO mode
	*/
	void setIOMode(diary_filter _mode);
	diary_filter getIOMode(void);

	/*
	* get ID of this diary
	*/
	int getID(void);

	/*
	* write a string in this diary
	*/
	void write(std::wstring _wstr, bool bInput);

	/*
	* write a line in this diary
	*/
	void writeln(std::wstring _wstr, bool bInput);

	/*
	* get & set Resume mode 
	*/
	bool setSuspendWrite(bool bWith);
	bool getSuspendWrite(void);

	/*
	* get & set Prefix mode
	*/
	void setPrefixMode(diary_prefix_time_format iPrefixTimeFormat);
	diary_prefix_time_format getPrefixMode(void);

	/*
	* get & set IO mode filter
	*/
	void setPrefixIoModeFilter(diary_prefix_time_filter mode);
	diary_prefix_time_filter getPrefixIoModeFilter(void);
};

#endif /* __DIARY_HXX__ */
/*--------------------------------------------------------------------------*/ 
bool compareDiary(Diary first,Diary second);
/*--------------------------------------------------------------------------*/ 
