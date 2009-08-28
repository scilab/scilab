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

#ifndef __IMP_LIST_HXX__
#define __IMP_LIST_HXX__

#include "types.hxx"

namespace types
{
	/*
	** Type
	*/
	class EXTERN_TYPES ImplicitList : public InternalType
	{
	private :
		double					m_dblStart;
		double					m_dblStep;
		double					m_dblEnd;
		MatrixPoly*			m_poStart;
		MatrixPoly*			m_poStep;
		MatrixPoly*			m_poEnd;
		int							m_iSize;

		int							m_iElement;

		InternalType::RealType		
										m_eStartType;
		InternalType::RealType		
										m_eStepType;
		InternalType::RealType		
										m_eEndType;

		void CreateImplicitList(double _dblStart, double _dblStep, double _dblEnd);

	public :
		ImplicitList();
		~ImplicitList();
		ImplicitList(double _dblStart, double _dblStep, double _dblEnd);
		ImplicitList(double _dblStart, double _dblEnd);
		ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd);

		ImplicitList* getAsImplicitList(void) { return this; }
		virtual				RealType getType(void) { return RealImplicitList; }

		double				start_get();
		MatrixPoly*		start_poly_get();
		void					start_set(double _dblStart);
		void					start_set(MatrixPoly*	_poPoly);

		double				step_get();
		MatrixPoly*		step_poly_get();
		void					step_set(double _dblStep);
		void					step_set(MatrixPoly* _poPoly);

		double				end_get();
		MatrixPoly*		end_poly_get();
		void					end_set(double _dblEnd);
		void					end_set(MatrixPoly *_poPoly);

		void					extract_matrix(double *_pData);
		double				extract_value(int _iOccur);
		void					compute();
		bool					computable();

		string				toString(int _iPrecision, int _iLineLen);


		InternalType::RealType 
									start_type_get();
		InternalType::RealType 
									step_type_get();
		InternalType::RealType 
									end_type_get();

		int						size_get();

	};
}

#endif /* !__IMP_LIST_HXX__ */
