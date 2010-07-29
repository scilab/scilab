/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "filemanager.hxx"
#include "os_wcsdup.h"

std::vector<types::File*> FileManager::m_fileList;
int FileManager::m_iCurrentFile = -1;

int FileManager::getFileID(wstring _stFilename)
{
    for(int i = 0 ; i < m_fileList.size() ; i++)
    {
        if(m_fileList[i]->getFilename() == _stFilename)
        {
            return i + 1;
        }
    }
    return -1;
}

int FileManager::getFileMaxID()
{
    return static_cast<int>(m_fileList.size());
}

bool FileManager::isOpened(wstring _stFilename)
{
    for(int i = 0 ; i < m_fileList.size() ; i++)
    {
        if(m_fileList[i]->getFilename() == _stFilename)
        {
            return true;
        }
    }
    return false;
}

types::File* FileManager::getFile(int _iID)
{
    if(_iID == -1 && m_iCurrentFile != -1)
    {
        return m_fileList[m_iCurrentFile - 1];
    }

    if(_iID <= m_fileList.size())
    {//1-indexed
        return m_fileList[_iID - 1];
    }

    return NULL;
}

int FileManager::addFile(types::File* _file)
{
    //if already opened, return previous ID
    //if(isOpened(_file->getFilename()) == true)
    //{
    //    int iFile = getFileID(_file->getFilename());
    //        _file->real_get()[0] = iFile;
    //    return iFile;
    //}

    //find first free space
    for(int i = 0 ; i < m_fileList.size() ; i++)
    {
        if(m_fileList[i] == NULL)
        {
            m_fileList[i] = _file;
            int iNewId = i + 1;
            m_iCurrentFile = iNewId;
            return iNewId;
        }
    }

    //no free space, add at the end
    m_fileList.push_back(_file);
    int iNewId = static_cast<int>(m_fileList.size());
    m_iCurrentFile = iNewId;
    return iNewId;
}

void FileManager::deleteFile(int _iID)
{
    if(_iID <= m_fileList.size())
    {//1-indexed
        //do not delete File object !!!
        //delete m_fileList[_iID - 1];
        m_fileList[_iID - 1] = NULL;
        if(m_iCurrentFile == _iID)
        {
            m_iCurrentFile = -1;
        }
    }

    //to clean end of list and remove empty spaces
    while(m_fileList.size() != 0 && m_fileList.back() == NULL)
    {
        m_fileList.pop_back();
    }
}

int FileManager::getCurrentFile()
{
    return m_iCurrentFile;
}

int* FileManager::getIDs()
{
    int iFileIndex  = 0;
    int* piIds       = NULL;

    piIds = new int[getOpenedCount()];
    for(int i = 0 ; i < m_fileList.size() ; i++)
    {
        if(m_fileList[i] != NULL)
        {
            piIds[iFileIndex++] = i + 1;
        }
    }

    return piIds;
}

int FileManager::getOpenedCount()
{
    int iCount = 0;
    for(int i = 0 ; i < m_fileList.size() ; i++)
    {
        if(m_fileList[i] != NULL)
        {
            iCount++;
        }
    }
    return iCount;
}

wchar_t** FileManager::getTypesAsString()
{
    int iFileIndex      = 0;
    wchar_t** pstTypes  = NULL;

    pstTypes = new wchar_t*[getOpenedCount()];
    for(int i = 0 ; i < m_fileList.size() ; i++)
    {
        if(m_fileList[i] != NULL)
        {
            pstTypes[iFileIndex++] = os_wcsdup(m_fileList[i]->getFileTypeAsString().c_str());
        }
    }

    return pstTypes;
}

wchar_t** FileManager::getFilenames()
{
    int iFileIndex          = 0;
    wchar_t** pstFilenames  = NULL;

    pstFilenames = new wchar_t*[getOpenedCount()];
    for(int i = 0 ; i < m_fileList.size() ; i++)
    {
        if(m_fileList[i] != NULL)
        {
            pstFilenames[iFileIndex++] = os_wcsdup(m_fileList[i]->getFilename().c_str());
        }
    }

    return pstFilenames;
}

double* FileManager::getModes()
{
    int iFileIndex      = 0;
    double* pdblModes   = NULL;

    pdblModes = new double[getOpenedCount()];
    for(int i = 0 ; i < m_fileList.size() ; i++)
    {
        if(m_fileList[i] != NULL)
        {
            pdblModes[iFileIndex++] = m_fileList[i]->getFileModeAsDouble();
        }
    }

    return pdblModes;
}

double* FileManager::getSwaps()
{
    int iFileIndex      = 0;
    double* pdblSwaps   = NULL;

    pdblSwaps = new double[getOpenedCount()];
    for(int i = 0 ; i < m_fileList.size() ; i++)
    {
        if(m_fileList[i] != NULL)
        {
            pdblSwaps[iFileIndex++] = static_cast<double>(m_fileList[i]->getFileSwap());
        }
    }

    return pdblSwaps;
}

