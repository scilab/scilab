function ok=ge_do_save(GraphList)   
//Copyright INRIA
//Author : Serge Steer 2002

// saves data structures GraphList 
//!
// Copyright INRIA

  if size(GraphList.name,'*')<2 then 
    path='./'
  else
    path=GraphList.name(2)
  end
  //open file

  fname=path+GraphList.name+'.graph'
  if fileinfo(fname)<>[] then
    if MSDOS then
      unix_s('del '+fname)
    else
      unix_s('rm '+fname)
    end
  end
  ok=execstr('save_graph(GraphList,fname)','errcatch')==0

endfunction
