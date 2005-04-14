function r=%c_b_c(varargin)
// Copyright INRIA
if  size(varargin)==2 then
  if type(varargin(1))==10 & type(varargin(2))==10 then
    r=asciimat(asciimat(varargin(1)):asciimat(varargin(2)))
  end
elseif size(varargin)==3 then
  if type(varargin(1))==10 & type(varargin(2))==10 & type(varargin(3))==10 then
    r=asciimat(asciimat(varargin(1)):asciimat(varargin(2)):asciimat(varargin(3)))
  end
else
  error(43)
end
endfunction
