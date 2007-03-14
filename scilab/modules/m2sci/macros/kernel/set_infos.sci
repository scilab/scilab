function []=set_infos(t,level)
// Copyright INRIA

// Add warning and error messages obtained while translating to display it in the end of file translation
global("m2sci_infos")

t(1)="L."+string(nblines)+": "+t(1)

select level
case 0 then
case 1 then
  m2sci_infos(level)=%t
  t="! "+t
case 2 then
  m2sci_infos(level)=%t
  t="!! "+t
end
m2sci_info(t,verbose_mode);
endfunction
