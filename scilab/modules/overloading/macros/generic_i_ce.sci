// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent Couvert
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function M=generic_i_ce(varargin)
    // Generic function for insertion of any type of data in a cell
    error(msprintf(_("%s: Wrong assignment: for insertion in cell, use e.g. x(i,j).entries=y.\n"),"generic_i_ce"));
endfunction
