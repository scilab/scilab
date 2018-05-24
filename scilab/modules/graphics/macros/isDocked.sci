// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function yn = isDocked(fh)
    // http://fileexchange.scilab.org/toolboxes/360000
    // Shared internal function used by clf(), fchamp(), etc.
    //
    // fh: matrix of figures handles
    // yn: matrix of booleans
    //
    // yn(i) is %T if fh(i) is docked, %F otherwise

    sf = size(fh)
    fh = fh(:)
    fs = matrix(fh.figure_size,2,-1)'
    as = matrix(fh.axes_size,2,-1)'

    yn = (fh.dockable=="on" & ((fs(:,1)-as(:,1)) > 20)')
    // A test on vertical dimensions is more complicated, due to switchable
    // menubar, toolbar and infobar.
    yn = matrix(yn, sf)
endfunction
