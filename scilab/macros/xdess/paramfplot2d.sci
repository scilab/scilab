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
//deff('y=f(x,t)','y=t*sin(x)')
//x=linspace(0,2*%pi,50);paramfplot2d(f,x,0:0.05:1);
[lhs,rhs]=argn(0)
x=x(:);
if rhs==4 then
   xmin=min(x);xmax=max(x);
   ymin=%inf;ymax=-%inf;
   for t=theta
      y=f(x,t); ymin=min(ymin,min(y)); ymax=max(ymax,max(y));
   end
   rect=[xmin,ymin,xmax,ymax];
end

if rhs==3 then   //set flag='no' find rect=[xmin,ymin,xmax,ymax];
   xmin=min(x);xmax=max(x);
   ymin=%inf;ymax=-%inf;
   for t=theta
        y=feval(x,t,f);// same as y=f(x,t); 
	ymin=min(ymin,min(y)); ymax=max(ymax,max(y));
   end
   flag='no'
   rect=[xmin,ymin,xmax,ymax];
end

plot2d(0,0,0,"010"," ",rect);  //First plot to set the rectangle 
//    Animated plot
xset('pixmap',1);
if flag=='no' then
  for t=theta
    xset("wwpc");
    plot2d(x,feval(x,t,f)) //feval(x,t,f) same as f(x,t) if f is a scilab fctn.
    xset("wshow");
  end
else
  for t=theta
    plot2d(x,feval(x,t,f)) //feval(x,t,f) same as f(x,t) if f is a scilab fctn.
    xset("wshow");
  end
end

xset("pixmap",0);
endfunction
