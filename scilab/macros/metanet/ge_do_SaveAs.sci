function [GraphList,ok]=ge_do_SaveAs(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002

//
// Copyright INRIA
  tit='Select a file path'
  fname=xgetfile('*.graph',emptystr(),tit)
  fname=stripblanks(fname)
  if fname==emptystr() then return,end

  [path,name,ext]=splitfilepath(fname)
  GraphList.node_number=size(GraphList.node_x,'*')
  select ext
    case 'graph' then
    if fileinfo(fname)<>[] then
      r=x_message(['File '+fname+ ' already exists'
		 'erase it'],['Yes','No'])
      if r==2 then ok=%f,return,end
      if MSDOS then
	unix_s('del '+fname)
      else
	unix_s('rm '+fname)
      end
    end
    ok=execstr('save_graph(GraphList,fname)','errcatch')==0
    if ok then 
      nf=length(fname)
      GraphList.name(2)=part(fname,1:nf-6)
    end
    
  else
    message('Only *.graph  files allowed');
    return
  end
endfunction
