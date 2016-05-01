// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
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
// optimsimplex_reflect --
//   Returns a new simplex by reflexion of current simplex, by reflection with respect
//   to the first vertex in the simplex.
//   This move is used in the centered simplex gradient.
// Arguments
//   fun : name of the function
//   data : user-defined data
//
function [ r , data ] = optimsimplex_reflect ( this , fun , data )
    nv = this.nbve;
    n = this.n;
    r = optimsimplex_new ( );
    r.n = n
    r.nbve = nv
    r.x = zeros ( nv , n )
    r.fv = zeros( nv , 1 )
    r.x(1,1:n) = this.x(1,1:n);
    r.fv(1) = this.fv(1);
    twox1 = 2*this.x(1,1:n) .*. ones(nv-1,1);
    r.x(2:nv,1:r.n) = twox1 - this.x(2:nv,1:n)  ;
    if (~isdef("data","local")) then
        r = optimsimplex_compsomefv ( r , fun , 2:nv )
    else
        [ r , data ] = optimsimplex_compsomefv ( r , fun , 2:nv , data )
    end
endfunction

