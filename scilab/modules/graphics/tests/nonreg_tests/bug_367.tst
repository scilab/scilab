// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 367 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=367
//
// <-- Short Description -->
// Apparently an error is raised in a script.

// should complain about strf (normally a strig of length 3).
x=rand(1,1000);
t1=x(1)-x(1);
x2=x(2:$);
x=x(1:$-1);
x3=[0 x(1:$-1)];
t=x2-x;
t2=x-x3;
clear x3;
clear x2;
t3=t.*t2;
clear t2;
clear t;
w=find(t3>0);
t3(w)=0;
clear w;
t3=abs(t3);
//t=[t1 t];
x0=0;
y0=0;
rep=[x0,y0,-1];
for i=1:5:100
    origen=i; //*100+1;
    xsetech([0,0,1,0.5]);
    plot(x(origen:origen+99));
    xsetech([0,0.5,1,0.5]);
    plot2d3('enn',1,t3(origen:origen+99)',2,);
    rep=xgetmouse();
    if rep(3)~=-1 then 
          p=locate(2);
          v=round(p(1,:)); 
          xset("window",2)
          xsetech([0,0,1,0.5]),
          plot(x(origen+v(1):origen+v(2)));
          xsetech([0,0.5,1,0.5]),
          plot2d3('enn',1,t3(origen+v(1):origen+v(2))',2);
          xclick();
          xdel();
    end  
    clf();
end;




