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
#ifndef __DIARY_HXX__
#define __DIARY_HXX__
/*--------------------------------------------------------------------------*/
#include <string>
#include "DiaryModesEnum.hxx"

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

    /* see DiaryModesEnum.hxx */
    diary_prefix_time_format PrefixTimeFormat;

    /* see DiaryModesEnum.hxx */
    diary_prefix_time_filter PrefixIoModeFilter;

    /* see DiaryModesEnum.hxx */
    diary_filter IoModeFilter;

    /*
    * used on Windows to replace carriage return
    */
    std::wstring replace(const std::wstring& text, const std::wstring& s, const std::wstring& replacement);

public:
    /*
    * constructor
    */
    Diary(const std::wstring& _wfilename, int _mode, int ID, bool autorename);

    /*
    * destructor
    */
    ~Diary();

    /*
    * get filename of this diary
    */
    std::wstring getFilename(void);

    /*
    * get & set IO mode
    */
    void setIOMode(diary_filter _mode);
    diary_filter getIOMode(void);

    /*
    * get ID of this diary
    */
    int getID(void) const;

    /*
    * write a string in this diary
    */
    void write(const std::wstring& _wstr, bool bInput);

    /*
    * write a line in this diary
    */
    void writeln(const std::wstring& _wstr, bool bInput);

    /*
    * get & set Resume mode
    */
    bool setSuspendWrite(bool bWith);
    bool getSuspendWrite(void);

    /*
    * get & set Prefix mode
    */
    void setPrefixMode(diary_prefix_time_format iPrefixTimeFormat);
    diary_prefix_time_format getPrefixMode(void);

    /*
    * get & set IO mode filter
    */
    void setPrefixIoModeFilter(diary_prefix_time_filter mode);
    diary_prefix_time_filter getPrefixIoModeFilter(void);
};

#endif /* __DIARY_HXX__ */
/*--------------------------------------------------------------------------*/
bool compareDiary(const Diary& first, const Diary& second);
/*--------------------------------------------------------------------------*/
