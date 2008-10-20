function r=%i_b_s(varargin)
//a:b where a is int and b double
//a:b:c where a is int,  b double and c every type
  if round(varargin(1))<>varargin(1) then
    error('Colon operands should have integer values')
  end
  if size(varargin)==2 then //a:b
    it=inttype(varargin(1))
    r=varargin(1):iconvert(varargin(2),it)
  else
    it=inttype(varargin(1))
    r=varargin(1):iconvert(varargin(2),it):iconvert(varargin(3),it)
  end
endfunction


