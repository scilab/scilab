function x=%hm_cumprod(m,d)
  if argn(2)==1 then
    x=hypermat(m.dims,cumprod(m.entries))
    return
  end
  dims=m.dims;
  if d=='m' then
    d=find(dims>1,1)
    if d==[] then x=cumprod(m.entries),return,end
  end

  if type(dims==8) then flag=1; dims=double(dims); else flag=0;end
  N=size(dims,'*');
  p1=prod(dims(1:d-1));//summation step
  p2=p1*dims(d);//step for next to sum
  ind=(0:p1:p2-1);// selection for summation
  deb=(1:p1)';
  I=deb*ones(ind)+ones(deb)*ind


  ind=(0:p2:prod(dims)-1)';
  I=ones(ind).*.I+ind.*.ones(I)
  x=cumprod(matrix(m.entries(I),-1,dims(d)),2)
  x(I)=matrix(x,-1,1)
  if flag==1 then dims=int32(dims);end
  x=hypermat(dims,x)
endfunction
