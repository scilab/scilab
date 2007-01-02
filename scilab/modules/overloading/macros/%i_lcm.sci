function [q,fact]=%i_lcm(p)
//p=lcm(p) computes the lcm of polynomial vector p
//[pp,fact]=lcm(p) computes besides the vector fact of factors 
//such that  p.*fact=pp*ones(p)
//!
// Copyright INRIA, S. Steer
  q=p(1);
  for k=2:size(p,'*')
    q=q*p(k)/%i_gcd([q,p(k)]);
  end
  fact=q./p
endfunction
