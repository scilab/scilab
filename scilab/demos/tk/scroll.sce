TK_EvalFile(tkpath+'cscroll')
while %t //wait for toplevel to disapear
  TK_EvalStr('set h [winfo exists .cscroll]');
  if TK_GetVar("h")=='0' then break,end
end
