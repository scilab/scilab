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

#include "localization.h"
#include "execvisitor.hxx"
#include "context.hxx"
#include <string>

#include "yaspio.hxx"

using std::string;
using ast::Exp;
using ast::ExecVisitor;

namespace ast
{
	void ExecVisitor::visit (const AssignExp  &e)
	{
		symbol::Context *pcontext = symbol::Context::getInstance();
		/*Create local exec visitor*/
		ExecVisitor* execMeR = new ExecVisitor();
		try
		{
			/*get king of left hand*/
			const AssignListExp *pList	= dynamic_cast<const AssignListExp*>(&e.left_exp_get());
			const CallExp *pCall		= dynamic_cast<const CallExp*>(&e.left_exp_get());
			const SimpleVar *pVar 		= NULL, 
			                *propName 	= NULL;
			ObjectMatrix *object		= NULL;
			
			/*get data*/
			InternalType *dataOld, *dataNew	= NULL;
			bool bNew			= false;
			bool bRet			= true;
			if(pList == NULL)
			{
				// To sum up, 5 possibilities for the left expression:
				//  (1) The modified object is a variable or, eg a (2) an object/structure property, eg a.b
				//  (3) The modified object is fully modified or (3) only a subscript of the matrix is modified
				//  (5) The modified object is a list of full variables.
				//  (1) => pVar != NULL, object = NULL, propName = NULL; (2) => the opposite
				//  (3) => pCall = NULL; (4) => pCall != NULL
				//  (5) => pList != NULL, pVar = pCall = object = propname = NULL
				const Exp *ref;
				if(pCall != NULL)
				{ // e = foo(...) => pCall = e, ref = foo
					ref = &pCall->name_get();
				}
				else
				{ // e = var => pCall = NULL, ref = e
					ref = &e.left_exp_get();
				}
				
				pVar = dynamic_cast<const SimpleVar*>(ref);
				if(pVar != NULL)
				{ // ref = foo => pVar = Foo, object & propName = NULL
					object = NULL;
					propName = NULL;
					dataOld = symbol::Context::getInstance()->get(pVar->name_get());
				}
				else
				{ // ref = foo.bar => pVar = NULL, object = foo, propName = bar
					ExecVisitor execMeF;
					
					const FieldExp *field = dynamic_cast<const FieldExp*>(ref);
					field->head_get()->accept(execMeF);
					
					object = dynamic_cast<ObjectMatrix*>(execMeF.result_get());
					propName = dynamic_cast<const SimpleVar*>(field->tail_get());
					
					if(pCall != NULL)
						dataOld = object->Get(propName->name_get().name_get());
				}
			}
			
			if(pCall)
			{//x(?) = ?
				int iProductElem				= (int)pCall->args_get().size();
				InternalType *pIT				= dataOld;
				bool bSeeAsVector				= iProductElem == 1;

				/*getting what to assign*/
				e.right_exp_get().accept(*execMeR);
				if(pIT == NULL)
				{//Var doesn't exist, create it with good dimensions
					bNew = true;
				}
				else
				{
					if(pIT->isRef(1) == true)
					{
						pIT = pIT->clone();
						bNew = true;
					}
				}

				int *piIndexSeq		= NULL;
				int *piMaxDim			= NULL;
				int *piDimSize			= new int[iProductElem];
				int iTotalCombi		= GetIndexList(pCall->args_get(), &piIndexSeq, &piMaxDim, pIT, piDimSize);
				/*We have the indexlist expanded and the max index*/

				InternalType *pOut	= NULL;

				//fisrt extract implicit list
				if(execMeR->result_get()->getType() == InternalType::RealImplicitList)
				{
					InternalType *pIL = execMeR->result_get()->getAsImplicitList()->extract_matrix();
					execMeR->result_set(pIL);
				}

				if(pIT == NULL)
				{//call static insert function
					switch(execMeR->result_get()->getType())
					{
					case InternalType::RealDouble : 
						pOut = Double::insert_new(iTotalCombi, piIndexSeq, piMaxDim, execMeR->result_get()->getAsDouble(), bSeeAsVector);
						break;
					case InternalType::RealBool : 
						pOut = Bool::insert_new(iTotalCombi, piIndexSeq, piMaxDim, execMeR->result_get()->getAsBool(), bSeeAsVector);
						break;
					case InternalType::RealString : 
						pOut = String::insert_new(iTotalCombi, piIndexSeq, piMaxDim, execMeR->result_get()->getAsString(), bSeeAsVector);
						break;
					case InternalType::RealInt : 
						pOut = Int::insert_new(iTotalCombi, piIndexSeq, piMaxDim, execMeR->result_get()->getAsInt(), bSeeAsVector);
						break;
					default : 
						//TOTO YaSp : overlaoding insertion
						break;
					}
				}
				else
				{//call type insert function
					switch(pIT->getType())
					{
					case InternalType::RealDouble : 
						bRet = pIT->getAsDouble()->insert(iTotalCombi, piIndexSeq, piMaxDim, (GenericType*)execMeR->result_get(), bSeeAsVector);
						break;
					case InternalType::RealBool : 
						bRet = pIT->getAsBool()->insert(iTotalCombi, piIndexSeq, piMaxDim, (GenericType*)execMeR->result_get(), bSeeAsVector);
						break;
					case InternalType::RealString : 
						bRet = pIT->getAsString()->insert(iTotalCombi, piIndexSeq, piMaxDim, (GenericType*)execMeR->result_get(), bSeeAsVector);
						break;
					case InternalType::RealInt : 
						bRet = pIT->getAsInt()->insert(iTotalCombi, piIndexSeq, piMaxDim, (GenericType*)execMeR->result_get(), bSeeAsVector);
						break;
					default : 
						//TOTO YaSp : overlaoding insertion
						break;
					}

					pOut = pIT;
				}

				if(pOut != NULL && bRet == true)
				{
					dataNew = pOut;
				}
				else
				{
					//manage error
					std::ostringstream os;
					os << _("Submatrix incorrectly defined.\n");
					os << ((Location)e.right_exp_get().location_get()).location_string_get() << std::endl;
					throw os.str();
				}
				delete piMaxDim;
				delete[] piDimSize;
			}
			else if(pVar || propName)
			{// x = ?
				/*getting what to assign*/
				execMeR->expected_size_set(1);
				e.right_exp_get().accept(*execMeR);

				if(execMeR->result_size_get() != 1)
				{
					std::ostringstream os;
					os << "Lhs != Rhs";
					os << ((Location)e.right_exp_get().location_get()).location_string_get() << std::endl;
					throw os.str();
				}
				
				InternalType *pIT	=	execMeR->result_get();
				if(pIT->isImplicitList())
				{
					InternalType *pTemp = ((ImplicitList*)pIT)->extract_matrix();
					delete pIT;
					pIT = pTemp;
				}

				const ReturnExp *pReturn = dynamic_cast<const ReturnExp*>(&e.right_exp_get());
				if(pReturn)
				{//ReturnExp so, put the value in the previous scope
					symbol::Context::getInstance()->put_in_previous_scope(pVar->name_get(), *((GenericType*)pIT));
					((AssignExp*)&e)->break_set();
				}
				else
				{
					symbol::Context::getInstance()->put(pVar->name_get(), *((GenericType*)pIT));
				}

				if(e.is_verbose())
				{
					std::ostringstream ostr;
					ostr << pVar->name_get() << " = " << std::endl;
					ostr << std::endl;
					ostr << pIT->toString(10,75) << std::endl;
					YaspWrite((char *)ostr.str().c_str());
				}
			}
			else if(pList)
			{//[x,y] = ?

				int iLhsCount = (int)pList->exps_get().size();

				/*getting what to assign*/
				execMeR->expected_size_set(iLhsCount);
				e.right_exp_get().accept(*execMeR);

				if(execMeR->result_size_get() != execMeR->expected_size_get())
				{
					std::ostringstream os;
					os << "Lhs != Rhs";
					throw os.str();
				}


				std::list<Exp *>::const_reverse_iterator it;
				int i = (int)iLhsCount - 1;
				for(it = pList->exps_get().rbegin() ; it != pList->exps_get().rend() ; it++)
				{
					const SimpleVar *pListVar	= dynamic_cast<const SimpleVar*>((*it));
					symbol::Context::getInstance()->put(pListVar->name_get(), *((GenericType*)execMeR->result_get(i)));
					if(e.is_verbose())
					{
						std::ostringstream ostr;
						ostr << pListVar->name_get() << " = " << std::endl;
						ostr << std::endl;
						ostr << execMeR->result_get(i)->toString(10,75) << std::endl;
						YaspWrite((char *)ostr.str().c_str());
					}
					i--;
				}

			}
			else
			{//Houston ...
				std::ostringstream os;
				os << "unknow script form";
				os << ((Location)e.right_exp_get().location_get()).location_string_get() << std::endl;
				throw os.str();
			}
			
			if(bNew)
			{
				if(pVar != NULL)
				{
					symbol::Context::getInstance()->put(pVar->name_get(), *dataNew);
				}
				else
				{
					object->Set(propName->name_get().name_get(), dataNew);
				}
			}

			if(e.is_verbose() && pList == NULL)
			{
				std::ostringstream ostr;
				if(pVar != NULL)
				{
					ostr << pVar->name_get() << " = " << std::endl;
				}
				else
				{
					ostr << object->toString(10,75) << "." << propName->name_get() << " = " << std::endl;
				}
				ostr << std::endl;
				ostr << dataNew->toString(10,75) << std::endl;
				YaspWrite((char *)ostr.str().c_str());
			}
		}
		catch(string sz)
		{
			throw sz;
		}
		delete execMeR;
	}
}

/*
std::vector<std::vector<int>> ExpandList(std::vector<std::vector<int>> *_List)
{
	std::vector<std::vector<int> > ReturnList;
	if(_List->size() == 1)
	{
		for(unsigned int i = 0 ; i< (*_List)[0].size() ; i++)
		{
			std::vector<int> SubList;
			SubList.push_back((*_List)[0][i]);
			ReturnList.push_back(SubList);
		}
	}
	else
	{
		std::vector<int> SubList;
		std::vector<std::vector<int> > TempList;
		SubList = *(_List->begin());
		_List->erase(_List->begin());
		TempList = ExpandList(_List);
		for(unsigned int k = 0 ; k < SubList.size() ; k++)
		{
			for(unsigned int j = 0 ; j < TempList.size() ; j++)
			{
				std::vector<int> SubList2;
				SubList2.push_back(SubList[k]);
				for(unsigned int i = 0 ; i < TempList[j].size() ; i++)
				{
					SubList2.push_back(TempList[j][i]);
				}
				ReturnList.push_back(SubList2);
			}
		}
	}
	return ReturnList;
}
*/
