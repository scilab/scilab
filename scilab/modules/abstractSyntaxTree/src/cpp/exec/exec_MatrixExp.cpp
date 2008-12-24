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

#include "execvisitor.hxx"

using std::string;

InternalType* AddElementToVariable(InternalType* _poDest, InternalType* _poSource, int _iRows, int _iCols);


namespace ast
{
	/*
	[1,2;3,4] with/without special character $ and :
	*/
	void ExecVisitor::visit (const MatrixExp &e)
	{
		try
		{
			int iRows = e.lines_get().size();
			int iCols	= -1;
			int iCurRow = 0;
			int iCurCol = 0;
			InternalType *poResult = NULL;

			std::list<MatrixLineExp *>::const_iterator	row;
			for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
			{
				std::list<Exp *>::const_iterator	col;

				//check if Cols size is always the same
				if(iCols != -1 && iCols != (*row)->columns_get().size())
				{
					std::ostringstream os;
					os << "inconsistent row/column dimensions";
					os << " (" << (*row)->location_get().first_line << "," << (*row)->location_get().first_column << ")" << std::endl;
					string szErr(os.str());
					throw szErr;
				}
				//store Cols size
				iCols = (*row)->columns_get().size();

				for (col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; ++col)
				{
					ExecVisitor* execMe = new ast::ExecVisitor();
					(*col)->accept (*execMe);

					if(poResult == NULL)
					{
						poResult = AddElementToVariable(poResult, execMe->result_get(), iRows, iCols);
					}
					else
					{
						poResult = AddElementToVariable(poResult, execMe->result_get(), iCurRow, iCurCol);
					}
/*
					if(row == e.lines_get().begin() && col == (*row)->columns_get().begin()) //first occurence
					{
						First = execMe->result_get();
						VarType = ((GenericType*)First)->getType();
						//Allow data
						if(VarType == GenericType::RealDouble)
						{
							pData = new Double(iRows, iCols, false);
						}
						else if(VarType == GenericType::RealBool)
						{
							pData = new Bool(iRows, iCols);
						}
						else if(VarType == GenericType::RealUInt)
						{
							pData = new UInt(iRows, iCols);
						}
						else if(VarType == GenericType::RealInt)
						{
							pData = new Int(iRows, iCols);
						}
						else if(VarType == GenericType::RealString)
						{
							pData = new String(iRows, iCols);
						}
					}

					if(((GenericType*)execMe->result_get())->getType() == GenericType::RealPoly)
					{
					}
					else if(VarType != ((GenericType*)execMe->result_get())->getType())
					{
						std::ostringstream os;
						Location loc = (*col)->location_get();
						os << "Incompatible types ";
						os << loc.location_string_get() << std::endl;
						string szErr(os.str());
						throw szErr;
					}

					if(VarType == GenericType::RealDouble)
					{
						if(((Double*)pData)->isComplex() == false && ((Double*)execMe->result_get())->img_get(0,0) != 0)
						{
							((Double*)pData)->complex_set(true);
						}
						((Double*)pData)->val_set(iCurRow, iCurCol, ((Double*)execMe->result_get())->real_get(0,0), ((Double*)execMe->result_get())->img_get(0,0));
					}
					else if(VarType == GenericType::RealBool)
					{
						((Bool*)pData)->bool_set(iCurRow, iCurCol, ((Bool*)execMe->result_get())->bool_get(0,0));
					}
					else if(VarType == GenericType::RealUInt)
					{
						//((Double*)pData)->val_set(iCurRow, iCurCol, ((Double*)execMe.result_get())->real_get(0,0), ((Double*)execMe.result_get())->img_get(0,0));
					}
					else if(VarType == GenericType::RealInt)
					{
						//((Double*)pData)->val_set(iCurRow, iCurCol, ((Double*)execMe.result_get())->real_get(0,0), ((Double*)execMe.result_get())->img_get(0,0));
					}
					else if(VarType == GenericType::RealString)
					{
						((String*)pData)->string_set(iCurRow, iCurCol, ((String*)execMe->result_get())->string_get(0,0));
					}
*/
					iCurCol++;
					delete execMe;
				}
				iCurCol = 0;
				iCurRow++;
			}
			result_set(poResult);
		}
		catch(string sz)
		{
			throw sz;
		}
	}
}


