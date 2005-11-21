function plotprofile(fun) 
  
  sep="="
  sep=part(sep,ones(1,72))

  if type(fun)==13 then
    lst=macr2lst(fun)
    count=profile(lst);count(:,3)=count(:,3).*count(:,1);
    txt=fun2string(lst,"fun")
    m=min(size(count,1),size(txt,1))
    txt=txt(1:m);count=count(1:m,:)
    txt(txt=="")=" "
    bnd=[];nf=0;
  elseif type(fun)==10 then
    nf=size(fun,"*")
    txt=[];bnd=[];count=[];
    for k=1:nf
      execstr("lst=macr2lst("+fun(k)+")")
      c=profile(lst);c(:,3)=c(:,3).*c(:,1);
      t=fun2string(lst,fun(k))
      m=min(size(c,1),size(t,1))
      t=t(1:m);c=c(1:m,:)
      t(t=="")=" "
      if txt<>[] then
        txt=[txt;"/////////////////////////////////////////////"]
        bnd=[bnd,size(txt,1)]
        count=[count;[0 0 0]]
      end
      txt=[txt;t]
      count=[count;c]
    end
  end
  n=size(count,1)
  if n==0 then 
    error("Perhaps the function(s) has not been loaded for profiling?")
  end
  
  //xset("window",win)
  step=ceil(n/15)
  nn=ceil((n+1)/step)
  nm=nn*step
  nlines=size(count,1)

  win=sum(winsid())+1
  scf(win)

  drawlater
  subplot(311)
  ncalls=count(:,1)
  mx=max(ncalls)
  plot2d3(1:nlines,ncalls,style=2,rect=[0,0,nm,max(mx*1.02,1)])
  xrects([(1:nlines)'-0.5,ncalls,ones(ncalls),ncalls]',2*ones(ncalls))
  xrects([(1:nlines)'-0.5,ncalls,ones(ncalls),ncalls]',0*ones(ncalls))
  legend("# calls",1)
  //separator, if more that 1 function is given
  xsegs(ones(2,1)*bnd,[0;mx]*ones(bnd),5*ones(bnd))
  xp=[0 bnd]+1;yp=mx*0.9;
  for k=1:nf, xstring(xp(k),yp," "+fun(k)+" ",0,1); end  

  
  subplot(312)
  ncompl=count(:,3)
  mx=max(1,max(ncompl))
  plot2d3(1:nlines,ncompl,style=3,rect=[0,0,nm,max(mx*1.02,1)])
  xrects([(1:nlines)'-0.5,ncompl,ones(ncompl),ncompl]',3*ones(ncompl))
  xrects([(1:nlines)'-0.5,ncompl,ones(ncompl),ncompl]',0*ones(ncompl))
  legend("Complexity",1)
  xsegs(ones(2,1)*bnd,[0;mx]*ones(bnd),5*ones(bnd))

  subplot(313)
  tcpu=count(:,2)
  mx=max(tcpu)
  plot2d3(1:nlines,tcpu,style=4,rect=[0,0,nm,max(mx*1.02,1e-6)])
  xtitle("total time "+string(sum(count(:,2)))+" sec")
  xrects([(1:nlines)'-0.5,tcpu,ones(tcpu),tcpu]',4*ones(tcpu))
  xrects([(1:nlines)'-0.5,tcpu,ones(tcpu),tcpu]',0*ones(tcpu))
  legend("Cpu Time",1)
  xsegs(ones(2,1)*bnd,[0;mx]*ones(bnd),5*ones(bnd))
  drawnow

  if ~MSDOS then
    delmenu(win,"3D Rot.")
    delmenu(win,"Edit")
    delmenu(win,"Editer")
  else
    delmenu(win,"3D &Rot.")
    delmenu(win,"&Edit")
    delmenu(win,"&Editer")
  end   
  addmenu(win,"Exit");str="execstr(Exit_"+string(win)+"(1))"
  xinfo("click to get corresponding line")
  ok=%t
  
  mputl(txt,TMPDIR+"/profiled.sci")
  withpad=with_scipad()
  profpath=TMPDIR+"/profiled.sci"
  if withpad then
    openinscipad(profpath)
  else //ouput text in a graphic window
    [h,M]=dispfuntxt(txt,1,0,%f)
  end

  while ok
    [c_i,c_x,c_y,cw,cm]=xclick()
    if (c_i <0 & cm==str) | c_i==-100 | ~or(winsid()==win) then break,end
    if cw==win & or(c_i==(0:5)) then
      k=min(n,max(1,round(c_x)))
      // show source code in another window
      xinfo("line "+string(k)+" :: "+txt(k))
      if withpad then
        scipad_hiliteline(k)
      else
        [h,M]=dispfuntxt(txt,k,h,M)
      end
    end
  end
  endprof()

endfunction

function endprof()
  if or(winsid()==(win+1)) then xdel(win+1);end
  if or(winsid()==(win)) then xdel(win);end
  if withpad then
    if ~TCL_ExistInterp("scipad") then return,end
    if ~TCL_ExistVar("pad","scipad")then return,end
    TCL_EvalStr("scipad eval {montretext $profiled}")
    TCL_EvalStr("scipad eval {closecur}")
  end
endfunction

function scipad_hiliteline(n)
  if ~TCL_ExistInterp("scipad") then //scipad has never been opened
     openinscipad(profpath)
  else
    if ~TCL_ExistVar("pad","scipad") then //scipad has been closed
      openinscipad(profpath)
    end
  end
  TCL_EvalStr("scipad eval {montretext $profiled}")
  TCL_EvalStr("scipad eval {set gotlnCommand "+string(n)+"}")
  TCL_EvalStr("scipad eval {[gettextareacur] mark set insert ""$gotlnCommand.0""}")
  //TCL_EvalStr("scipad eval {catch {keyposn [gettextareacur]}}")
  TCL_EvalStr("scipad eval {[gettextareacur] see insert}")
  TCL_EvalStr("scipad eval {set i1 [$textareacur index ""insert linestart""]}")
  TCL_EvalStr("scipad eval {set i2 [$textareacur index ""insert lineend""]}")
  TCL_EvalStr("scipad eval {$textareacur tag add sel $i1 $i2}")
  TCL_EvalStr("scipad eval {selectline}")

endfunction

function r=with_scipad()
  if with_tk() then
    if ~TCL_ExistInterp("scipad") then scipad(),end
    r=%t
  else
    r=%f
  end
endfunction

function openinscipad(path)
  if TCL_ExistInterp("scipad") then
     if ~TCL_ExistVar("pad","scipad") then scipad(),end
  else
    scipad()
  end
  TCL_EvalStr("scipad eval {openfile """+path+"""}")
  TCL_EvalStr("scipad eval {set profiled [lindex $listoftextarea end]}")
endfunction

function [h,M]=dispfuntxt(txt,k,h,M)
  //function used to display code in a graphic window
  lbl=string(1:size(txt,1))';lbl=part(lbl,1:max(length(lbl)));
  t=lbl+": "+txt;
  if ~or(winsid()==(win+1)) then
    
    xset("window",win+1);
    set figure_style old;
    xset("wpdim",400,600);
    xset("wdim",400,600);xset("wresize",0);
    curwin=win+1
    if ~MSDOS then
      delmenu(curwin,"3D Rot.")
      delmenu(curwin,"UnZoom")
      delmenu(curwin,"Zoom")
      delmenu(curwin,"Edit")
      delmenu(curwin,"File")
    else
      hidetoolbar(curwin)
 	// French
      	delmenu(curwin,"&Fichier")
      	delmenu(curwin,"&Editer")
      	delmenu(curwin,"&Outils")
      	// English
      	delmenu(curwin,"&File")
      	delmenu(curwin,"&Edit")
      	delmenu(curwin,"&Tools")
  	 end     
    
    xsetech(wrect=[0 0 1 1],frect=[0 0 400 600],arect=[0.1 0 0 0])
    w=xstringl(0,0,t);h=w(4);w=max(400,w(3))
    if h>600 then
      xset("wdim",w,h)
      xsetech(wrect=[0 0 1 1],frect=[0 0 w h],arect=[0.1 0 0 0])
      M=%t
    elseif h<300 then
      xset("wpdim",400,300)
      xset("wdim",w,h)
      xsetech(wrect=[0 0 1 1],frect=[0 0 w h],arect=[0.1 0 0 0])
      M=%t
    else
      M=%f
    end
  else
    xset("window",win+1);
  end
  xclear();
  x=0;y=h;
  if k>1 then
    t1=t(1:k-1);
    w=xstringl(0,0,t1);w=w(4);
    y=y-w;xstring(x,y,t1);
  end
  xset("dashes",5)
  w=xstringl(0,0,t(k));w=w(4);
  y=y-w;xstring(x,y,t(k));
  yp=y;
  xset("dashes",33)

  if k<n then
    t1=t(k+1:$);
    w=xstringl(0,0,t1);w=w(4);
    y=y-w;xstring(x,y,t1)
  end
  if M then xset("viewport",x,-300+(k*(h/size(txt,1)))),end
  xset("window",win)
  xpause(10000)
endfunction
