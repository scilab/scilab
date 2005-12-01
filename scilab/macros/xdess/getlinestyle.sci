function k=getlinestyle()
[lhs,rhs]=argn(0)
win=max(winsid()+1)
xset('window',win);

if ~MSDOS then
  delmenu(win,'3D Rot.')
  delmenu(win,'UnZoom')
  delmenu(win,'Zoom')
  delmenu(win,"Edit")
  delmenu(win,'File')
  delmenu(win,'Insert')
else
  hidetoolbar(win)
 // French
  delmenu(win,'&Fichier')
  delmenu(win,'&Editer')
  delmenu(win,'&Outils')
  delmenu(win,'&Inserer')
  // English
  delmenu(win,'&File')
  delmenu(win,'&Edit')
  delmenu(win,'&Tools')
  delmenu(win,'&Insert')
end
//Event handler and menu definition
deff('evh(gwin,x,y,ibut)',..
     ['global pos done'
      'if or(ibut==(0:5)) then '
      '   [x,y]=xchange(x,y,''i2f'')'
      '   pos=[x,y],done=0,'
      'end'
      'if ibut==-1000 then ,done=3,end'
     ])
deff('menu_ok(k,gwin)','global done;done=1')
deff('menu_cancel(k,gwin)','global done;done=2')


set('figure_style','new')
f=gcf();
cur_draw_mode = f.immediate_drawing;
f.immediate_drawing='off'
a=gca();
a.axes_visible='off';
a.data_bounds=[0 0;5 12];
a.font_size=3;

x=(1:5)';
xtitle("      Select line style k by clicking in rectangle or line")
R=[]
for k=1:6
  xstringb(0,2*k-1,"k = "+string(k),1,2);s=gce();
  s.font_size=3;
  xrect(0, 2*k+1, 1, 2);R=[R,gce()]
end
a.thickness=2
for k=1:6
  xset("line style",k);
  xpoly(x,2*k*ones(x));p=gce();
  p.mark_mode='off',p.line_style=k;
end
f.immediate_drawing=cur_draw_mode;

global pos done;done=-1;
ksel=1;
seteventhandler('evh')
addmenu(win,'Ok',list(2,'menu_ok'));
addmenu(win,'Cancel',list(2,'menu_cancel'));

f.pixmap='on';

k1=[]; //default

while %t
  select done
  case 0 then //click somehere
    cx=pos(1);cy=pos(2)
    k1=round(cy/2);k1=min(k1,6);k1=max(1,k1);
    R(ksel).thickness=1;
    R(k1).thickness=3;
    ksel=k1
    show_pixmap()
  case 1 then  // ok button clicked
    k=k1;break,
  case 2 then   // cancel button clicked
    k=[];break,
  case 3 then //the window has been blosed
    k=[],
    clearglobal pos done
    return
  end
  xpause(100)
end
seteventhandler('')
xdel(win)
clearglobal pos done
endfunction
