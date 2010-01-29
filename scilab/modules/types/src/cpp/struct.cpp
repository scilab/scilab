/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "symbol.hxx"
#include "struct.hxx"

namespace types 
{
  /**
   ** Constructor & Destructor (public)
   */
  Struct::Struct() : Container()
  {
    m_plData = new std::map<symbol::Symbol, InternalType *>();
    m_iRows = 1;
    m_iCols = 1;
    m_iSize = 0;
  }

  Struct::~Struct() 
  {
    if(isDeletable() == true)
      {
	delete m_plData;
      }
  }

  /** 
   ** Private Copy Constructor and data Access
   */
  Struct::Struct(Struct *_oStructCopyMe)
  {
    std::map<symbol::Symbol, InternalType *>::iterator itValues;
    m_plData = new std::map<symbol::Symbol, InternalType *>();

    for (itValues = _oStructCopyMe->getData()->begin();
	 itValues != _oStructCopyMe->getData()->end();
	 itValues++)
      {
	(*m_plData)[(*itValues).first] = ((*itValues).second)->clone();
      }
  }

  std::map<symbol::Symbol, InternalType *> *Struct::getData()
  {
    return m_plData;
  }

  /**
   ** size_get
   ** Return the number of elements in struct
   */
  int Struct::size_get() 
  {
    return m_plData->size();
  }

  /**
   ** add(symbol::Symbol*_psKey, InternalType *_typedValue)
   ** Append the given value to the struct
   */ 
  void Struct::add(symbol::Symbol _sKey, InternalType *_typedValue)
  {
    /* Look if we are replacing some existing value */
    if (exists(_sKey))
      {
	(*m_plData)[_sKey]->DecreaseRef();
	if ((*m_plData)[_sKey]->isDeletable())
	  {
	    delete (*m_plData)[_sKey];
	  }
      }
      _typedValue->IncreaseRef();
    (*m_plData)[_sKey] = _typedValue;
  }

  /**
   ** get(symbol::Symbol*_psKey)
   ** Append the given value to the end of the List
   */
  InternalType* Struct::get(symbol::Symbol _sKey)
  {
    return (*m_plData)[_sKey];
  }

  bool Struct::exists(symbol::Symbol _sKey)
  {
    std::map<symbol::Symbol, InternalType *>::iterator it = m_plData->find(_sKey);
    if (it ==  m_plData->end())
      {
	return false;
      }
    return true;
  }
  
  /**
   ** Clone
   ** Create a new Struct and Copy all values.
   */
  Struct *Struct::clone()
  {
    return new Struct(this);
  }

  /**
   ** toString to display Structs
   ** FIXME : Find a better indentation process
   */
  std::string Struct::toString(int _iPrecision, int _iLineLen)
  {
    std::ostringstream ostr;
      
    if (size_get() == 0)
      {
	ostr << "Empty struct" << std::endl;
      }
    else
      {
	int iPosition = 1;
	std::map<symbol::Symbol, InternalType *>::iterator itValues;
	for (itValues = m_plData->begin() ; itValues != m_plData->end() ; ++itValues)
	  {
	    ostr << (itValues->first).name_get() << " : ";
	    switch  ((itValues->second)->getType()) 
	      {
	      case RealStruct :
		ostr << "[ " << (itValues->second)->getAsStruct()->rows_get()
		     << " x " << (itValues->second)->getAsStruct()->cols_get()
		     << " struct ]";
		break;
	      default :
		ostr << (itValues->second)->toString(_iPrecision, _iLineLen);
	      }
	  }
      }
    return ostr.str();
  }
}
