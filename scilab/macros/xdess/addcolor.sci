function new=addcolor(c)
// add colors to the current colormap
//!
// Copyright INRIA
if size(c,'c')<>3 then
  error('addcolor : argument is not a color table')
end
if maxi(c)>1|mini(c)<0 then
  error('addcolor : argument is not a color table')
end
cmap=xget('colormap')
ncol=size(cmap,'r')
ncol0=ncol
new=[]
if xget('use color')==1 then 
  for k=1:size(c,'r')
    ck=c(k,:)
    k2=find(abs(cmap-ones(ncol,1)*ck)*ones(3,1)<=1.d-5)
    if k2==[] then 
      cmap=[cmap;ck],
      ncol=ncol+1
      new=[new ncol]
    else
      new=[new k2(1)]
    end

  end
end
if ncol>ncol0 then xset('colormap',cmap),end
endfunction
