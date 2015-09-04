/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    InternalType*           clone();

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

    virtual int getNbInputArgument(void);
    virtual int getNbOutputArgument(void);

    bool operator==(const InternalType& it);

private :
    std::wstring            m_stPath;
    Macro*                  m_pMacro;
};
}


#endif /* !__MACROFILE_HXX__ */
