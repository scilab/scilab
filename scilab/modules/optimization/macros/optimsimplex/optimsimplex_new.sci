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
//   * the vertex #kve is stored in x(kve,1:n)
//     with kve = 1 , k
// Arguments
//   coords : list of point coordinates in the simplex
//   fun : the function to compute at vertices
//   data : user-defined data, passed to the function
// Uses :
//   newobj = optimsimplex_new ( )
//   newobj = optimsimplex_new ( coords )
//   newobj = optimsimplex_new ( coords , fun )
//   [ newobj , data ] = optimsimplex_new ( coords , fun , data )
//   newobj = optimsimplex_new ( "axes" , x0 )
//   newobj = optimsimplex_new ( "axes" , x0 , fun )
//   newobj = optimsimplex_new ( "axes" , x0 , fun , len )
//   [ newobj , data ] = optimsimplex_new ( "axes" , x0 , fun , len , data )
//   newobj = optimsimplex_new ( "spendley" , x0 )
//   newobj = optimsimplex_new ( "spendley" , x0 , fun )
//   newobj = optimsimplex_new ( "spendley" , x0 , fun , len )
//   [ newobj , data ] = optimsimplex_new ( "spendley" , x0 , fun , len , data )
//   newobj = optimsimplex_new ( "pfeffer" , x0 )
//   newobj = optimsimplex_new ( "pfeffer" , x0 , fun )
//   newobj = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual )
//   newobj = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual , deltazero )
//   [ newobj , data ] = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual , deltazero , data )
//   newobj = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax , nbve )
//   [ newobj , data ] = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax , nbve  , data )
//   newobj = optimsimplex_new ( "oriented" , simplex0 )
//   newobj = optimsimplex_new ( "oriented" , simplex0 , fun )
//   [ newobj , data ] = optimsimplex_new ( "oriented" , simplex0 , fun , data )
//
function [ newobj , data ] = optimsimplex_new ( varargin )
  [lhs,rhs]=argn();
  if rhs>7 then
    errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 0 to 7 are expected."), "optimsimplex_new", rhs);
    error(errmsg)
  end
  if rhs == 0 then 
    //   newobj = optimsimplex_new ( )
    newobj = optimsimplex_coords ( )
    return;
  end
  var1 = varargin(1);
  if typeof(var1) == "string" then
    stype = varargin(1);
    select stype
    case "axes" then
      //   newobj = optimsimplex_new ( "axes" , x0 )
      //   newobj = optimsimplex_new ( "axes" , x0 , fun )
      //   newobj = optimsimplex_new ( "axes" , x0 , fun , len )
      //   [ newobj , data ] = optimsimplex_new ( "axes" , x0 , fun , len , data )
      if rhs<2 | rhs > 5 then
        errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 to 5 are expected."), "optimsimplex_new", rhs);
        error(errmsg)
      end
      x0   = varargin(2);
      if rhs==2 then
        newobj = optimsimplex_axes ( x0 )
      elseif rhs==3 then
        fun  = varargin(3);
        newobj = optimsimplex_axes ( x0 , fun )
      elseif rhs==4 then
        fun  = varargin(3);
        len  = varargin(4);
        newobj = optimsimplex_axes ( x0 , fun , len )
      elseif rhs==5 then
        fun  = varargin(3);
        len  = varargin(4);
        data = varargin(5);
        [ newobj , data ] = optimsimplex_axes ( x0 , fun , len , data )
      end
    case "spendley" then
      //   newobj = optimsimplex_new ( "spendley" , x0 )
      //   newobj = optimsimplex_new ( "spendley" , x0 , fun )
      //   newobj = optimsimplex_new ( "spendley" , x0 , fun , len )
      //   [ newobj , data ] = optimsimplex_new ( "spendley" , x0 , fun , len , data )
      if rhs<2 | rhs > 5 then
        errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 to 5 are expected."), "optimsimplex_new", rhs);
        error(errmsg)
      end
      if rhs==2 then
        x0   = varargin(2);
        newobj = optimsimplex_spendley ( x0 )
      elseif rhs==3 then
        x0   = varargin(2);
        fun  = varargin(3);
        newobj = optimsimplex_spendley ( x0 , fun )
      elseif rhs==4 then
        x0   = varargin(2);
        fun  = varargin(3);
        len  = varargin(4);
        newobj = optimsimplex_spendley ( x0 , fun , len )
      elseif rhs==5 then
        x0   = varargin(2);
        fun  = varargin(3);
        len  = varargin(4);
        data = varargin(5);
        [ newobj , data ] = optimsimplex_spendley ( x0 , fun , len , data )
      end
    case "pfeffer" then
      //   newobj = optimsimplex_new ( "pfeffer" , x0 )
      //   newobj = optimsimplex_new ( "pfeffer" , x0 , fun )
      //   newobj = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual )
      //   newobj = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual , deltazero )
      //   [ newobj , data ] = optimsimplex_new ( "pfeffer" , x0 , fun , deltausual , deltazero , data )
      if rhs<2 | rhs > 6 then
        errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 to 6 are expected."), "optimsimplex_new", rhs);
        error(errmsg)
      end
      x0   = varargin(2);
      if rhs==2 then
        newobj = optimsimplex_pfeffer ( x0 )
      elseif rhs==3 then
        fun  = varargin(3);
        newobj = optimsimplex_pfeffer ( x0 , fun )
      elseif rhs==4 then
        fun         = varargin(3);
        deltausual  = varargin(4);
        newobj = optimsimplex_pfeffer ( x0 , fun , deltausual )
      elseif rhs==5 then
        fun        = varargin(3);
        deltausual = varargin(4);
        deltazero  = varargin(5);
        newobj = optimsimplex_pfeffer ( x0 , fun , deltausual , deltazero )
      elseif rhs==6 then
        fun        = varargin(3);
        deltausual = varargin(4);
        deltazero  = varargin(5);
        data       = varargin(6);
        [ newobj , data ] = optimsimplex_pfeffer ( x0 , fun , deltausual , deltazero , data )
      end
    case "randbounds" then
      //   newobj = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax )
      //   newobj = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax , nbve )
      //   [ newobj , data ] = optimsimplex_new ( "randbounds" , x0 , fun , boundsmin , boundsmax , nbve  , data )
      if rhs<5 | rhs > 7 then
        errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 6 to 7 are expected."), "optimsimplex_new", rhs);
        error(errmsg)
      end
      x0   = varargin(2);
      fun  = varargin(3);
      boundsmin = varargin(4);
      boundsmax = varargin(5);
      if rhs==5 then
        newobj = optimsimplex_randbounds ( x0 , fun , boundsmin , boundsmax )
      elseif rhs==6 then
        nbve = varargin(6);
        newobj = optimsimplex_randbounds ( x0 , fun , boundsmin , boundsmax , nbve )
      elseif rhs==7 then
        nbve = varargin(6);
        data       = varargin(7);
        [ newobj , data ] = optimsimplex_randbounds ( x0 , fun , boundsmin , boundsmax , nbve , data )
      end
    case "oriented" then
      //   newobj = optimsimplex_new ( "oriented" , simplex0 )
      //   newobj = optimsimplex_new ( "oriented" , simplex0 , fun )
      //   [ newobj , data ] = optimsimplex_new ( "oriented" , simplex0 , fun , data )
      if rhs<2 | rhs > 4 then
        errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 to 4 are expected."), "optimsimplex_new", rhs);
        error(errmsg)
      end
      simplex0   = varargin(2);
      if rhs==2 then
        newobj = optimsimplex_oriented ( simplex0 )
      elseif rhs==3 then
        fun   = varargin(3);
        newobj = optimsimplex_oriented ( simplex0 , fun )
      elseif rhs==4 then
        fun   = varargin(3);
        data   = varargin(4);
        [ newobj , data ] = optimsimplex_oriented ( simplex0 , fun , data )
      end
    else
      errmsg = msprintf(gettext("%s: Unknown key %s"),"optimsimplex_new",key)
      error(errmsg)
    end
  else
    //   newobj = optimsimplex_new ( coords )
    //   newobj = optimsimplex_new ( coords , fun )
    //   [ newobj , data ] = optimsimplex_new ( coords , fun , data )
    if rhs < 1 | rhs > 3 then
      errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 1 to 3 are expected."), "optimsimplex_new", rhs);
      error(errmsg)
    end
    if rhs == 1
      coords = varargin(1);
      newobj = optimsimplex_coords ( coords )
    elseif rhs == 2
      coords = varargin(1);
      fun = varargin(2);
      newobj = optimsimplex_coords ( coords , fun )
    elseif rhs == 3
      coords = varargin(1);
      fun = varargin(2);
      data = varargin(3);
      [ newobj , data ] = optimsimplex_coords ( coords , fun , data )
    end
  end    
