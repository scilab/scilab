function res=%sp_norm(S,flag)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then flag=2;end //norm(S)=norm(S,2)
[m,n]=size(S)

if m==1|n==1 then //vector norm
  [ij,v]=spget(S);
  res=norm(v,flag);
  return
end

select flag
case 1 then
  res=maxi(ones(1,m)*abs(S))
case 2 then
  if m<n then
    S=S*S'
  elseif m>n then
    S=S'*S
  end
  tol=1.d-5;
  x = sum(abs(S),1)';
  res = norm(x);
  if res==0 then return,end
  x = x/res;res0 = 0;
  while abs(res-res0) > tol*res
    res0 = res;   Sx = S*x; res = norm(Sx);
    x = S'*Sx;
    x = x/norm(x);
  end
  if m<>n then res=sqrt(res),end
case %inf then
  res=maxi(abs(S)*ones(n,1))
case 'inf' then
  res=maxi(abs(S)*ones(n,1))
case 'fro' then
  [ij,v]=spget(S);
  res=sqrt(sum(abs(v.*v)))
else
  [ij,v]=spget(S);
  res=norm(v,flag);
end
endfunction
