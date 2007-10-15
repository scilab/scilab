function ExportAll_()
// Copyright INRIA
Cmenu=[]
%exp_dir=tk_getdirectory()
if %exp_dir<>[] then 
  systexport(scs_m,%exp_dir),
  if MSDOS then
	fname=%exp_dir+'\'+'navigator'
  else
	fname=%exp_dir+'/'+'navigator'
  end

if 0 then
  driv=driver();
  driver('Pos');
  set_posfig_dim(400,600)
  xinit(fname);
else
  win_nag=max(winsid())+1
  scf(win_nag)
end

  gh_axes=gca();
  
  
  gh_navig = gcf()
  
  y0 = 0    ;
  x0 = 0    ;
  path = [] ;
  larg = 0  ;
  [xx, yy, lp] = build_scs_tree(scs_m); 

  
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
      xrect(x0(kx),bnds(2,2),x0(kx+1)-x0(kx),bnds(2,2)-bnds(1,2))
    end
  
  end
  drawnow()


if 0 then
  xend();
  set_posfig_dim(0,0)
  driver(driv);
  fname=pathconvert(fname,%f,%t,'w')
else 
  if MSDOS then
    fname=pathconvert(fname,%f,%t,'w')
  end
  xs2ps(win_nag,fname)
  xdel(win_nag)
end

  opt=' ';//' -landscape '
  if MSDOS then
      comm=pathconvert(SCI+'\bin\BEpsf',%f,%f,'w')
      rep=unix_g(comm+' '+opt+'""'+fname+'""')
  else
      rep=unix_g(SCI+'/bin/BEpsf '+opt+fname)
  end
    
  if rep<>[] then 
      message(['Problem generating ps file.';'perhaps directory not writable'] )
  end
end
endfunction
