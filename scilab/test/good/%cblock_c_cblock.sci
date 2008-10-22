function a=%cblock_c_cblock(a,b)
  na=length(a)
  if na==1 then a=b,return,end
  if length(b)==1 then return,end
  v1=getfield(na,a)
  v2=getfield(2,b)
  
  if size(v1,1)<>size(v2,1) then error(5),end
  if type(v1)==type(v2) then
    setfield(na,[v1 v2],a)
  else
    setfield(na+1,v2,a);na=na+1
  end
  
  for k=3:length(b)
    setfield(na+1,getfield(k,b),a)
    na=na+1
  end
endfunction
