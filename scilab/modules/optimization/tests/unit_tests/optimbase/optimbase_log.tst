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




function y = rosenbrock (x)
    y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test basic verbose
//
warning("off"); // WARNING_EMPTY_OPS
opt = optimbase_new ();
computed = optimbase_cget(opt,"-verbose");
assert_checkequal ( computed , 0 );
opt = optimbase_configure(opt,"-verbose",1);
computed = optimbase_cget(opt,"-verbose");
assert_checkequal ( computed , 1 );
optimbase_log ( opt , "My interesting message" );
opt = optimbase_configure(opt,"-verbose",0);
// Log a message relative to the stopping rule
computed = optimbase_cget(opt,"-verbosetermination");
assert_checkequal ( computed , 0 );
opt = optimbase_configure(opt,"-verbosetermination",1);
computed = optimbase_cget(opt,"-verbosetermination");
assert_checkequal ( computed , 1 );
opt = optimbase_stoplog ( opt , "My interesting stop message" );
opt = optimbase_configure(opt,"-verbosetermination",0);
opt = optimbase_destroy(opt);

//
// Test logging into a file
//
opt = optimbase_new ();
opt = optimbase_configure(opt,"-verbose",1);
computed = optimbase_cget(opt,"-logfile");
assert_checkequal ( computed , "" );
opt = optimbase_configure(opt,"-logfile",fullfile(TMPDIR,"mylogfile.txt"));
computed = optimbase_cget(opt,"-logfile");
assert_checkequal ( computed , fullfile(TMPDIR,"mylogfile.txt") );
opt = optimbase_log ( opt , "My interesting message" );
opt = optimbase_configure(opt,"-verbose",0);
opt = optimbase_log ( opt , "My NOT interesting message" );
opt = optimbase_destroy(opt);
computed = fileinfo ( fullfile(TMPDIR,"mylogfile.txt") );
assert_checkequal ( ( computed <> [] ) , %t );
// Check content
fd = mopen( fullfile(TMPDIR,"mylogfile.txt") , "r" );
computed = mgetl ( fd , 1 ); // The date is not checked
computed = mgetl ( fd , 1 );
assert_checkequal ( computed , "My interesting message" );
computed = meof ( fd );
assert_checkequal ( computed , 0 );
mclose ( fd );
computed = deletefile ( fullfile(TMPDIR,"mylogfile.txt") );
assert_checkequal ( computed , %t );


