/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __STRING_HH__
#define __STRING_HH__

#include <string>
#include <list>
#include "types.hxx"

namespace types
{
  class String : public GenericType
  {
  public :
    String(int _iRows, int _iCols);
    String(const char *_pcData);

    char**		string_get() const;
    char*			string_get(int _iRows, int _iCols) const;

    bool			string_set(const char **_pcData);
    bool			string_set(int _iRows, int _iCols, const char *_pcData);

    void whoAmI();

    String *getAsString(void);

  protected :
    RealType getType();//			{ return RealString; }

  private :
		char** m_pcData;

		void all_delete();
		void string_delete(int _iRows, int _iCols);
		void string_delete(int _iPos);
		bool string_set(int _iPos, const char *_pcData);

		void CreateString(int _iRows, int _iCols);
  };
}
#endif /* !__STRING_HH__ */
