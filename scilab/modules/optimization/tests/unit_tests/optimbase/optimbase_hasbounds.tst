// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- CLI SHELL MODE -->


//
// Test hasbounds method
//
// Case where there is no bound
opt = optimbase_new ();
[ opt , hasbounds ] = optimbase_hasbounds ( opt );
assert_checkequal ( hasbounds , %F );
opt = optimbase_destroy( opt );
// Case where there are bounds
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
[ opt , hasbounds ] = optimbase_hasbounds ( opt );
assert_checkequal ( hasbounds , %T );
opt = optimbase_destroy(opt);

