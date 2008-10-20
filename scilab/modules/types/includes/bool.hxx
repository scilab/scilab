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

#ifndef __BOOL_HH__
#define __BOOL_HH__

#include "types.hxx"

namespace types
{
  class Bool : public GenericType
  {
  public :
    Bool(bool _bReal);
    Bool(int _iRows, int _iCols);
    Bool(int _iRows, int _iCols, bool **_pbData);
		void CreateBool(int _iRows, int _iCols, bool **_pbData);
		~Bool();

		/*data management*/
		bool*					bool_get() const;
		bool					bool_get(int _iRows, int _iCols) const;

		bool					bool_set(bool *_pbData);
		bool					bool_set(int _iRow, int _iCol, bool _bData);


		/*zero or one set filler*/
		bool					false_set();
		bool					true_set();

		/*Config management*/
    void					whoAmI();
		bool					isComplex();

    Bool*					getAsBool(void);

  protected :
		RealType			getType(void);

		/*clean values array*/
		void					all_delete();

  private :
    bool*					m_pbData;
  };
}
#endif /* ! __BOOL_HH__ */
