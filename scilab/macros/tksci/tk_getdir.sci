function p=tk_getdir(startdir,title)
  if ~with_tk() then error('Tcl/Tk interface not defined'),end
  arg=''
  if exists('startdir','local')==1 then 
    startdir=pathconvert(startdir,%f,%t)
    if strindex(startdir,' ')<>[] then startdir='""'+startdir+'""',end
    arg=arg+' -initialdir '+startdir,
  end
  if exists('title','local')==1 then arg=arg+' -title ""'+title+'""',end
  arg=arg+' -parent $root '
  TCL_EvalStr("set scifilepath [tk_chooseDirectory"+arg+"]")
  p=TCL_GetVar('scifilepath')
endfunction
