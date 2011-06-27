/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2011 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __ELEM_FUNC_GW_HXX__
#define __ELEM_FUNC_GW_HXX__

#include "cpp_gateway_prototype.hxx"

#ifdef _MSC_VER
	#if ELEM_FUNC_GW_EXPORTS
		#define EXTERN_EF_GW __declspec (dllexport)
	#else
		#define EXTERN_EF_GW __declspec (dllimport)
	#endif
#else
	#define EXTERN_EF_GW
#endif

class ElemFuncModule
{
private :
	ElemFuncModule(){};
	~ElemFuncModule(){};
public :
	EXTERN_EF_GW static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_isequal);
CPP_GATEWAY_PROTOTYPE(sci_size);
CPP_GATEWAY_PROTOTYPE(sci_eye);
CPP_GATEWAY_PROTOTYPE(sci_zeros);
CPP_GATEWAY_PROTOTYPE(sci_ones);

#endif /* __ELEM_FUNC_GW_HXX__ */
