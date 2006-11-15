function r=%s_b_i(varargin)
  if size(varargin)==2 then
    r=varargin(1):double(varargin(2))
  else 
    r=varargin(1):double(varargin(2)):real(double(varargin(3)))
  end
endfunction
