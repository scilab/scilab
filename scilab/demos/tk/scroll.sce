TCL_EvalFile(tkpath+'cscroll')
while %t //wait for toplevel to disapear
  TCL_EvalStr('set h [winfo exists .cscroll]');
  if TCL_GetVar("h")=='0' then break,end
  sleep(1);
end
