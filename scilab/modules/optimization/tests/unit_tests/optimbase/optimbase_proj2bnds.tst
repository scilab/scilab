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
// Test proj2bnds method
//
//
// Test with bounds
//
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-verbose",1);
opt = optimbase_configure ( opt , "-boundsmin" , [-5.0 -5.0] );
opt = optimbase_configure ( opt , "-boundsmax" , [5.0 5.0] );
[ opt , p ] = optimbase_proj2bnds ( opt ,  [0.0 0.0] );
assert_checkequal ( p , [0.0 0.0] );
[ opt , p ] = optimbase_proj2bnds ( opt ,  [-6.0 6.0] );
assert_checkequal ( p , [-5.0 5.0] );
opt = optimbase_destroy(opt);
//
// Test without bounds
//
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-verbose",1);
[ opt , p ] = optimbase_proj2bnds ( opt ,  [0.0 0.0] );
assert_checkequal ( p , [0.0 0.0] );
opt = optimbase_destroy(opt);