endfunction

function [ newobj , data ] = optimsimplex_coords ( coords , fun , data )
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
  // The coordinates of the vertices, with size nbve x n
  newobj.x = [];
  // The function values, with size nbve x 1 
  newobj.fv = [];
  //
  // Take input arguments into account
  //
  if coords<>[] then
    nbve = size(coords,1)
    n = size(coords,2)
    if nbve < n + 1 then
      error(msprintf(gettext("%s: The numbers of columns of coords is %d but is expected to be at least %d"),...
        "optimsimplex_new" , nbve , n + 1))
    end
    newobj.n = n;
    newobj.nbve = nbve;
    newobj.x(1:nbve,1:n) = coords(1:nbve,1:n);
    if fun<>[] then
      if (~isdef('data','local')) then
        newobj = optimsimplex_computefv ( newobj , fun )
      else
        [ newobj , data ] = optimsimplex_computefv ( newobj , fun , data )
      end
    end
  end
endfunction
//
// optimsimplex_axes --
//   Configure the current simplex so that it is computed from the axes and a length.
// Arguments
//   x0 : the initial point, as a row vector
//   fun : name of the function
//   length : the length of the simplex
//     If length is a value, that unique length
//     is used in all directions.
//     If length is a vector with n values, each 
//     length is used with the corresponding 
//     direction.
//   data : user-defined data
//
function [ newobj , data ] = optimsimplex_axes ( x0 , fun , len , data )
  newobj = optimsimplex_coords ( )
  if size(x0,1)<>1 then
    errmsg = msprintf(gettext("%s: The x0 vector is expected to be a row matrix, but current shape is %d x %d"),"optimsimplex_axes",size(x0,1),size(x0,2));
    error(errmsg);
  end
  if (~isdef('fun','local')) then
    fun = [];
  end
  if (~isdef('len','local')) then
    len = 1.0;
  else
    xlen1 = size(len,1)
    if size(x0,1)<>1 then
      errmsg = msprintf(gettext("%s: The len vector is expected to be a row matrix, but current shape is %d x %d"),"optimsimplex_axes",size(len,1),size(len,2));
      error(errmsg);
    end
  end
  n = length(x0);
  newobj.n = n;
  newobj.nbve = n + 1;
  nl=length(len)
  if nl==1 then
    xlen = len * ones(1,n)
  else
    xlen = len
  end
  newobj.x = zeros ( newobj.nbve , n )
  newobj.fv = zeros ( newobj.nbve , 1 )
  //
  // Set all points
  //
  nv = newobj.nbve;
  newobj.x ( 1:nv , : ) = x0 (1:n) .*. ones(nv,1);
  newobj.x ( 2:nv , : ) = newobj.x ( 2:nv , : ) + diag(xlen);
  // Compute Function Value
  if fun <> [] then
    if (~isdef('data','local')) then
      newobj = optimsimplex_computefv ( newobj , fun )
    else
      [ newobj , data ] = optimsimplex_computefv ( newobj , fun , data )
    end
  end
