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

#ifndef __MATRIXPOLY_HH__
#define __MATRIXPOLY_HH__

#include "types.hxx"

#ifdef isComplex
#undef isComplex
#endif

namespace types
{
	class MatrixPoly : public GenericType
	{
  public :
									MatrixPoly();
									MatrixPoly(string _szVarName, int _iRows, int _iCols, int *_piRank);
		virtual				~MatrixPoly();

	  // FIXME : Should not return NULL
	  MatrixPoly*		clone() { return NULL; }

		GenericType*	get(int _iPos);

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
		Double*				extract_coef(int _iRank);
		bool					insert_coef(int _iRank, Double* _pCoef);

		bool					insert(int _iRows, int _iCols, MatrixPoly* _poSource);

		string				toString(int _iPrecison, int _iLineLen);
		string				GetRowString(int _iPrecison, int _iLineLen, bool _bComplex);
		string				GetColString(int _iPrecison, int _iLineLen, bool _bComplex);
		string				GetMatrixString(int _iPrecison, int _iLineLen, bool _bComplex);


		MatrixPoly&		operator= (MatrixPoly& poPoly);

		bool					operator==(const InternalType& it);
		bool					operator!=(const InternalType& it);

	private :
		Poly*					m_poPolyMatrix;
		bool					m_bComplex;
		string				m_szVarName;
	};
}
#endif /* !__MATRIXPOLY_HH__ */
