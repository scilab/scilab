function p=tk_savefile(file_mask,path,title,foo)
  if ~with_tk() then error('Tcl/Tk interface not defined'),end
  arg=''
  if exists('file_mask','local')==1 then 
    TCL_SetVar("ftypes",'{""Requested Extensions"" {'+file_mask+'} }')
    arg=arg+' -filetypes $ftypes',
  end
  if exists('path','local')==1 then arg=arg+' -initialdir ""'+path+'""',end
  if exists('title','local')==1 then arg=arg+' -title ""'+title+'""',end
  if exists('ini','local')==1 then arg=arg+' -initialfile ""'+ini+'""',end
  TCL_EvalStr('set scifilepath [tk_getSaveFile'+arg+']')
  p=TCL_GetVar('scifilepath')
endfunction
