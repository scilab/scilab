// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
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
// optimsimplex_shrink --
//   Shrink the simplex with given coefficient sigma
//   and returns an updated simplex.
//   The shrink is performed with respect to the 1st point
//   in the simplex.
// Arguments
//   fun : the function to compute at vertices
//   sigma : shrinkage factor (default value = 0.5)
//   data : user-defined data
//
function [ this , data ] = optimsimplex_shrink ( this , fun , sigma , data )
    if (~isdef("sigma","local")) then
        sigma = 0.5;
    end
    nv = this.nbve;
    mv1 = this.x(1,:) .*. ones ( nv - 1 , 1 );
    newx = ( 1.0 - sigma ) * mv1(1:nv-1,:) + sigma * this.x ( 2 : nv , : ) ;
    this.x(2:nv,:) = newx(1:nv-1,:);
    if (~isdef("data","local")) then
        this = optimsimplex_compsomefv ( this , fun , 2 : nv )
    else
        [ this , data ] = optimsimplex_compsomefv ( this , fun , 2 : nv , data )
    end
endfunction

