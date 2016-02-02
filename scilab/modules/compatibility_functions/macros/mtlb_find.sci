// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [i,j,v]=mtlb_find(b)
    // Translation function for find() Matlab function

    [lhs,rhs]=argn(0)

    // Because find does not work on complex values
    // And isreal does not word with Booleans
    c=b
    if type(b)<>4 & type(b)<>6 then
        if ~isreal(b) then
            c=abs(b)
        end
    end

    // One output arg
    if lhs==1 then
        i=find(c)
        if size(b,1)<>1 then
            i=i'
        end
        // Two output args
    elseif lhs==2 then
        [i,j]=find(c)
        if size(b,1)<>1 then
            i=i'
            j=j'
        end
        // Three output args
    else
        [i,j]=find(c)
        if i<>[] then
            v = b(i+size(b,1)*(j-1))
        else
            v = []
        end
        if size(b,1)<>1 then
            i=i'
            j=j'
        end
    end
endfunction
