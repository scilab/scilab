function [J,J2]=derivative(f,x0,h)
// Approximate derivatives of a function f: R^n --> R^p
// Derivatives are evaluated by finite difference:f_i(x+h)-f_i(x))/h
// 
// f: R^n --> R^p  = Scilab function.
// J = p x n Jacobian matrix
// J2 = p x (p x n) Second derivative matrix
// f(x) = f(x0) + D1f(x0) *dx + 1/2* D2f(x0) *(dx.*.dx) +...
// J = D1f(x0) ; J2=D2f(x0)
[lhs,rhs]=argn(0);
if rhs==2 then h=0.0000001;end
n=size(x0,1);
s=f(x0);
m=size(s,1);
for l=1:m
   fl=s(l);
for k=1:n
   z=x0;
   z(k)=x0(k)+h;
   w=f(z);
   J(l,k)=(w(l)-fl)/h;
end
end
if lhs==1 then J2=[];return;end
deff('z=phi(x)','z=jaco(f,x,h),z=z(:)');
x=x0;
W=jaco(phi,x,h);
p=size(f(x),1)
n=size(x,1);
J2=zeros(p,n^2);
W=W(:);
col=1;
l=1;L=p;
for k=1:n^2
  J2(:,col)=W(l:L);
  col=col+1;
  l=L+1;L=l+p-1;
end
//deff('y=f(x)','y=[sin(x(1))*cos(x(2));x(1)^2*x(2)^2;1+x(1)*x(2)^2]')
//x0=[1;2];eps=0.001;h=eps*[2;5];[J,J2]=derivative(f,x0);[f(x0+h)-f(x0)-J*h ,0.5*J2*(h.*.h)]



function J=jaco(F,y,h)
//J=jacobian(F) at y
//Auxiliary function
[lhs,rhs]=argn(0);
if rhs==2 then h=0.000001;end
n=size(x0,1);
s=F(y);
m=size(s,1);
for l=1:m
   fl=s(l);
for k=1:n
   z=y;
   z(k)=y(k)+h;
   w=F(z);
      J(l,k)=(w(l)-fl)/h;
end
end


