function s=%st_e(varargin)
//substruct x(i,j,k...)
w=varargin($);
sz=size(getfield(1,w),'*')
[indx,I]=convertindex(w.dims,varargin(1:$-1));
s=mlist(getfield(1,w),int32([1,1]));
for k=3:sz
    ww=getfield(k,w);
    ext=ww(I);
    setfield(k,ext,s)
  end
endfunction
