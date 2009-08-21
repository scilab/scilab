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

	// 0 UNIX epoch
	// 1 ISO_8601
	int Prefixmode;

	// 0 prefix @ beginning of Input and Output
	// 1 prefix only @ beginning of Input
	// 2 prefix only @ beginning of Output
	// 3 no prefix
	int PrefixIoModeFilter;

	// 0 all (output and input)
	// 1 input
	// 2 output
	int IoModeFilter;

	/*
	* used on Windows to replace carriage return
	*/
	std::wstring replace(std::wstring text, std::wstring s, std::wstring replacement);

public:
	/*
	* constructor
	*/
	Diary(std::wstring _wfilename,int _mode,int ID);

	/*
	* destructor
	*/
	~Diary();

	/*
	* get filename of this diray
	*/
	std::wstring getFilename(void);

	/*
	* get & set IO mode
	*/
	void setIOMode(int _mode);
	int getIOMode(void);

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
	void setPrefixMode(int iPrefixMode);
	int getPrefixMode(void);

	/*
	* get & set IO mode filter
	*/
	void setPrefixIoModeFilter(int mode);
	int getPrefixIoModeFilter(void);
};

#endif /* __DIARY_HXX__ */
/*--------------------------------------------------------------------------*/ 
bool compareDiary(Diary first,Diary second);
/*--------------------------------------------------------------------------*/ 
