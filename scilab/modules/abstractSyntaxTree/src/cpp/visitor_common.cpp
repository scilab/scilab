/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <numeric>

#include "conditionvisitor.hxx"

bool bConditionState(ast::ConditionVisitor *exec)
{
	if(exec->is_boolean_result())
	{
		return exec->result_bool_get();
	}
	else if(((types::GenericType*)exec->result_get())->isDouble() && ((types::Double*)exec->result_get())->isComplex() == false)
	{
		types::Double *pR		= (types::Double*)exec->result_get();
		double *pReal	= pR->real_get();

		for(int i = 0 ; i < pR->size_get() ; i++)
		{
			if(pReal[i] == 0)
			{
				return false;
			}
		}
	}
	else if(((types::GenericType*)exec->result_get())->isBool())
	{
		types::Bool *pB		= (types::Bool*)exec->result_get();
		int *piData	= pB->bool_get();

		for(int i = 0 ; i < pB->size_get() ; i++)
		{
			if(piData[i] == 0)
			{
				return false;
				break;
			}
		}
	}
	else if(((types::GenericType*)exec->result_get())->isInt())
	{
	}
	else
	{
		return false;
	}
	return true;
}

void ExpandList(int ** _piList, int *_piListSize, int _iListSizeSize, int *_piResultList)
{
#define ORIGINAL_IMPLEM 
#ifdef ORIGINAL_IMPLEM
	for(int i = _iListSizeSize - 1 ; i >= 0 ; i--)
	{
		int iPreOcc = 1;
		int iPostOcc = 1;
		for(int k = 0 ; k < i ; k++)
		{
			if(k != i)
			{
				iPreOcc *= _piListSize[k];
			}
		}

		for(int k = i + 1 ; k < _iListSizeSize ; k++)
		{
			if(k != i)
			{
				iPostOcc *= _piListSize[k];
			}
		}

		int iSize = _piListSize[i];
		for(int m = iSize - 1 ; m >= 0  ; m--)
		{
			for(int j1 = iPreOcc - 1 ; j1 >= 0 ; j1--)
			{
				for(int j2 = iPostOcc - 1 ; j2 >= 0 ; j2--)
				{
					//Bloc Offset				-> j1 * ( iPostOcc * iSize) * _iListSizeSize
					//Offset in Bloc		-> m * iPostOcc * _iListSizeSize
					//Offset in SubBloc -> j2 * _iListSizeSize + i
					int iPos = j1 * ( iPostOcc * iSize) * _iListSizeSize + m * iPostOcc * _iListSizeSize + j2 * _iListSizeSize + i;
					_piResultList[iPos] = _piList[i][m];

				}
			}
		}
	}
#else
	int iPreOcc= 1;
	int iPostOcc= std::accumulate(_piListSize, _piListSize + _iListSizeSize, 1, multiplies<int>());
	for(int i =0; i!= _iListSizeSize;++i)
	{
		int const iSize = _piListSize[i];
		int const delta(iPostOcc * _iListSizeSize);
		int* ptr = _piResultList + i;

		iPostOcc /= iSize;

		for(int m(0); m != iSize; ++m,ptr += delta)
		{
			int const data=_piList[i][m];
			for(int j1(0); j1 != iPreOcc; ++j1, ptr +=  delta * iSize)
			{
				for(int j2(0); j2!= iPostOcc ; ++j2, ptr+=  _iListSizeSize) 
				{
					*ptr= data;
				}
			}
		}
		iPreOcc *= iSize;
	}
#endif
}

int GetVarMaxDim(types::InternalType *_pIT, int _iCurrentDim, int _iMaxDim)
{
	if(_pIT == NULL)
	{
		return 0;
	}
	if(_iCurrentDim == 0)
	{
		if(_iMaxDim != 1)
		{
			return ((types::GenericType*)_pIT)->rows_get();
		}
		else
		{
			return ((types::GenericType*)_pIT)->size_get();
		}
	}
	else if(_iCurrentDim == 1)
	{
		return ((types::GenericType*)_pIT)->cols_get();
	}
	else
	{//more than 2 dimansions ? :(
		return ((types::GenericType*)_pIT)->size_get();
	}
}

/*
 * Generate destination variable from _poSource type and size parameters
 */
types::InternalType* allocDest(types::InternalType* _poSource, int _iRows, int _iCols)
{
    types::InternalType* poResult = NULL;
    switch(_poSource->getType())
    {
    case types::GenericType::RealDouble :
        poResult = new types::Double(_iRows, _iCols, false);
        break;
    case types::GenericType::RealBool :
        poResult = new types::Bool(_iRows, _iCols);
        break;
    case types::GenericType::RealInt :
        poResult = types::Int::createInt(_iRows, _iCols, _poSource->getAsInt()->getIntType());
        break;
    case types::GenericType::RealString :
        poResult = new types::String(_iRows, _iCols);
        break;
    case types::GenericType::RealPoly :
        {
            int* piRank = new int[_iRows * _iCols];
            for(int i = 0 ; i < _iRows * _iCols ; i++)
            {
                piRank[i] = 1;
            }
            poResult = new types::MatrixPoly(_poSource->getAsPoly()->var_get(), _iRows, _iCols, piRank);
            break;
        }
    case types::InternalType::RealImplicitList :
        poResult = new types::ImplicitList();
        break;
    }
    return poResult;
}

