/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SLINT_CNES_STANDARD_HXX__
#define __SLINT_CNES_STANDARD_HXX__

#include <string>

#include "StandardType.hxx"

namespace slint
{

namespace CNES
{

class Standard
{

    const StandardType standard;

public:

    Standard(const StandardType & _standard) : standard(_standard) { }

    inline const StandardType & getStandard() const
    {
        return standard;
    }

    static Standard createFromXml(const std::wstring & path);

};


} // namespace CNES

} // namespace slint

#endif // __SLINT_CNES_STANDARD_HXX__
