/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <sstream>
#include "function.hxx"

namespace types
{
  Function *Function::createFunction(std::string _stName, GW_FUNC _pFunc, std::string _stModule)
  {
    return new Function(_stName, _pFunc, _stModule);
  }

  Function *Function::createFunction(std::string _stName, OLDGW_FUNC _pFunc, std::string _stModule)
  {
    return new WrapFunction(_stName, _pFunc, _stModule);
  }

  Function::Function(std::string _stName, GW_FUNC _pFunc, std::string _stModule):
    Callable(),
    m_pFunc(_pFunc)
  {
    setName(_stName);
    setModule(_stModule);
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

  Function::ReturnValue Function::call(typed_list &in, int _iRetCount, typed_list &out)
  {
    return this->m_pFunc(in, _iRetCount, out);
  }

  /*--------------*/
  /*		whoIAm		*/
  /*--------------*/
  void Function::whoAmI() 
  { 
    std::cout << "types::Function"; 
  }

  WrapFunction::WrapFunction(std::string _stName, OLDGW_FUNC _pFunc, std::string _stModule)
  {
    m_stName = _stName;
    m_pOldFunc = _pFunc;
    m_stModule = _stModule;
  }

  Function::ReturnValue WrapFunction::call(typed_list &in, int _iRetCount, typed_list &out) 
  {
    ReturnValue RetVal = Callable::OK;
    GatewayStruct* pStr = new GatewayStruct();

    pStr->m_pin = &in;
    pStr->m_pout = &out;
    pStr->m_piRetCount = &_iRetCount;
    pStr->m_pstName = (char*)m_stName.c_str();

    int iRet = m_pOldFunc((char*)m_stName.c_str(), (int*)pStr);

    if(iRet != 0)
      {
	RetVal = Callable::Error;
      }

    delete pStr;
    return RetVal;
  }

  std::string Function::toString(int _iPrecision, int _iLineLen)
  {
    std::ostringstream ostr;

    // FIXME : Implement me.
    ostr << "FIXME : Implement Function::toString" << std::endl;

    return ostr.str();
  }

}
