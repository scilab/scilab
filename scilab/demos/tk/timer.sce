TK_EvalFile(tkpath+'timer')
while %t //wait for toplevel to disapear
  TK_EvalStr('set h [winfo exists .vscale]');
  if TK_GetVar("h")=='0' then break,end
end
