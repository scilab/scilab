TK_EvalFile(tkpath+'rolodex')
while %t //wait for toplevel to disapear
  TK_EvalStr('set h [winfo exists .top]');
  if TK_GetVar("h")=='0' then break,end
end

