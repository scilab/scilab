function [nh]=h2norm(g,tol)
//
//                 /+00
//     2           |          *
//  |g| =1/(2*%pi).|trace[g(jw).g(jw)]dw
//     2           |
//                 /-00
// Copyright INRIA
if type(g)==1,if norm(g)==0,nh=0,return,end,end,
[lhs,rhs]=argn(0),
if rhs==1 then tol=1000*%eps,end;
g1=g(1);
if g1(1)=='lss' then
  if norm(g(5))>0 then error('non zero D'),end;
  sp=spec(g(2)),
  if maxi(real(sp))>=-tol then
    error('unstable system!'),
  end,
  w=obs_gram(g(2),g(4),'c'),
  nh=abs(sqrt(sum(diag(g(3)'*w*g(3))))),return,
else,
  num=g(2),den=g(3),
  s=poly(0,varn(den)),
  [t1,t2]=size(num),
  for i=1:t1,
    for j=1:t2,
      n=num(i,j),d=den(i,j),
      if coeff(n)==0 then 
	nh(i,j)=0,
      else
	if degree(n)>=degree(d) then
	  error('improper system'),
	end
	pol=roots(d),
	if maxi(real(pol))>-tol then
	error('unstable system!'),end,
	nt=horner(n,-s),dt=horner(d,-s),
	nh(i,j)=residu(n*nt,d,dt),
      end,
    end
  end
  nh=sqrt(sum(nh)),return,
end
endfunction
