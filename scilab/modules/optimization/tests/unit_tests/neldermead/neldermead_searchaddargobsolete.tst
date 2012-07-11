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
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

// This test is designed to produce a warning:
// this warning is localized.
// This is why we do not check the ref.
// Checking the ref file under Linux fails, because
// <-- ENGLISH IMPOSED --> has no effect there.
// See : http://bugzilla.scilab.org/show_bug.cgi?id=9284



x0 = [11.0 140.0]';


//
// In this case, the mydata variable is passed
// explicitely by the neldermead class.
// So the actual name "mydata" does not matter
// and whatever variable name can be used.
//
function [ y , index ] = rosenbrock2 ( x , index , mydata )
  a = mydata.a
  y = 100*(x(2)-x(1)^2)^2 + ( a - x(1))^2;
endfunction

//
// Check backward compatibility with the "-costfargument" option.
//
mystuff = tlist(["T_MYSTUFF","a"]);
mystuff.a = 12.0;
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock2);
nm = neldermead_configure(nm,"-costfargument",mystuff);
nm = neldermead_configure(nm,"-x0",x0);
nm = neldermead_configure(nm,"-maxfunevals",%inf);
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_search(nm);
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
nm = neldermead_destroy(nm);

