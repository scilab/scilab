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

#ifndef __FUNCTION_HXX__
#define __FUNCTION_HXX__

#include <string>
#include "types.hxx"

namespace types
{
	class EXTERN_TYPES Function : public InternalType
	{
	public :
		enum ReturnValue
		{
			AllGood,
			WrongParamNumber,
			WrongParamType
		};

		Function * 	getAsFunction(void);
		RealType getType(void) { return RealFunction; }

		typedef Function::ReturnValue (*GW_FUNC)(typed_list &in, int* _piRetCount, typed_list &out); 

    void					whoAmI();
		Function(std::string _szName, GW_FUNC _pFunc, std::string _szModule);
		~Function();
	//private:
		std::string			m_szName;
		GW_FUNC					m_pFunc;
		std::string			m_szModule;
	};

}


#endif /* !__FUNCTION_HXX__ */
