function n=%s_i_i(i,j,f2,n)
[lhs,rhs]=argn(0)
if rhs==3 then
  n=f2;f2=j
  is=inttype(n)
  n(i)=iconvert(f2,is)
else
  is=inttype(n)
  n(i,j)=iconvert(f2,is)
end

