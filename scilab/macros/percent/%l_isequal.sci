function r=%l_isequal(x1,varargin)
  n=size(varargin)
  if n==0 then error('isequal requires at least two input arguments'),end

  r=%t
  m=lstsize(x1)

  for l=1:m
    for k=1:n
      r=r&isequal(getfield(l,x1),getfield(l,varargin(k)))
      if ~r then break,end
    end
  end
endfunction
