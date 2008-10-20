/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "execvisitor.hxx"
#include "context.hxx"
#include <time.h>
#include <string>
#include <sstream>

using std::string;

namespace ast
{
	static int level = -1;

	static void DEBUG_START_NODE(void)
	{
		++level;
	}

	static void DEBUG_END_NODE(void)
	{
		--level;
	}

	static void DEBUG(string str)
	{
		for(int i = 0 ; i < level; ++i)
		{
			std::cerr << "  ";
		}
		if (level > 0)
		{
			std::cerr << "|_./ ";
		}
		std::cerr << str << std::endl;
	}


	void ExecVisitor::visit (const MatrixExp &e)
	{
		try
		{
			int iRows = e.lines_get().size();
			int iCols	= -1;
			int iCurRow = 0;
			int iCurCol = 0;
			InternalType::RealType VarType = GenericType::RealDouble;
			InternalType *First	= NULL;
			InternalType *pData = NULL;

			std::list<MatrixLineExp *>::const_iterator	row;
			for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
			{
				std::list<Exp *>::const_iterator	col;

				if(iCols != -1 && iCols != (*row)->columns_get().size())
				{
					std::ostringstream os;
					os << "inconsistent row/column dimensions";
					os << " (" << (*row)->location_get().first_line << "," << (*row)->location_get().first_column << ")" << std::endl;
					string szErr(os.str());
					throw szErr;
				}

				for (col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; ++col)
				{
					ExecVisitor* execMe = new ast::ExecVisitor();
					(*col)->accept (*execMe);

					if(row == e.lines_get().begin() && col == (*row)->columns_get().begin()) //first occurence
					{
						First = execMe->result_get();
						VarType = ((GenericType*)First)->getType();
						iCols = (*row)->columns_get().size();
						/*Allow data*/
						if(VarType == GenericType::RealDouble)
						{
							pData = new Double(iRows, iCols, true);
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

					if(VarType != ((GenericType*)execMe->result_get())->getType())
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
					iCurCol++;
					delete execMe;
				}
				iCurCol = 0;
				iCurRow++;
			}
			result_set(pData);
		}
		catch(string sz)
		{
			throw sz;
		}

	}

	void ExecVisitor::visit (const MatrixLineExp &e)
	{
	}
	/** \} */

	void ExecVisitor::visit (const CellExp &e)
	{
	  // FIXME : Must be a container for all Types.
		try
		{
			int iRows = e.lines_get().size();
			int iCols	= -1;
			int iCurRow = 0;
			int iCurCol = 0;
			GenericType::RealType VarType = GenericType::RealDouble;
			GenericType *First	= NULL;
			GenericType *pData = NULL;

			std::list<CellLineExp *>::const_iterator	row;
			for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
			{
				std::list<Exp *>::const_iterator	col;

				if(iCols != -1 && iCols != (*row)->columns_get().size())
				{
					std::ostringstream os;
					os << "inconsistent row/column dimensions";
					os << " (" << (*row)->location_get().first_line << "," << (*row)->location_get().first_column << ")" << std::endl;
					string szErr(os.str());
					throw szErr;
				}

				for (col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; ++col)
				{
					ExecVisitor execMe = *new ast::ExecVisitor();
					(*col)->accept (execMe);

					if(row == e.lines_get().begin() && col == (*row)->columns_get().begin()) //first occurence
					{
						First = (GenericType*)execMe.result_get();
						VarType = First->getType();
						iCols = (*row)->columns_get().size();
						/*Allow data*/
						if(VarType == GenericType::RealDouble)
						{
							pData = new Double(iRows, iCols, true);
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

					if(VarType != execMe.result_get()->getType())
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
						((Double*)pData)->val_set(iCurRow, iCurCol, ((Double*)execMe.result_get())->real_get(0,0), ((Double*)execMe.result_get())->img_get(0,0));
					}
					else if(VarType == GenericType::RealBool)
					{
						((Bool*)pData)->bool_set(iCurRow, iCurCol, ((Bool*)execMe.result_get())->bool_get(0,0));
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
						((String*)pData)->string_set(iCurRow, iCurCol, ((String*)execMe.result_get())->string_get(0,0));
					}
					iCurCol++;
				}
				iCurCol = 0;
				iCurRow++;
			}
			result_set(pData);
		}
		catch(string sz)
		{
			throw sz;
		}

	}

	void ExecVisitor::visit (const CellLineExp &e)
	{
	}
	/** \} */



	/** \name Visit Constant Expressions nodes.
	** \{ */
	void ExecVisitor::visit (const StringExp &e)
	{
		String *psz = new String(e.value_get().c_str());
		result_set(psz);
	}

	void ExecVisitor::visit (const CommentExp &e)
	{
	}

	void ExecVisitor::visit (const IntExp  &e)
	{
	}

	void ExecVisitor::visit (const FloatExp  &e)
	{
		std::ostringstream stream;
		stream << e.value_get();
	}

	void ExecVisitor::visit (const DoubleExp  &e)
	{
		Double *pdbl = new Double(e.value_get());
		result_set(pdbl);
	}

	void ExecVisitor::visit (const BoolExp  &e)
	{
		Bool *pb = new Bool(e.value_get());
		result_set(pb);
	}

	void ExecVisitor::visit (const NilExp &e)
	{
	}

	void ExecVisitor::visit (const SimpleVar &e)
	{
		result_set(symbol::Context::getInstance()->get(e.name_get()));
	}

	void ExecVisitor::visit (const ColonVar &e)
	{
	}

	void ExecVisitor::visit (const DollarVar &e)
	{
	}

	void ExecVisitor::visit (const ArrayListVar &e)
	{
	}

	void ExecVisitor::visit (const FieldExp &e)
	{
	}

	void ExecVisitor::visit (const AssignExp  &e)
	{
		/*Create local exec visitor*/
		ExecVisitor* execMe = new ast::ExecVisitor();
		try
		{
			/*getting what to assign*/
			e.right_exp_get().accept(*execMe);

			/*get symbol*/
			const SimpleVar *Var = dynamic_cast<const SimpleVar*>(&e.left_exp_get());
			
			InternalType *pVar	=	execMe->result_get();
			if(pVar->isList())
			{
				double *pData = NULL;
				Double *pTemp = new Double(((ImplicitList*)pVar)->size_get(),1, &pData);
				((ImplicitList*)pVar)->extract_matrix(pData);
				delete pVar;
				pVar = pTemp;
			}

			symbol::Context::getInstance()->put(Var->name_get(), *((GenericType*)pVar));
		}
		catch(string sz)
		{
			throw sz;
		}
		delete execMe;
	}

	void ExecVisitor::visit(const CallExp &e)
	{
		ExecVisitor *execMe = new ast::ExecVisitor();
		std::list<Exp *>::const_iterator	i;
		for (i = e.args_get().begin (); i != e.args_get().end (); ++i)
		{
			(*i)->accept (*this);
		}

		delete execMe;
	}

	void ExecVisitor::visit (const IfExp  &e)
	{
			//Create local exec visitor
			ExecVisitor *execMeTest		= new ast::ExecVisitor();
			ExecVisitor *execMeAction = new ast::ExecVisitor();
			bool bTestStatus					= false;

			//condition
			e.test_get().accept(*execMeTest);
			if(((GenericType*)execMeTest->result_get())->isDouble() && ((Double*)execMeTest->result_get())->isComplex() == false)
			{
				bTestStatus		= true; // default value in this case
				Double *pR		= (Double*)execMeTest->result_get();
				double *pReal	= pR->real_get();

				for(int i = 0 ; i < pR->size_get() ; i++)
				{
					if(pReal[i] == 0)
					{
						bTestStatus = false;
						break;
					}
				}
			}
			else if(((GenericType*)execMeTest->result_get())->isBool())
			{
				bTestStatus		= true; // default value in this case
				Bool *pB		= (Bool*)execMeTest->result_get();
				bool *pData		= pB->bool_get();

				for(int i = 0 ; i < pB->size_get() ; i++)
				{
					if(pData[i] == false)
					{
						bTestStatus = false;
						break;
					}
				}
			}
			else if(((GenericType*)execMeTest->result_get())->isUInt())
			{
			}
			else if(((GenericType*)execMeTest->result_get())->isInt())
			{
			}
			else
			{
					std::ostringstream os;
					os << "first argument is incorrect";
					os << " (" << e.test_get().location_get().first_line << "," << e.test_get().location_get().first_column << ")" << std::endl;
					string szErr(os.str());
					throw szErr;
			}

			if(bTestStatus == true)
			{//condition == true
				e.then_get().accept(*execMeTest);
			}
			else
			{//condition == false
				e.else_get().accept(*execMeTest);
			}
		delete execMeAction;
		delete execMeTest;
	}

	void ExecVisitor::visit (const TryCatchExp  &e)
	{
	}

	void ExecVisitor::visit (const WhileExp  &e)
	{
	}

	void ExecVisitor::visit (const ForExp  &e)
	{
		ExecVisitor *execMe = new ast::ExecVisitor();
		e.vardec_get().accept(*execMe);

		if(execMe->result_get()->getType() == InternalType::RealImplicitList)
		{
			ImplicitList* pVar = (ImplicitList*)execMe->result_get();
			for(int i = 0 ; i < pVar->size_get() ; i++)
			{
				double dblVal = pVar->extract_value(i);
				Double *pdbl = new Double(dblVal);
				symbol::Context::getInstance()->put(e.vardec_get().name_get(), *(GenericType*)pdbl);
				e.body_get().accept(*execMe);
			}
		}
		else
		{
		}

		delete execMe;
	}

	void ExecVisitor::visit (const BreakExp &e)
	{
	}

	void ExecVisitor::visit (const ReturnExp &e)
	{
	}

	void ExecVisitor::visit (const SeqExp  &e)
	{
		ExecVisitor *execMe = new ast::ExecVisitor();
/*		std::list<Exp *>::const_iterator	i;

		std::list<Exp *> pList	= e.exps_get();
		while(pList.size())
		{
			i = pList.begin();
			(*i)->accept (*execMe);
			//delete (*i);
			//pList.erase(i);
		}
*/
		std::list<Exp *>::const_iterator	i;
		for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
		{
			(*i)->accept (*this);
		}
		delete execMe;
	}

	void ExecVisitor::visit (const ArrayListExp  &e)
	{
	}
	/** \} */

	/** \name Visit Single Operation nodes.
	** \{ */
	void ExecVisitor::visit (const NotExp &e)
	{
	}

	void ExecVisitor::visit (const TransposeExp &e)
	{
	}
	/** \} */

	/** \name Visit Declaration nodes.
	** \{ */
	/** \brief Visit Var declarations. */
	void ExecVisitor::visit (const VarDec  &e)
	{
		/*Create local exec visitor*/
		ExecVisitor* execMe = new ast::ExecVisitor();
		try
		{
			/*getting what to assign*/
			e.init_get().accept(*execMe);
			result_set(execMe->result_get());
		}
		catch(string sz)
		{
			throw sz;
		}
		delete execMe;
	}

	void ExecVisitor::visit (const FunctionDec  &e)
	{
	}
	/** \} */

	/** \name Visit Type dedicated Expressions related node.
	** \{ */
	void ExecVisitor::visit(const ListExp &e)
	{
			ExecVisitor*	execMe = new ast::ExecVisitor();

			e.start_get().accept(*execMe);
			Double *pStart	= (Double*)execMe->result_get();

			e.step_get().accept(*execMe);
			Double *pStep		= (Double*)execMe->result_get();

			e.end_get().accept(*execMe);
			Double *pEnd		= (Double*)execMe->result_get();

			ImplicitList *pIL	= new ImplicitList(pStart->real_get(0,0), pStep->real_get(0,0), pEnd->real_get(0,0));
			result_set(pIL);
			delete execMe;
	}
	/** \} */

	InternalType	*ExecVisitor::result_get(void)
	{
		return _result;
	}

	void ExecVisitor::result_set(const InternalType *e)
	{
		_result = (InternalType *)e;
	}

}
