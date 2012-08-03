// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1540 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1540
//
x = %inf ;
printf ("x = %d\nx = %f\nx = %e\nx = %g\n" ,x,x,x,x) ;

x = %nan ;
printf ("x = %d\nx = %f\nx = %e\nx = %g\n" ,x,x,x,x) ;   

x = %nan ;
y = %inf ;
printf ("x = %d\ny = %f\nx = %e\ny = %g\n" ,x,x,x,x) ;   

x = %inf ;
printf ("x = %d\nx = %f\nx = %e\nx = %g\n" ,x,x,x,x) ;

x = %nan ;
printf ("x = %d\nx = %f\nx = %e\nx = %g\n" ,x,x,x,x) ;   

x = %nan ;
r = sprintf ("x = %d",x) ;   
if r <> 'x = Nan' then pause,end

y = %inf ;
r = sprintf ("y = %d",y) ;   
if r <> 'y = Inf' then pause,end

x = %nan ;
r = msprintf ("x = %d",x) ;   
if r <> 'x = Nan' then pause,end

y = %inf ;
r = msprintf ("y = %d",y) ;   
if r <> 'y = Inf' then pause,end
