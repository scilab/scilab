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


#include "function.hxx"

namespace types
{
	GatewayParam* GatewayParam::me;

  Function *Function::createFunction(std::string _szName, GW_FUNC _pFunc, std::string _szModule)
  {
    return new Function(_szName, _pFunc, _szModule);
  }

  Function *Function::createFunction(std::string _szName, OLDGW_FUNC _pFunc, std::string _szModule)
  {
    return new WrapFunction(_szName, _pFunc, _szModule);
  }


	Function::Function(std::string _szName, GW_FUNC _pFunc, std::string _szModule)
	{
		m_szName	= _szName;
		m_pFunc		= _pFunc;
		m_szModule	= _szModule;
	}

	Function::~Function()
	{
		if(isDeletable() == true)
		{
		}
	}

	Function* Function::getAsFunction(void)		
	{ 
		return this; 
	}

  Function::ReturnValue Function::call(typed_list &in, int* _piRetCount, typed_list &out)
  {
    return this->m_pFunc(in, _piRetCount, out);
  }

	/*--------------*/
	/*		whoIAm		*/
	/*--------------*/
	void Function::whoAmI() 
	{ 
		std::cout << "types::Function"; 
	}

	WrapFunction::WrapFunction(std::string _szName, OLDGW_FUNC _pFunc, std::string _szModule)
	{
		m_szName	 = _szName;
		m_pOldFunc = _pFunc;
		m_szModule = _szModule;
	}

	Function::ReturnValue WrapFunction::call(typed_list &in, int* _piRetCount, typed_list &out) 
	{
		printf("Fake Stack put ...\n");
		GatewayParam* pGW = types::GatewayParam::getInstance();
		GatewayStruct* pStr = new GatewayStruct();

		pStr->m_pin = &in;
		pStr->m_pout = &out;
		pStr->m_piRetCount = _piRetCount;

		pGW->put(_piRetCount, pStr);

		printf("Call old style GW ...\n");
		int iRet = m_pOldFunc("name", _piRetCount);
		printf("Fake Stack get ...\n");

		if(iRet != 0)
		{
			return WrongParamType;
		}
		return AllGood;
	}

	GatewayParam* GatewayParam::getInstance(void)
  {
		if (me == 0)
		{
			me = new GatewayParam();
		}
		return me;
  }

	GatewayStruct*	GatewayParam::get(int* _pKey) const
	{
		std::map<int*, GatewayStruct*>::const_iterator it_gwParam;
		it_gwParam = m_GWList.find(_pKey);
		if(it_gwParam == m_GWList.end())
		{
			return NULL;
		}

		return it_gwParam->second;
	}

	bool GatewayParam::put(int* _pKey, GatewayStruct* _pParam)
	{
		m_GWList[_pKey] = _pParam;
		return true;
	}

}
