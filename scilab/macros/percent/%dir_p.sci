function %dir_p(L)
  ll= lines();ll=max(ll(1),20)
  name=L.name;
  if name==[] then mprintf("%s \n","[]"); return,end
  nc=6
  l1=length(name)
  while %t
    l2=l1;
    n=ceil(size(l1,'*')/nc)
    l2(n*nc)=0
    l=max(matrix(l2,-1,nc),'r');
    if sum(l)<ll|nc==1 then break,end
    nc=nc-1
  end
  name(n*nc)='';
  name=matrix(name,-1,nc)
  txt=emptystr(n,1)
  for k=1:nc
    txt=txt+part(name(:,k),1:l(k)+1)
  end
  mprintf("%s \n",txt)
  
endfunction

