// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_new --
//   Creates a new simplex object.
// Note
//   * n is the dimension of the space
//   * one vertex is a point in the space, defined
//     by its n coordinates and associated with a function
//     value
//   * the simplex is made of k>=n+1 vertices
//   * the function value for vertex #kve is in fv(kve),
//     with kve = 1 , k
//   * the vertex #kve is stored in x(1:n,kve)
//     with kve = 1 , k
// Arguments
//   coords : list of point coordinates in the simplex
//   fun : the function to compute at vertices
//   data : user-defined data, passed to the function
//
function [ newobj , data ] = optimsimplex_new ( coords , fun , data )
  if (~isdef('coords','local')) then
    coords = [];
  end
   if (~isdef('fun','local')) then
    fun = [];
  end
  newobj = tlist(["T_SIMPLEX",...
    "verbose","x","n","fv","nbve"]);
  newobj.verbose = 0;
  // The dimension of the space
  newobj.n = 0;
  // The number of vertices
  newobj.nbve = 0;
  // The coordinates of the vertices, with size n x nbve
  newobj.x = [];
  // The function values, with size nbve
  newobj.fv = [];
  //
  // Take input arguments into account
  //
  if coords<>[] then
    n = size(coords,1)
    nbve = size(coords,2)
    if nbve < n + 1 then
      error(sprintf("The numbers of columns of coords is %d but is expected to be at least %d",...
        nbve , n + 1))
    end
    newobj.n = n;
    newobj.nbve = nbve;
    newobj.x(1:n,1:nbve) = coords(1:n,1:nbve);
    if fun<>[] then
      for j = 1 : nbve
        if (~isdef('data','local')) then
          newobj.fv(j)  = fun (newobj.x(1:newobj.n,j));
        else
          [ newobj.fv(j) , data ]  = fun (newobj.x(1:newobj.n,j) , data );
        end
      end
    end
  end
endfunction

