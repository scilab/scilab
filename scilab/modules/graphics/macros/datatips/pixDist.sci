// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function d=pixDist(p1,p2)
    //computes the distance in pixels of two points
    [x1,y1]=xchange([p1(1) p2(1)],[p1(2) p2(2)],"f2i")
    d=max(abs([x1(2)-x1(1),y1(2)-y1(1)]))
endfunction
