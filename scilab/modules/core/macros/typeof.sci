function [tf]=typeof(object)
// Copyright INRIA
select type(object)
case 1 then tf=gettext('messages','constant');
case 2 then tf=gettext('messages','polynomial');
case 4 then tf=gettext('messages','boolean');
case 5 then tf=gettext('messages','sparse');
case 6 then tf=gettext('messages','boolean sparse');
case 7 then tf=gettext('messages','Matlab sparse');
case 8 then 
  it=inttype(object)
  tf=''
  if int(it/10)==1 then
    tf='u'
    it=modulo(it,10)
  end
  tf=tf+'int'+string(8*it)
case 9 then tf=gettext('messages','handle');
case 10 then tf=gettext('messages','string');
case 11 then tf=gettext('messages','function');
case 13 then tf=gettext('messages','function');
case 14 then tf=gettext('messages','library');
case 128 then tf=gettext('messages','pointer');
case 129 then tf=gettext('messages','size implicit');
case 15 then tf=gettext('messages','list');
case 16 then
  o1=object(1);
  select o1(1)
  case 'r' then 
    tf=gettext('messages','rational');
  case 'lss' then 
    tf=gettext('messages','state-space');
  else
    tf=o1(1)
  end
case 17 then
   o1=getfield(1,object)
   select o1(1)
   case 'hm' then
     tf=gettext('messages','hypermat');
   else
     tf=o1(1)
   end
 else
   [tp,nm]=typename()
   k=find(tp==type(object))
   if k<>[] then
     tf=nm(k(1))
   else
     tf=gettext('messages','unknown');
   end
 end
endfunction
