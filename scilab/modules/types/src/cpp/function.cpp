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
#include "core_math.h"

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

	Function::ReturnValue Function::call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc)
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

  Function::ReturnValue WrapFunction::call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc) 
  {
      ReturnValue retVal = Callable::OK;
      GatewayStruct* pStr = new GatewayStruct();

      _iRetCount = Max(1, _iRetCount);
      pStr->m_pIn = &in;
      pStr->m_pOut = m_pTempOut;
      pStr->m_piRetCount = &_iRetCount;
      pStr->m_pstName = (char*)m_stName.c_str();
      pStr->m_pOutOrder = new int[_iRetCount < 1 ? 1 : _iRetCount];
      memset(pStr->m_pOutOrder, 0xFF, (_iRetCount < 1 ? 1 : _iRetCount) * sizeof(int));
      memset(pStr->m_pOut, 0x00, MAX_OUTPUT_VARIABLE * sizeof(InternalType*));

      //call gateway
      int iRet = m_pOldFunc((char*)m_stName.c_str(), (int*)pStr);

      if(iRet != 0)
      {
          retVal = Callable::Error;
      }
      else
      {
          //replace output argument in good order following m_pOutOrder
          for(int i = 0 ; i < _iRetCount ; i++)
          {
              //take care about return value count
              if(pStr->m_pOutOrder[i] == -1)
              {
                  break;
              }

              int iPos = (int)(pStr->m_pOutOrder[i] - in.size() - 1);
              out.push_back(m_pTempOut[iPos]);
              m_pTempOut[iPos] = NULL;
          }
      }

      //clean temp output variable array
      for(int i = 0 ; i < MAX_OUTPUT_VARIABLE ; i++)
      {
          if(m_pTempOut[i] != NULL)
          {
              delete m_pTempOut[i];
          }
      }

      delete[] pStr->m_pOutOrder;
      delete pStr;
      return retVal;
  }

  std::string Function::toString(int _iPrecision, int _iLineLen)
  {
    std::ostringstream ostr;

    // FIXME : Implement me.
    ostr << "FIXME : Implement Function::toString" << std::endl;

    return ostr.str();
  }

}
