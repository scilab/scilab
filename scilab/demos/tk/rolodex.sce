TCL_EvalFile(tkpath+'rolodex')
while %t //wait for toplevel to disapear
  TCL_EvalStr('set h [winfo exists .top]');
  if TCL_GetVar("h")=='0' then break,end
end