/*
_iRows : Position if _poDest allready initialized else size of the matrix
_iCols : Position if _poDest allready initialized else size of the matrix
*/
InternalType* AddElementToVariable(InternalType* _poDest, InternalType* _poSource, int _iRows, int _iCols)
{
	InternalType *poResult	= NULL;
	InternalType::RealType TypeSource	= ((GenericType*)_poSource)->getType();
	InternalType::RealType TypeDest		=	InternalType::RealInternal;
	int iCurRow = _iRows;
	int iCurCol = _iCols;

	if(_poDest == NULL)
	{
		switch(TypeSource)
		{
		case GenericType::RealDouble :
			poResult = new Double(_iRows, _iCols, false);
			break;
		case GenericType::RealBool :
			poResult = new Bool(_iRows, _iCols);
			break;
		case GenericType::RealUInt :
			poResult = new UInt(_iRows, _iCols);
			break;
		case GenericType::RealInt :
			poResult = new Int(_iRows, _iCols);
			break;
		case GenericType::RealString :
			poResult = new String(_iRows, _iCols);
			break;
		case GenericType::RealPoly :
			{
				int* piRank = new int[_iRows * _iCols];
				for(int i = 0 ; i < _iRows * _iCols ; i++)
				{
					piRank[i] = 1;
				}
				poResult = new MatrixPoly(_poSource->getAsPoly()->var_get(), _iRows, _iCols, piRank);
				break;
			}
		case InternalType::RealImplicitList :
			poResult = new ImplicitList();
			break;
		}
		iCurCol		= 0;
		iCurRow		= 0;
		TypeDest	=	TypeSource;
	}
	else
	{
		TypeDest		= ((GenericType*)_poDest)->getType();
		poResult = _poDest;
	}


	if(TypeDest != TypeSource)
	{//check if source type is compatible with the old one
		switch(TypeDest)
		{
		case GenericType::RealDouble :
			if(TypeSource == GenericType::RealPoly)
			{
				Double *poDest = (Double*)_poDest;
				//Convert Dest to RealPoly
				int *piRank = new int[poDest->size_get()];
				for(int i = 0 ; i < poDest->size_get() ; i++)
				{
					piRank[i] = 1;
				}

				poResult = new MatrixPoly(((MatrixPoly*)_poSource)->var_get(), poDest->rows_get(), poDest->cols_get(),  piRank);

				double *pR = poDest->real_get();
				double *pI = poDest->img_get();
				for(int i = 0 ; i < poDest->size_get() ; i++)
				{
					Double *pdbl = NULL;
					if(poDest->isComplex())
					{
						pdbl = new Double(pR[i], pI[i]);
					}
					else
					{
						pdbl = new Double(pR[i]);
					}

					((MatrixPoly*)poResult)->poly_set(i, pdbl);
					delete pdbl;
				}

				((MatrixPoly*)poResult)->poly_set(iCurRow, iCurCol, ((MatrixPoly*)_poSource)->poly_get(0)->coef_get());
			}
			break;
		case GenericType::RealPoly :
			if(TypeSource == GenericType::RealDouble)
			{
				//Add Source like coef of the new element
				Poly* pPolyOut	= poResult->getAsPoly()->poly_get(iCurRow, iCurCol);

				pPolyOut->rank_set(1);
				pPolyOut->coef_set((Double*)_poSource);
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
		case GenericType::RealDouble :
			if(poResult->getAsDouble()->isComplex() == false && _poSource->getAsDouble()->img_get(0,0) != 0)
			{
				poResult->getAsDouble()->complex_set(true);
			}
			poResult->getAsDouble()->val_set(iCurRow, iCurCol, _poSource->getAsDouble()->real_get(0,0), _poSource->getAsDouble()->img_get(0,0));
			break;
		case GenericType::RealPoly :
			{
/*
				Double *pDbl = _poSource->getAsPoly()->poly_get(0)->coef_get();
				pDbl->toString(10,100);
				_poDest->getAsPoly()->poly_set(iCurRow, iCurCol, _poSource->getAsPoly()->poly_get(0)->coef_get());
*/

				Poly* pPolyIn		= _poSource->getAsPoly()->poly_get(0);
				if(_poSource->getAsPoly()->isComplex())
				{
					poResult->getAsPoly()->complex_set(true);
				}
				Poly* pPolyOut	= poResult->getAsPoly()->poly_get(iCurRow, iCurCol);

				pPolyOut->rank_set(pPolyIn->rank_get());
				pPolyOut->coef_set(pPolyIn->coef_get());

				break;
			}
		case GenericType::RealBool:
			poResult->getAsBool()->bool_set(iCurRow, iCurCol, _poSource->getAsBool()->bool_get(0,0));
			break;
		case GenericType::RealUInt:
			//((Double*)poResult)->val_set(iCurRow, iCurCol, ((Double*)_poSource)->real_get(0,0), ((Double*)_poSource)->img_get(0,0));
			break;
		case GenericType::RealInt :
			//((Double*)poResult)->val_set(iCurRow, iCurCol, ((Double*)_poSource)->real_get(0,0), ((Double*)_poSource)->img_get(0,0));
			break;
		case GenericType::RealString :
			poResult->getAsString()->string_set(iCurRow, iCurCol, _poSource->getAsString()->string_get(0,0));
			break;
		case GenericType::RealImplicitList :
			{
				if(_poSource->getAsList()->start_type_get() == InternalType::RealPoly)
				{
					poResult->getAsList()->start_set(_poSource->getAsList()->start_poly_get());
				}
				else
				{
					poResult->getAsList()->start_set(_poSource->getAsList()->start_get());
				}

				if(_poSource->getAsList()->step_type_get() == InternalType::RealPoly)
				{
					poResult->getAsList()->step_set(_poSource->getAsList()->step_poly_get());
				}
				else
				{
					poResult->getAsList()->step_set(_poSource->getAsList()->step_get());
				}

				if(_poSource->getAsList()->end_type_get() == InternalType::RealPoly)
				{
					poResult->getAsList()->end_set(_poSource->getAsList()->end_poly_get());
				}
				else
				{
					poResult->getAsList()->end_set(_poSource->getAsList()->end_get());
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

