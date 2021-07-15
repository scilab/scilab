// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2021 - Clément DAVID
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
// along with this program.

function b = %l_issquare(l)
    b = length(l) == 1 && issquare(l(1))
endfunction
