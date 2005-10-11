function [e,a]=pen2ea(f)
// Copyright INRIA
  e=coeff(f,1);
  a=-coeff(f,0);
endfunction
