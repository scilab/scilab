function r=%s_b_s(varargin)
  //a:b:c where a and b are double
  if round(varargin(1))<>varargin(1)|round(varargin(2))<>varargin(2) then
    error('Colon operands should have integer values')
  end
  select type(varargin(3))
  case 8 then
    it=inttype(varargin(3))
    pause
    r=iconvert(varargin(1),it):iconvert(varargin(2),it):varargin(3)
  else
    error('Not yet implemented')
  end
endfunction


