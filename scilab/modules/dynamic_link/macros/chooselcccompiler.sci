//-----------------------------------------------------------------------------
// Allan CORNET
// INRIA 2007
// force to use LCC-Win32 compiler
//-----------------------------------------------------------------------------
function res=chooselcccompiler()
  res=findlcccompiler();
  if (res) then
    global LCC;
    LCC=res;
    clear LCC;
  end
  configure_lcc();
endfunction
//-----------------------------------------------------------------------------