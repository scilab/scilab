function %g=numdiff(%f,%x,%dx)
// given a function %f from R^n to R^p
//computes the matrix g such as 
//    [   d f    ]
//    [      i   ]
//g = [   ----   ]
// ij [   d x    ]
//    [      j   ]
// using finite difference methods
if type(%f)==15 then
  params=%f;params(1)=null();
  %f=%f(1)
else
  params=list()
end
if %f==10 then //hard coded function given by its name
  error('hard coded function not allowed, create a Scilab function using call""')
end

%x=%x(:);
%n=size(%x,'*')
if argn(2)<3 then
  %dx=sqrt(%eps)*(1+1d-3*abs(%x))
end
if size(params)==0 then
  %y0=%f(%x)
  %g(size(%y0,1),%n)=0
  for %j=1:%n
    %v=0*%x;%v(%j)=%dx(%j);
    %g(:,%j)=(-%y0+%f(%x+%v))/%dx(%j);
  end
else
  %y0=%f(%x,params(:))
  %g(size(%y0,1),%n)=0
  for %j=1:%n
    %v=0*%x;%v(%j)=%dx(%j);
    %g(:,%j)=(-%y0+%f(%x+%v,params(:)))/%dx(%j);
  end 
end
endfunction
