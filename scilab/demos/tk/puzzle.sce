TK_EvalFile(tkpath+'puzzle')
while %t //wait for toplevel to disapear
  order=TK_GetVar('order');
  TK_EvalStr('set h [winfo exists .puzzle]');
  if TK_GetVar("h")=='0' then break,end
end
disp(order)
