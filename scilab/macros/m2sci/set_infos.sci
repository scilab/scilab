function set_infos(t,level)
global('m2sci_infos')
select level
case 0 then
  txt=[txt;' ';'// '+t]
case 1 then
  m2sci_infos(level)=%t
  txt=[txt;' ';'//! '+t]
case 2 then
  m2sci_infos(level)=%t
  if logfile>0 then write(logfile,t,'(a)'),end
  txt=[txt;' ';'//!! '+t]
end
txt=resume(txt)
