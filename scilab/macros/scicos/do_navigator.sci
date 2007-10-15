function [Tree,windows] = do_navigator(scs_m,windows)
// Copyright INRIA

// build the tree representation
y0 = 0    ;
x0 = 0    ;
path = [] ;
larg = 0  ;
[xx, yy, lp] = build_scs_tree(scs_m); 

//*----------------------------------------------------------------------------
// open tree window
kw = find(windows(:,1)==100000) //** look for an already open navigator window 

if kw==[] then //if no navigator windows 
  windows = [windows;[100000,get_new_window(windows)]] ;
  kw = size(windows,1) ; 
end

//** xset('window',windows(kw,2))
navig_win_id = windows(kw,2) ;

gh_navig = scf(navig_win_id) ; 

//** xbasc();
clf(gh_navig) ;
//**----------------------------------------------------------------------------

n_node = size(lp) ; 
mnx = min(xx)-0.1 ;
mxx = max(xx)+0.1 ;

if n_node<=20 then
  dx  = (mxx-mnx)
  mnx = mnx-dx/5
  x0  = [mnx,mnx+dx/5]
else
  dx  = (mxx-mnx) ;
  mnx = mnx-dx/2  ;
  x0  = [mnx,mnx+dx/4,mnx+dx/2] ;
end

//** axes settings 
gh_navig_axes = gh_navig.children ; //** axes handle
gh_navig_axes.tight_limits = "on"  ; //** set the limit "gh_axes.data_bounds" in "hard mode" 

//** xsetech(wrect=[...], frect=[..], logflag="..", arect=[...]) 
//**
//** The sub-window is specified with the parameter wrect = [x, y, w, h] (upper-left (x,y) point, width, height).
//** The values in 'wrect' are specified using normalized 0-1 proportional values.

//** frect = [xmin,ymin,xmax,ymax] is used to set the graphics scale and is just like the rect argument of plot2d.
//** If frect is not given, the current value of the graphic scale remains unchanged. 
//** The default value of rect is [0,0,1,1] (at window creation, when switching back to default value with
//** xset('default') or when clearing graphic recorded events xbasc()). 

//** xsetech([-1/6 -1/6 8/6 8/6],bnds) ; //** Aaaaaaaaaaa!

//** The default margin are [ 0.125 0.125 0.125 0.125 ]
arect =[ 0.125 0.125 0.125 0.125 ]; //** margins (default normalized values)
gh_navig_axes.margins = arect ;           //**

wrect = [-1/6 -1/6 8/6 8/6] ;
gh_navig_axes.axes_bounds = wrect ; //** default : axes_bounds = [0,0 , 1,1] = [xmin ymin with height] 

//** map the diagram size on the window size
//**    (1,1)  (1,2)        (2,1)    (2,2)
bnds = [mnx,  min(yy)-0.2 ; mxx,  max(yy)+0.2];   
gh_navig_axes.data_bounds = bnds  ; //** default : data_bounds = [0,0 ; 1,1] = [xmin ymin ; xmax ymax ]

// draw tree
xsegs(xx,yy,1) ; //** draw unconnected segments  
  
xx = [xx(1,1);xx(2,:)'] ;
yy = [yy(1,1);yy(2,:)'] ;

plot2d(xx,yy,-9,'000')  ; 

xtitle('Navigator window') ; 

for k=1 : size(xx,1)
  xstring( xx(k), yy(k), string(k) ) ;
end

r  = xstringl(x0(1),y0,'X') ;
h  = r(4)                   ;
y0 = bnds(2,2)-h              ;
kx = 1                      ;

xrect(x0(kx), bnds(2,2), x0(kx+1)-x0(kx), bnds(2,2)-bnds(1,2));

//** xclip([x0(kx),bnds(4),x0(kx+1)-x0(kx),bnds(4)-bnds(2)])

for k=1:size(xx,1)
  
  if k==1 then path=[]; else path=lp(k-1);end
  Path=list();
  
  for pk=path
    Path=lstcat(Path,'objs',pk,'model','rpar');
  end
  
  Path = lstcat(Path,'props','title',1);

  xstring(x0(kx),y0,string(k)+': '+scs_m(Path)) ;
  
  y0=y0-h;
  
  if k==20 then 
    y0 = bnds(2,2)-h;
    kx = kx+1
    //** xclip() 
    xrect(x0(kx),bnds(2,2),x0(kx+1)-x0(kx),bnds(2,2)-bnds(1,2))
    //** xclip([x0(kx),bnds(4),x0(kx+1)-x0(kx),bnds(4)-bnds(2)])
  end

end

//** xclip()

//build data structure
Tree = tlist(['scs_tree','x','y','paths','orig'],xx,yy,lp,super_path)

//** xset('window',curwin) //** put the focus in the default window 
scf(curwin); 

endfunction
