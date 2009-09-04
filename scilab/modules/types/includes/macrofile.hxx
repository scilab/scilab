/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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

#include <list>
#include "callable.hxx"
#include "types.hxx"
#include "symbol.hxx"
#include "seqexp.hxx"

namespace types
{
  class MacroFile : public Callable
  {
  public :
		MacroFile *getAsMacroFile(void);
    RealType getType(void);

    void whoAmI();

		MacroFile(): Callable(){};
		MacroFile(std::string _stName, string _stPath);
		virtual ~MacroFile(){};
		Callable::ReturnValue call(typed_list &in, int _iRetCount, typed_list &out);
  
  public :
		Macro*					m_pMacro;
    std::string			m_stName;
    std::string			m_stPath;
  };
}


#endif /* !__MACRO_HXX__ */
