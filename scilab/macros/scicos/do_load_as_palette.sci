function [palettes,windows]=do_load_as_palette(palettes,windows)
// Copyright INRIA
[ok,scs_m,cpr,edited]=do_load()
if ~ok then return,end


maxpal=-mini([-200;windows(:,1)]) 
kpal=maxpal+1

lastwin=curwin

curwin=get_new_window(windows)
if or(curwin==winsid()) then
  
  xdel(curwin);
end
windows=[windows;[-kpal curwin]]
palettes(kpal)=scs_m
//
xset('window',curwin),xselect();
xset('alufunction',3)
set_background()
if pixmap then xset('pixmap',1),end,xbasc();
rect=dig_bound(scs_m);
  if rect<>[] then
    %wsiz=[rect(3)-rect(1),rect(4)-rect(2)];
  else
    %wsiz=[400,600]
  end

  xset('wpdim',%zoom*%wsiz(1),%zoom*%wsiz(2))
xset('wresize',0)
window_set_size(1)


options=default_options()
drawobjs(palettes(kpal))
if pixmap then xset('wshow'),end
xinfo('Palette: may be used to copy  blocks or regions')
xset('window',lastwin)

