// Allan CORNET
// INRIA 2005
function ret=TK_SetVar(Var,Val)
  printf('\nWarning TK_SetVar obsolete please use TCL_SetVar.\n');
  ret=TCL_SetVar(Var,Val)
endfunction