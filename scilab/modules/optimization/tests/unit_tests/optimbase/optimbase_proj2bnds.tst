// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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

