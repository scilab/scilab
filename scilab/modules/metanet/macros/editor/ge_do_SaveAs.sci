function [GraphList,ok,fname]=ge_do_SaveAs(GraphList,fname)
//Copyright INRIA
//Author : Serge Steer 2002

//
// Copyright INRIA
  tit='Select a file path'
  fname=stripblanks(fname)
  if fname=='' then
    path=fname
  else
    [path,name,ext]=splitfilepath(fname)
  end
  if stripblanks(path)=='' then path='./',end
  fname=tk_savefile('*.graph',path,tit)
  
  fname=stripblanks(fname)
  if fname==emptystr() then ok=%f,return,end

  [path,name,ext]=splitfilepath(fname)
  
//following test already done by tk_savefile
//   if fileinfo(fname)<>[] then
//     r=x_message(['File '+fname+ ' already exists'
// 		 'erase it'],['Yes','No'])
//     if r==2 then ok=%f,return,end
//   end
  ok=execstr('save(fname,GraphList)','errcatch')==0
  if ok then 
    GraphList.name=name
  else
    x_message([lasterror();'';'Graph has not been saved'])
  end    
endfunction
