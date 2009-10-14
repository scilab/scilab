// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// nmplot_boxproblemA.sce --
//   Show that the Box algorithm is able to reproduce the 
//   numerical experiment presented in Box's paper.
//

mprintf("Illustrates Box'' algorithm on Box problem A.\n");
mprintf("Defining Box Problem A function...\n");

//
//  Reference:
//
//   M.J.Box, 
//   "A new method of constrained optimization 
//   and a comparison with other methods".
//   The Computer Journal, Volume 8, Number 1, 1965, 42--52
//   Problem A
//
//   Algorithm 454: the complex method for constrained optimization [E4]
//   Communications of the ACM, Volume 16 ,  Issue 8  (August 1973)
//   Pages: 487 - 489   
//
//   Richardson and Kuester Results :
//   F=1.0000
//   X1 = 3.0000
//   X2 = 1.7320
//   Iterations : 68
//
//
// Note 
//    The maximum bound for the parameter x1
//    is not indicated in Box's paper, but indicated in "Algo 454".
//    The maximum bound for x2 is set to 100/sqrt(3) and satisfies the constraint on x2.
//    The original problem was to maximize the cost function.
//    Here, the problem is to minimize the cost function.

//
// boxproblemB --
//   Computes the Box problem B cost function and 
//   inequality constraints.
//
// Arguments
//   x: the point where to compute the function
//   index : the stuff to compute
//   data : the parameters of Box cost function
//
function [ f , c , index ] = boxproblemB ( x , index )
  f = []
  c = []
  x3 = x(1) + sqrt(3.0) * x(2)
  if ( index==1 | index==3 ) then
    f = -(9.0 - (x(1) - 3.0) ^ 2) * x(2) ^ 3 / 27.0 / sqrt(3.0)
  end
  if ( index==2 | index==3 ) then
      c1 = x(1) / sqrt(3.0) - x(2)
      c2 = x3
      c3 = 6.0 - x3
      c = [c1 c2 c3]
  end
endfunction


//
// Initialize the random number generator, so that the results are always the
// same.
//
rand("seed" , 0)

x0 = [1.0 0.5].';
// Compute f(x0) : should be close to -0.0133645895646
fx0 = boxproblemB ( x0 );
mprintf("Computed fx0 = %e (expected = %e)\n",fx0 , -0.0133645895646 );
result = boxproblemB ( x0 , 2 );
mprintf("Computed Constraints(x0) = [%e %e %e]\n", ...
  result(1), result(2), result(3) );
mprintf("Expected Constraints(x0) = [%e %e %e]\n", ...
  0.0773503 , 1.8660254 , 4.1339746 );


xopt = [3.0 1.7320508075688774].';
// Compute f(xopt) : should be -1.0
fopt = boxproblemB ( xopt );
mprintf("Computed fopt = %e (expected = %e)\n", fopt , -1.0 );

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",boxproblemB);
nm = neldermead_configure(nm,"-x0",x0);
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-logfile" , "boxproblemB.txt" );
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-boundsmin",[0.0 0.0]);
nm = neldermead_configure(nm,"-boundsmax",[100.0 57.735026918962582]);
// Configure like Box
nm = neldermead_configure(nm,"-simplex0method","randbounds");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_configure(nm,"-tolxmethod" , %f );
nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
nm = neldermead_configure(nm,"-boxtermination" , %t );
nm = neldermead_configure(nm,"-boxtolf" , 0.001 );
nm = neldermead_configure(nm,"-boxboundsalpha" , 0.0001 );

//
// Check that the cost function is correctly connected.
// The index must be provided, because the additionnal argument "data"
// comes after.
//
[ nm , result ] = neldermead_function ( nm , x0 , 1 );
//
// Perform optimization
//
nm = neldermead_search(nm);
neldermead_display(nm);
xcomp = neldermead_get(nm,"-xopt");
mprintf("x computed=%s\n",strcat(string(xcomp)," "));
mprintf("x expected=%s\n",strcat(string(xopt)," "));
shift = norm(xcomp-xopt)/norm(xopt);
mprintf("Shift =%f\n",shift);
fcomp = neldermead_get(nm,"-fopt");
mprintf("f computed=%f\n",fcomp);
mprintf("f expected=%f\n",fopt);
shift = abs(fcomp-fopt)/abs(fopt);
mprintf("Shift =%f\n",shift);
nm = neldermead_destroy(nm);
deletefile ( "boxproblemB.txt" )

