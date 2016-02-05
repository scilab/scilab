/*--------------------------------------------------------------------------*/
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __DIARYLIST_HXX__
#define __DIARYLIST_HXX__
/*--------------------------------------------------------------------------*/
#include <string>
#include <list>
#include "Diary.hxx"
#include "DiaryModesEnum.hxx"
/*--------------------------------------------------------------------------*/
class DiaryList
{
public:
    /* constructor */
    DiaryList();

    /* destructor */
    ~DiaryList();

    /* open a diary */
    int openDiary(const std::wstring& _wfilename, int _mode, bool autorename);
    int openDiary(const std::wstring& _wfilename, bool autorename);

    /* close a diary by ID */
    bool closeDiary(int ID_diary);

    /* close all diaries */
    void closeAllDiaries(void);

    /* get IDs of diaries */
    int * getIDs(int *sizeID);

    /* get filenames of diaries */
    std::list<std::wstring> getFilenames();

    /* get filename by IDs */
    std::wstring getFilename(int ID_diary);

    /* write string in diaries */
    void write(const std::wstring& _wstr, bool bInput);
    void writeln(const std::wstring& _wstr, bool bInput);

    /* test if exists a diary by ID */
    bool exists(int ID_diary);

    /* test if exists a diary by filename */
    bool exists(const std::wstring& _wfilename);

    /* set & get Resume mode */
    void setSuspendWrite(bool bWith);
    bool *getSuspendWrite(int *sizeboolArray);

    void setSuspendWrite(int ID_diary, bool bWith);
    bool getSuspendWrite(int ID_diary);

    /* get ID by filename */
    int getID(const std::wstring& _wfilename);

    /* set & get mode */
    void setFilterMode(int ID_diary, diary_filter mode);

    void setPrefixMode(int ID_diary, diary_prefix_time_format iPrefixMode);
    diary_prefix_time_format getPrefixMode(int ID_diary);

    void setPrefixIoModeFilter(int ID_diary, diary_prefix_time_filter mode);
    diary_prefix_time_filter getPrefixIoModeFilter(int ID_diary);

private:
    /* list of diary */
    std::list<Diary> LSTDIARY;

    /* find a ID free */
    int findFreeID(void);
};
#endif /* __DIARYLIST_HXX__ */
/*--------------------------------------------------------------------------*/

