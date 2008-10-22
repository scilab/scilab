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

#ifndef __UINT_HH__
#define __UINT_HH__

#include "types.hxx"

namespace types
{
	class UInt : public GenericType
	{
  public :
		~UInt();
		UInt(unsigned int _iReal);
    UInt(int _iRows, int _iCols, bool _bComplex = false);
    UInt(int _iRows, int _iCols, int unsigned **_puiReal);
    UInt(int _iRows, int _iCols, int unsigned **_puiReal, int unsigned **_puiImg);


		/*data management*/
		int unsigned*	real_get() const;
		int unsigned	real_get(int _iRows, int _iCols) const;
		int unsigned*	img_get() const;
		int unsigned	img_get(int _iRows, int _iCols) const;

		bool					real_set(int *_puiReal);
		bool					img_set(int *_puiImg);



		/*zero or one set filler*/
		bool					zero_set();
		bool					one_set();

		/*Config management*/
    void					whoAmI();
		bool					isComplex();

    UInt*					getAsUInt(void);

  protected :
		RealType			getType(void);

		/*clean values array*/
		void					real_delete();
		void					img_delete(bool _bSetReal = false);
		void					all_delete(bool _bSetReal = false);

		/*Internal "constructor*/
		void					CreateUInt(int _iRows, int _iCols, int unsigned **_puiReal, int unsigned **_puiImg);

  private :
    int unsigned	*m_puiReal;
    int unsigned	*m_puiImg;
		bool					m_bComplex;
	};
}
#endif /* !__UINT_HH__ */
