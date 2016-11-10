/*--------------------------------------------------------------------------*/
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <fstream>
#include <iostream>
#include "Diary.hxx"
#include "getDiaryDate.hxx"
#include "getUniqueFilename.hxx"

extern "C"
{
#include "getFullFilename.h"
#include "charEncoding.h"
#include "sci_malloc.h"
}
/*--------------------------------------------------------------------------*/
Diary::Diary(const std::string& _filename, int _mode, int ID, bool autorename)
{
    std::ios::openmode ofstream_mode;

    std::string fullfilename = getUniqueFilename(_filename);
    if (autorename)
    {
        fullfilename = getUniqueFilename(_filename);

        char* s = getFullFilename(fullfilename.data());
        fullfilename = s;
        FREE(s);
    }
    else
    {
        char* s = getFullFilename(_filename.data());
        fullfilename = s;
        FREE(s);
    }

    suspendwrite = false;

    PrefixTimeFormat = PREFIX_TIME_FORMAT_UNIX_EPOCH;
    IoModeFilter = DIARY_FILTER_INPUT_AND_OUTPUT;   // default command & input
    PrefixIoModeFilter = PREFIX_FILTER_NONE;    // no prefix

    if (_mode == 0)
    {
        ofstream_mode = std::ios::trunc | std::ios::binary;
    }
    else
    {
        ofstream_mode = std::ios::app | std::ios::binary;
    }

    std::ofstream fileDiary(fullfilename, ofstream_mode);

    if (fileDiary.bad())
    {
        filename = std::string("");
        fileAttribMode = -1;
        setID(-1);
    }
    else
    {
        filename = fullfilename;
        fileAttribMode = ofstream_mode;
        setID(ID);
    }
    fileDiary.close();
}

/*--------------------------------------------------------------------------*/
Diary::~Diary()
{
    filename = "";
    fileAttribMode = -1;
    setID(-1);
}

/*--------------------------------------------------------------------------*/
std::string Diary::getFilename(void)
{
    return filename;
}

/*--------------------------------------------------------------------------*/
void Diary::write(const std::string& _str, bool bInput)
{
    if (!suspendwrite)
    {
        std::ios::openmode ofstream_mode = std::ios::app | std::ios::binary;

        std::ofstream fileDiary(filename.c_str(), ofstream_mode);
        if (fileDiary.good())
        {
            const char *line = NULL;
            std::string st = _str;
#ifdef _MSC_VER
            /* carriage return for Windows */
            st = replace(st, "\n", "\r\n");
            st = replace(st, "\r\r", "\r");
#endif
            line = st.data();

            if (bInput)         // input
            {
                if ((IoModeFilter == DIARY_FILTER_INPUT_AND_OUTPUT) || (IoModeFilter == DIARY_FILTER_ONLY_INPUT))
                {
                    if ((PrefixIoModeFilter == PREFIX_FILTER_INPUT_AND_OUTPUT) || (PrefixIoModeFilter == PREFIX_FILTER_ONLY_INPUT))
                    {
                        fileDiary << getDiaryDate(PrefixTimeFormat) << " ";
                    }
                    if (line)
                    {
                        fileDiary << line;
                    }
                }
            }
            else                // output
            {
                if ((IoModeFilter == DIARY_FILTER_INPUT_AND_OUTPUT) || (IoModeFilter == DIARY_FILTER_ONLY_OUTPUT))
                {
                    if ((PrefixIoModeFilter == PREFIX_FILTER_INPUT_AND_OUTPUT) || (PrefixIoModeFilter == PREFIX_FILTER_ONLY_OUTPUT))
                    {
                        fileDiary << getDiaryDate(PrefixTimeFormat) << " ";
                    }
                    if (line)
                    {
                        fileDiary << line;
                    }
                }
            }
        }
        fileDiary.close();
    }
}

/*--------------------------------------------------------------------------*/
void Diary::writeln(const std::string& _str, bool bInput)
{
#define ENDLINE "\n"
    write(_str + ENDLINE, bInput);
}

/*--------------------------------------------------------------------------*/
int Diary::getID(void) const 
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
    bool previous = suspendwrite;

    suspendwrite = bWith;
    return previous;
}

/*--------------------------------------------------------------------------*/
bool Diary::getSuspendWrite(void)
{
    return suspendwrite;
}

/*--------------------------------------------------------------------------*/
std::string Diary::replace(const std::string& text, const std::string& s, const std::string& replacement)
{
    std::string::size_type pos = 0;
    std::string ret = text;
    while (pos != std::string::npos)
    {
        pos = ret.find(s, pos);
        if (pos == std::string::npos)
            // no more 's' in '*this'
        {
            break;
        }

        ret.replace(pos, s.length(), replacement);
        pos += replacement.length();
    }
    return ret;
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
bool compareDiary(const Diary& first, const Diary& second)
{
    if (first.getID() < second.getID())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*--------------------------------------------------------------------------*/
