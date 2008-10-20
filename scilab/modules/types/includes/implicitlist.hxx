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

#include "internal.hxx"
#include "alltypes.hxx"

namespace types
{
	/*
	** Type
	*/
	class ImplicitList : public InternalType
	{
	private :
		double		m_dblStart;
		double		m_dblStep;
		double		m_dblEnd;
		int				m_iSize;

		int				m_iElement;

		void CreateImplicitList(double _dblStart, double _dblStep, double _dblEnd);

	public :
		ImplicitList();
		ImplicitList(double _dblStart, double _dblStep, double _dblEnd);
		ImplicitList(double _dblStart, double _dblEnd);
	
		ImplicitList* getAsList(void) { return this; }
		virtual RealType getType(void) { return RealImplicitList; }

		double	start_get();
		void		start_set(double _dblStart);
		double	step_get();
		void		step_set(double _dblStep);
		double	end_get();
		void		end_set(double _dblEnd);

		void		extract_matrix(double *_pData);
		double	extract_value(int _iOccur);

		int		size_get();
	};
}

#endif /* !__IMP_LIST_HXX__ */