types::InternalType* AddElementToVariableFromCol(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols, int *_piCols)
{
    types::InternalType *poResult	            = NULL;
    types::InternalType::RealType TypeSource	= ((types::GenericType*)_poSource)->getType();
    types::InternalType::RealType TypeDest		= types::InternalType::RealInternal;
    int iCurRow                                 = _iRows;
    int iCurCol                                 = _iCols;


    if(_poDest == NULL)
    {//First call, alloc _poSource
        poResult    = allocDest(_poSource, _iRows, _iCols);
        TypeDest	= TypeSource;
        iCurCol	    = 0;
        iCurRow		= 0;
    }
    else
    {
        TypeDest    = ((types::GenericType*)_poDest)->getType();
        poResult    = _poDest;
    }

    if(TypeDest != TypeSource)
    {//check if source type is compatible with dest type
    }
    else
    {
        switch(TypeDest)
        {
        case types::GenericType::RealDouble :
            if(poResult->getAsDouble()->isComplex() == false && _poSource->getAsDouble()->isComplex() == true)
            {
                poResult->getAsDouble()->complex_set(true);
            }

            poResult->getAsDouble()->fillFromCol(*_piCols, _poSource->getAsDouble());
            *_piCols += _poSource->getAsDouble()->cols_get();

            break;
        default:
            break;
        }
        return poResult;
    }
    return NULL;
}

types::InternalType* AddElementToVariableFromRow(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols, int *_piRows)
{
	types::InternalType *poResult	            = NULL;
	types::InternalType::RealType TypeSource	= ((types::GenericType*)_poSource)->getType();
	types::InternalType::RealType TypeDest		= types::InternalType::RealInternal;
	int iCurRow                                 = _iRows;
	int iCurCol                                 = _iCols;

    if(_poDest == NULL)
    {//First call, alloc _poSource
        poResult    = allocDest(_poSource, _iRows, _iCols);
        iCurCol	    = 0;
        iCurRow		= 0;
        TypeDest	= TypeSource;
    }
    else
    {
        TypeDest	= ((types::GenericType*)_poDest)->getType();
        poResult    = _poDest;
    }


    if(TypeDest != TypeSource)
    {//check if source type is compatible with dest type
    }
    else
    {
        switch(TypeDest)
        {
        case types::GenericType::RealDouble :
            if(poResult->getAsDouble()->isComplex() == false && _poSource->getAsDouble()->isComplex() == true)
            {
                poResult->getAsDouble()->complex_set(true);
            }

            poResult->getAsDouble()->fillFromRow(*_piRows, _poSource->getAsDouble());
            *_piRows += _poSource->getAsDouble()->rows_get();

            break;
        default:
            break;
        }
        return poResult;
    }
    return NULL;
}


