function palettes=drawpal(palettes,win_height)
// drawpal - dessine une palette de blocks
//%Syntaxe
// datas=drawpal(palettes)
//%Parametres
// palettes   : liste dont le premier champ est le nom de la palette et les champs
//          suivant la description des blocks
// win    : numero de la fenetre
//!
//origine S Steer INRIA 1993
// Copyright INRIA
[lhs,rhs]=argn(0)

//compute blocks position
nm=size(palettes)
if rhs<2 then 
  if nm<15 then
    win_height=330,
  else
    win_height=400,
  end
end
text_height=20
lm=20
hm=20
dym=hm/6
dxm=lm/3
xm=lm
ym=hm
hmx=0
lmargin=hm
ym=lmargin


for km=1:nm-1
  o=palettes(km+1);
  geom=o(2);sz=geom(2)
  
  if o(1)=='Text' then
    [r1,r2]=xgetech()
    r2=r2(3:4)-r2(1:2)
    oldfont=xget('font');  xset('font',o(3)(9)(1),o(3)(9)(2))
    r=xstringl(0,0,o(3)(8))
    xset('font',oldfont(1),oldfont(2))
    sz=(r(3:4)./r2)*win_height
    if ym+sz(2)*(1+2/7)>win_height-text_height then
      ym=lmargin
      xm=xm+hmx+dxm
      hmx=0
    end
    geom(2)=sz
    geom(1)=[xm+r(1),ym+r(2)]
    hmx=maxi(hmx,sz(1)*(1+2/7))
    ym=ym+sz(2)*(1+2/7)+dym
  elseif o(1)=='Block' then
    if ym+hm*sz(2)*(1+2/7)>win_height-text_height then
      ym=lmargin
      xm=xm+hmx+dxm
      hmx=0
    end
    geom(1)=[xm,ym];geom(2)=[lm,hm].*[sz(1) sz(2)];
    hmx=maxi(hmx,lm*sz(1)*(1+2/7))
    ym=ym+hm*sz(2)*(1+2/7)+dym
  else
    message('Unexpected object type skipped')
  end
  o(2)=geom;
  palettes(km+1)=o
  ym=ym+hm*sz(2)*(1+2/7)+dym
end
wd=[maxi(400,xm+hmx+hmx/2),maxi(300,win_height)]
curwin=xget('window')
xset('default')
unsetmenu(curwin,'File',1) //clear
unsetmenu(curwin,'File',7) //close
unsetmenu(curwin,'File',6) //load
unsetmenu(curwin,'3D Rot.')
xset('wdim',wd(1),wd(2))
xsetech([-1 -1 8 8]/6,[0 0 wd(1) wd(2)])
for km=1:nm-1
  drawobj(palettes(km+1));
end
wpar=palettes(1)
rect=xstringl(0,wd(1),wpar(2)(1))
w=rect(3);h=rect(4)
x=(wd(1)-rect(3))/2
y=wd(2)-h


xstring(x,y,wpar(2)(1))
endfunction
