function r=%c_b_s(varargin)
// Copyright INRIA
if size(varargin)==3 then
  if type(varargin(1))==10 & type(varargin(2))==1 & type(varargin(3))==10 then
    r=asciimat(asciimat(varargin(1)):varargin(2):asciimat(varargin(3)))
  end
else
  error(43)
end
endfunction
