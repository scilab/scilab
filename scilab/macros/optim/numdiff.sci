function %g=numdiff(%f,%x,tol)
// given a function %f from R^n to R^p
//computes the matrix g such as 
//    [   d f    ]
//    [      i   ]
//g = [   ----   ]
// ij [   d x    ]
//    [      j   ]
// using finite difference methods
[lhs,rhs]=argn(0)
if rhs<3 then
  tol=sqrt(%eps)*[1 1d-3]
end
if type(%f)==15 then
  params=%f;params(1)=null();
  %f=%f(1)
end

%x=%x(:);
%n=size(%x,'*')
%dx=sqrt(%eps)*(1+1d-3*abs(%x))
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
