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
#include "int.hxx"
#include "double.hxx"
#include "matrixpoly.hxx"

namespace types
{
	/*
	** Type
	*/
	class ImplicitList : public InternalType
	{
	private :
		InternalType*		m_poStart;
		InternalType*		m_poStep;
		InternalType*		m_poEnd;
		long long				m_iSize; //can be a big value

		InternalType::RealType		
										m_eStartType;
		InternalType::RealType		
										m_eStepType;
		InternalType::RealType		
										m_eEndType;
		InternalType::RealType		
										m_eOutType;
		Int::IntType		m_eOutSubType;

		bool m_bComputed;

	public :
		ImplicitList();
		virtual ~ImplicitList();
		ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd);

		ImplicitList* clone();

		ImplicitList* getAsImplicitList(void) { return this; }
		RealType getType(void) { return RealImplicitList; }

		RealType			getOutputType();
		Int::IntType	getOutputSubType();

		InternalType*	start_get();
		void					start_set(InternalType*	_poIT);

		InternalType*	step_get();
		void					step_set(InternalType* _poIT);

		InternalType*	end_get();
		void					end_set(InternalType* _poIT);

		bool					compute();
		bool					computable();

		string				toString(int _iPrecision, int _iLineLen);


		InternalType::RealType 
									start_type_get();
		InternalType::RealType 
									step_type_get();
		InternalType::RealType 
									end_type_get();

		long long			size_get();

		//extract single value in a InternalType
		InternalType* extract_value(int _iOccur); //Single value
		double				extract_value_double(int _iOccur);
		long long			extract_value_int(int _iOccur);
		//extract matrix in a Internaltype
		InternalType* extract_matrix();
		//extract double
		void					extract_matrix(double *_pdbl);
		//extract integer
		void					extract_matrix(char *_pc);
		void					extract_matrix(short *_ps);
		void					extract_matrix(int *_pi);
		void					extract_matrix(long long *_pll);
		//extract unsigned integer
		void					extract_matrix(unsigned char *_puc);
		void					extract_matrix(unsigned short *_pus);
		void					extract_matrix(unsigned int *_pui);
		void					extract_matrix(unsigned long long *_pull);


	};
}

#endif /* !__IMP_LIST_HXX__ */
