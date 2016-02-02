/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "diary_manager.hxx"
#include "DiaryList.hxx"
#include "diary.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
static DiaryList *SCIDIARY = NULL;
/*--------------------------------------------------------------------------*/
static int createDiaryManager(void)
{
    if (SCIDIARY == NULL)
    {
        SCIDIARY = new DiaryList();
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
static int destroyDiaryManager(void)
{
    if (SCIDIARY)
    {
        delete SCIDIARY;
        SCIDIARY = NULL;
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
wchar_t *getDiaryFilename(int _Id)
{
    wchar_t *wcFilename = NULL;
    if (SCIDIARY)
    {
        if (SCIDIARY->getFilename(_Id).compare(L""))
        {
            wcFilename = (wchar_t*) MALLOC(sizeof(wchar_t) * (SCIDIARY->getFilename(_Id).length() + 1));
            if (wcFilename)
            {
                wcscpy(wcFilename, SCIDIARY->getFilename(_Id).c_str());
            }
        }
    }
    return wcFilename;
}
/*--------------------------------------------------------------------------*/
wchar_t **getDiaryFilenames(int *array_size)
{
    *array_size = 0;
    if (SCIDIARY)
    {
        std::list<std::wstring> wstringFilenames = SCIDIARY->getFilenames();
        *array_size = (int)wstringFilenames.size();
        if (array_size > 0)
        {
            wchar_t **wcFilenames = (wchar_t **) MALLOC (sizeof(wchar_t*) * (*array_size));
            int i = 0;
            for (const auto& filename : wstringFilenames)
            {
                wcFilenames[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (filename.length() + 1));
                wcscpy(wcFilenames[i], filename.data());
                ++i;
            }
            return wcFilenames;
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
int *getDiaryIDs(int *array_size)
{
    *array_size = 0;
    if (SCIDIARY)
    {
        int *iIDs = SCIDIARY->getIDs(array_size);
        return iIDs;
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
double *getDiaryIDsAsDouble(int *array_size)
{
    int *iIDs = getDiaryIDs(array_size);
    if (*array_size > 0)
    {
        if (iIDs)
        {
            double *dIDs = new double[*array_size];
            for (int i = 0; i < *array_size; i++)
            {
                dIDs[i] = (double)iIDs[i];
            }
            delete [] iIDs;
            return dIDs;
        }
        else
        {
            *array_size = 0;
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
int diaryCloseAll(void)
{
    destroyDiaryManager();
    return 0;
}
/*--------------------------------------------------------------------------*/
int diaryClose(int _iId)
{
    if (SCIDIARY)
    {
        if (_iId > 0)
        {
            if (SCIDIARY->closeDiary(_iId))
            {
                return 0;
            }
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryClose(wchar_t *filename)
{
    if (SCIDIARY)
    {
        int iID = SCIDIARY->getID(filename);
        if (iID > 0)
        {
            if (SCIDIARY->closeDiary(iID))
            {
                return 0;
            }
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryPauseAll(void)
{
    if (SCIDIARY)
    {
        SCIDIARY->setSuspendWrite(true);
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryPause(int _iId)
{
    if (SCIDIARY)
    {
        SCIDIARY->setSuspendWrite(_iId, true);
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryPause(wchar_t *filename)
{
    if (SCIDIARY)
    {
        int iID = SCIDIARY->getID(std::wstring(filename));
        if (iID != -1)
        {
            SCIDIARY->setSuspendWrite(iID, true);
            return 0;
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryResumeAll(void)
{
    if (SCIDIARY)
    {
        SCIDIARY->setSuspendWrite(false);
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryResume(int _iId)
{
    if (SCIDIARY)
    {
        SCIDIARY->setSuspendWrite(_iId, false);
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryResume(wchar_t *filename)
{
    if (SCIDIARY)
    {
        int iID = SCIDIARY->getID(std::wstring(filename));
        if (iID != -1)
        {
            SCIDIARY->setSuspendWrite(iID, false);
            return 0;
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryExists(int _iId)
{
    if (SCIDIARY)
    {
        if (SCIDIARY->exists(_iId))
        {
            return 0;
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryExists(wchar_t *filename)
{
    if (SCIDIARY)
    {
        if (SCIDIARY->exists(std::wstring(filename)))
        {
            return 0;
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryNew(wchar_t *filename, bool autorename)
{
    createDiaryManager();

    if (SCIDIARY)
    {
        return SCIDIARY->openDiary(std::wstring(filename), autorename);
    }

    return -1;
}
/*--------------------------------------------------------------------------*/
int diaryAppend(wchar_t *filename)
{
    createDiaryManager();
    if (SCIDIARY)
    {
        return SCIDIARY->openDiary(std::wstring(filename), 1, false);
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
int diaryWrite(const wchar_t *wstr, BOOL bInput)
{
    if (SCIDIARY)
    {
        if (bInput)
        {
            SCIDIARY->write(std::wstring(wstr), true);
        }
        else
        {
            SCIDIARY->write(std::wstring(wstr), false);
        }
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryWriteln(const wchar_t *wstr, BOOL bInput)
{
    if (SCIDIARY)
    {
        if (bInput)
        {
            SCIDIARY->writeln(std::wstring(wstr), true);
        }
        else
        {
            SCIDIARY->writeln(std::wstring(wstr), false);
        }
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diarySetFilterMode(int _iId, diary_filter mode)
{
    if (SCIDIARY)
    {
        SCIDIARY->setFilterMode(_iId, mode);
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diarySetPrefixMode(int ID_diary, diary_prefix_time_format iPrefixMode)
{
    if (SCIDIARY)
    {
        SCIDIARY->setPrefixMode(ID_diary, iPrefixMode);
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryGetPrefixMode(int ID_diary)
{
    if (SCIDIARY)
    {
        return SCIDIARY->getPrefixMode(ID_diary);
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
int diarySetPrefixIoModeFilter(int ID_diary, diary_prefix_time_filter mode)
{
    if (SCIDIARY)
    {
        SCIDIARY->setPrefixIoModeFilter(ID_diary, mode);
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
diary_prefix_time_filter diaryGetPrefixIoModeFilter(int ID_diary)
{
    if (SCIDIARY)
    {
        return SCIDIARY->getPrefixIoModeFilter(ID_diary);
    }
    return PREFIX_FILTER_ERROR;
}
/*--------------------------------------------------------------------------*/
