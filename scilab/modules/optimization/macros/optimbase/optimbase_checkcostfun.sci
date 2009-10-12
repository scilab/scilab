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
  if ( this.x0 == [] ) then
    errmsg = msprintf ( gettext ( "%s: Cannot check cost function when x0 is empty" ) , "optimbase_checkcostfun" )
    error ( errmsg );
  end
  //
  // If there are nonlinear constraints and no derivatives, check that the index is correctly managed.
  //
  if ( ( this.nbineqconst > 0 ) & ( ~this.withderivatives ) ) then
    //
    index = 1;
    cmd = "[ this , f , c , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,1)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    //
    index = 2;
    cmd = "[ this , f , c , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,2)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
    //
    index = 5;
    cmd = "[ this , f , c , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,5)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    this = optimbase_checkshape ( this , "c" , c , index , 1 , this.nbineqconst );
    //
    index = 6;
    cmd = "[ this , f , c , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,6)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
    this = optimbase_checkshape ( this , "c" , c , index , 1 , this.nbineqconst );
  end    
  //
  // If there are no nonlinear constraints and no derivatives, check that the index is correctly managed.
  //
  if ( ( this.nbineqconst == 0 ) & ( ~this.withderivatives ) ) then
    //
    index = 1;
    cmd = "[ this , f , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,1)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    //
    index = 2;
    cmd = "[ this , f , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,2)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
  end    
  //
  // If there are no nonlinear constraints and derivatives, check that the index is correctly managed.
  //
  if ( ( this.nbineqconst == 0 ) & ( this.withderivatives ) ) then
    //
    index = 1;
    cmd = "[ this , f , g , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,1)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    //
    index = 2;
    cmd = "[ this , f , g , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,2)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
    //
    index = 3;
    cmd = "[ this , f , g , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,3)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    this = optimbase_checkshape ( this , "g" , g , index , 1 , this.numberofvariables );
    //
    index = 4;
    cmd = "[ this , f , g , index ] = optimbase_function ( this , this.x0 , index )";
    ierr=execstr(cmd,"errcatch");
    if ierr <> 0 then
      errmsg = msprintf ( gettext ( "%s: Cannot evaluate cost function from costf(x0,4)." ) , "optimbase_checkcostfun" )
      error ( errmsg );
    end
    this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
    this = optimbase_checkshape ( this , "g" , g , index , 1 , this.numberofvariables );
  end    
endfunction


//
// optimbase_checkcostfun --
//   Check that the cost function is correctly connected.
//   Generate an error if there is one.
//
function this = optimbase_checkshape ( this , varname , data , index , expectednrows , expectedncols )
    if size(data,1) <> expectednrows then
      errmsg = msprintf ( gettext ( "%s: The matrix %s from costf(x0,%d) has %d rows, instead of %d." ) , "optimbase_checkcostfun" , varname , index , size(data,1) , expectednrows )
      error ( errmsg );
    end
    if size(data,2) <> expectedncols then
      errmsg = msprintf ( gettext ( "%s: The matrix %s from costf(x0,%d) has %d columns, instead of %d." ) , "optimbase_checkcostfun" , varname , index , size(data,2) , expectedncols )
      error ( errmsg );
    end
endfunction

