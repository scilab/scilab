function o=obsv_mat(a,c)
// Copyright INRIA
[lhs,rhs]=argn(0)
select type(a)
 case 1  then
    if rhs==1 then error('2 arguments : a,c'),end
    [m,n]=size(a)
    if m<>n then error(20,1),end
    [mb,nb]=size(c);if nb<>n then error(60),end
  case 16 then
    flag=a(1)
    if flag(1)<>'lss' then error(91,1),end
    [a,c]=a([2,4])
    [n,n]=size(a)
 else error('(a,c) pair or syslin list')
end;
o=c;for k=1:n-1, o=[c;o*a],end
endfunction
