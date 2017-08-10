// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function r=%i_o_i(a,b)
    typea = inttype(a)
    typeb = inttype(b)
    typer = 0;

    if typea > 10 & typeb > 10 then
        typer = 10;
    end

    typer = typer + max(modulo(typea, 10), modulo(typeb, 10));

    newa = iconvert(a, typer);
    newb = iconvert(b, typer);
    r = newa == newb;
endfunction
