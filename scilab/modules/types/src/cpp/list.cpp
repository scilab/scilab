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
#include "double.hxx"
#include "list.hxx"
#include "listundefined.hxx"
#include "listinsert.hxx"

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

		m_iSize = (int)m_plData->size();
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
		m_iSize = (int)m_plData->size();
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

		//check input param
		if(_bAsVector == false)
		{
			std::cout << "Extract from list must be \"as vector\"" << std::endl;
		}

		if(	_bAsVector && _piMaxDim[0] > size_get())
		{
			//retrun empty list
			return outList;
		}

		for(int i = 0 ; i < _iSeqCount ; i++)
		{
			InternalType* pIT = (*m_plData)[_piSeqCoord[i] - 1];
			outList.push_back(pIT->clone());
		}

		return outList;
	}

	bool List::insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, vector<types::InternalType*>* _poSource, bool _bAsVector)
	{
		//check input param
		if(_bAsVector == false)
		{
			std::cout << "Insertion in list must be \"as vector\"" << std::endl;
			return false;
		}

		if(_poSource->size() != _iSeqCount)
		{
			return false;
		}


		for(int i = 0 ; i < _iSeqCount ; i++)
		{
			if((*_poSource)[i]->isListDelete())
			{//delete item
				if((_piSeqCoord[i] - 1) < m_plData->size())
				{
					m_plData->erase(m_plData->begin() + (_piSeqCoord[i] - 1));
				}
			}
			else if((*_poSource)[i]->isListInsert())
			{//insert item
				ListInsert* pInsert = (*_poSource)[i]->getAsListInsert();
				if(m_plData->size() < _piSeqCoord[i])
				{//try to insert after the last index, increase list size and assign value
					while(m_plData->size() < _piSeqCoord[i])
					{//incease list size and fill with "Undefined"
						m_plData->push_back(new ListUndefined());
						m_iSize = size_get();
					}
					(*m_plData)[_piSeqCoord[i] - 1] = pInsert->insert_get();
				}
				else
				{
					m_plData->insert(m_plData->begin() + (_piSeqCoord[i] - 1), pInsert->insert_get());
				}
			}
			else
			{
				while(m_plData->size() < _piSeqCoord[i])
				{//incease list size and fill with "Undefined"
					m_plData->push_back(new ListUndefined());
					m_iSize = size_get();
				}
				(*m_plData)[_piSeqCoord[i] - 1] = (*_poSource)[i]->clone();
			}
		}
		return true;
	}
}
