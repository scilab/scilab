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

InternalType* AddElementToVariable(InternalType* _poDest, InternalType* _poSource, int _iRows, int _iCols, int *_piRows, int *_piCols);


namespace ast
{
	/*
	[1,2;3,4] with/without special character $ and :
	*/
	void ExecVisitor::visit (const MatrixExp &e)
	{
		try
		{
//			int iRows = e.lines_get().size();
//			int iCols	= -1;
			int iRows = 0;
			int iCols	= -1;
			int iCurRow = -1;
			int iCurCol = 0;
			InternalType *poResult = NULL;

			std::list<MatrixLineExp *>::const_iterator	row;
			std::list<Exp *>::const_iterator	col;
			//store all element after evaluation
			if(e.lines_get().size() == 0)
			{
				poResult = new Double(0,0);
			}
			else
			{
				list<list<InternalType*> > MatrixList;
				for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
				{
					list<InternalType*> RowList;
					for (col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; ++col)
					{
						ExecVisitor* execMe = new ast::ExecVisitor();
						(*col)->accept (*execMe);
						if(execMe->result_get()->getType() == InternalType::RealImplicitList)
						{
							if(execMe->result_get()->getAsImplicitList()->computable() == true)
							{
								execMe->result_set(execMe->result_get()->getAsImplicitList()->extract_matrix());
								iCurCol += ((GenericType*)execMe->result_get())->cols_get();
							}
							else
							{
								iCurCol++;
							}
						}
						else
						{
							iCurCol += ((GenericType*)execMe->result_get())->cols_get();
						}

						if(iCurRow == -1)
						{
							iCurRow = ((GenericType*)execMe->result_get())->rows_get();
						}
						else if(iCurRow != ((GenericType*)execMe->result_get())->rows_get())
						{
							std::ostringstream os;
							os << "inconsistent row/column dimensions";
							os << ((Location)(*row)->location_get()).location_string_get() << std::endl;
							throw os.str();
						}

						InternalType *pResult = execMe->result_get();
						RowList.push_back(pResult);

						//tips to delete only execvisitor but not data
						pResult->IncreaseRef();
						delete execMe;
						pResult->DecreaseRef();
					}

					if(iCols <= 0)
					{
						iCols = iCurCol;
					}
					else if(iCols != 0 && iCols != iCurCol)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << ((Location)(*row)->location_get()).location_string_get() << std::endl;
						throw os.str();
					}

					iRows += iCurRow;
					iCurCol = 0;
					iCurRow = -1;
					MatrixList.push_back(RowList);
				}

				list<list<InternalType*> >::const_iterator it_ML;
				list<InternalType*>::const_iterator it_RL;

				int iAddRow = 0;
				iCurRow			= 0;
				iCurCol			= 0;
				for(it_ML = MatrixList.begin() ; it_ML != MatrixList.end() ; it_ML++)
				{
					int iAddCol = 0;
					for(it_RL = (*it_ML).begin() ; it_RL != (*it_ML).end() ; it_RL++)
					{
						if(poResult == NULL)
						{
							poResult = AddElementToVariable(poResult, *it_RL, iRows, iCols, &iAddRow, &iAddCol);

							if((*it_RL)->isDeletable() == true)
							{
								if((*it_RL)->getType() == InternalType::RealDouble)
								{
									delete (*it_RL)->getAsDouble();
								}
								else
								{
									delete (*it_RL);
								}
							}
							iCurCol += iAddCol;
						}
						else
						{
							poResult = AddElementToVariable(poResult, *it_RL, iCurRow, iCurCol, &iAddRow, &iAddCol);
							if((*it_RL)->isDeletable() == true)
							{
								if((*it_RL)->getType() == InternalType::RealDouble)
								{
									delete (*it_RL)->getAsDouble();
								}
								else
								{
									delete (*it_RL);
								}
							}
							iCurCol += iAddCol;
						}
					}
					iCurRow += iAddRow;
					iCurCol = 0;
				}
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
InternalType* AddElementToVariable(InternalType* _poDest, InternalType* _poSource, int _iRows, int _iCols, int *_piRows, int *_piCols)
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
		case GenericType::RealInt :
			poResult = Int::createInt(_iRows, _iCols, _poSource->getAsInt()->getIntType());
			//poResult = new Int(_iRows, _iCols, _poSource->getAsInt()->getIntType());
			break;
		case GenericType::RealString :
			poResult = new String(_iRows, _iCols);
			break;
		case GenericType::RealObject :
			poResult = new ObjectMatrix(_iRows, _iCols);
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
		case GenericType::RealPoly :
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
					Poly* pPolyOut	= poResult->getAsPoly()->poly_get(iCurRow, iCurCol);
					Poly* pPolyIn		= _poSource->getAsPoly()->poly_get(0);

					pPolyOut->rank_set(pPolyIn->rank_get());
					pPolyOut->coef_set(pPolyIn->coef_get());
				}

				*_piRows = _poSource->getAsPoly()->rows_get();
				*_piCols = _poSource->getAsPoly()->cols_get();
				break;
			}
		case GenericType::RealBool:
			poResult->getAsBool()->bool_set(iCurRow, iCurCol, _poSource->getAsBool()->bool_get(0,0));
			*_piRows = _poSource->getAsBool()->rows_get();
			*_piCols = _poSource->getAsBool()->cols_get();
			break;
		case GenericType::RealInt :
			//((Double*)poResult)->val_set(iCurRow, iCurCol, ((Double*)_poSource)->real_get(0,0), ((Double*)_poSource)->img_get(0,0));
			break;
		case GenericType::RealString :
			poResult->getAsString()->string_set(iCurRow, iCurCol, _poSource->getAsString()->string_get(0,0));
			*_piRows = _poSource->getAsString()->rows_get();
			*_piCols = _poSource->getAsString()->cols_get();
			break;
		case GenericType::RealObject :
			poResult->getAsObject()->Insert(iCurRow, iCurCol, _poSource->getAsObject());
			*_piRows = dynamic_cast<GenericType*>(_poSource)->rows_get();
			*_piCols = dynamic_cast<GenericType*>(_poSource)->cols_get();
			break;
		case GenericType::RealImplicitList :
			{
				if(_poSource->getAsImplicitList()->start_type_get() == InternalType::RealPoly)
				{
					poResult->getAsImplicitList()->start_set(_poSource->getAsImplicitList()->start_get());
				}
				else
				{
					poResult->getAsImplicitList()->start_set(_poSource->getAsImplicitList()->start_get());
				}

				if(_poSource->getAsImplicitList()->step_type_get() == InternalType::RealPoly)
				{
					poResult->getAsImplicitList()->step_set(_poSource->getAsImplicitList()->step_get());
				}
				else
				{
					poResult->getAsImplicitList()->step_set(_poSource->getAsImplicitList()->step_get());
				}

				if(_poSource->getAsImplicitList()->end_type_get() == InternalType::RealPoly)
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

