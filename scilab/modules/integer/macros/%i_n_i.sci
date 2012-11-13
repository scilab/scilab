// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function r=%i_n_i(a,b)
    typea = inttype(a)
    typeb = inttype(b)
    typer = 0;

    if typea > 10 & typeb > 10 then
        typer = 10;
    end

    typer = typer + max(modulo(typea, 10), modulo(typeb, 10));

    newa = iconvert(a, typer);
    newb = iconvert(b, typer);
    r = newa <> newb;
endfunction
