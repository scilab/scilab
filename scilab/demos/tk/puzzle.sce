TCL_EvalFile(tkpath+'puzzle')
while %t //wait for toplevel to disapear
  order=TCL_GetVar('order');
  TCL_EvalStr('set h [winfo exists .puzzle]');
  if TCL_GetVar("h")=='0' then break,end
end
disp(order)
