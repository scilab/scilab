function p=tk_getfile(file_mask,dir,title,foo)
  if ~with_tk() then error('Tcl/Tk interface not defined'),end
  arg=''
  if exists('file_mask','local')==1 then 
    TK_SetVar("ftypes",'{""Requested Extensions"" {'+file_mask+'} }')
    arg=arg+' -filetypes $ftypes',
  end
  if exists('dir','local')==1 then arg=arg+' -initialdir '+dir,end
    if exists('title','local')==1 then arg=arg+' -title ""'+title+'""',end
  TK_EvalStr('set scifilepath [tk_getOpenFile'+arg+']')
  p=TK_GetVar('scifilepath')
endfunction
