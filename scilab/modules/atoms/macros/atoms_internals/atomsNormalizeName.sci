// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU <sylvestre.ledru@scilab.org>
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Substitution of special characters in the toolbox's name

function str = atomsNormalizeName(str)
    str = strsubst(str, " ", "_")
    str = strsubst(str, "é", "e")
    str = strsubst(str, "è", "e")
    str = strsubst(str, "ê", "e")
    str = strsubst(str, "à", "a")
    str = strsubst(str, "â", "a")
    str = strsubst(str, "ù", "u")
    str = strsubst(str, "ô", "o")
    str = strsubst(str, "î", "i")
    str = strsubst(str, "ç", "c")
    str = strsubst(str, "/", "")
    str = strsubst(str, "\", "")
    return str
endfunction
