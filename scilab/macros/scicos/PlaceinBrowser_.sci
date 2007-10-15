function PlaceinBrowser_()
// Copyright INRIA
  Cmenu=[]
  %superpath='root,'+strcat(string(super_path),',')
  %superpath1='root,'+strcat(string(super_path(1)),',')
  TCL_EvalStr('.scsTree.t selection clear')  
  TCL_EvalStr('.scsTree.t opentree '+%superpath1)
  TCL_EvalStr('.scsTree.t selection add '+%superpath)
  TCL_EvalStr('.scsTree.t see '+%superpath)
endfunction
