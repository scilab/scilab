// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
// Copyright (C) 2003 - Jean-Sebastien Giet & Bruno Pincon
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function [m] = tabul(X, order)
    //
    //  PURPOSE
    //     This function computes the frequency of values of
    //     the components of a vector or matrix X of numbers or
    //     string characters.
    //
    //      If X  is a numerical  vector or matrix then
    //      m  is a  two column matrix who contains in
    //      the first column the distinct values of X
    //      and  in the other column the number of occurrences
    //      of those values (m(i,2) is the number of occurrences
    //      of m(i,1)).
    //
    //      If X is a vector or matrix of strings, m is  a list
    //      whose  first member is a string vector composed with
    //      the distinct values of X and the second member is a
    //      vector whose components are the number of occurrences
    //      of those values ( m(i)(2) is the number of occurrences
    //      of the string m(i)(1) ).
    //
    //      The optional parameter order must be "d" or "i"
    //      (by default order = "d") and it gives the order
    //      of the distinct vector values of X (first column
    //      or first part of m) :
    //         order = "d" means that these values are sorted
    //                     in decreasing order
    //               = "i" means by increasing order
    //
    //  AUTHORS
    //      Original version by Carlos Klimann
    //      This version by Jean-Sebastien Giet & Bruno Pincon
    //
    //  date: 1999-04-09 (original version)
    //        2003-Mars-26 (new version)
    //
    //  NOTES
    //      The new version :
    //          is faster (by using no more loop)
    //          performs a complete check of the input arguments
    //          add the order option
    //          used gsort in place of sort : the sort function
    //          sorts strings vector not the usual way !

    rhs = argn(2)
    if rhs<1 | 2<rhs then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"tabul",1,2)),
    elseif rhs == 1 then
        order = "d"
    end
    typeX = type(X)
    if typeX ~= 1 & typeX ~= 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Vector, matrix of numbers or strings expected.\n"),"tabul",1))
    end
    if type(order) ~= 10 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),"tabul",2,"i","d"))
    end
    if order~="i" &  order~="d" then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),"tabul",2,"i","d"))
    end
    if ( X == [] ) then
        m = %nan
        return
    end

    X = X(:)
    X = gsort(X,"g",order)
    n = size(X,"*")
    ind = [find(X(1:$-1)~=X(2:$)) n]

    val = X(ind)
    occ = diff([0 ind])'

    if typeX == 1 then
        m = [val occ]
    else // X (and so val) is a vector of strings
        m = list(val,occ)
    end

endfunction
