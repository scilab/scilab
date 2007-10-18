function [tf]=typeof(object)
// Copyright INRIA
select type(object)
case 1 then tf=gettext('constant');
case 2 then tf=gettext('polynomial');
case 4 then tf=gettext('boolean');
case 5 then tf=gettext('sparse');
case 6 then tf=gettext('boolean sparse');
case 7 then tf=gettext('Matlab sparse');
case 8 then 
  it=inttype(object)
  tf=''
  if int(it/10)==1 then
    tf='u'
    it=modulo(it,10)
  end
  tf=tf+'int'+string(8*it)
case 9 then tf=gettext('handle');
case 10 then tf=gettext('string');
case 11 then tf=gettext('function');
case 13 then tf=gettext('function');
case 14 then tf=gettext('library');
case 128 then tf=gettext('pointer');
case 129 then tf=gettext('size implicit');
case 15 then tf=gettext('list');
case 16 then
  o1=object(1);
  select o1(1)
  case 'r' then 
    tf=gettext('rational');
  case 'lss' then 
    tf=gettext('state-space');
  else
    tf=o1(1)
  end
case 17 then
   o1=getfield(1,object)
   select o1(1)
   case 'hm' then
     tf=gettext('hypermat');
   else
     tf=o1(1)
   end
 else
   [tp,nm]=typename()
   k=find(tp==type(object))
   if k<>[] then
     tf=nm(k(1))
   else
     tf=gettext('unknown');
   end
 end
endfunction
