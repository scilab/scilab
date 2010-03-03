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
#include "list.hxx"

namespace types 
{
	/**
	** Constructor & Destructor (public)
	*/
	List::List() : Container()
	{
		m_plData = new std::vector<InternalType *>();
	}

	List::~List() 
	{
		if(isDeletable() == true)
		{
			delete m_plData;
		}
	}

	/** 
	** Private Copy Constructor and data Access
	*/
	List::List(List *_oListCopyMe)
	{
		std::vector<InternalType *>::iterator itValues;
		m_plData = new std::vector<InternalType *>(_oListCopyMe->getData()->size());

		for(int i = 0 ; i < _oListCopyMe->getData()->size() ; i++)
		{
			InternalType* pIT = (*_oListCopyMe->getData())[i];
			(*m_plData)[i] = pIT->clone();

		}
	}

	std::vector<InternalType *> *List::getData()
	{
		return m_plData;
	}

	/**
	** size_get
	** Return the number of elements in list
	*/
	int List::size_get() 
	{
		return (int)m_plData->size();
	}

	/**
	** append(InternalType *_typedValue)
	** Append the given value to the end of the List
	*/
	void List::append(InternalType *_typedValue)
	{
		m_plData->push_back(_typedValue);
	}

	/**
	** Clone
	** Create a new List and Copy all values.
	*/
	List *List::clone()
	{
		return new List(this);
	}

	/**
	** toString to display Lists
	** FIXME : Find a better indentation process
	*/
	std::string List::toString(int _iPrecision, int _iLineLen)
	{
		std::ostringstream ostr;

		if (size_get() == 0)
		{
			ostr << "()" << std::endl;
		}
		else
		{
			int iPosition = 1;
			std::vector<InternalType *>::iterator itValues;
			for (itValues = m_plData->begin() ; itValues != m_plData->end() ; ++itValues, ++iPosition)
			{
				ostr << "     (" << iPosition << ")" << std::endl;
				ostr << (*itValues)->toString(_iPrecision, _iLineLen) << std::endl;
			}
		}
		return ostr.str();
	}

	std::vector<InternalType*>	List::extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
	{
		std::vector<InternalType*> outList;
		int iRowsOut	= 0;
		int iColsOut	= 0;

		//check input param

		if(	_bAsVector && _piMaxDim[0] > size_get() ||
			_bAsVector == false && _piMaxDim[0] > rows_get() ||
			_bAsVector == false && _piMaxDim[1] > cols_get())
		{
			return outList;
		}

		if(_bAsVector)
		{//a([])
			if(rows_get() == 1)
			{
				iRowsOut	= 1;
				iColsOut	= _piDimSize[0];
			}
			else
			{
				iRowsOut	= _piDimSize[0];
				iColsOut	= 1;
			}
		}
		else
		{//a([],[])
			iRowsOut	= _piDimSize[0];
			iColsOut	= _piDimSize[1];
		}

		if(_bAsVector)
		{
			for(int i = 0 ; i < _iSeqCount ; i++)
			{
				InternalType* pIT = (*m_plData)[_piSeqCoord[i] - 1];
				outList.push_back(pIT->clone());
			}
		}
		else
		{
			std::cout << "Extract from list must be \"as vector\"" << std::endl;
		}

		return outList;
	}
}
