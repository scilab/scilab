/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
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

#ifndef __MACROFILE_HXX__
#define __MACROFILE_HXX__

#include <string>
#include "callable.hxx"
#include "macro.hxx"

namespace types
{
class EXTERN_AST MacroFile : public Callable
{
public :
    MacroFile(): Callable() {};
    MacroFile(const std::wstring& _stName, const std::wstring& _stPath, const std::wstring& _stModule);
    virtual                 ~MacroFile();

    //FIXME : Should not return NULL
    MacroFile*              clone();

    inline ScilabType       getType(void)
    {
        return ScilabMacroFile;
    }
    inline ScilabId         getId(void)
    {
        return IdMacroFile;
    }

    bool                    isMacroFile()
    {
        return true;
    }

    void                    whoAmI();

    bool                    toString(std::wostringstream& ostr);

    Callable::ReturnValue   call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) override;
    bool                    parse(void);

    Macro*                  getMacro(void);

    void                    setLines(int _iFirstLine, int _iLastLine);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"function";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"function";
    }

    inline const std::wstring & getPath()
    {
        return m_stPath;
    }

    virtual int getNbInputArgument(void);
    virtual int getNbOutputArgument(void);

    bool operator==(const InternalType& it);

private :
    std::wstring            m_stPath;
    Macro*                  m_pMacro;
};
}


#endif /* !__MACROFILE_HXX__ */
