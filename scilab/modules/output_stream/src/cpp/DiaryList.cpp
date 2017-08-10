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
#include "DiaryList.hxx"

extern "C"
{
#include "getFullFilename.h"
#include "sci_malloc.h"
}
/*--------------------------------------------------------------------------*/
DiaryList::DiaryList()
{
}
/*--------------------------------------------------------------------------*/
DiaryList::~DiaryList()
{
    closeAllDiaries();
}
/*--------------------------------------------------------------------------*/
int DiaryList::openDiary(const std::wstring& _wfilename, bool autorename)
{
    return openDiary(_wfilename, 0, autorename);
}
/*--------------------------------------------------------------------------*/
int DiaryList::openDiary(const std::wstring& _wfilename, int _mode, bool autorename)
{
    int ID = -1;
    Diary newDiary(_wfilename, _mode, findFreeID(), autorename);
    if (newDiary.getID() != -1)
    {
        LSTDIARY.push_back(newDiary);
        ID = newDiary.getID();
    }
    return ID;
}
/*--------------------------------------------------------------------------*/
bool DiaryList::closeDiary(int ID_diary)
{
    std::list<Diary>::iterator iter = LSTDIARY.begin();
    while (iter != LSTDIARY.end())
    {
        if (iter->getID() == ID_diary)
        {
            LSTDIARY.erase(iter++);
            return true;
        }
        else
        {
            iter++;
        }
    }
    return false;
}
/*--------------------------------------------------------------------------*/
void DiaryList::closeAllDiaries(void)
{
    LSTDIARY.clear();
}
/*--------------------------------------------------------------------------*/
int * DiaryList::getIDs(int *sizeID)
{
    int *IDs  = NULL;
    *sizeID = 0;

    LSTDIARY.sort(compareDiary);

    *sizeID = (int)LSTDIARY.size();

    if (*sizeID > 0)
    {
        std::list<Diary>::iterator iter;
        int i = 0;
        IDs = new int[*sizeID];
        for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
        {
            IDs[i++] = iter->getID();
        }
    }
    return IDs;
}
/*--------------------------------------------------------------------------*/
std::wstring DiaryList::getFilename(int ID_diary)
{
    std::wstring wFilename(L"");
    std::list<Diary>::iterator i;
    for ( i = LSTDIARY.begin(); i != LSTDIARY.end(); i++)
    {
        if ( i->getID() == ID_diary )
        {
            wFilename = i->getFilename();
        }
    }
    return wFilename;
}
/*--------------------------------------------------------------------------*/
std::list<std::wstring> DiaryList::getFilenames()
{
    LSTDIARY.sort(compareDiary);
    int size = (int)LSTDIARY.size();
    std::list<std::wstring> wFilenames(size);

    if (size > 0)
    {
        for (auto& iter : LSTDIARY)
        {
            wFilenames.push_back(iter.getFilename());
        }
    }
    return wFilenames;
}
/*--------------------------------------------------------------------------*/
void DiaryList::write(const std::wstring& _wstr, bool bInput)
{
    std::list<Diary>::iterator i;
    for ( i = LSTDIARY.begin(); i != LSTDIARY.end(); i++)
    {
        i->write(_wstr, bInput);
    }
}
/*--------------------------------------------------------------------------*/
void DiaryList::writeln(const std::wstring& _wstr, bool bInput)
{
    std::list<Diary>::iterator i;
    for ( i = LSTDIARY.begin(); i != LSTDIARY.end(); i++)
    {
        i->writeln(_wstr, bInput);
    }
}
/*--------------------------------------------------------------------------*/
bool DiaryList::exists(int ID_diary)
{
    std::list<Diary>::iterator i;
    for ( i = LSTDIARY.begin(); i != LSTDIARY.end(); i++)
    {
        if ( i->getID() == ID_diary )
        {
            return true;
        }
    }
    return false;
}
/*--------------------------------------------------------------------------*/
bool DiaryList::exists(const std::wstring& _wfilename)
{
    std::list<Diary>::iterator i;
    for ( i = LSTDIARY.begin(); i != LSTDIARY.end(); i++)
    {
        wchar_t* wc = getFullFilenameW(_wfilename.data());
        int comp = i->getFilename().compare(wc);
        FREE(wc);
        if (comp == 0)
        {
            return true;
        }
    }
    return false;
}
/*--------------------------------------------------------------------------*/
int DiaryList::findFreeID(void)
{
    int freeID = 1;
    std::list<Diary>::iterator i;

    LSTDIARY.sort(compareDiary);

    for ( i = LSTDIARY.begin(); i != LSTDIARY.end(); i++)
    {
        if ( freeID >= i->getID() )
        {
            freeID++;
        }
    }
    return freeID;
}
/*--------------------------------------------------------------------------*/
void DiaryList::setSuspendWrite(bool bWith)
{
    std::list<Diary>::iterator iter;

    for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
    {
        iter->setSuspendWrite(bWith);
    }
}
/*--------------------------------------------------------------------------*/
bool *DiaryList::getSuspendWrite(int *sizeboolArray)
{
    bool *bSuspend = NULL;
    *sizeboolArray = 0;

    LSTDIARY.sort(compareDiary);

    *sizeboolArray = (int)LSTDIARY.size();
    if (*sizeboolArray > 0)
    {
        int i = 0;
        bSuspend = new bool[*sizeboolArray];
        std::list<Diary>::iterator iter;
        for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
        {
            bSuspend[i++] = iter->getSuspendWrite();
        }
    }
    return bSuspend;
}
/*--------------------------------------------------------------------------*/
void DiaryList::setSuspendWrite(int  ID_diary, bool bWith)
{
    std::list<Diary>::iterator iter;

    for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
    {
        if (iter->getID() == ID_diary)
        {
            iter->setSuspendWrite(bWith);
        }
    }
}
/*--------------------------------------------------------------------------*/
bool DiaryList::getSuspendWrite(int  ID_diary)
{
    std::list<Diary>::iterator iter;

    for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
    {
        if (iter->getID() == ID_diary)
        {
            return iter->getSuspendWrite();
        }
    }
    return false;
}
/*--------------------------------------------------------------------------*/
int DiaryList::getID(const std::wstring& _wfilename)
{
    std::list<Diary>::iterator i;
    for ( i = LSTDIARY.begin(); i != LSTDIARY.end(); i++)
    {
        wchar_t* ws = getFullFilenameW(_wfilename.data());
        int comp = i->getFilename().compare(ws);
        FREE(ws);
        if (comp == 0)
        {
            return i->getID();
        }
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
void DiaryList::setFilterMode(int ID_diary, diary_filter mode)
{
    std::list<Diary>::iterator iter;

    for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
    {
        if (iter->getID() == ID_diary)
        {
            return iter->setIOMode(mode);
        }
    }
}
/*--------------------------------------------------------------------------*/
void DiaryList::setPrefixMode(int ID_diary, diary_prefix_time_format iPrefixMode)
{
    std::list<Diary>::iterator iter;

    for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
    {
        if (iter->getID() == ID_diary)
        {
            iter->setPrefixMode(iPrefixMode);
            break;
        }
    }
}
/*--------------------------------------------------------------------------*/
diary_prefix_time_format DiaryList::getPrefixMode(int ID_diary)
{
    std::list<Diary>::iterator iter;

    for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
    {
        if (iter->getID() == ID_diary)
        {
            return iter->getPrefixMode();
        }
    }
    return PREFIX_TIME_FORMAT_ERROR;
}
/*--------------------------------------------------------------------------*/
void DiaryList::setPrefixIoModeFilter(int ID_diary, diary_prefix_time_filter mode)
{
    std::list<Diary>::iterator iter;

    for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
    {
        if (iter->getID() == ID_diary)
        {
            iter->setPrefixIoModeFilter(mode);
            break;
        }
    }
}
/*--------------------------------------------------------------------------*/
diary_prefix_time_filter DiaryList::getPrefixIoModeFilter(int ID_diary)
{
    std::list<Diary>::iterator iter;

    for ( iter = LSTDIARY.begin(); iter != LSTDIARY.end(); iter++)
    {
        if (iter->getID() == ID_diary)
        {
            return iter->getPrefixIoModeFilter();
        }
    }
    return PREFIX_FILTER_ERROR;
}
/*--------------------------------------------------------------------------*/

