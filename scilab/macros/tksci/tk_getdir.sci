function p=tk_getdir(dir,title)
  if ~with_tk() then error('Tcl/Tk interface not defined'),end
  arg=''
  if exists('dir','local')==1 then 
    arg=arg+' -initialdir '+dir,
  end
  if exists('title','local')==1 then arg=arg+' -title ""'+title+'""',end
  arg=arg+' -parent $root '
  TK_EvalStr("set scifilepath [tk_chooseDirectory"+arg+"]")
  p=TK_GetVar('scifilepath')
endfunction
