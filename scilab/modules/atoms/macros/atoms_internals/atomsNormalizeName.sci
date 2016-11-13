// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU <sylvestre.ledru@scilab.org>
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
