function s=%st_e(varargin)
  s=varargin($)
  sz=size(getfield(1,s),'*')
  
  for k=2:sz
    x=getfield(k,s)
    if size(x,'*')>1 then
      x=x(varargin(1:$-1))
      setfield(k,x,s)
    end
  end
endfunction
