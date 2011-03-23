// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d = assert_computedigits ( varargin )
  // Returns the number of significant digits in computed result.

  [lhs,rhs]=argn()
  if ( and ( rhs <> [2 3] ) ) then
    errmsg = sprintf ( gettext ( "%s: Wrong number of input arguments: %d to %d expected.\n") , "assert_computedigits" , 2 , 3 )
    error(errmsg)
  end
  computed = varargin ( 1 )
  expected = varargin ( 2 )
  basis = argindefault ( rhs , varargin , 3 , 10 )
  //
  // Check types of variables
  if ( typeof(computed) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Wrong type for argument %d: Matrix expected.\n") , "assert_computedigits" , 1 )
    error(errmsg)
  end
  if ( typeof(expected) <> "constant" ) then
    errmsg = sprintf ( gettext ( "%s: Wrong type for argument %d: Matrix expected.\n") , "assert_computedigits" , 2 )
    error(errmsg)
  end
  //
  // Check sizes of variables
  if ( size(expected) <> size(computed) ) then
    errmsg = sprintf ( gettext ( "%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n") , "assert_computedigits" , 1 , 2 )
    error(errmsg)
  end
  if ( size(basis,"*") <> 1 ) then
    errmsg = sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_computedigits" , 3 , 1 , 1 )
    error(errmsg)
  end
  //
  nre = size(expected,"r")
  nce = size(expected,"c")
  // Update shape
  expected = expected (:)
  computed = computed (:)
  //
  d = zeros(expected)
  //
  n = size(expected,"*")
  for i = 1 : n
    dre = computedigits_data ( real(expected(i)) , real(computed(i)) , basis )
    dim = computedigits_data ( imag(expected(i)) , imag(computed(i)) , basis )
    d(i) = min(dre,dim)
  end
  //
  // Reshape
  d = matrix(d,nre,nce)
endfunction

function argin = argindefault ( rhs , vararglist , ivar , default )
  // Returns the value of the input argument #ivar.
  // If this argument was not provided, or was equal to the 
  // empty matrix, returns the default value.
  if ( rhs < ivar ) then
    argin = default
  else
    if ( vararglist(ivar) <> [] ) then
      argin = vararglist(ivar)
    else
      argin = default
    end
  end
endfunction
function d = computedigits_data ( expected , computed , basis )
    dmin = 0
    dmax = -log(2^(-53))/log(basis)
    //
    if ( isnan(expected) & isnan(computed) ) then
        d = dmax
    elseif ( isnan(expected) & ~isnan(computed) ) then
        d = dmin
    elseif ( ~isnan(expected) & isnan(computed) ) then
        d = dmin
        // From now, both expected and computed are non-nan
    elseif ( expected == 0 & computed == 0 ) then
        d = dmax
    elseif ( expected == 0 & computed <> 0 ) then
        d = dmin
        // From now, expected is non-zero
    elseif ( expected == computed ) then
        d = dmax
        // From now, expected and computed are different
    elseif ( expected == %inf & computed <> %inf ) then
        d = dmin
    elseif ( expected == -%inf & computed <> -%inf ) then
        d = dmin
        // From now, neither of computed, nor expected is infinity
    else
        // The max function does not ensure that the sign bit of d is positive.
        // For example : 
        // ieee(2); z=max(-0,0); 1/z is -%inf
        // To force this, consider the special case where the relative error is 
        // larger than 1.
        relerr = abs(computed-expected) / abs(expected)
        if ( relerr >= 1 ) then
            d = dmin
        else
            sigdig = -log ( relerr ) / log(basis)
            d = max ( sigdig , dmin )
        end
    end
endfunction

