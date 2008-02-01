function ok=ge_do_save(GraphList,path)   
//Copyright INRIA
//Author : Serge Steer 2002

// saves data structures GraphList 
//!
// Copyright INRIA

  ok=%t
  fname=path
  save(fname,GraphList)
endfunction
