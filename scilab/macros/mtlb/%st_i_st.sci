function x=%st_i_st(varargin)
//insert the struct varargin($-1) into the struct varargin($)
//at position varargin(1), varargin(2),varargin(3),...
//insert the struct varargin($-1) as the field varargin(1)
//of the struct varargin($)
x=varargin($);
w=varargin($-1);all_fields=getfield(1,w);fields=all_fields(3:$);
N=size(fields,'*');
if type(varargin(1))==10
flds=getfield(1,x);flds=[flds,varargin(1)];
setfield(1,flds,x);setfield(lstsize(x)+1,w,x);
return
end  
[indx,I]=convertindex(double(x.dims),varargin(1:$-2))
if size(getfield(1,x),'*')==N+2
for k=1:N
liste=getfield(k+2,x);
tmp=getfield(k+2,w);
if type(tmp)==15 then
for i=I(:)'
liste(i)=tmp(i);
end
//liste(I)=getfield(k+2,w); ?
setfield(k+2,liste,x);
else
liste(I)=tmp;
setfield(k+2,liste,x);
end
end
else
//field in w which is non-existing in x
setfield(1,getfield(1,w),x);
liste=list();
for kkk=1:prod(indx)
liste(kkk)=[];
end
liste(I)=getfield(N+2,w);
setfield(N+2,liste,x);
end
return
endfunction
