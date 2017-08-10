// compute the square of distance between a point and the ellipse
// in 2D included in an axis aligned rectangle whose upper left
// corner is upperLeft and its wifth and heigth is defined.
function [dist] = dist2Ellipse( point, upperLeft, width, heigth )
    width2  = width  / 2. ;
    heigth2 = heigth / 2. ;
    centerC = [ upperLeft(1) + width2, upperLeft(2) - heigth2 ] ; // center of the ellipse

    // clicked point in the circle frame
    pointC  = [ (point(1) - centerC(1)) / width2, (point(2) - centerC(2)) / heigth2 ] ;

    // get the vector between the point and the closest on the circle
    diffclose = ( 1 - 1 / norm( pointC ) ) * pointC ;
    //closest = pointC / sqrt( pointC(1) * pointC(1) + pointC(2) * pointC(2) ) ;

    // get the difference between the two
    //ffclose = pointC - closest ;

    // bring it back to the current frame value
    diffclose(1) = diffclose(1) * width2  ;
    diffclose(2) = diffclose(2) * heigth2 ;

    // get the distance with the closest point
    dist = norm( diffclose ) ;

endfunction
