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

#ifndef __PMATRIXOLY_HH__
#define __PMATRIXOLY_HH__

#include "alltypes.hxx"

namespace types
{
	class MatrixPoly : public GenericType
	{
  public :
		MatrixPoly();
		MatrixPoly(string _szVarName, int _iRows, int _iCols, int *_piRank);
		~MatrixPoly();

		Poly*					poly_get(int _iRows, int _iCols);
		Poly*					poly_get(int _iIdx);
		bool					poly_set(int _iRows, int _iCols, Double *_pdblCoef);
		bool					poly_set(int _iIdx, Double *_pdblCoef);

		MatrixPoly*		getAsPoly();
		RealType			getType();
		void					whoAmI(void);
		bool					isComplex(void);
		void					complex_set(bool _bComplex);
		string				var_get();
		bool					rank_get(int *_piRank);
		int 					rank_max_get(void);
		Double*				evaluate(Double* _pdblValue);
		void					update_rank(void);
		Double* 			coef_get(void);
		void					coef_set(Double *_pCoef);

		std::string		toString(int _iPrecison, int _iLineLen);



		MatrixPoly&		operator= (MatrixPoly& poPoly);
	private :
		Poly*					m_poPolyMatrix;
		bool					m_bComplex;
		string				m_szVarName;
	};
}
#endif /* !__PMATRIXOLY_HH__ */
