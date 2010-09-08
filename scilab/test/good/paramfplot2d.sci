// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function paramfplot2d(f,x,theta,flag,rect)
//animated plot of x-->f(x,t) for t=theta(1),theta(2),etc
//x=N-vector of x-values 
//f(x,t)=N-vector of y-values.
//f: mapping x,t -> f(x,t) = R^N valued function for x= vector of R^N and t=real number. 
//f can be a either Scilab function or a dynamically linked routine since
// y=f(x,t) is evaluated as y=feval(x(:),t,f). See feval.
// Here y should be a column vector.
//theta = row vector of parameters theta=[theta(1), theta(2),... theta(M)]
// Optional parameters
//flag = 'yes' (screen is cleared between two consecutive plots).
//flag = 'no'  (screen is not cleared between two consecutive plots).
//
//rect = "rectangle" [xmin, xmax, ymin, ymax] (1 x 4 real vector),
// containing a-priori lower and upper bounds for x and f(t,x).
//function y=f(x,t),y=abs(cos(1.5*x+4*t)).*sin(x+10*t),endfunction
//x=linspace(0,20*%pi,500);theta=0:0.05:5;
[lhs,rhs]=argn(0)
if rhs<3 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "paramfplot2d", 3));
end

x=x(:);
if rhs<5 then //compute the data bounds
   xmin=min(x);xmax=max(x);
   ymin=%inf;ymax=-%inf;
   for t=theta
      y=f(x,t); ymin=min(ymin,min(y)); ymax=max(ymax,max(y));
   end
   rect=[xmin,xmax,ymin,ymax];
end
if rhs<4 then flag='no';end
realtimeinit(0.1);
 
clf();
fig=gcf();
a=gca();
a.data_bounds=matrix(rect,2,2);
a.axes_visible='on';
fig.pixmap='on'; //double buffer mode
y=feval(x,theta(1),f);
xpoly(x,y(:));p=gce(); //the polyline handle
realtime(0);
if flag=='no' then
  for k=1:size(theta,'*')
    realtime(k);
    y=feval(x,theta(k),f);
    p.data(:,2)=y(:);
    show_pixmap()
  end
else
  for k=1:size(theta,'*')
    realtime(k);
    plot2d(x,feval(x,theta(k),f))
    show_pixmap()
  end
end
fig.pixmap='off';

endfunction
