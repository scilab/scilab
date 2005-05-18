function manedit(manitem,editor)
// manitem : character string giving a manitem 
//
// Copyright INRIA
  path=gethelpfile(manitem)

  if path<>[] then 
    px=strsubst(path,'.htm','.xml')
    if fileinfo(px)<>[] then 
      path=px
    else
      ierr=execstr('t=type('+manitem+')','errcatch')
      if ierr==0&t==13 then
	path=TMPDIR+'/'+manitem+'.xml'
	help_skeleton(manitem,TMPDIR)
      else
	path=[]
      end
    end
  end
  if path<>[] then scipad(path),end
endfunction
