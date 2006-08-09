function [p,fact]=lcm(p)
//p=lcm(p) computes the lcm of polynomial vector p
//[pp,fact]=lcm(p) computes besides the vector fact of factors 
//such that  p.*fact=pp*ones(p)
//!
// Copyright INRIA
  if type(p)==8 then 
    if argn(1)==2 then [p,fact]=%i_lcm(p),else p=%i_lcm(p),end
    return,
  end

  [m,n]=size(p),
  p=matrix(p,m*n,1),
  p0=p(1);fact=1;
  for l=2:m*n,
    [u,v]=simp(p0,p(l)),
    p0=p0*v,
    fact=[v*fact,u],
  end,
  fact=matrix(fact,m,n),
  p=p0;
endfunction
