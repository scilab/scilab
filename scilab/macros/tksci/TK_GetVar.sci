// Allan CORNET
// INRIA 2005
function ret=TK_GetVar(Var)
  printf('\nWarning TK_GetVar obsolete please use TCL_GetVar.\n');
  ret=TCL_GetVar(Var)
endfunction