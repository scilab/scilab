// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
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
    // If there are no nonlinear constraints and no derivatives, check that the index is correctly managed.
    //
    if ( ( this.nbineqconst == 0 ) & ( ~this.withderivatives ) ) then
        //
        index = 1;
        cmd = "[ this , f , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,index]=costf(x0,1)" , ierr )
        this = optimbase_checkshape ( this , "index" , index , 1 , 1 , 1 );
        //
        index = 2;
        cmd = "[ this , f , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,index]=costf(x0,2)" , ierr )
        this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
        this = optimbase_checkshape ( this , "index" , index , 2 , 1 , 1 );
    end    
    //
    // If there are nonlinear constraints and no derivatives, check that the index is correctly managed.
    //
    if ( ( this.nbineqconst > 0 ) & ( ~this.withderivatives ) ) then
        //
        index = 1;
        cmd = "[ this , f , c , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,c,index]=costf(x0,1)" , ierr )
        this = optimbase_checkshape ( this , "index" , index , 1 , 1 , 1 );
        //
        index = 2;
        cmd = "[ this , f , c , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,c,index]=costf(x0,2)" , ierr )
        this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
        this = optimbase_checkshape ( this , "index" , index , 2 , 1 , 1 );
        //
        index = 5;
        cmd = "[ this , f , c , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,c,index]=costf(x0,5)" , ierr )
        this = optimbase_checkshape ( this , "c" , c , index , 1 , this.nbineqconst );
        this = optimbase_checkshape ( this , "index" , index , 5 , 1 , 1 );
        //
        index = 6;
        cmd = "[ this , f , c , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,c,index]=costf(x0,6)" , ierr )
        this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
        this = optimbase_checkshape ( this , "c" , c , index , 1 , this.nbineqconst );
        this = optimbase_checkshape ( this , "index" , index , 6 , 1 , 1 );
    end    
    //
    // If there are no nonlinear constraints and derivatives, check that the index is correctly managed.
    //
    if ( ( this.nbineqconst == 0 ) & ( this.withderivatives ) ) then
        //
        index = 1;
        cmd = "[ this , f , g , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,g,index]=costf(x0,1)" , ierr )
        this = optimbase_checkshape ( this , "index" , index , 1 , 1 , 1 );
        //
        index = 2;
        cmd = "[ this , f , g , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,g,index]=costf(x0,2)" , ierr )
        this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
        this = optimbase_checkshape ( this , "index" , index , 2 , 1 , 1 );
        //
        index = 3;
        cmd = "[ this , f , g , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,g,index]=costf(x0,2)" , ierr )
        this = optimbase_checkshape ( this , "g" , g , index , 1 , this.numberofvariables );
        this = optimbase_checkshape ( this , "index" , index , 3 , 1 , 1 );
        //
        index = 4;
        cmd = "[ this , f , g , index ] = optimbase_function ( this , this.x0 , index )";
        ierr=execstr(cmd,"errcatch");
        optimbase_checkstatus ( "[f,g,index]=costf(x0,4)" , ierr)
        this = optimbase_checkshape ( this , "f" , f , index , 1 , 1 );
        this = optimbase_checkshape ( this , "g" , g , index , 1 , this.numberofvariables );
        this = optimbase_checkshape ( this , "index" , index , 4 , 1 , 1 );
    end    
endfunction

//
// optimbase_checkstatus --
//   Generates an error if ierr is nonzero, that is, 
//   if the cost function could not be evaluated.
//
function optimbase_checkstatus ( cmdstr , ierr )
    if ( ierr <> 0 ) then
        lamsg = lasterror()
        lclmsg = gettext ( "%s: Cannot evaluate cost function with ""%s"": %s" )
        errmsg = msprintf (lclmsg , "optimbase_checkcostfun",cmdstr, lamsg )
        error ( errmsg );
    end
endfunction

//
// optimbase_checkcostfun --
//   Check that the cost function is correctly connected.
//   Generate an error if there is one.
//
function this = optimbase_checkshape ( this , varname , value , index , expectednrows , expectedncols )
    if ( size(value,1) <> expectednrows ) then
        errmsg = msprintf ( gettext ( "%s: The matrix %s from costf(x0,%d) has %d rows, instead of %d." ) , "optimbase_checkcostfun" , varname , index , size(value,1) , expectednrows )
        error ( errmsg );
    end
    if ( size(value,2) <> expectedncols ) then
        errmsg = msprintf ( gettext ( "%s: The matrix %s from costf(x0,%d) has %d columns, instead of %d." ) , "optimbase_checkcostfun" , varname , index , size(value,2) , expectedncols )
        error ( errmsg );
    end
endfunction

