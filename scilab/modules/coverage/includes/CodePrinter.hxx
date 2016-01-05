/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CODE_PRINTER_HXX__
#define __CODE_PRINTER_HXX__

#include <string>

#include "exp.hxx"

namespace coverage
{

class CodePrinter
{

public:

    virtual void handleDefault(const std::string& seq) = 0;
    virtual void handleOperator(const std::string& seq) = 0;
    virtual void handleOpenClose(const std::string& seq) = 0;
    virtual void handleFunctionKwds(const std::string& seq) = 0;
    virtual void handleStructureKwds(const std::string& seq) = 0;
    virtual void handleControlKwds(const std::string& seq) = 0;
    virtual void handleConstants(const std::string& seq) = 0;
    virtual void handleCommands(const std::string& seq) = 0;
    virtual void handleMacros(const std::string& seq) = 0;
    virtual void handleFunctionName(const std::string& seq) = 0;
    virtual void handleFunctionNameDec(const std::string& seq) = 0;
    virtual void handleName(const std::string& seq) = 0;
    virtual void handleInOutArgsDec(const std::string& seq) = 0;
    virtual void handleInOutArgs(const std::string& seq) = 0;
    virtual void handleNumber(const std::string& seq) = 0;
    virtual void handleSpecial(const std::string& seq) = 0;
    virtual void handleString(const std::string& seq) = 0;
    virtual void handleNothing(const std::string& seq) = 0;
    virtual void handleField(const std::string& seq) = 0;
    virtual void handleComment(const std::string& seq) = 0;
    virtual void handleNewLine() = 0;
    virtual void handleExpStart(const ast::Exp * e) = 0;
    virtual void handleExpEnd(const ast::Exp * e) = 0;
    virtual unsigned int getLineCharCount() const = 0;
    virtual std::size_t getIndentSize() const = 0;
    virtual void incIndent() = 0;
    virtual void decIndent() = 0;
};

} // namespace coverage


#endif // __CODE_PRINTER_HXX__
