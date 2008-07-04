//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [%pt,scs_m] = do_region2block(%pt,scs_m)

//** Very complex and critical functions inside : handle with care ;) 
//**
//** 11 Jan 2007 : 'Block' / 'Text' bug validation: this function is OK.
//** Alan, 15/10/7 : patch for rotated blocks
//**  

  win = %win;
  xc = %pt(1); yc = %pt(2);
  %pt = [] ;
  
  scs_m_save = scs_m
  nc_save    = needcompile;
  
  //** block select function 
  [scs_mb, rect, prt] = get_region2(xc,yc,win) ; //** see file "get_region2.sci"

  if rect==[] then //** if no rectangle 
    return
  end
  
  if lstsize(scs_mb.objs)==0 then //** if no object selected 
    return
  end
  
  //superblock should not inherit the context nor the name
  scs_mb.props.context=' ' 
  scs_mb.props.title(1)='Untitled'
  ox=rect(1); oy=rect(2)+rect(4); w=rect(3), h=rect(4)
  
  n=0
  W=max(600,rect(3))
  H=max(400,rect(4))
  
  sup = SUPER_f('define')
  sup.graphics.orig   = [rect(1)+rect(3)/2-20, rect(2)+rect(4)/2-20]
  sup.graphics.sz     = [40 40]
  
  sup.model.in        = 1
  sup.model.out       = 1
  sup.model.rpar      = scs_mb
  sup.model.blocktype = 'h'
  sup.model.dep_ut    = [%f %f]
  
  // open the superblock in editor
  [ok,sup] = adjust_s_ports(sup) //** looks OK because works on specific 'Block'
  
  //** prepare the object that we need to destroy 
  del=[]
  
  for k=1:lstsize(scs_m.objs)
    o = scs_m.objs(k)
    if typeof(o)=='Block'| typeof(o)=='Text' then //** OK
      // check if block is outside rectangle
      orig = o.graphics.orig
      sz = o.graphics.sz
      x  = [0 1 1 0]*sz(1)+orig(1)
      y  = [0 0 1 1]*sz(2)+orig(2)
      ok = %f
      
      for kk=1:4
	data=[(ox-x(kk))'*(ox+w-x(kk)),(oy-h-y(kk))'*(oy-y(kk))];
	if data(1)<0 & data(2)<0 then
	  ok = %t;
	  del= [del k];
	  break;
	end
      end //** for()
    end //** of if() 
  end ;//** of for()
  
  needreplay = replayifnecessary() ;

  //** from here you are in drawlater mode () ; 
  drawlater(); 

  [scs_m,DEL] = do_delete2(scs_m,del,%t) ; //** VERY dangerous here !

  // add super block
  drawobj(sup)
  
  scs_m.objs($+1) = sup
  
  // connect it
  nn = lstsize(scs_m.objs)  //superblock number
  nnk = nn
  
  for k=1:size(prt,1)
    k1=prt(k,6);tp1=prt(k,8);
    ksup=prt(k,1);tpsup=prt(k,3)
    typ=prt(k,5)
    o1=scs_m.objs(k1) // block origin of the link outside the superblock
    
    if typ>0 then //regular link
      if tp1==0 then  //link connected to an output port of o1
	
	if typ>1 then //implicit regular link
	  [x,y,vtyp] = getoutputports(o1)
	else //explicit regular link
	  [x,y,vtyp] = getoutputs(o1)
	end

        if x<>[] & y<>[] then
          xxx=rotate([x;y],...
                     o1.graphics.theta*%pi/180,...
                     [o1.graphics.orig(1)+o1.graphics.sz(1)/2;...
                      o1.graphics.orig(2)+o1.graphics.sz(2)/2]);
          x=xxx(1,:);
          y=xxx(2,:);
        end
	
	if tpsup==1 then //link connected to an input port of the superblock 
	  
	  if typ>1 then //implicit regular link
	    [xn,yn,vtypn] = getinputports(sup)
	  else	    //explicit regular link
	    [xn,yn,vtypn]=getinputs(sup),
	  end
	  
	  Psup='pin'
	  
	else //link connected to an output port of the superblock 
	  
	  if typ>1 then //implicit regular link
	    [xn,yn,vtypn]=getoutputports(sup)
	  else //explicit regular link
	    [xn,yn,vtypn]=getoutputs(sup),
	  end
	  
	  Psup='pout'
	  
	end
	
	p = prt(k,7)
	pn=prt(k,2)
	xl=[x(p);xn(pn)]
	yl=[y(p);yn(pn)]
	
	from=prt(k,6:8)
	to=[nn,prt(k,2:3)]
	o1.graphics.pout(prt(k,7))=nnk+1
	scs_m.objs(nn).graphics(Psup)(prt(k,2))=nnk+1
	
      else //link connected to an input port of o1
	
	if typ>1 then //implicit regular link
	  [x,y,vtyp]=getinputports(o1)
	else
	  [x,y,vtyp]=getinputs(o1)
	end

        if x<>[] & y<>[] then
          xxx=rotate([x;y],...
                     o1.graphics.theta*%pi/180,...
                     [o1.graphics.orig(1)+o1.graphics.sz(1)/2;...
                      o1.graphics.orig(2)+o1.graphics.sz(2)/2]);
          x=xxx(1,:);
          y=xxx(2,:);
        end
	
	if tpsup==1 then //link connected to an input port of the superblock 
	  if typ>1 then //implicit regular link
	    [xn,yn,vtypn]=getinputports(sup)
	  else //explicit regular link
	    [xn,yn,vtypn]=getinputs(sup),
	  end
	  Psup='pin'
	else  //link connected to an output port of the superblock 
	  if typ>1 then //implicit regular link
	    [xn,yn,vtypn]=getoutputports(sup)
	  else //explicit regular link
	    [xn,yn,vtypn]=getoutputs(sup),
	  end
	  Psup='pout'
	end
	
	p=prt(k,7)
	pn=prt(k,2)
	
	xl=[xn(pn);x(p)]
	yl=[yn(pn);y(p)]
	from=[nn,prt(k,2:3)]
	to=prt(k,6:8)
	o1.graphics.pin(prt(k,7))=nnk+1
	scs_m.objs(nn).graphics(Psup)(prt(k,2))=nnk+1
      end
      
    else //event link
      if tpsup==1 then //link connected to an event input port of the superblock 
	[x,y,vtyp]=getoutputs(o1)

        if x<>[] & y<>[] then
          xxx=rotate([x;y],...
                     o1.graphics.theta*%pi/180,...
                     [o1.graphics.orig(1)+o1.graphics.sz(1)/2;...
                      o1.graphics.orig(2)+o1.graphics.sz(2)/2]);
          x=xxx(1,:);
          y=xxx(2,:);
        end

	[xn,yn,vtypn]=getinputs(sup),
	p=prt(k,7)+size(find(vtyp==1),'*')+size(find(vtyp==2),'*')
	pn=prt(k,2)+size(find(vtypn==1),'*')+size(find(vtypn==2),'*')
	xl=[x(p);xn(pn)]
	yl=[y(p);yn(pn)]
	from=prt(k,6:8)
	to=[nn,prt(k,2:3)]
	o1.graphics.peout(prt(k,7))=nnk+1
	scs_m.objs(nn).graphics.pein(prt(k,2))=nnk+1
      else //link connected to an event output port of the superblock 
	[x,y,vtyp]=getinputs(o1)

        if x<>[] & y<>[] then
          xxx=rotate([x;y],...
                     o1.graphics.theta*%pi/180,...
                     [o1.graphics.orig(1)+o1.graphics.sz(1)/2;...
                      o1.graphics.orig(2)+o1.graphics.sz(2)/2]);
          x=xxx(1,:);
          y=xxx(2,:);
        end

	[xn,yn,vtypn]=getoutputs(sup),
	p=prt(k,7)+size(find(vtyp==1),'*')+size(find(vtyp==2),'*')
	pn=prt(k,2)+size(find(vtypn==1),'*')+size(find(vtypn==2),'*')
	xl=[xn(pn);x(p)]
	yl=[yn(pn);y(p)]
	from=[nn,prt(k,2:3)]
	to=prt(k,6:8)
	o1.graphics.pein(prt(k,7))=nnk+1
	scs_m.objs(nn).graphics.peout(prt(k,2))=nnk+1
      end
    end
    
    if xl(1)<>xl(2)&yl(1)<>yl(2) then //oblique link
      if prt(k,4)>0 then //regular port
	xl=[xl(1);xl(1)+(xl(2)-xl(1))/2;xl(1)+(xl(2)-xl(1))/2;xl(2)]
	yl=[yl(1);yl(1);yl(2);yl(2)]
      else
	xl=[xl(1);xl(1);xl(2);xl(2)]
	yl=[yl(1);yl(1)+(yl(2)-yl(1))/2;yl(1)+(yl(2)-yl(1))/2;yl(2)]
      end
    end
    lk = scicos_link(xx=xl,yy=yl,ct=prt(k,4:5),from=from,to=to)
    drawobj(lk)
    scs_m.objs($+1)=lk
    scs_m.objs(k1)=o1
    nnk=nnk+1
  end
  
  drawnow(); //** at the end of the operation the diagram is updated 

  [scs_m_save,nc_save,enable_undo,edited,needcompile,needreplay] = resume(scs_m_save,nc_save,%t,%t,4,needreplay)

endfunction
