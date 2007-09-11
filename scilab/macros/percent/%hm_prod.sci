function x=%hm_prod(m,d)
  if argn(2)==1 then
    x=prod(m.entries)
    return
  end
  dims=m.dims;
  if d=='m' then
    d=find(dims>1,1)
    if d==[] then x=prod(m.entries),return,end
  end

  if type(dims==8) then flag=1; dims=double(dims); else flag=0;end
  N=size(dims,'*');
  p1=prod(dims(1:d-1));//product step
  p2=p1*dims(d);//step for next to prod
  ind=(0:p1:p2-1)';// selection for product
  deb=(1:p1);
  I=ind*ones(deb)+ones(ind)*deb 

  ind=(0:p2:prod(dims)-1);
  I=ones(ind).*.I+ind.*.ones(I)

  x=prod(matrix(m.entries(I),dims(d),-1),1)
  dims(d)=1
  while  dims($)==1 then dims($)=[],end
  if d==N then
    dims=dims(1:$)
  else
    dims(d)=1
  end
  if size(dims,'*')==2 then 
    x=matrix(x,dims(1),dims(2))
  elseif dims<>[] then
    if flag==1 then dims=int32(dims);end
    x=hypermat(dims,x)
  end
endfunction
