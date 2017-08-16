/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#include "data/MacroSignature.hxx"
#include "data/MacroDef.hxx"
#include "serializervisitor.hxx"

static std::size_t uchar_array_hasher(unsigned char const* vec, std::size_t len) {
    std::size_t ret = len;
    for (int i = 0; i < len; ++i) {
        ret ^= std::hash<unsigned char>()(vec[i]);
    }
    return ret;
}

namespace analysis {

MacroSignature::MacroSignature(MacroDef & macrodef, const unsigned int _lhs, const std::vector<TIType> & in)
    : name(macrodef.getName()), lhs(_lhs), tuple(in)
{
    const ast::SeqExp& exp = macrodef.getOriginalBody();
    ast::SerializeVisitor serialMacro(exp.getOriginal());
    unsigned char* serial = serialMacro.serialize();
    unsigned int size = *((unsigned int*)serial);
    exp_hash = uchar_array_hasher(serial, size);
    free(serial);
}
}