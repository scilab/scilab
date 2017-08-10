// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// same as before but return the value in pixels
function dist = pixDist2Arc( point, upperLeft, width, heigth, sector1, sector2 )

    [dist, difference] = dist2Arc( point, upperLeft, width, heigth, sector1, sector2 ) ;
    // convert to pixels
    // get the length of the difference vector
    // we construct it by getting two points
    [origin(1),origin(2)] = xchange(0,0,"f2i");
    [extremity(1),extremity(2)] = xchange(difference(1),difference(2),"f2i");
    dist = norm( extremity - origin ) ;

endfunction