endfunction

//
// optimsimplex_spendley --
//   Configure the current simplex so that it is computed from Spendley's method, 
//   i.e. a regular simplex made of k = n+1 vertices.
// Arguments
//   x0 : the initial point, as a row vector
//   fun : name of the function
//   len : the length of the simplex
//   data : user-defined data
//
function [ newobj , data ] = optimsimplex_spendley ( x0 , fun , len , data )
  newobj = optimsimplex_coords ( )
  if size(x0,1)<>1 then
    errmsg = msprintf(gettext("%s: The x0 vector is expected to be a row matrix, but current shape is %d x %d"),"optimsimplex_spendley",size(x0,1),size(x0,2));
    error(errmsg);
  end
  if (~isdef('fun','local')) then
    fun = [];
  end
  if (~isdef('len','local')) then
   len = 1.0;
  else
    if ( size(len,1)<>1 | size(len,2)<>1 ) then
      errmsg = msprintf(gettext("%s: The len vector is expected to be a scalar, but current shape is %d x %d"),"optimsimplex_spendley",size(len,1),size(len,2));
      error(errmsg);
    end
  end
  n = length(x0);
  newobj.n = n;
  newobj.nbve = n + 1;
  newobj.x = zeros ( n+1  , n )
  newobj.fv = zeros ( n+1 , 1 )
  //
  // Compute p (diagonal term) , q (off-diagonal term)
  //
  p  = (n - 1.0 + sqrt(n + 1))/(n * sqrt(2.0))
  q = (sqrt(n + 1) - 1.0)/(n * sqrt(2.0))
  //
  // Set all points
  //
  nv = newobj.nbve;
  newobj.x ( 1:nv , : ) = x0 (1:n) .*. ones(nv,1);
  newobj.x ( 2:nv , : ) = newobj.x ( 2:nv , : ) + diag(ones(n,1)*p) + q*ones(n,n) - diag(ones(n,1)*q);
  // Compute Function Value
  if fun <> [] then
    if (~isdef('data','local')) then
      newobj = optimsimplex_computefv ( newobj , fun )
    else
      [ newobj , data ] = optimsimplex_computefv ( newobj , fun , data )
    end
  end
