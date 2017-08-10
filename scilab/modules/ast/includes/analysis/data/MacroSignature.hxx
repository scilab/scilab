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

#ifndef __MACRO_SIGNATURE_HXX__
#define __MACRO_SIGNATURE_HXX__

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "tools.hxx"
#include "TITypeSignatureTuple.hxx"

namespace analysis
{

class MacroDef;
class DataManager;

/**
 * \struct MacroSignature
 * \brief A MacroSignature is defined by a macro name, the number of LHS and the types of the arguments
 */
struct MacroSignature
{
    const std::wstring name;
    const unsigned int lhs;
    TITypeSignatureTuple tuple;

    MacroSignature(MacroDef & macrodef, const unsigned int _lhs, const std::vector<TIType> & in) : name(macrodef.getName()), lhs(_lhs), tuple(in) { }

    friend std::wostream & operator<<(std::wostream & out, const MacroSignature & signature)
    {
        out << L"Signature{"
            << L"name:" << signature.name
            << L", lhs:" << signature.lhs
            << L", types:" << signature.tuple
            << L"}";

        return out;
    }

    /**
     * \struct Hash
     * \brief Hash struct to be used in unordered_* container
     */
    struct Hash
    {
        inline std::size_t operator()(const MacroSignature & sign) const
        {
            return tools::hash_combine(std::hash<std::wstring>()(sign.name), sign.lhs, sign.tuple.hash());
        }
    };

    /**
     * \struct Eq
     * \brief Eq struct to be used in unordered_* container
     */
    struct Eq
    {
        inline bool operator()(const MacroSignature & L, const MacroSignature & R) const
        {
            return L.lhs == R.lhs && L.name == R.name && L.tuple == R.tuple;
        }
    };
};

} // namespace analysis

#endif // __MACRO_SIGNATURE_HXX__
