function [a,b,c]=obsvss(a,b,c,tol)
// Copyright INRIA
[lhs,rhs]=argn(0)
select type(a)
case 1
 if lhs<>3 then error('3 outputs args: ac,bc,cc'),end
 select rhs
   case 3 then tol = 100*%eps
   case 4 then ,
   else error('3 or 4 inputs : a,b,c [,tol]')
 end;
case 16
 flag=a(1);
 if flag(1)<>'lss' then error(91,1),end;
 if lhs<>1 then error('1 output argument: sl'),end
 select rhs
   case 1 then tol=100*%eps
   case 2 then tol=b
   else error('1 or 2 inputs: sl [,tol]')
 end;
 [a,b,c,d,x0,dom]=a(2:7)
end;
//
[no,u]=contr(a',c',tol)
u=u(:,1:no)
a=u'*a*u;b=u'*b;c=c*u
if lhs==1 then a=syslin(dom,a,b,c,d,u'*x0),end
endfunction
