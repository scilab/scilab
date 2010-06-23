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

#ifndef __OVERLOAD_HXX__
#define __OVERLOAD_HXX__

#include "function.hxx"
#include "visitor.hxx"
#include "opexp.hxx"

class Overload
{
public:
    /**
     ** This function will build an overload function name
     ** depending on input parameters
     ** and maybe on number of result expected <further implementation>
     */
    static std::string buildOverloadName(std::string _stFunctionName, types::typed_list &in, int _iRetCount);

    /**
     */
    static types::Function::ReturnValue generateNameAndCall(std::string _stFunctionName, types::typed_list &in, int _iRetCount,
                                             types::typed_list &out, ast::ConstVisitor *_execMe);
    /**
     */
    static types::Function::ReturnValue call(std::string _stFunctionName, types::typed_list &in, int _iRetCount,
                                             types::typed_list &out, ast::ConstVisitor *_execMe);

    static std::string getNameFromOper(ast::OpExp::Oper);

};

#endif /* !__OVERLOAD_HXX__ */
