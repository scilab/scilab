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

#include "originalvisitor.hxx"
#include "visitor_common.hxx"

using std::string;
using namespace types;

namespace ast
{
	/*
	[1,2;3,4] with/without special character $ and :
	*/
	void OriginalVisitor::visit (const MatrixExp &e)
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
				vector<vector<InternalType*> > MatrixList;
				for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
				{
					vector<InternalType*> RowList;
					for (col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; ++col)
					{
						OriginalVisitor execMe;
						(*col)->accept(execMe);
						if(execMe.result_get()->getType() == InternalType::RealImplicitList)
						{
							if(execMe.result_get()->getAsImplicitList()->computable() == true)
							{
								execMe.result_set(execMe.result_get()->getAsImplicitList()->extract_matrix());
								iCurCol += ((GenericType*)execMe.result_get())->cols_get();
							}
							else
							{
								iCurCol++;
							}
						}
						else
						{
							iCurCol += ((GenericType*)execMe.result_get())->cols_get();
						}

						if(iCurRow == -1)
						{
							iCurRow = ((GenericType*)execMe.result_get())->rows_get();
						}
						else if(iCurRow != ((GenericType*)execMe.result_get())->rows_get())
						{
							std::ostringstream os;
							os << "inconsistent row/column dimensions";
							os << ((Location)(*row)->location_get()).location_string_get() << std::endl;
							throw os.str();
						}

						InternalType *pResult = execMe.result_get();
						RowList.push_back(pResult);

						//tips to delete only visitor but not data
						pResult->IncreaseRef();
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

				vector<vector<InternalType*> >::const_iterator it_ML;
				vector<InternalType*>::const_iterator it_RL;

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
							//tips to allow delete data
							(*it_RL)->DecreaseRef();
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

