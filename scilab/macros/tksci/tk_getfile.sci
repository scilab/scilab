function p=tk_getfile(file_mask,path,title,foo)
  if ~with_tk() then error('Tcl/Tk interface not defined'),end
  arg=''
  if exists('file_mask','local')==1 then 
    TK_SetVar("ftypes",'{""Requested Extensions"" {'+file_mask+'} }')
    arg=arg+' -filetypes $ftypes'
  end
  if exists('path','local')==1 then 
   	if MSDOS then
    		path=pathconvert(path,%f,%t,'w')
    		path=strsubst(path,"\","/")
    		if strindex(path,' ')<>[] then
    			path='""'+path+'""'
    			arg=arg+' -initialdir '+path
    		else
    			arg=arg+' -initialdir  ""'+path +'""'
    		end
    	else
    		path=pathconvert(path,%f,%t)
    		if strindex(path,' ')<>[] then path='""'+path+'""',end
    		arg=arg+' -initialdir  ""'+path +'""'
    	end
  else
      	if MSDOS then
    		global %tk_getfile_defaultpath
    		if exists('%tk_getfile_defaultpath','global') == 1 then
    			strsubst(%tk_getfile_defaultpath,'\','/')
    			arg=arg+' -initialdir  ""'+%tk_getfile_defaultpath +'""'
    		end
    	end
  end
  if exists('title','local')==1 then 
    arg=arg+' -title ""'+title+'""',
  end
  TK_EvalStr('set scifilepath [tk_getOpenFile'+arg+']')
  p=TK_GetVar('scifilepath')
  if MSDOS then
  	if ~( p=='' ) then
  		global %tk_getfile_defaultpath;
   		%tk_getfile_defaultpath=dirname(p);
   	end
  end
endfunction