endfunction

//
// optimsimplex_pfeffer --
//   Configure the current simplex so that it is computed from Pfeffer's method, 
//   i.e. a relative delta for non-zero values and an absolute delta 
//   for zero values.
// Arguments
//   x0 : the initial point, as a row vector
//   fun : name of the function
//   deltausual : the absolute delta for non-zero values
//   deltazero : the absolute delta for zero values
//   data : user-defined data
// References
//   "Global Optimization Of Lennard-Jones Atomic Clusters"
//   Ellen Fan, Thesis, February 26, 2002, McMaster University
//   Method due to L. Pfeffer at Stanford
//
function [ newobj , data ] = optimsimplex_pfeffer ( x0 , fun , deltausual , deltazero , data )
  newobj = optimsimplex_coords ( )
  if size(x0,1)<>1 then
    errmsg = msprintf(gettext("%s: The x0 vector is expected to be a row matrix, but current shape is %d x %d"),"optimsimplex_pfeffer",size(x0,1),size(x0,2));
    error(errmsg);
  end
  if (~isdef('fun','local')) then
    fun = [];
  end
   if (~isdef('deltausual','local')) then
    deltausual = 0.05
  end
   if (~isdef('deltazero','local')) then
    deltazero = 0.0075
  end
  if size(x0,1)<>1 then
    errmsg = msprintf(gettext("%s: The x0 vector is expected to be a row matrix, but current shape is %d x %d"),"optimsimplex_pfeffer",size(x0,1),size(x0,2));
    error(errmsg);
  end
  n = length(x0);
  newobj.n = n;
  newobj.nbve = n + 1;
  newobj.x = zeros ( n+1 , n )
  newobj.fv = zeros ( n+1 , 1 )
  //
  // Set 1st point
  //
  newobj.x ( 1 , 1:n ) = x0 (1,1:n)
  //
  // Set points #2 to #n+1
  //
  for j = 2 : newobj.n+1
    newobj.x ( j,1:n ) = x0 (1:n)
    if ( x0( j-1 ) == 0.0 ) then
      newobj.x ( j , j-1 ) = deltazero
    else
      newobj.x ( j , j-1 ) = newobj.x ( j , j-1 ) + deltausual * x0( j-1 )
    end
  end
  // Compute Function Value
  if fun <> [] then
    if (~isdef('data','local')) then
      newobj = optimsimplex_computefv ( newobj , fun )
    else
      [ newobj , data ] = optimsimplex_computefv ( newobj , fun , data )
    end
  end
endfunction

