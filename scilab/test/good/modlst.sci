function [lst]=modlst(lst)
//
//!
//count=count+1
// Copyright INRIA
nlst=size(lst);top=0
ilst=0
pos=[]
while ilst<nlst
  ilst=ilst+1
  if type(lst(ilst))==15 then
    lst(ilst)=modlst(lst(ilst))
  else
    op=lst(ilst)
    opn=op(1)
    if opn=='5' then
       if op(2)=='23' then
 
         i2=pos(top);i1=pos(top-1)
         a1=lst(i1)
         a2=lst(i2)
         if a1(1:2)==['5','23'] then
             // [ [a,b,...],x ]
             lst(i1)='0';
             op(3)=addf(a1(3),'1');lst(ilst)=op
             top=top-1
             pos(top)=ilst
         else
             top=top-1
             pos(top)=ilst
         end
       elseif op(2)=='27' then
 
         i2=pos(top);i1=pos(top-1)
         a1=lst(i1)
         a2=lst(i2)
         if a1(1:2)==['5','27'] then
             // [ [a;b;...];x ]
             lst(i1)='0';
             op(3)=addf(a1(3),'1');lst(ilst)=op
             top=top-1
             pos(top)=ilst
         elseif a1(1:2)==['5','23']&a2(1:2)==['5','23'] then
             lst(i1)='0'
             lst(i2)='0'
             op=['5','33','2',a1(3)];lst(ilst)=op;
         elseif a1(1:2)==['5','28']&a2(1:2)==['5','23'] then
             lst(i1)='0';
             lst(i2)='0'
             op(3)=addf(a1(3),'1');lst(ilst)=op;
             top=top-1
             pos(top)=ilst
         else
             top=top-1
             pos(top)=ilst
         end
       else
         rhs=abs(evstr(op(3)));lhs=evstr(op(4))
         pos((top-rhs+1):(top-rhs+lhs))=ones(lhs,1)*ilst
         top=top-rhs+lhs
 
       end
    elseif opn=='20' then
       rhs=abs(evstr(op(3)));lhs=evstr(op(4))
       pos((top-rhs+1):(top-rhs+lhs))=ones(lhs,1)*ilst
       top=top-rhs+lhs
    elseif opn=='2'|opn=='3'|opn=='4'|opn=='6'|opn=='23' then
       top=top+1
       pos(top)=ilst
//    else
    end
  end
end
 
lst1=list();k1=0;
for ll=lst;
  if type(ll)==15 then
     k1=k1+1;lst1(k1)=ll
  elseif ll(1)<>'0' then
     k1=k1+1;lst1(k1)=ll,
  end
end
lst=lst1;
endfunction
