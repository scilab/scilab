/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#include "filemanager.hxx"

extern "C"
{
#include "os_string.h"
#include <stdio.h>
}

std::vector<types::File*> FileManager::m_fileList;
int FileManager::m_iCurrentFile = -1;

int FileManager::getFileID(const std::wstring& _stFilename)
{
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()) ; i++)
    {
        if (m_fileList[i] != NULL && m_fileList[i]->getFilename() == _stFilename)
        {
            return i;
        }
    }
    return -1;
}

int FileManager::getFileMaxID()
{
    return static_cast<int>(m_fileList.size());
}

bool FileManager::isOpened(const std::wstring& _stFilename)
{
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()) ; i++)
    {
        if (m_fileList[i] != NULL && m_fileList[i]->getFilename() == _stFilename)
        {
            return true;
        }
    }
    return false;
}

types::File* FileManager::getFile(int _iID)
{
    if (_iID == -1 && m_iCurrentFile == -1)
    {
        return NULL;
    }

    if (_iID == -1 && m_iCurrentFile != -1)
    {
        return m_fileList[m_iCurrentFile];
    }

    if (_iID < static_cast<int>(m_fileList.size()))
    {
        return m_fileList[_iID];
    }

    return NULL;
}

int FileManager::addFile(types::File* _file)
{
    //if already opened, return previous ID
    //if(isOpened(_file->getFilename()) == true)
    //{
    //    int iFile = getFileID(_file->getFilename());
    //        _file->getReal()[0] = iFile;
    //    return iFile;
    //}

    //find first free space
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()); i++)
    {
        if (m_fileList[i] == NULL)
        {
            m_fileList[i] = _file;
            m_iCurrentFile = i;
            return i;
        }
    }

    //no free space, add at the end
    int iNewId = static_cast<int>(m_fileList.size());
    m_fileList.push_back(_file);
    m_iCurrentFile = iNewId;
    return iNewId;
}

int FileManager::getFirstFreeFileID()
{
    //find first free space
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()); i++)
    {
        if (m_fileList[i] == NULL)
        {
            m_iCurrentFile = i;
            return i;
        }
    }

    //no free space, add at the end
    int iNewId = static_cast<int>(m_fileList.size());
    m_fileList.push_back(NULL);
    m_iCurrentFile = iNewId;
    return iNewId;
}

void FileManager::deleteFile(int _iID)
{
    if (0 < _iID && _iID < static_cast<int>(m_fileList.size()))
    {
        delete m_fileList[_iID];
        m_fileList[_iID] = NULL;
        if (m_iCurrentFile == _iID)
        {
            m_iCurrentFile = -1;
        }
    }

    //to clean end of list and remove empty spaces
    while (m_fileList.size() != 0 && m_fileList.back() == NULL)
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
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()); i++)
    {
        if (m_fileList[i] != NULL)
        {
            piIds[iFileIndex++] = i;
        }
    }

    return piIds;
}

int FileManager::getOpenedCount()
{
    int iCount = 0;
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()); i++)
    {
        if (m_fileList[i] != NULL)
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
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()); i++)
    {
        if (m_fileList[i] != NULL)
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

    pstFilenames = (wchar_t**) MALLOC(getOpenedCount() * sizeof(wchar_t*));
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()); i++)
    {
        if (m_fileList[i] != NULL)
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
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()); i++)
    {
        if (m_fileList[i] != NULL)
        {
            pdblModes[iFileIndex++] = (double)(m_fileList[i]->getFileModeAsInt());
        }
    }

    return pdblModes;
}

double* FileManager::getSwaps()
{
    int iFileIndex      = 0;
    double* pdblSwaps   = NULL;

    pdblSwaps = new double[getOpenedCount()];
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()); i++)
    {
        if (m_fileList[i] != NULL)
        {
            pdblSwaps[iFileIndex++] = static_cast<double>(m_fileList[i]->getFileSwap());
        }
    }

    return pdblSwaps;
}

void FileManager::initialize()
{
    types::File* pErr = new types::File();
    pErr->setFileMode(L"wb");
    pErr->setFileDesc(stderr);
    pErr->setFileSwap(0);
    pErr->setFileType(3);
    pErr->setFilename(L"stderr");

    types::File* pIn = new types::File();
    pIn->setFileMode(L"rb");
    pIn->setFileDesc(stdin);
    pIn->setFileSwap(0);
    pIn->setFileType(3);
    pIn->setFilename(L"stdin");

    types::File* pOut = new types::File();
    pOut->setFileMode(L"wb");
    pOut->setFileDesc(stdout);
    pOut->setFileSwap(0);
    pOut->setFileType(3);
    pOut->setFilename(L"stdout");

    //put pErr at position 0
    m_fileList.push_back(pErr);

    //insert free space
    m_fileList.push_back(NULL);
    m_fileList.push_back(NULL);
    m_fileList.push_back(NULL);
    m_fileList.push_back(NULL);

    //put pIn at position 5
    m_fileList.push_back(pIn);
    //put pOut at position 6
    m_fileList.push_back(pOut);
}

void FileManager::destroy()
{
    for (int i = 0 ; i < static_cast<int>(m_fileList.size()); i++)
    {
        if (m_fileList[i] != NULL)
        {
            delete m_fileList[i];
        }
    }

    m_fileList.clear();
}
