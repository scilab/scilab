function [f,g,ind]=NDcost(x,ind,fun,varargin)
//external for optim 
//computes gradient using Code differentiation
  if argn(2)<4 then varargin=list(),end
  if ind==2|ind==4 then
    f=fun(x,varargin(:))
  end
  if ind==3|ind==4 then
    g=derivative(list(fun,varargin(:)),x)
  end
endfunction
