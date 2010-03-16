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

#ifndef AST_CONDITIONVISITOR_HXX
#define AST_CONDITIONVISITOR_HXX

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>

#include "execvisitor.hxx"
#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"
#include "alltypes.hxx"

namespace ast
{
	class EXTERN_AST ConditionVisitor : public ExecVisitor
	{
	public:
		ConditionVisitor(){}
		~ConditionVisitor(){}
		virtual void visit (const OpExp &e);

		bool is_boolean_result(){return _bBooleanManagement;}
		bool result_bool_get(){return _bResult;}
/*-------------.
		| Attributes.  |
		`-------------*/
	protected:
		bool _bResult;
		bool _bBooleanManagement;

		void result_bool_set(bool _bResult);

	};
}
#endif // !AST_CONDITIONVISITOR_HXX
