function x=%i_prod(varargin)
  x=varargin(1)
  it=inttype(x)
  x=prod(double(x),varargin(2:$))
  x=iconvert(x,it)
endfunction
