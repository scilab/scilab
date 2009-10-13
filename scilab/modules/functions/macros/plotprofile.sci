// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
    error(msprintf(gettext("%s: Wrong values for input argument: No profile data in the function\n"),"plotprofile"))
  end
  
  //xset("window",win)
  step=ceil(n/15)
  nn=ceil((n+1)/step)
  nm=nn*step
  nlines=size(count,1)

  win=sum(winsid())+1
  scf(win)

  drawlater
  axes=[];marker=[];
  subplot(311)
  ncalls=count(:,1); mx=max(ncalls)
  plot2d([],[],rect=[0.5,0,nm+0.5,max(mx*1.02,1)]); axes=gca()
  xrects([(1:nlines)'-0.5,ncalls,ones(ncalls),ncalls]',2*ones(ncalls))
  xrects([(1:nlines)'-0.5,ncalls,ones(ncalls),ncalls]',0*ones(ncalls))
  xarrows([1,1],[1.5*mx,0],5,5); marker=gce()
  xtitle("# calls")
  //separator, if more that 1 function is given
  xsegs(ones(2,1)*bnd,[0;1.5*mx]*ones(bnd),0*ones(bnd))
  xp=[0 bnd]+1;yp=mx*0.9;
  for k=1:nf, xstring(xp(k),yp," "+fun(k)+" ",0,1); end  

  
  subplot(312)
  ncompl=count(:,3); mx=max(1,max(ncompl))
  plot2d([],[],rect=[0.5,0,nm+0.5,max(mx*1.02,1)]); axes(2)=gca()
  xrects([(1:nlines)'-0.5,ncompl,ones(ncompl),ncompl]',3*ones(ncompl))
  xrects([(1:nlines)'-0.5,ncompl,ones(ncompl),ncompl]',0*ones(ncompl))
  xarrows([1,1],[1.5*mx,0],5,5); marker(2)=gce()
  xtitle(gettext("Complexity"))
  xsegs(ones(2,1)*bnd,[0;1.5*mx]*ones(bnd),0*ones(bnd))

  subplot(313)
  tcpu=count(:,2); mx=max(tcpu)
  plot2d([],[],rect=[0.5,0,nm+0.5,max(mx*1.02,1e-6)]); axes(3)=gca()
  xtitle("Cpu time (total "+string(sum(count(:,2)))+" sec)","line")
  xrects([(1:nlines)'-0.5,tcpu,ones(tcpu),tcpu]',4*ones(tcpu))
  xrects([(1:nlines)'-0.5,tcpu,ones(tcpu),tcpu]',0*ones(tcpu))
  xarrows([1,1],[1.5*mx,0],5,5); marker(3)=gce()
  xsegs(ones(2,1)*bnd,[0;1.5*mx]*ones(bnd),0*ones(bnd))
  drawnow
  
  for i=1:3; axes(i).tight_limits="on"; end

  delmenu(win,gettext("Tools"));
  delmenu(win,gettext("Edit"));
  delmenu(win,gettext("?"));
  
  addmenu(win,gettext("Exit"),list(0,"Exit"));
  str = "execstr(Exit_"+string(win)+"(1))"
  
  xinfo(gettext("Click to get corresponding line, move with a-z."))

  profpath = TMPDIR+"/profiled.sci"
  mputl(txt,profpath);
  editor_highlightline(profpath, 1);
  
  k=1
  while %t
    if ~or(winsid()==win) then break,end
    [c_i,c_x,c_y,cw,cm]=xclick()
    if (c_i <0 & cm==str) | c_i==-100 | ~or(winsid()==win) then break,end
//if one of the panels has been zoomed, zoom (horizontally)all of them
// (partially, this is a workaround for bug #1618)
// [the update is carried on only after the next xclick -- why?]
    Xb=[axes(1).zoom_box([1,3])', axes(2).zoom_box([1,3])',..
        axes(3).zoom_box([1,3])']
    drawlater
//treat here also if all the panes were zoomed but key navigation
// brought the cursor out of window
    if ~and(Xb(1,:)==Xb(1,1)) | ~and(Xb(2,:)==Xb(2,1)) | size(Xb,2)<>3 |..
       ~and(k<Xb(1,:)) | ~and(k>Xb(2,:)) then
       if Xb<>[] then
          newXmin=min(max(Xb(1,:)),k-1); newXmax=max(min(Xb(2,:)),k+1)
          for i=1:3
          // vertical zoom is restored to default
            yrange=axes(i).data_bounds(:,2)
            axes(i).zoom_box=[newXmin,yrange(1),newXmax,yrange(2)]; 
          end
       else
          for i=1:3 axes(i).zoom_box=[]; end
       end
    end
    if cw==win then 
      if or(c_i==(0:5)) then
        k=min(n,max(1,round(c_x)))
      end
      if or(c_i==[65 97 65361]) then
        k=max(1,k-1)
      end      
      if or(c_i==[90 122 65363]) then
        k=min(n,k+1)
      end
      kstr = string(k);
      if isempty(kstr) then
	kstr = "";
      end
      ncallsstr = string(ncalls(k));
      if isempty(ncallsstr) then
	ncallsstr = "";
      end
      tcpustr = string(tcpu(k));
      if isempty(tcpustr) then
	tcpustr = "";
      end
      if ncalls(k)==1 then
        msg=msprintf(gettext("line %s [%s call, %s sec] :: "), kstr, ncallsstr, tcpustr) + txt(k);
      else
        msg=msprintf(gettext("line %s [%s calls, %s sec] :: "), kstr, ncallsstr, tcpustr) + txt(k);
      end
      
      editor_highlightline(profpath,k)
    end
    for i=1:3
      d=marker(i).data; 
      marker(i).data=[k,d(1,2);k,d(2,2)]
    end
    drawnow
    xinfo(msg)
  end
  endprof()

endfunction

function endprof()
  if or(winsid()==(win+1)) then xdel(win+1);end
  if or(winsid()==(win)) then xdel(win);end
endfunction

