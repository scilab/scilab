function f=%p_l_s(p,m)
// f=%p_l_s(p,m) <=>f= p\m.
//!
// Copyright INRIA
[mp,np]=size(p)
if mp*np<>1 then 
  f=invr(p)*m,
else
  [l,c]=size(m)
  if m<>[] then m=m+poly(0,varn(p),'c'),end
  if mp==-1&l*c==1|l==-1 then
    f=tlist(['r','num','den','dt'],m,p*eye(),[])
  else
    f=tlist(['r','num','den','dt'],m,p*ones(l,c),[])
  end
end
endfunction
