function [nArgOut,vectInput]=check3dFun(funName,func,X,Y)

// In this function we determine wether the function 'func'
// has the syntax [z]=func(x,y) or [x,y,z]=func(u,v). 
// We also test if the function accepts vector input
// arguments.
//
// This allow to understand in a 3d plot function (surf, mesh)
// if the user has requested a 3d plot of a parametric or
// non-parametric surface. 

// First We get the two strings in and out containing the
// names of input and output arguments :

[out,in,text]=string(func);

nArgOut=max(size(out));
nArgIn=max(size(in))

if nArgIn~=2
   error(sprintf('%s : function must accept two input arguments',funName));
end

if nArgOut~=1 & nArgOut~=3
   error(sprintf('%s : function must have 1 or 3 output arguments',funName));
end

// Now we test if func accepts vector inputs (we test with the X,Y
// pair provided by the user)

n=min(length(X),length(Y));
X=X(1:n);
Y=Y(1:n);

if nArgOut==1;

   ierr=execstr('Z=func(X,Y)','errcatch');
   if ierr==0
      vectInput=(and(size(Z)==size(X)));
   else
      vectInput=%F;
   end

elseif nArgOut==3
  
  // now we test if func accepts vector inputs (as above)
  
  ierr=execstr('[xf,yf,zf]=func(X,Y)','errcatch');
 
  vectInput=(ierr==0 & and([size(xf)==size(yf) ... 
                            size(yf)==size(zf) ...
			    size(zf)==size(X)]));
end

endfunction
