function c=cont_mat(a,b)
//c=cont_mat(a,b) or c=cont_mat(sl) is the controllability matrix.
// of the pair a,b or of the system sl=[a,b,c,d] (syslin list)
//                 2       n
//i.e. c=[b, ab, ab,...; ab ]  
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
select type(a)
 case 1  then
    if rhs==1 then error('2 arguments : a,b'),end
    [m,n]=size(a)
    if m<>n then error(20,1),end
    [mb,nb]=size(b);if mb<>n then error(60),end

 case 16 then
    if a(1)<>'lss' then error(91,1),end
    [a,b]=a([2,3])
    [n,n]=size(a)  
 else error('a,b pair or syslin list')
end;
c=b;for k=1:n-1, c=[b,a*c],end
endfunction
