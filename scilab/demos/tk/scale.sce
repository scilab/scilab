getf(tkpath+'demredraw.sci')
//ancien mode graphique
SaveGraphicMode=get('old_style')
set old_style on

xbasc();plot3d();done=%t
TK_EvalFile(tkpath+'vscale')
while %t //wait for toplevel to disapear
  TK_EvalStr('set h [winfo exists .vscale]');
  if TK_GetVar("h")=='0' then break,end
end

set('old_style',stripblanks(SaveGraphicMode));