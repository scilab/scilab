function [f,g,ind]=NDcost(x,ind,fun,varargin)
//external for optim 
//computes gradient using Code differentiation
  if argn(2)<4 then varargin=list(),end
  f=fun(x)
  g=derivative(list(fun,varargin(:)),x)
endfunction
