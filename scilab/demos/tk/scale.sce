getf(tkpath+'demredraw.sci')

xbasc();set figure_style new;plot3d();done=%t
TK_EvalFile(tkpath+'vscale')
while %t //wait for toplevel to disapear
  TK_EvalStr('set h [winfo exists .vscale]');
  if TK_GetVar("h")=='0' then break,end
end
