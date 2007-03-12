function [tf]=typeof(object)
// Copyright INRIA
select type(object)
case 1 then tf=gettext('messages','core_message_164');
case 2 then tf=gettext('messages','core_message_165');
case 4 then tf=gettext('messages','core_message_166');
case 5 then tf=gettext('messages','core_message_167');
case 6 then tf=gettext('messages','core_message_168');
case 7 then tf=gettext('messages','core_message_169');
case 8 then 
  it=inttype(object)
  tf=''
  if int(it/10)==1 then
    tf='u'
    it=modulo(it,10)
  end
  tf=tf+'int'+string(8*it)
case 9 then tf=gettext('messages','core_message_170');
case 10 then tf=gettext('messages','core_message_171');
case 11 then tf=gettext('messages','core_message_172');
case 13 then tf=gettext('messages','core_message_172');
case 14 then tf=gettext('messages','core_message_173');
case 128 then tf=gettext('messages','core_message_174');
case 129 then tf=gettext('messages','core_message_175');
case 15 then tf=gettext('messages','core_message_176');
case 16 then
  o1=object(1);
  select o1(1)
  case 'r' then 
    tf=gettext('messages','core_message_177');
  case 'lss' then 
    tf=gettext('messages','core_message_178');
  else
    tf=o1(1)
  end
case 17 then
   o1=getfield(1,object)
   select o1(1)
   case 'hm' then
     tf=gettext('messages','core_message_179');
   else
     tf=o1(1)
   end
 else
   [tp,nm]=typename()
   k=find(tp==type(object))
   if k<>[] then
     tf=nm(k(1))
   else
     tf=gettext('messages','core_message_180');
   end
 end
endfunction
