function [stk,nwrk,txt,top]=%t2for(nwrk)
// genere le code fortarn relatif a la transposition
//!
// Copyright INRIA
s2=stk(top)
if s2(4)<>'1'&s2(5)<>'1' then
  [out,nwrk,txt]=outname(nwrk,s2(3),s2(4),s2(5))
  txt=[txt;gencall(['mtran',s2(1),s2(4),out,s2(5),s2(4),s2(5)])];
  stk=list(out,'-1',s2(3),s2(5),s2(4))
else
  stk=list(s2(1),s2(2),s2(3),s2(5),s2(4))
end
endfunction
