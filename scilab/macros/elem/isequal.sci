function r=isequal(x1,varargin)
n=size(varargin)
if n==0 then error('isequal requires at least two input arguments'),end
r=%t
for k=1:n
  r=r&and(x1==varargin(k))
  if ~r then break,end
end
endfunction
