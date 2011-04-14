// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->




//
// Test optimbase_checkbounds method
//
//
opt = optimbase_new ();
opt = optimbase_configure ( opt , "-numberofvariables",2);
opt = optimbase_configure ( opt , "-verbose",1);
// The bounds are consistent
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
[ opt , isok ] = optimbase_checkbounds ( opt );
assert_checkequal ( isok , %T );
// The min bound does not have a consistent size
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0 10.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
[ opt , isok ] = optimbase_checkbounds ( opt );
assert_checkequal ( isok , %F );
// The max bound does not have a consistent size
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0 10.0] );
[ opt , isok ] = optimbase_checkbounds ( opt );
assert_checkequal ( isok , %F );
// The bounds are not consistent
opt = optimbase_configure ( opt , "-boundsmin" , [5.0 5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [-5.0 -5.0] );
[ opt , isok ] = optimbase_checkbounds ( opt );
assert_checkequal ( isok , %F );
opt = optimbase_destroy(opt);

