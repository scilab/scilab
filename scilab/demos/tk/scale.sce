getf(tkpath+'demredraw.sci')

xbasc();set figure_style new;plot3d();done=%t
TCL_EvalFile(tkpath+'vscale')
while %t //wait for toplevel to disapear
  TCL_EvalStr('set h [winfo exists .vscale]');
  if TCL_GetVar("h")=='0' then break,end
  sleep(1)
end
