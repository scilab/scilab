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

#ifndef __SCI_FILE_HXX__
#define __SCI_FILE_HXX__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "exp.hxx"
#include "functiondec.hxx"

namespace slint
{

class SciFile
{
    std::wstring filename;
    const wchar_t * code;
    unsigned int codeLength;
    const ast::Exp * tree;
    std::vector<std::pair<unsigned int, unsigned int>> lines;
    const ast::FunctionDec * main;
    std::unordered_map<std::wstring, const ast::FunctionDec *> privateFunctions;

public:

    SciFile();
    SciFile(const std::wstring & filename, const wchar_t * _code, const ast::Exp * _tree);

    ~SciFile();

    const std::wstring & getFilename() const;
    const wchar_t * getCode() const;
    unsigned int getCodeLength() const;
    const ast::Exp * getTree() const;
    bool checkLineLength(const unsigned int max, std::vector<unsigned int> & out) const;
    unsigned int countLines() const;
    unsigned int countLines(const unsigned from, const unsigned to) const;
    bool getPosition(const Location & loc, std::pair<unsigned int, unsigned int> & out) const;
    bool getFromPositionToEOL(const Location & loc, std::pair<unsigned int, unsigned int> & out) const;
    bool isPrivateFunction(const symbol::Symbol & sym) const;
    const ast::FunctionDec * getPrivateFunction(const std::wstring & name) const;
    const ast::FunctionDec * getMain() const;

private:

    void initLines();
    void analyzeTree();

    bool isEmptyLine(const wchar_t * line, const unsigned len) const;
};

typedef std::shared_ptr<SciFile> SciFilePtr;

} // namespace slint

#endif // __SCI_FILE_HXX__
