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
	msprintf(gettext("%s called at line %s of exec_file"),part(mac(k-1),1:lm), string(linn(k)))]
  elseif mac(k)=='execstr' then
    txt=[txt;
        	msprintf(gettext("%s called at line %s of execstr instruction."),part(mac(k-1),1:lm), string(linn(k)))]
  elseif mac(k)=='pause' then
     txt=[txt;
        msprintf(gettext("%s called under pause"),part(mac(k-1),1:lm)) ]
  else
    txt=[txt;
        msprintf(gettext("%s called at line %s of macro %s"), part(mac(k-1),1:lm), string(linn(k)),mac(k))]
  end
end
write(%io(2),txt)
endfunction
