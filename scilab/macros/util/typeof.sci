function [tf]=typeof(object)
// Copyright INRIA
select type(object)
case 1 then tf='constant';
case 2 then tf='polynomial';
case 4 then tf='boolean';
case 5 then tf='sparse';
case 6 then tf='boolean sparse';
case 7 then tf='Matlab sparse';
case 8 then 
  it=inttype(object)
  tf=''
  if int(it/10)==1 then
    tf='u'
    it=modulo(it,10)
  end
  tf=tf+'int'+string(8*it)
case 9 then tf='handle';
case 10 then tf='string';
case 11 then tf='function';
case 13 then tf='function';
case 14 then tf='library';
case 128 then tf='pointer';
case 129 then tf='size implicit';  
case 15 then tf='list'
case 16 then
  o1=object(1);
  select o1(1)
  case 'r' then 
    tf='rational';
  case 'lss' then 
    tf='state-space'
  else
    tf=o1(1)
  end
case 17 then
   o1=getfield(1,object)
   select o1(1)
   case 'hm' then
     tf='hypermat'
   else
     tf=o1(1)
   end
 else
   [tp,nm]=typename()
   k=find(tp==type(object))
   if k<>[] then
     tf=nm(k(1))
   else
     tf='unknown'
   end
 end
endfunction
