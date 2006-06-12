function r=%l_isequal(x1,varargin)
  narg=size(varargin)
  if narg==0 then error('isequal requires at least two input arguments'),end

  r=%t
  m=lstsize(x1)

  for k=1:narg
   n=lstsize(varargin(k))
   r=r&(n==m)
   if ~r then return,end
  end

  for k=1:narg
    for l=1:m
      r=r&isequal(getfield(l,x1),getfield(l,varargin(k)))
      if ~r then return,end
    end
  end
endfunction

