function s=%st_E(varargin)
  s=varargin(2)
  f=getfield(1,s);f=f(3:$)
  for k=1:size(f,'*')
     s(f(k))=[]
  end
  s.dims=[1 1]
endfunction
