/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