//
// optimsimplex_randbounds --
//   Configure the current simplex so that it is computed by taking the bounds
//   into account with random scaling.
// Arguments
//   x0 : the initial point
//   nbpts : the number of vertices in the simplex (+ the initial point)
//   fun : name of the function
//   boundsmin : array of minimum bounds
//   boundsmax : array of maximum bounds
//   nbve : total number of vertices in the simplex
//   data : user-defined data
//
function [ newobj , data ] = optimsimplex_randbounds ( x0 , fun , boundsmin , boundsmax , nbve  , data )
  newobj = optimsimplex_coords ( )
  if size(x0,1)<>1 then
    errmsg = msprintf(gettext("%s: The x0 vector is expected to be a row matrix, but current shape is %d x %d"),"optimsimplex_randbounds",size(x0,1),size(x0,2));
    error(errmsg);
  end
  if ( size(boundsmin,1)<>1 ) then
    errmsg = msprintf(gettext("%s: The boundsmin vector is expected to be a row matrix, but current shape is %d x %d"),"optimsimplex_randbounds",size(boundsmin,1),size(boundsmin,2));
    error(errmsg);
  end
  if ( size(boundsmax,1)<>1 ) then
    errmsg = msprintf(gettext("%s: The boundsmax vector is expected to be a row matrix, but current shape is %d x %d"),"optimsimplex_randbounds",size(boundsmax,1),size(boundsmax,2));
    error(errmsg);
  end
    n = length ( x0 )
    if (~isdef('nbve','local')) then
      nbve = n + 1;
    end
    newobj.n = n;
    newobj.nbve = nbve;
    newobj.x = zeros ( nbve , n )
    newobj.fv = zeros ( nbve , 1 )
    //
    // Set 1st point
    //
    newobj.x ( 1 , 1:n ) = x0 (1:n)
    //
    // Set points #2 to #nbve, by randomizing the bounds
    //
    bminmat = boundsmin( 1,1:n ) .*. ones(nbve-1,1);
    bmaxmat = boundsmax( 1,1:n ) .*. ones(nbve-1,1);
    thetas = rand(n,nbve-1);
    newobj.x ( 2:nbve , 1:n ) = bminmat + (thetas.') .* (bmaxmat - bminmat)
    // Compute Function Value
    if (~isdef('data','local')) then
      newobj = optimsimplex_computefv ( newobj , fun )
    else
      [ newobj , data ] = optimsimplex_computefv ( newobj , fun , data )
    end
endfunction

//
// optimsimplex_oriented --
//   Returns an oriented simplex, in sorted order. 
//   This simplex may be used, as Kelley suggests
//   for a restart of Nelder-Mead algorithm.
// Arguments
//   fun : the function to compute at vertices
//   data : user-defined data
//   simplex0 : the initial simplex
//
function [ newobj , data ] = optimsimplex_oriented ( simplex0 , fun , data )
  if ( simplex0.nbve <> simplex0.n+1 ) then
    errmsg = msprintf(gettext ( "%s: The oriented simplex can be computed only with a simplex made of n+1 points, but the dimension is %d and the number of vertices is %d") , "optimsimplex_oriented", simplex0.n , simplex0.nbve)
    error(errmsg)
  end
  if (~isdef('fun','local')) then
    fun = [];
  end
  sgrad = optimsimplex_gradientfv ( simplex0 )
  ssize = optimsimplex_size ( simplex0 , "sigmaminus" )
  n = simplex0.n
  // Compute the betas
  ipos = find(sgrad >= 0.0);
  ineg = find(sgrad < 0.0);
  betav(ipos) = ssize;
  betav(ineg) = -ssize;
  betav = -0.5 * betav
  // Prepare a matrix with beta as diagonal terms
  mid = diag(betav);
  // Compute simplex
  newobj = optimsimplex_new()
  newobj.n = simplex0.n
  newobj.nbve = simplex0.n+1
  newobj.x = zeros ( n+1 , n )
  newobj.fv = zeros ( n+1 , 1 )
  // Store all points
  x1 = simplex0.x ( 1 , 1:n )
  newobj.x ( 1:n+1, 1:n ) = x1 ( 1 , 1:n ) .*. ones(n+1,1)
  // Retrieve the function value for the first simplex
  // This saves one function evaluation
  newobj.fv ( 1 ) = simplex0.fv ( 1 )
  newobj.x ( 2:n+1, 1:n ) = mid ( 1:n , 1:n ) + newobj.x ( 2:n+1, 1:n )
  if fun <> [] then
    if (~isdef('data','local')) then
      newobj = optimsimplex_compsomefv ( newobj , fun , 2:n+1 )
    else
      [ newobj , data ] = optimsimplex_compsomefv ( newobj , fun , 2:n+1 , data )
    end
  end
endfunction

