function whereami()
// whereami - display calling tree 
//!
// Copyright INRIA
[linn,mac]=where()
nn=prod(size(linn))
lm=maxi(length(mac))
txt=[]    
for k=2:nn
  if mac(k)=='exec' then
    txt=[txt;
        part(mac(k-1),1:lm)+gettext('messages','core_message_160')+string(linn(k))+gettext('messages','core_message_161')]
  elseif mac(k)=='execstr' then
    txt=[txt;
        part(mac(k-1),1:lm)+gettext('messages','core_message_160')+string(linn(k))+gettext('messages','core_message_162')]
  elseif mac(k)=='pause' then
     txt=[txt;
        part(mac(k-1),1:lm)+' called under pause']
  else
    txt=[txt;
        part(mac(k-1),1:lm)+gettext('messages','core_message_160')+string(linn(k))+gettext('messages','core_message_163')+mac(k)]
  end
end
write(%io(2),txt)
endfunction
