RGB1=[0 0 0]
TCL_SetVar('rgb',tkpath+'rgb.txt');
TCL_EvalFile(tkpath+'tcolor')
while %t //wait for toplevel to disapear
  if or(RGB<>RGB1) then disp(RGB);RGB1=RGB;end
  TCL_EvalStr('set h [winfo exists .foo]');
  if TCL_GetVar("h")=='0' then break,end
end
