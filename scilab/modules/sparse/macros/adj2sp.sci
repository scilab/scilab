// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


function A=adj2sp(varargin)
    // adjacency to sparse conversion.
    // See adj2sp.xml for help.
    //
    // Check number of arguments
    [lhs, rhs] = argn()
    if ( and(rhs <> [3 4]) ) then
        lstr = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        error ( msprintf(lstr,"adj2sp",3,4))
    end
    if ( and(lhs <> [0 1]) ) then
        lstr = gettext("%s: Wrong number of output arguments: %d to %d expected.\n")
        error ( msprintf(lstr,"adj2sp",0,1))
    end
    //
    // Get arguments
    xadj = varargin(1)
    iadj = varargin(2)
    v = varargin(3)
    //
    // Get size of the matrix
    if ( rhs == 3 ) then
        m = max(iadj)
        n = size(xadj,"*")-1
        mn = [m,n]
    else
        mn = varargin(4)
    end
    //
    // Check type of arguments
    if ( typeof(xadj) <> "constant" ) then
        lstr = gettext("%s: Wrong type for input argument #%d.\n")
        error ( msprintf(lstr,"adj2sp",1))
    end
    if ( typeof(iadj) <> "constant" ) then
        lstr = gettext("%s: Wrong type for input argument #%d.\n")
        error ( msprintf(lstr,"adj2sp",2))
    end
    if ( typeof(mn) <> "constant" ) then
        lstr = gettext("%s: Wrong type for input argument #%d.\n")
        error ( msprintf(lstr,"adj2sp",3))
    end
    //
    // Check size of arguments
    if ( size(mn,"*") <> 2 ) then
        lstr = gettext("%s: Wrong size for input argument #%d: %s has size %d, but %d is expected.\n")
        error ( msprintf(lstr,"adj2sp",4,"mn",size(mn,"*"),2))
    end
    //
    // Check content of arguments
    if ( mn(1) < 0 ) then
        lstr = gettext("%s: Wrong value for input argument #%d: %s=%d, but a positive value is expected.\n")
        error ( msprintf(lstr,"adj2sp",4,"mn(1)",mn(1)))
    end
    if ( mn(2) < 0 ) then
        lstr = gettext("%s: Wrong value for input argument #%d: %s=%d, but a positive value is expected.\n")
        error ( msprintf(lstr,"adj2sp",4,"mn(2)",mn(2)))
    end
    if ( int(mn(1)) <> mn(1) ) then
        lstr = gettext("%s: Wrong value for input argument #%d: %s=%s, but a floating point integer is expected.\n")
        error ( msprintf(lstr,"adj2sp",4,"mn(1)",string(mn(1))))
    end
    if ( int(mn(2)) <> mn(2) ) then
        lstr = gettext("%s: Wrong value for input argument #%d: %s=%s, but a floating point integer is expected.\n")
        error ( msprintf(lstr,"adj2sp",4,"mn(2)",string(mn(2))))
    end
    if ( mn(1) <> max(iadj) ) then
        lstr = gettext("%s: Wrong value for input argument #%d: %s=%d, which does not match %s: %d.\n")
        error ( msprintf(lstr,"adj2sp",4,"mn(1)",mn(1),"max(iadj)",max(iadj)))
    end
    if ( mn(2) <> size(xadj,"*")-1 ) then
        lstr = gettext("%s: Wrong value for input argument #%d: %s=%d, which does not match size of %s: %d.\n")
        error ( msprintf(lstr,"adj2sp",4,"mn(2)",mn(2),"xadj",size(xadj,"*")))
    end
    //
    // Make xadj, iadj, mn column matrices
    xadj = xadj(:)
    iadj = iadj(:)
    mn = mn(:)
    //
    // Proceed...
    nza = size(iadj,"*")
    j = fadj2sp(xadj,mn(2),nza)
    A = sparse([j,iadj],v,[mn(2),mn(1)])'
endfunction

