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
// Here, the cost function is OK
//
function [ y , index ] = rosenbrockOk ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-x0",[1.1 1.1]');
opt = optimbase_configure(opt,"-function",rosenbrockOk);
opt = optimbase_checkcostfun(opt);
opt = optimbase_destroy(opt);

//
// Here, the cost function is not callable
//
function [ y , index ] = rosenbrock2 ( x , index )
  y = fdsmklqfjdsf;
endfunction
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-x0",[1.1 1.1]');
opt = optimbase_configure(opt,"-function",rosenbrock2);
cmd = "opt = optimbase_checkcostfun(opt);";
lclmsg = "%s: Cannot evaluate cost function with ""%s"": %s";
scimsg = msprintf(_("Undefined variable: %s\n"), "fdsmklqfjdsf");
assert_checkerror(cmd,lclmsg,[],"optimbase_checkcostfun","[f,index]=costf(x0,1)", scimsg);
opt = optimbase_destroy(opt);

//
// Here, the cost function is callable, but returns a matrix,
// instead of a scalar.
//
function [ y , index ] = rosenbrock3 ( x , index )
  y = ones(10,10);
endfunction
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-x0",[1.1 1.1]');
opt = optimbase_configure(opt,"-function",rosenbrock3);
cmd = "opt = optimbase_checkcostfun(opt);";
assert_checkerror(cmd,"%s: The matrix %s from costf(x0,%d) has %d rows, instead of %d.",[],"optimbase_checkcostfun","f",2,10,1);
opt = optimbase_destroy(opt);

//
// Test with good non linear constraints
//
function [ f , c , index ] = optimtestcase ( x , index )
  f = []
  c = []
  if ( index == 2 | index == 6 ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( index == 5 | index == 6 ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3]
  end
endfunction
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",4);
opt = optimbase_configure(opt,"-function",optimtestcase);
opt = optimbase_configure(opt,"-x0",[0.0 0.0 0.0 0.0]');
opt = optimbase_configure(opt,"-nbineqconst",3);
opt = optimbase_checkcostfun(opt);
opt = optimbase_destroy(opt);

//
// Test a wrong configuration: we forgot to configure "-nbineqconst"
//
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",4);
opt = optimbase_configure(opt,"-function",optimtestcase);
opt = optimbase_configure(opt,"-x0",[0.0 0.0 0.0 0.0]');
cmd="opt = optimbase_checkcostfun(opt)";
errmsg = "%s: The matrix %s from costf(x0,%d) has %d rows, instead of %d.";
assert_checkerror(cmd,errmsg,[], "optimbase_checkcostfun" , "index" , 1 , 0 , 1);
opt = optimbase_destroy(opt);

//
// Test with wrong  non linear constraints f(x0,2) is not a row vector
// The cost function transposes the column vector.
//
function [ f , c , index ] = optimtestcase2 ( x , index )
  f = []
  c = []
  if ( index == 2 | index == 6 ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( index == 5 | index == 6 ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3].'
  end
endfunction
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",4);
opt = optimbase_configure(opt,"-function",optimtestcase2);
opt = optimbase_configure(opt,"-x0",[0.0 0.0 0.0 0.0]');
opt = optimbase_configure(opt,"-nbineqconst",3);
opt = optimbase_checkcostfun(opt);
opt = optimbase_destroy(opt);

//
// Test with wrong  non linear constraints f(x0,2) is a row vector with 5 components instead of 3
//
function [ f , c , index ] = optimtestcase3 ( x , index )
  f = []
  c = []
  if ( index == 2 | index == 6 ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( index == 5 | index == 6 ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3 0.0 0.0]
  end
endfunction
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",4);
opt = optimbase_configure(opt,"-function",optimtestcase3);
opt = optimbase_configure(opt,"-x0",[0.0 0.0 0.0 0.0]');
opt = optimbase_configure(opt,"-nbineqconst",3);
cmd = "opt = optimbase_checkcostfun(opt);";
assert_checkerror(cmd,"%s: The matrix %s from costf(x0,%d) has %d columns, instead of %d.",[],"optimbase_checkcostfun","c",5,5,3);
opt = optimbase_destroy(opt);

//
// Test with wrong  non linear constraints f(x0,3) is a column vector
// The cost function transposes the column vector.
//
function [ f , c , index ] = optimtestcase4 ( x , index )
  f = []
  c = []
  if ( index == 2 | index == 6 ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( index == 5 | index == 6 ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3].'
  end
endfunction
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",4);
opt = optimbase_configure(opt,"-function",optimtestcase4);
opt = optimbase_configure(opt,"-x0",[0.0 0.0 0.0 0.0]');
opt = optimbase_configure(opt,"-nbineqconst",3);
opt = optimbase_checkcostfun(opt);
opt = optimbase_destroy(opt);

//
// Test with wrong  non linear constraints f(x0,3) is a row vector with 5 columns instead of 4
//
function [ f , c , index ] = optimtestcase5 ( x , index )
  f = []
  c = []
  if ( index == 2 | index == 6 ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( index == 5 | index == 6 ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3 0.0]
  end
endfunction
opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",4);
opt = optimbase_configure(opt,"-function",optimtestcase5);
opt = optimbase_configure(opt,"-x0",[0.0 0.0 0.0 0.0]');
opt = optimbase_configure(opt,"-nbineqconst",3);
cmd = "opt = optimbase_checkcostfun(opt);";
assert_checkerror(cmd,"%s: The matrix %s from costf(x0,%d) has %d columns, instead of %d.",[],"optimbase_checkcostfun","c",5,4,3);
opt = optimbase_destroy(opt);


//
// Test with correct rosenbrock function
//
function [ f , g , index ] = rosenbrock ( x , index )
  f = 100.0 *(x(2)-x(1)^2)^2 + (1-x(1))^2;
  g(1,1) = - 400. * ( x(2) - x(1)**2 ) * x(1) -2. * ( 1. - x(1) )
  g(1,2) = 200. * ( x(2) - x(1)**2 )
endfunction

opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-function", rosenbrock );
opt = optimbase_configure(opt,"-withderivatives",%t);
opt = optimbase_configure(opt,"-x0",[-1.2 1.0].');
opt = optimbase_checkcostfun(opt);
opt = optimbase_destroy(opt);
//
// Test with not correct rosenbrock function : g is a column vector instead of row vector
// The cost function transposes the column vector.
function [ f , g , index ] = rosenbrock4 ( x , index )
  f = 100.0 *(x(2)-x(1)^2)^2 + (1-x(1))^2;
  g(1) = - 400. * ( x(2) - x(1)**2 ) * x(1) -2. * ( 1. - x(1) )
  g(2) = 200. * ( x(2) - x(1)**2 )
endfunction

opt = optimbase_new ();
opt = optimbase_configure(opt,"-numberofvariables",2);
opt = optimbase_configure(opt,"-function", rosenbrock4 );
opt = optimbase_configure(opt,"-withderivatives",%t);
opt = optimbase_configure(opt,"-x0",[-1.2 1.0].');
opt = optimbase_checkcostfun(opt);
opt = optimbase_destroy(opt);

