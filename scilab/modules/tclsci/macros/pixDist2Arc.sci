// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
