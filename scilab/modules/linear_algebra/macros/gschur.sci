// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function [As,Es,Z,dim]= gschur(A,E,extern)

    if argn(2)<=2 then
        warning("Obsolete function. Please use schur instead.")
        if argn(1)==2 then
            [As,Es]= schur(A,E)
        else
            [As,Es,Q,Z]= schur(A,E);Q=Q'
        end
    elseif type(extern)==10 then
        if convstr(extern)=="c"|convstr(extern)=="d" then
            warning(msprintf(gettext("%s: Obsolete function. Please replace ''%s'' by ''%s''."),"gschur","gschur","schur"));
            if argn(1)==4 then
                [As,Es,Z,dim]= schur(A,E,extern)
            elseif argn(1)==2 then
                [As,Es,Z,dim]= schur(A,E,extern)
                Es=dim;As=Z;
            end
        else // hard coded
            warning(msprintf(gettext("%s: Obsolete function. Former external functions cannot be used anymore."),"gschur"));
            //impossible to redefine
        end
    else //coded by a scilab function
        //---- old------------------
        //flag=extern(x)
        //x(1) ==1 ==> x(2:3)=[al,be]
        //x(1) ==2 ==> x(2:3)=[s,p]
        warning(msprintf(gettext("%s: Obsolete function. Please replace ''%s'' by ''%s''."),"gschur","gschur","schur"));
        deff("t=%_rule(Alpha,Beta)",["if imag(Alpha)==0 then"
        "  t=extern([1,real(Alpha),Beta])==1"
        "else"
        "  c=Alpha/Beta"
        "  t=extern([2,real(c+c''),real(c*c'')])==1"
        "end;"])
        [As,Es,Z,dim]= schur(A,E,%_rule)
    end
endfunction

