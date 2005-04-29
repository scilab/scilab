function plotprofile(fun) 
  
  sep='='
  sep=part(sep,ones(1,72))

  if type(fun)==13 then
    lst=macr2lst(fun)
    count=profile(lst);count(:,3)=count(:,3).*count(:,1);
    txt=fun2string(lst,'fun')
    m=min(size(count,1),size(txt,1))
    txt=txt(1:m);count=count(1:m,:)
    txt(txt=='')=' '
    bnd=[];nf=0;
  elseif type(fun)==10 then
    nf=size(fun,'*')
    txt=[];bnd=[];count=[];
    for k=1:nf
      execstr('lst=macr2lst('+fun(k)+')')
      c=profile(lst);c(:,3)=c(:,3).*c(:,1);
      t=fun2string(lst,fun(k))
      m=min(size(c,1),size(t,1))
      t=t(1:m);c=c(1:m,:)
      t(t=='')=' '
      if txt<>[] then
	txt=[txt;'/////////////////////////////////////////////']
	bnd=[bnd,size(txt,1)]
	count=[count;[0 0 0]]
      end
      txt=[txt;t]
      count=[count;c]
    end
  end
  n=size(count,1)


  //xset('window',win)
  step=ceil(n/15)
  nn=ceil((n+1)/step)
  nm=nn*step
  nc=size(count,1)

  win=sum(winsid())+1
  scf(win)

  subplot(311)
  mx=max(count(:,1))
  plot2d3(1:size(count,1),count(:,1),style=1,rect=[0,0,nm,mx])
  legend('# calls',1)
  xsegs(ones(2,1)*bnd,[0;mx]*ones(bnd),5*ones(bnd))
  xp=[0 bnd];yp=mx*1.05;for k=1:nf,xstring(xp(k),yp,fun(k));end  

  
  subplot(312)
  mx=max(1,max(count(:,3)))
  plot2d3(1:size(count,1),count(:,3),style=2,rect=[0,0,nm,mx])
  legend('Complexity',1)
    
  xsegs(ones(2,1)*bnd,[0;mx]*ones(bnd),5*ones(bnd))

  subplot(313)
  mx=max(count(:,2))
  plot2d3(1:size(count,1),count(:,2),style=3,rect=[0,0,nm,mx])
  legend('Cpu Time',1)
  xsegs(ones(2,1)*bnd,[0;mx]*ones(bnd),5*ones(bnd))

  if ~MSDOS then
    delmenu(win,'3D Rot.')
  else
    delmenu(win,'3D &Rot.')
  end   
  addmenu(win,'Exit');str='execstr(Exit_'+string(win)+'(1))'
  xinfo('click to get corresponding line')
  ok=%t
  
  mputl(txt,TMPDIR+'/profiled.sci')
  withpad=with_scipad()
  profpath=TMPDIR+'/profiled.sci'
  if withpad then
    openinscipad(profpath)
  else //ouput text in a graphic window
    [h,M]=dispfuntxt(txt,1,0,%f)
  end

while ok
    [c_i,c_x,c_y,cw,cm]=xclick()
    if (c_i <0&cm==str)|c_i==-100 then break,end
    if cw==win&or(c_i==(0:5)) then
      k=min(n,max(1,round(c_x)))
      // show source code in another window
      xinfo('line : '+string(k))
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
    TCL_EvalStr("set isscipadinterp [interp exists scipad]")
    if TCL_GetVar("isscipadinterp")=='0' then return,end
    TCL_EvalStr('scipad eval {montretext $profiled}')
    TCL_EvalStr('scipad eval {closecur}')
  end
endfunction

function scipad_hiliteline(n)
  TCL_EvalStr("set isscipadinterp [interp exists scipad]")
  if TCL_GetVar("isscipadinterp")=='0' then //scipad has been closed
     openinscipad(profpath)
  end
  TCL_EvalStr('scipad eval {montretext $profiled}')
  TCL_EvalStr('scipad eval {set gotlnCommand '+string(n)+'}')
  TCL_EvalStr('scipad eval {[gettextareacur] mark set insert ""$gotlnCommand.0""}')
  //TCL_EvalStr('scipad eval {catch {keyposn [gettextareacur]}}')
  TCL_EvalStr('scipad eval {[gettextareacur] see insert}')
  TCL_EvalStr('scipad eval {set i1 [$textareacur index ""insert linestart""]}')
  TCL_EvalStr('scipad eval {set i2 [$textareacur index ""insert lineend""]}')
  TCL_EvalStr('scipad eval {$textareacur tag add sel $i1 $i2}')
  TCL_EvalStr('scipad eval {selectline}')

endfunction

function r=with_scipad()
  if with_tk() then
    TCL_EvalStr("set isscipadinterp [interp exists scipad]")
    if TCL_GetVar("isscipadinterp")=='0' then scipad(),end
    r=%t
  else
    r=%f
  end
endfunction

function openinscipad(path)
  TCL_EvalStr("set isscipadinterp [interp exists scipad]")
  if TCL_GetVar("isscipadinterp")=='0' then scipad(),end
  TCL_EvalStr('scipad eval {openfile ""'+path+'""}')
  TCL_EvalStr('scipad eval {set profiled [lindex $listoftextarea end]}')
endfunction

function [h,M]=dispfuntxt(txt,k,h,M)
  //function used to display code in a graphic window
  lbl=string(1:size(txt,1))';lbl=part(lbl,1:max(length(lbl)));
  t=lbl+': '+txt;
  if ~or(winsid()==(win+1)) then
    
    xset('window',win+1);
    set figure_style old;
    xset('wpdim',400,600);
    xset('wdim',400,600);xset('wresize',0);
    curwin=win+1
    if ~MSDOS then
      delmenu(curwin,'3D Rot.')
      delmenu(curwin,'UnZoom')
      delmenu(curwin,'Zoom')
      delmenu(curwin,'Edit')
      delmenu(curwin,'File')
    else
        hidetoolbar(curwin)
 	// French
  	delmenu(curwin,'&Fichier')
  	delmenu(curwin,'&Editer')
  	delmenu(curwin,'&Outils')
  	// English
  	delmenu(curwin,'&File')
  	delmenu(curwin,'&Edit')
  	delmenu(curwin,'&Tools')
  	end     
    
    xsetech(wrect=[0 0 1 1],frect=[0 0 400 600],arect=[0.1 0 0 0])
    w=xstringl(0,0,t);h=w(4);w=max(400,w(3))
    if h>600 then
      xset('wdim',w,h)
      xsetech(wrect=[0 0 1 1],frect=[0 0 w h],arect=[0.1 0 0 0])
      M=%t
    elseif h<300 then
      xset('wpdim',400,300)
      xset('wdim',w,h)
      xsetech(wrect=[0 0 1 1],frect=[0 0 w h],arect=[0.1 0 0 0])
      M=%t
    else
      M=%f
    end
  else
    xset('window',win+1);
  end
  xclear();
  x=0;y=h;
  if k>1 then
    t1=t(1:k-1);
    w=xstringl(0,0,t1);w=w(4);
    y=y-w;xstring(x,y,t1);
  end
  xset('dashes',5)
  w=xstringl(0,0,t(k));w=w(4);
  y=y-w;xstring(x,y,t(k));
  yp=y;
  xset('dashes',33)


  if k<n then
    t1=t(k+1:$);
    w=xstringl(0,0,t1);w=w(4);
    y=y-w;xstring(x,y,t1)
  end
  if M then xset('viewport',x,-300+(k*(h/size(txt,1)))),end
  xset('window',win)
  xpause(10000)
endfunction
