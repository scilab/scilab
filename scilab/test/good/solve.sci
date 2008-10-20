function x=solve(a,b)
// x=solve(A,b) solves A*x = b when A is an upper triagular matrix
// made of character strings.
//!
//origin F Delebecque S. Steer INRIA 1987
//
// Copyright INRIA
[na,ma]=size(a),[mb,nb]=size(b)
pivot=a(na,na);
if pivot<>'1' then
  for k=1:nb,
    x(1,k)=ldivf(pivot,b(na,k))
  end
else
  x=b(na,:)
end
// update
if na>1 then
   for l=1:na-1,
   pivot=mulf('-1',a(l,na))
      for k=1:nb,
        b(l,k)=addf(b(l,k),mulf(pivot,x(k)))
      end
   end
   y=solve(a(1:na-1,1:na-1),b(1:na-1,:))
   x=[y;x]
end
endfunction
