// Allan CORNET
// INRIA 2005
function ret=TK_SetVar(Var,Val)
  printf(gettext('messages','Warning TK_SetVar obsolete please use TCL_SetVar.'));
  ret=TCL_SetVar(Var,Val)
endfunction