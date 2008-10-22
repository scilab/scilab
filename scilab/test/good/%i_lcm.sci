function [q,fact]=%i_lcm(p)
//p=lcm(p) computes the lcm of polynomial vector p
//[pp,fact]=lcm(p) computes besides the vector fact of factors 
//such that  p.*fact=pp*ones(p)
//!
// Copyright INRIA
  [x,uu]=%i_gcd(p)
  q=prod(p/x($))*x($)
  fact=q./p
endfunction
