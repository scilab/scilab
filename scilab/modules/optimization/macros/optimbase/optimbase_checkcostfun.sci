// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimbase_checkcostfun --
//   Check that the cost function is correctly connected.
//   Generate an error if there is one.
//
function this = optimbase_checkcostfun ( this )
  //
  // Check that f(x0) works and is a scalar
  //
  cmd = "[ this , fx0 ] = optimbase_function ( this , this.x0 )";
  ierr=execstr(cmd,"errcatch");
  if ierr <> 0 then
    errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function at x0." ) , "optimbase_checkcostfun" )
    error ( errmsg );
  end
  if ( size(fx0,1) <> 1 ) | ( size(fx0,2) <> 1 ) then
    errmsg = msprintf ( gettext ( "%s: Call to cost function with x0 is not a scalar, but a %d x %d matrix." ) , "optimbase_checkcostfun" , size(fx0,1) , size(fx0,2) )
    error ( errmsg );
  end
  //
  // If there are nonlinear constraints, check that the index is correctly managed.
  //
  if this.nbineqconst > 0 then
    // index = 1
    index = 1;
    cmd = "[ this , fx0 ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function at (x0,index=1)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    // index = 2
    index = 2;
    cmd = "[ this , result ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function at (x0,index=2)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    if size(result,1) <> 1 then
      errmsg = msprintf ( gettext ( "%s: The result of the cost function at (x0,index=2) has %d rows, instead of only 1." ) , "optimbase_checkcostfun" , size(result,1))
      error ( errmsg );
    end
    if ( size(result,2) <> this.nbineqconst ) then
      errmsg = msprintf ( gettext ( "%s: The result of the cost function at (x0,index=2) has %d columns, instead of the number of constraints %d." ) , "optimbase_checkcostfun" , size(result,2) , this.nbineqconst )
      error ( errmsg );
    end
    // index = 3
    index = 3;
    cmd = "[ this , result ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function at (x0,index=3)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    if size(result,1) <> 1 then
      errmsg = msprintf ( gettext ( "%s: The result of the cost function at (x0,index=3) has %d rows, instead of only 1." ) , "optimbase_checkcostfun" , size(result,1))
      error ( errmsg );
    end
    if ( size(result,2) <> this.nbineqconst + 1 ) then
      errmsg = msprintf ( gettext ( "%s: The result of the cost function at (x0,index=3) has %d columns, instead of the number of constraints %d + 1." ) , "optimbase_checkcostfun" , size(result,2) , this.nbineqconst )
      error ( errmsg );
    end
  end    
endfunction

