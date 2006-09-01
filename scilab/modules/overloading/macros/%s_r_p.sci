function f=%s_r_p(m,p)
//f=M/p  M:scalar matrix p=polynomial
//!
// Copyright INRIA

[mp,np]=size(p)
if mp*np<>1 then 
  f=m*invr(p),
else
  [l,c]=size(m)
  if m<>[] then m=m+poly(0,varn(p),'c'),end
  if mp==-1&l*c==1|l==-1 then
    f=rlist(m,p*eye(),[])
  else
    f=simp(rlist(m,p*ones(l,c),[]))
  end
end
endfunction
