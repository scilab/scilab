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
#ifndef __DIARYLIST_HXX__
#define __DIARYLIST_HXX__
/*--------------------------------------------------------------------------*/ 
#include <string>
#include <list>
#include "Diary.hxx"
/*--------------------------------------------------------------------------*/ 
class DiaryList
{
public:
	DiaryList();
	~DiaryList();

	int openDiary(std::wstring _wfilename,int _mode);
	int openDiary(std::wstring _wfilename);

	bool closeDiary(int ID_diary);
	void closeAllDiaries(void);

	int * getIDs(int *sizeID);
	std::wstring * getFilenames(int *sizeFilenames);

	std::wstring getFilename(int ID_diary);

	void write(std::wstring _wstr, bool bInput);
	void writeln(std::wstring _wstr, bool bInput);

	bool exists(int ID_diary);
	bool exists(std::wstring _wfilename);

	void setSuspendWrite(bool bWith);
	bool *getSuspendWrite(int *sizeboolArray);

	void setSuspendWrite(int ID_diary, bool bWith);
	bool getSuspendWrite(int ID_diary);

	int getID(std::wstring _wfilename);

	void setFilterMode(int ID_diary, int mode);

	void setPrefixMode(int ID_diary,int iPrefixMode);
	int getPrefixMode(int ID_diary);

	void setPrefixIoModeFilter(int ID_diary,int mode);
	int getPrefixIoModeFilter(int ID_diary);


private:
	std::list<Diary> LSTDIARY;
	int findFreeID(void);
};
#endif /* __DIARYLIST_HXX__ */
/*--------------------------------------------------------------------------*/ 

