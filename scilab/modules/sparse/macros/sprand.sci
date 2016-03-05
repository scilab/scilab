// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function a=sprand(m,n,density,typ)

    //---- Check arguments----------------------------------------------
    rhs = argn(2)

    if ( rhs < 3 | rhs > 4 ) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "sprand" , 3 , 4 ));
    end

    if ( rhs < 4 ) then
        typ="def";
    end

    if type(typ)<>10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"sprand",4));
    end

    if and(typ<>["u";"n";"uniform";"normal";"def";"nor"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),"sprand",4,"uniform","normal"));
    end
    if typ == "u" | typ == "uniform" then //"uniform" is the syntax for uniform distribution with rand, the equivalent with grand is "def"
        typ = "def";
    elseif typ == "n" | typ == "normal" then //"normal" is the syntax for normal distribution with rand, the equivalent with grand is "nor"
        typ = "nor";
    end

    density=max(min(density,1),0);

    nel=m*n*density; //the objective number of non zero elements
    if nel==0 then
        a=sparse([],[],[m,n])
        return
    end

    //---- generate a sequence of increments----------------------------
    mdist = 1/density //the mean distance between to consecutive index
    nel1  = (2.2-density)*nel; //generate more increments than requested nnz elements
    if nel1<1
        ij = 1;
    else
        ij    = round(1+grand(nel1,1,"exp",(mdist-1)))
    end

    //---- sum the increments to get the index--------------------------
    ij=cumsum(ij);

    //---- eliminate the index with exceed the maximum matrix index
    ij(find(ij>m*n))=[];
    nel1=size(ij,"*");
    if nel1==0 then
        a=sparse([],[],[m,n]);
        return
    end
    //---- generate the row and column indices from the 1D index--------

    ij=ind2sub([m,n],ij);

    //----  generates the random non zeros elements --------------------
    //according to the requested law and create the sparse matrix
    if typ == "nor" then // Because of the syntax of grand, we have two cases, one with "def" and one with "nor"
        a=sparse(ij,grand(nel1,1,typ,0,1),[m,n]);
    elseif typ == "def" then
        a=sparse(ij,grand(nel1,1,typ),[m,n]);
    end


endfunction

