RGB1=[0 0 0]
TK_SetVar('rgb',tkpath+'rgb.txt');
TK_EvalFile(tkpath+'tcolor')
while %t //wait for toplevel to disapear
  if or(RGB<>RGB1) then disp(RGB);RGB1=RGB;end
  TK_EvalStr('set h [winfo exists .foo]');
  if TK_GetVar("h")=='0' then break,end
end
