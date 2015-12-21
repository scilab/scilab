// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// compute the distance between a point and the arc
// in 2D included in an axis aligned rectangle whose upper left
// corner is upperLeft and its wifth and heigth is defined.
function [dist,diffClose] = dist2Arc( point, upperLeft, width, heigth, sector1, sector2 )


    if ( width == 0 | heigth == 0 ) then
        dist = %inf ;
        diffClose = [%inf,%inf];
        return ;
    end

    // convert the sector into radiant angle
    angle1 =  sector1            * %pi / 180. ;
    angle2 = (sector1 + sector2) * %pi / 180. ;

    width2  = width  / 2. ;
    heigth2 = heigth / 2. ;
    centerC = [ upperLeft(1) + width2, upperLeft(2) - heigth2 ] ; // center of the ellipse

    // clicked point in the circle frame
    pointC  = [ (point(1) - centerC(1)) / width2, (point(2) - centerC(2)) / heigth2 ] ;

    // get the projection of the clicked point on the circle
    closest = pointC / norm( pointC ) ;

    // now a quite tricky part. The aim is to find
    // if the closest point is in the drawing sector
    // ie if it is between bound1 and bound2 on the circle
    // maybe a eayer solution exists.

    // get the boundaries of the displayed angle
    // the closest point is not on the arc it is one of the two
    // boundaries
    bound1 = [cos(angle1),sin(angle1)] ;
    bound2 = [cos(angle2),sin(angle2)] ;

    // now get the vector of bissecting line between the two bounds
    // with the orientation toward the arc
    b2b1       = bound1 -  bound2 ;
    bissect(1) = -b2b1(2)         ;
    bissect(2) =  b2b1(1)         ;

    // get the position of the point along this axis
    side = closest(1) * bissect(1) + closest(2) * bissect(2) ;

    // get the position of one of the bound (same value for both)
    boundPos = bound1(1) * bissect(1) + bound1(2) * bissect(2) ;

    if side > boundPos  then
        // the closest point is on the arc
        diffClose = ( pointC - closest ) .* [width2,heigth2] ;
        // bring it back to the current frame value
        //diffclose = diffclose .* [width2,heigth2] ;

        // get the distance with the closest point
        dist = norm( diffClose ) ;

    else
        // the closest point is one of the bounds
        // return back to the real coordinates
        bound1 = centerC + bound1 .* [width2,heigth2];
        bound2 = centerC + bound2 .* [width2,heigth2];

        // get the minimum distance
        dist  = norm( bound1 - point ) ;
        dist2 = norm( bound2 - point ) ;
        if dist > dist2 then
            diffClose = bound1 - point ;
        else
            dist = dist2 ;
            diffClose = bound2 - point ;
        end
        //dist = min( norm( bound1 - point ), norm( bound2 - point ) ) ;
    end

endfunction
