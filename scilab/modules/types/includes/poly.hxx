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

#ifndef __POLY_HH__
#define __POLY_HH__

#include <list>
#include "types.hxx"

using std::string;
using std::list;

namespace types
{
	class Poly : public GenericType
	{
  public :
									Poly();
									Poly(double** _pdblCoefR, int _iRank);
									Poly(double** _pdblCoefR, double** _pdblcoefI, int _iRank);
									Poly(Double** _poCoefR, int _iRank);
									~Poly();

	  // FIXME : Should not return NULL;
	  Poly*					clone() { return NULL; }

		Poly*					getAsSinglePoly(void);
		/*Config management*/
    void					whoAmI();
		bool					isComplex();
		void					complex_set(bool _bComplex);

		int						rank_get();
		bool					rank_set(int _iRank, bool bSave = false);
		Double*				coef_get();
		double*				coef_real_get();
		double*				coef_img_get();
		bool					coef_set(Double *_poPow);
		bool					coef_set(double *_pdblCoefR, double *_pdblCoefI);
		bool					evaluate(double _dblInR, double _dblInI, double *_pdblOutR, double *_pdblOutI);
		void					update_rank(void);

		void					CreatePoly(double**_pdblCoefR, double**_pdblCoefI, int _iRank);
		void					toStringReal(int _iPrecision, int _iLineLen, string _szVar, list<string>* _pListExp , list<string>* _pListCoef);
		void					toStringImg(int _iPrecision, int _iLineLen, string _szVar, list<string>* _pListExp , list<string>* _pListCoef);

		std::string			toString(int _iPrecision, int _iLineLen);

		bool					operator==(const InternalType& it);
		bool					operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string     getTypeStr() {return string("poly");}
    protected :
		RealType			getType(void);

	private : 
		void					toStringInternal(double *_pdblVal, int _iPrecision, int _iLineLen, string _szVar, list<string>* _pListExp , list<string>* _pListCoef);


  private :
		bool					m_bComplex;
		Double*				m_pdblCoef;
		int						m_iRank;
	};
}

#endif /* !__POLY_HH__ */