/*
_iRows : Position if _poDest allready initialized else size of the matrix
_iCols : Position if _poDest allready initialized else size of the matrix
*/
types::InternalType* AddElementToVariable(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols, int *_piRows, int *_piCols)
{
	types::InternalType *poResult	= NULL;
	types::InternalType::RealType TypeSource	= ((types::GenericType*)_poSource)->getType();
	types::InternalType::RealType TypeDest		=	types::InternalType::RealInternal;
	int iCurRow = _iRows;
	int iCurCol = _iCols;

	if(_poDest == NULL)
	{
		switch(TypeSource)
		{
		case types::GenericType::RealDouble :
			poResult = new types::Double(_iRows, _iCols, false);
			break;
		case types::GenericType::RealBool :
			poResult = new types::Bool(_iRows, _iCols);
			break;
		case types::GenericType::RealInt :
			poResult = types::Int::createInt(_iRows, _iCols, _poSource->getAsInt()->getIntType());
			//poResult = new Int(_iRows, _iCols, _poSource->getAsInt()->getIntType());
			break;
		case types::GenericType::RealString :
			poResult = new types::String(_iRows, _iCols);
			break;
		case types::GenericType::RealPoly :
			{
				int* piRank = new int[_iRows * _iCols];
				for(int i = 0 ; i < _iRows * _iCols ; i++)
				{
					piRank[i] = 1;
				}
				poResult = new types::MatrixPoly(_poSource->getAsPoly()->var_get(), _iRows, _iCols, piRank);
				break;
			}
		case types::InternalType::RealImplicitList :
			poResult = new types::ImplicitList();
			break;
		}
		iCurCol		= 0;
		iCurRow		= 0;
		TypeDest	=	TypeSource;
	}
	else
	{
		TypeDest		= ((types::GenericType*)_poDest)->getType();
		poResult = _poDest;
	}


	if(TypeDest != TypeSource)
	{//check if source type is compatible with dest type
		switch(TypeDest)
		{
		case types::GenericType::RealDouble :
			if(TypeSource == types::GenericType::RealPoly)
			{
				types::Double *poDest = (types::Double*)_poDest;
				//Convert Dest to RealPoly
				int *piRank = new int[poDest->size_get()];
				for(int i = 0 ; i < poDest->size_get() ; i++)
				{
					piRank[i] = 1;
				}

				poResult = new types::MatrixPoly(((types::MatrixPoly*)_poSource)->var_get(), poDest->rows_get(), poDest->cols_get(),  piRank);

				double *pR = poDest->real_get();
				double *pI = poDest->img_get();
				for(int i = 0 ; i < poDest->size_get() ; i++)
				{
					types::Double *pdbl = NULL;
					if(poDest->isComplex())
					{
						pdbl = new types::Double(pR[i], pI[i]);
					}
					else
					{
						pdbl = new types::Double(pR[i]);
					}

					((types::MatrixPoly*)poResult)->poly_set(i, pdbl);
					delete pdbl;
				}

				((types::MatrixPoly*)poResult)->poly_set(iCurRow, iCurCol, ((types::MatrixPoly*)_poSource)->poly_get(0)->coef_get());
			}
			break;
		case types::GenericType::RealPoly :
			if(TypeSource == types::GenericType::RealDouble)
			{
				//Add Source like coef of the new element
				types::Poly* pPolyOut	= poResult->getAsPoly()->poly_get(iCurRow, iCurCol);

				pPolyOut->rank_set(1);
				pPolyOut->coef_set((types::Double*)_poSource);
			}
			break;
		default:
			break;
		}
		return poResult;
	}
	else
	{//Just add the new value in the current item
		switch(TypeDest)
		{
		case types::GenericType::RealDouble :
			if(poResult->getAsDouble()->isComplex() == false && _poSource->getAsDouble()->isComplex() == true)
			{
				poResult->getAsDouble()->complex_set(true);
			}

			if(_poSource->getAsDouble()->size_get() != 1)
			{
				poResult->getAsDouble()->append(iCurRow, iCurCol, _poSource->getAsDouble());
			}
			else
			{
				poResult->getAsDouble()->val_set(iCurRow, iCurCol, _poSource->getAsDouble()->real_get(0,0), _poSource->getAsDouble()->img_get(0,0));
			}

			*_piRows = _poSource->getAsDouble()->rows_get();
			*_piCols = _poSource->getAsDouble()->cols_get();
			break;
		case types::GenericType::RealPoly :
			{
				if(_poSource->getAsPoly()->isComplex())
				{
					poResult->getAsPoly()->complex_set(true);
				}

				if(_poSource->getAsPoly()->size_get() != 1)
				{
					poResult->getAsPoly()->insert(iCurRow, iCurCol, _poSource->getAsPoly());
				}
				else
				{
					types::Poly* pPolyOut	= poResult->getAsPoly()->poly_get(iCurRow, iCurCol);
					types::Poly* pPolyIn		= _poSource->getAsPoly()->poly_get(0);

					pPolyOut->rank_set(pPolyIn->rank_get());
					pPolyOut->coef_set(pPolyIn->coef_get());
				}

				*_piRows = _poSource->getAsPoly()->rows_get();
				*_piCols = _poSource->getAsPoly()->cols_get();
				break;
			}
		case types::GenericType::RealBool:
			poResult->getAsBool()->bool_set(iCurRow, iCurCol, _poSource->getAsBool()->bool_get(0,0));
			*_piRows = _poSource->getAsBool()->rows_get();
			*_piCols = _poSource->getAsBool()->cols_get();
			break;
		case types::GenericType::RealInt :
			//((Double*)poResult)->val_set(iCurRow, iCurCol, ((Double*)_poSource)->real_get(0,0), ((Double*)_poSource)->img_get(0,0));
			break;
		case types::GenericType::RealString :
			poResult->getAsString()->string_set(iCurRow, iCurCol, _poSource->getAsString()->string_get(0,0));
			*_piRows = _poSource->getAsString()->rows_get();
			*_piCols = _poSource->getAsString()->cols_get();
			break;
		case types::GenericType::RealImplicitList :
			{
				if(_poSource->getAsImplicitList()->start_type_get() == types::InternalType::RealPoly)
				{
					poResult->getAsImplicitList()->start_set(_poSource->getAsImplicitList()->start_get());
				}
				else
				{
					poResult->getAsImplicitList()->start_set(_poSource->getAsImplicitList()->start_get());
				}

				if(_poSource->getAsImplicitList()->step_type_get() == types::InternalType::RealPoly)
				{
					poResult->getAsImplicitList()->step_set(_poSource->getAsImplicitList()->step_get());
				}
				else
				{
					poResult->getAsImplicitList()->step_set(_poSource->getAsImplicitList()->step_get());
				}

				if(_poSource->getAsImplicitList()->end_type_get() == types::InternalType::RealPoly)
				{
					poResult->getAsImplicitList()->end_set(_poSource->getAsImplicitList()->end_get());
				}
				else
				{
					poResult->getAsImplicitList()->end_set(_poSource->getAsImplicitList()->end_get());
				}
				break;
			}
		default:
			break;
		}
		return poResult;
	}
	return NULL;
}

