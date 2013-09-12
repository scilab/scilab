// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function [xadj,iadj,v]=sp2adj(A)
    // sparse conversion to adjacency
    // See sp2adj.xml for help.
    //
    // Check number of arguments
    [lhs, rhs] = argn()
    if ( rhs <> 1 ) then
        lstr = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        error ( msprintf(lstr,"sp2adj",1,1))
    end
    if ( and(lhs <> [0 1 2 3]) ) then
        lstr = gettext("%s: Wrong number of output arguments: %d to %d expected.\n")
        error ( msprintf(lstr,"sp2adj",0,3))
    end
    //
    // Check type of arguments
    if ( typeof(A) <> "sparse" ) then
        lstr = gettext("%s: Wrong type for input argument #%d.\n")
        error ( msprintf(lstr,"sp2adj",1))
    end
    //
    // Check size of arguments
    // Nothing to do
    //
    // Check content of arguments
    // Nothing to do
    //
    [ij,v,n]=spget(A')
    N=n(1)
    if ( ij == [] ) then,
        xadj=ones(n(2)+1,1)
        iadj=[]
        v=[]
    else,
        [xadj,la,iadj]=ta2lpd(ij(:,1)',ij(:,2)',N+1,N)
        xadj=xadj(:)
        iadj=iadj(:)
    end
endfunction
