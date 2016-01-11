/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
char* getDiaryFilename(int _Id)
{
    char* filename = NULL;
    if (SCIDIARY)
    {
        if (SCIDIARY->getFilename(_Id).compare(""))
        {
            filename = (char*)MALLOC(sizeof(char) * (SCIDIARY->getFilename(_Id).length() + 1));
            if (filename)
            {
                strcpy(filename, SCIDIARY->getFilename(_Id).c_str());
            }
        }
    }
    return filename;
}
/*--------------------------------------------------------------------------*/
char **getDiaryFilenames(int *array_size)
{
    *array_size = 0;
    if (SCIDIARY)
    {
        std::list<std::string> stringFilenames = SCIDIARY->getFilenames();
        *array_size = (int)stringFilenames.size();
        if (array_size > 0)
        {
            char **filenames = (char **) MALLOC (sizeof(char*) * (*array_size));
            int i = 0;
            for (const auto& filename : stringFilenames)
            {
                filenames[i] = (char*)MALLOC(sizeof(char) * (filename.length() + 1));
                strcpy(filenames[i], filename.data());
                ++i;
            }
            return filenames;
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
int diaryClose(const char *filename)
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
int diaryPause(const char *filename)
{
    if (SCIDIARY)
    {
        int iID = SCIDIARY->getID(filename);
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
int diaryResume(const char *filename)
{
    if (SCIDIARY)
    {
        int iID = SCIDIARY->getID(filename);
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
int diaryExists(const char *filename)
{
    if (SCIDIARY)
    {
        if (SCIDIARY->exists(filename))
        {
            return 0;
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryNew(const char *filename, bool autorename)
{
    createDiaryManager();

    if (SCIDIARY)
    {
        return SCIDIARY->openDiary(filename, autorename);
    }

    return -1;
}
/*--------------------------------------------------------------------------*/
int diaryAppend(const char *filename)
{
    createDiaryManager();
    if (SCIDIARY)
    {
        return SCIDIARY->openDiary(filename, 1, false);
    }
    return -1;
}
/*--------------------------------------------------------------------------*/
int diaryWrite(const char *str, BOOL bInput)
{
    if (SCIDIARY)
    {
        if (bInput)
        {
            SCIDIARY->write(str, true);
        }
        else
        {
            SCIDIARY->write(str, false);
        }
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
int diaryWriteln(const char *str, BOOL bInput)
{
    if (SCIDIARY)
    {
        if (bInput)
        {
            SCIDIARY->writeln(str, true);
        }
        else
        {
            SCIDIARY->writeln(str, false);
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
