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

#ifndef __DOUBLE_HH__
#define __DOUBLE_HH__

#include "types.hxx"

namespace types
{
	class Double : public GenericType
	{
  public :
		~Double();
		Double(double _dblReal);
    Double(int _iRows, int _iCols, bool _bComplex = false);
    Double(int _iRows, int _iCols, double **_pdblReal);
    Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);


		/*data management*/
		double*				real_get() const;
		double				real_get(int _iRow, int _iCol) const;
		double*				img_get() const;
		double				img_get(int _iRow, int _iCol) const;

		bool					real_set(double *_pdblReal);
		bool					img_set(double *_pdblImg);

		bool					val_set(int _iRow, int _iCols, double _dblReal);
		bool					val_set(int _iRow, int _iCols, double _dblReal, double _dblImg);

		/*zero or one set filler*/
		bool					zero_set();
		bool					one_set();

		/*Config management*/
    void					whoAmI();
		bool					isComplex();
		void					complex_set(bool _bComplex);

    Double*				getAsDouble(void);

  protected :
		RealType			getType(void);

		/*clean values array*/
		void					real_delete();
		void					img_delete(bool _bSetReal = false);
		void					all_delete(bool _bSetReal = false);

		/*Internal "constructor*/
		void					CreateDouble(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);

  private :
    double	*m_pdblReal;
    double	*m_pdblImg;
		bool		m_bComplex;
	};
}

#endif /* !__DOUBLE_HH__ */
