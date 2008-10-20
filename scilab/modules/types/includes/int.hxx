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

#ifndef __INT_HH__
#define __INT_HH__

#include "types.hxx"

namespace types
{
	class Int : public GenericType
	{
  public :
    Int(int _iReal);
    Int(int _iRows, int _iCols, bool _bComplex = false);
    Int(int _iRows, int _iCols, int **_puiReal);
    Int(int _iRows, int _iCols, int **_puiReal, int **_puiImg);
		void CreateInt(int _iRows, int _iCols, int **_puiReal, int **_puiImg);
		~Int();

		/*data management*/
		int*					real_get() const;
		int						real_get(int _iRows, int _iCols) const;
		int*					img_get() const;
		int						img_get(int _iRows, int _iCols) const;

		bool					real_set(int *_puiReal);
		bool					img_set(int *_puiImg);



		/*zero or one set filler*/
		bool					zero_set();
		bool					one_set();

		/*Config management*/
    void					whoAmI();
		bool					isComplex();

    Int*					getAsInt(void);

  protected :
		RealType			getType(void);

		/*clean values array*/
		void					real_delete();
		void					img_delete(bool _bSetReal = false);
		void					all_delete(bool _bSetReal = false);

  private :
    int*					m_piReal;
    int*					m_piImg;
		bool					m_bComplex;
	};
}
#endif /* !__INT_HH__ */
