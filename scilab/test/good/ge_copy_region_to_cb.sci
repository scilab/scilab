function ge_copy_region_to_cb(GraphList,xc,yc)
//Copyright INRIA
//Author : Serge Steer 2002

  ge_disablemenus()
  [reg,rect]=ge_get_region(xc,yc,win)
  if rect==[] then ge_enablemenus();return,end
  global GClipBoard
  GClipBoard=list(reg,rect)
  ge_enablemenus()
endfunction
  
