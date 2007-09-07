function new=addcolor(c)
// add colors to the current colormap
//!
// Copyright INRIA

//Modif. to accept 3x1 column vector
  if or(size(c)==1) then c=matrix(c,1,-1),end
  
  if size(c,'c')<>3 | max(c)>1 | min(c)<0 then
    error('addcolor : argument is not a color table')
  end
  f=gcf();
  if type(f)==1 then//old mode
    cmap=xget('colormap')
  else
    cmap=f.color_map;
  end
  ncol=size(cmap,'r')
  ncol0=ncol
  new=[]
  for k=1:size(c,'r')
    ck=c(k,:)
    k2=find(sum(abs(cmap-ones(ncol,1)*ck),2)<=1.d-5)
    if k2==[] then 
      cmap=[cmap;ck],
      ncol=ncol+1
      new=[new ncol]
    else
      new=[new k2(1)]
    end 
  end
  if ncol>ncol0 then //at least one color added
    if type(f)==1 //old mode
      xset('colormap',cmap)
    else
      f.color_map=cmap
    end
  end
  
endfunction
