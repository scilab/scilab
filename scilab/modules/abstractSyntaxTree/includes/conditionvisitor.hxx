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

#include "originalvisitor.hxx"

namespace ast
{
    class EXTERN_AST ConditionVisitor : public OriginalVisitor
	{
	public:
		ConditionVisitor(){m_bBooleanManagement=false;m_bResult=false;}
		~ConditionVisitor(){}
		virtual void visit (const OpExp &e);
		virtual void visit (const LogicalOpExp &e);

		bool is_boolean_result(){return m_bBooleanManagement;}
		bool result_bool_get(){return m_bResult;}
/*-------------.
		| Attributes.  |
		`-------------*/
	protected:
		bool m_bResult;
		bool m_bBooleanManagement;

		void result_bool_set(bool _bResult);

	};
}
#endif // !AST_CONDITIONVISITOR_HXX
