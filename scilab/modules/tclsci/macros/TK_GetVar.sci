// Allan CORNET
// INRIA 2005
function ret=TK_GetVar(Var)
  printf(gettext('Warning TK_GetVar obsolete please use TCL_GetVar.'));
  ret=TCL_GetVar(Var)
endfunction