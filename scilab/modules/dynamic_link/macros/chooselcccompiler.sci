//==========================================
// INRIA 2008
// Allan CORNET
//==========================================
// force to use LCC-Win32 compiler
//==========================================
function res = chooselcccompiler()

  res = findlcccompiler();
  if (res) then
    global LCC;
    LCC=res;
    clear LCC;
    configure_lcc();
  end
  
endfunction
//==========================================
