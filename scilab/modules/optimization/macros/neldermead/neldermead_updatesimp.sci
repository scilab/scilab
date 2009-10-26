// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// neldermead_updatesimp --
//   Update the initial simplex simplex0 for a restart.
// Arguments
//   <>
//
function this = neldermead_updatesimp ( this )
  simplex0 = optimsimplex_new ( );
  xopt = optimbase_get ( this.optbase , "-xopt" );
  select this.restartsimplexmethod
  case "oriented" then
    [ simplex0 , this ] = optimsimplex_new ( "oriented" , this.simplexopt , neldermead_costf , this );
  case "axes" then
    simplex0 = optimsimplex_destroy ( simplex0 )
    [ simplex0 , this ] = optimsimplex_new ( "axes" , ...
      xopt.' , neldermead_costf , this.simplex0length , this );
  case "spendley" then
    simplex0 = optimsimplex_destroy ( simplex0 )
    [ simplex0 , this ] = optimsimplex_new ( "spendley" , ...
      xopt.' , neldermead_costf , this.simplex0length , this );
  case "pfeffer" then
    simplex0 = optimsimplex_destroy ( simplex0 )
    [ simplex0 , this ] = optimsimplex_new ( "pfeffer" , ...
      xopt.' , neldermead_costf , this.simplex0deltausual , ...
      this.simplex0deltazero , this );
  case "randbounds" then
    if ( this.boxnbpoints=="2n" ) then
      this.boxnbpointseff = 2 * this.numberofvariables
    else
      this.boxnbpointseff = this.boxnbpoints
    end
    if ( ~hasbounds ) then
      errmsg = msprintf ( gettext("%s: Randomized bounds initial simplex is not available without bounds." ) , "neldermead_updatesimp" )
      error ( errmsg )
    end
    simplex0 = optimsimplex_destroy ( simplex0 )
    [ simplex0 , this ] = optimsimplex_new ( "randbounds" , xopt.' , ...
      neldermead_costf , this.optbase.boundsmin , this.optbase.boundsmax , ...
      this.boxnbpointseff  , this )
  else
    errmsg = msprintf(gettext("%s: Unknown restart simplex method %s"), "neldermead_updatesimp", this.restartsimplexmethod)
    error(errmsg)
  end
  //
  // Scale the simplex into the bounds and the nonlinear inequality constraints, if any.
  // Caution !
  // The initial simplex may be computed with an "axis-by-axis" simplex,
  // so that it does not satisfies bounds constraints.
  // The scaling should therefore take into accounts for bounds.
  // TODO : project vertices into bounds
  //
  nbve = optimsimplex_getnbve ( simplex0 );
  this = neldermead_log (this,"Before scaling:");
  str = optimsimplex_tostring ( simplex0 )
  for i = 1:nbve
    this = neldermead_log (this,str(i));
  end
  [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
  [ this.optbase , hasnlcons ] = optimbase_hasnlcons ( this.optbase );
  if ( hasbounds | hasnlcons ) then
    this = neldermead_log (this,sprintf("Scaling initial simplex into nonlinear inequality constraints..."));
    nbve = optimsimplex_getnbve ( simplex0 )
    for ive = 1 : nbve
      // x is a row vector
      x = optimsimplex_getx ( simplex0 , ive );
      this = neldermead_log (this,sprintf("Scaling vertex #%d/%d at [%s]... " , ...
        ive , nbve , _strvec(x)));
      // Transpose x because xopt is a column vector : xp is now a column vector
      [ this , status , xp ] = _scaleinconstraints ( this , x.' , xopt );
      if ( ~status ) then
        errmsg = msprintf(gettext("Impossible to scale the vertex #%d/%d at [%s] into inequality constraints"), "neldermead_updatesimp", ...
          ive , nbve , strcat(string(x)," "));
        error(errmsg);
      end
      if ( or(x <> xp) ) then
        index = 2
        if ( hasnlcons ) then
          [ this.optbase , fv , c , index ] = optimbase_function ( this.optbase , xp , index );
        else
          [ this.optbase , fv , index ] = optimbase_function ( this.optbase , xp , index );
        end
        // Transpose xp because optimsimplex takes row coordinate vectors.
        simplex0 = optimsimplex_setve ( simplex0 , ive , fv , xp.' )
      end
    end
  end
  this = neldermead_log (this,"After scaling:");
  str = optimsimplex_tostring ( simplex0 )
  for i = 1:nbve
    this = neldermead_log (this,str(i));
  end
  this.simplex0 = optimsimplex_destroy ( this.simplex0 )
  this.simplex0 = simplex0;
  this.simplexsize0 = optimsimplex_size ( simplex0 );
  this.simplex0 = optimsimplex_sort ( this.simplex0 )
endfunction

