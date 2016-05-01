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
Diary::Diary(const std::wstring& _wfilename, int _mode, int ID, bool autorename)
{
    std::ios::openmode wofstream_mode;

    std::wstring fullfilename = getUniqueFilename(_wfilename);
    if (autorename)
    {
        fullfilename = getUniqueFilename(_wfilename);

        wchar_t* ws = getFullFilenameW(fullfilename.data());
        fullfilename = ws;
        FREE(ws);
    }
    else
    {
        wchar_t* ws = getFullFilenameW(_wfilename.data());
        fullfilename = ws;
        FREE(ws);
    }

    suspendwrite = false;

    PrefixTimeFormat = PREFIX_TIME_FORMAT_UNIX_EPOCH;
    IoModeFilter = DIARY_FILTER_INPUT_AND_OUTPUT;   // default command & input
    PrefixIoModeFilter = PREFIX_FILTER_NONE;    // no prefix

    if (_mode == 0)
    {
        wofstream_mode = std::ios::trunc | std::ios::binary;
    }
    else
    {
        wofstream_mode = std::ios::app | std::ios::binary;
    }

#ifdef _MSC_VER
    std::wofstream fileDiary(fullfilename, wofstream_mode);
#else
    wchar_t *wcfile = (wchar_t *) fullfilename.c_str();
    char *filename = wide_string_to_UTF8(wcfile);

    std::ofstream fileDiary(filename, wofstream_mode);

    if (filename)
    {
        FREE(filename);
        filename = NULL;
    }
#endif

    if (fileDiary.bad())
    {
        wfilename = std::wstring(L"");
        fileAttribMode = -1;
        setID(-1);
    }
    else
    {
        wfilename = fullfilename;
        fileAttribMode = wofstream_mode;
        setID(ID);
    }
    fileDiary.close();
}

/*--------------------------------------------------------------------------*/
Diary::~Diary()
{
    wfilename = L"";
    fileAttribMode = -1;
    setID(-1);
}

/*--------------------------------------------------------------------------*/
std::wstring Diary::getFilename(void)
{
    return wfilename;
}

/*--------------------------------------------------------------------------*/
void Diary::write(const std::wstring& _wstr, bool bInput)
{
    if (!suspendwrite)
    {
        std::ios::openmode wofstream_mode = std::ios::app | std::ios::binary;

#ifdef _MSC_VER
        std::wofstream fileDiary(wfilename.c_str(), wofstream_mode);
#else
        wchar_t *wcfile = (wchar_t *) wfilename.c_str();
        char *filename = wide_string_to_UTF8(wcfile);

        std::ofstream fileDiary(filename, wofstream_mode);
        if (filename)
        {
            FREE(filename);
            filename = NULL;
        }
#endif

        if (fileDiary.good())
        {
            char *line = NULL;
            std::wstring wst = _wstr;
#ifdef _MSC_VER
            /* carriage return for Windows */
            wst = replace(wst, std::wstring(L"\n"), std::wstring(L"\r\n"));
            wst = replace(wst, std::wstring(L"\r\r"), std::wstring(L"\r"));
#endif
            line = wide_string_to_UTF8(wst.data());

            if (bInput)         // input
            {
                if ((IoModeFilter == DIARY_FILTER_INPUT_AND_OUTPUT) || (IoModeFilter == DIARY_FILTER_ONLY_INPUT))
                {
                    if ((PrefixIoModeFilter == PREFIX_FILTER_INPUT_AND_OUTPUT) || (PrefixIoModeFilter == PREFIX_FILTER_ONLY_INPUT))
                    {
                        char *timeInfo = wide_string_to_UTF8((wchar_t *) getDiaryDate(PrefixTimeFormat).c_str());

                        if (timeInfo)
                        {
                            fileDiary << timeInfo << " ";
                            FREE(timeInfo);
                            timeInfo = NULL;
                        }
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
                        char *timeInfo = wide_string_to_UTF8((wchar_t *) getDiaryDate(PrefixTimeFormat).c_str());

                        if (timeInfo)
                        {
                            fileDiary << timeInfo << " ";
                            FREE(timeInfo);
                            timeInfo = NULL;
                        }
                    }
                    if (line)
                    {
                        fileDiary << line;
                    }
                }
            }

            if (line)
            {
                FREE(line);
                line = NULL;
            }
        }
        fileDiary.close();
    }
}

/*--------------------------------------------------------------------------*/
void Diary::writeln(const std::wstring& _wstr, bool bInput)
{
#define ENDLINE L"\n"
    write(_wstr + ENDLINE, bInput);
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
std::wstring Diary::replace(const std::wstring& text, const std::wstring& s, const std::wstring& replacement)
{
    std::wstring::size_type pos = 0;
    std::wstring ret = text;
    while (pos != std::wstring::npos)
    {
        pos = ret.find(s, pos);
        if (pos == std::wstring::npos)
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
