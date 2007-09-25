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
        part(mac(k-1),1:lm)+gettext('messages',' called at line ')+string(linn(k))+gettext('messages',' of exec_file.')]
  elseif mac(k)=='execstr' then
    txt=[txt;
        part(mac(k-1),1:lm)+gettext('messages',' called at line ')+string(linn(k))+gettext('messages',' of execstr instruction.')]
  elseif mac(k)=='pause' then
     txt=[txt;
        part(mac(k-1),1:lm)+' called under pause']
  else
    txt=[txt;
        part(mac(k-1),1:lm)+gettext('messages',' called at line ')+string(linn(k))+gettext('messages',' of macro ')+mac(k)]
  end
end
write(%io(2),txt)
endfunction
