//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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

function [reg, rect, prt, is_flip] = get_region2(xc, yc, win)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//
//Creates in reg the superblock formed with the objects selected 
//
//prt is a matrix, each row contains information on the links passing
//throught the superblock rectangle.
//    [io,old_internal_block_#, superblock_port_#, link_type, link_color,..
//       external_block_#, port_#, link_number]
//    io = 1 : regular link from extern to superblock
//    io = 2 : regular link from superblock to extern 
//    io = 3 : event link from extern to superblock
//    io = 4 : event link from superblock to extern 
//    io = 5 : implicit link from extern to superblock
//    io = 6 : implicit link from superblock to extern 
//
//
//
//** 29 Jun 2006 
//** xx/01/07 : Alan - review (introduce rubberbox here)
//
//** 11 Jan 2007 : 'Block' / 'Text' bug validation: this function is OK.
//** 
  ok = %t
  wins = curwin ; //** save the semi-global current window variable 
  scf(win); //** set the current wind. from calling arg.
  is_flip=[]
  //** window filter 
  reg = list(); rect = []
  kc = find (win==windows(:,2) )
  if kc==[] then
    messagebox("This window is not an active palette",'modal') ;
    return ; //** ---> Exit point 
  elseif windows(kc,1)<0 then // click inside a palette window 
    kpal  = -windows(kc,1)  ;
    scs_m = palettes(kpal)  ;
  elseif win==curwin then     //click inside the current window 
    scs_m = scs_m
  elseif pal_mode&win==lastwin then 
    scs_m = scs_m_s
  elseif slevel>1 then
    execstr('scs_m=scs_m_'+string(windows(kc,1))) ;
  else
    messagebox("This window is not an active palette",'modal')
    return ;
  end

  drawnow(); //** this calling look unuseful 
  //** residual old code 
  // [ox, oy, w, h, ok] = get_rectangle(xc,yc) ; //** <-- key function !

  [rect, button] = scicos_rubberbox([xc; yc; 0; 0], %t) ;
  if or(button == [2 5 12 -1000]) then //** right button exit action OR active window has been closed
    prt  = [] ;
    rect = [] ;
    return ; //** ---> Exit point
  end
  ox=rect(1), oy=rect(2), w=rect(3), h=rect(4);
  
  clear rect ; //** just for safety because the variable is reused later  

  [keep, del] = get_blocks_in_rect(scs_m,ox,oy,w,h); //** see file "get_blocks_in_rect.sci"
  //** this function looks OK because filter in both 'Block' and 'Text'
  // keep: objects in the region 
  // del : object outside the region

  //** ... some 'Block' are not allowed ;)
  for bkeep=keep
    if typeof(scs_m.objs(bkeep))=='Block' then
      if or(scs_m.objs(bkeep).gui==['IN_f' 
		    'OUT_f'
		    'CLKINV_f'
		    'CLKIN_f'
		    'CLKOUTV_f'
		    'CLKOUT_f'
                    'INIMPL_f'
		    'OUTIMPL_f']) then
	messagebox('Input/Output ports are not allowed in the region.','modal')
	prt=[];rect=[];return
      end //** some 'Block' are not allowed 
    end //** look for dangerous 'Block'
  end //** ... scanning 

  //preserve information on splitted links
  prt = splitted_links(scs_m,keep,del) //** the 'Text' does not have 'Links' ;)

  [reg,DEL] = do_delete2(scs_m,del,%f) //** potentialy dangerous.....
                                       //** but the gr arg is always false %f
  
  rect = [ox,oy-h,w,h] ;
  scf(wins); 
  
  nin   = 0
  nout  = 0
  ncin  = 0
  ncout = 0
 
  //add input and output ports
  for k=1:size(prt,1)
    nreg=size(reg.objs)
    k1=prt(k,1); typ=prt(k,5);tp=prt(k,3)
    o1=reg.objs(k1) //block inside the region
    orient=o1.graphics.flip
    is_flip=[is_flip,orient]
    
    if tp==1 then //input port
      // build the link between block and port
      if typ>1 then //implicit regular link
	[x,y,vtyp]=getinputports(o1)
	from=[nreg+1,1,0] //added port
	to=prt(k,1:3)
      else	    //explicit regular link
	[x,y,vtyp]=getinputs(o1),
	from=[nreg+1,1,0] //added port
	to=prt(k,1:3)
      end

      if x<>[] & y<>[] then
        xxx=rotate([x;y],...
                   o1.graphics.theta*%pi/180,...
                   [o1.graphics.orig(1)+o1.graphics.sz(1)/2;...
                    o1.graphics.orig(2)+o1.graphics.sz(2)/2]);
        x=xxx(1,:);
        y=xxx(2,:);
      end

      if typ>0 then //input regular port
	x=x(prt(k,2))
	y=y(prt(k,2))
	nin=nin+1
	if typ==1 then sp=IN_f('define'),else sp=INIMPL_f('define'),end
	sz=20*sp.graphics.sz
	sp.graphics.sz=sz; //sz
	sp.graphics.flip=orient; //flip
	sp.graphics.exprs=string(nin); //expr
	sp.graphics.pout=nreg+2;//pout
	sp.model.ipar=nin; //port number

	o1.graphics.pin(prt(k,2))=nreg+2
	if orient then  //not flipped
	  sp.graphics.orig=[x-2*sz(1),y-sz(2)/2]; //orig
	  xl=[x-sz(1);x]
	  yl=[y;y]
	else // flipped
	  sp.graphics.orig=[x+sz(1),y-sz(2)/2]; //orig
	  xl=[x+sz(1);x]
	  yl=[y;y]
	end
	prt(k,2)=nin
      else //input event port
	p=prt(k,2)+size(find(vtyp==1),'*')
	x=x(p)
	y=y(p)
	ncin=ncin+1
	sp=CLKINV_f('define')
	sz=20*sp.graphics.sz
	sp.graphics.orig=[x-sz(1)/2,y+sz(2)]; //orig
	sp.graphics.sz=sz; //sz
	sp.graphics.exprs=string(ncin); //expr
	sp.graphics.peout=nreg+2;//peout
	sp.model.ipar=ncin; //port number
	
	o1.graphics.pein(prt(k,2))=nreg+2
	xl=[x;x]
	yl=[y+sz(2);y]
	prt(k,2)=ncin
      end
    else //  output port
      if typ>1 then //implicit regular link
	[x,y,vtyp]=getoutputports(o1)
	to=[nreg+1,1,1]
	from=prt(k,1:3)
      else //explicit regular link
	[x,y,vtyp]=getoutputs(o1)
	to=[nreg+1,1,1]
	from=prt(k,1:3)
      end
      if x<>[] & y<>[] then
        xxx=rotate([x;y],...
                   o1.graphics.theta*%pi/180,...
                   [o1.graphics.orig(1)+o1.graphics.sz(1)/2;...
                    o1.graphics.orig(2)+o1.graphics.sz(2)/2]);
        x=xxx(1,:);
        y=xxx(2,:);
      end
      if typ>0 then //output regular port
	x=x(prt(k,2))
	y=y(prt(k,2))
	nout=nout+1
	if typ==1 then sp=OUT_f('define'),else sp=OUTIMPL_f('define'),end
	sz=20*sp.graphics.sz
	sp.graphics.sz=sz; //sz
	sp.graphics.flip=orient; //flip
	sp.graphics.exprs=string(nout); //expr
	sp.graphics.pin=nreg+2;//pin
	sp.model.ipar=nout; //port number
	o1.graphics.pout(prt(k,2))=nreg+2
	sz=sp.graphics.sz
	if orient then  //not flipped 
	  sp.graphics.orig=[x+sz(1),y-sz(2)/2]; //orig
	  xl=[x;x+sz(1)]
	  yl=[y;y]
	else //flipped
	  sp.graphics.orig=[x-2*sz(1),y-sz(2)/2]; //orig
	  xl=[x;x-sz(1)]
	  yl=[y;y]
	end
	prt(k,2)=nout
      else //output event port
	p=prt(k,2)+size(find(vtyp==1),'*')
	x=x(p)
	y=y(p)
	ncout=ncout+1
	sp=CLKOUTV_f('define')
	sz=20*sp.graphics.sz
	sp.graphics.orig=[x-sz(1)/2,y-2*sz(2)]; //orig
	sp.graphics.sz=sz; //sz
	sp.graphics.exprs=string(ncout); //expr
	sp.graphics.pein=nreg+2;//pein
	sp.model.ipar=ncout; //port number
	o1.graphics.peout(prt(k,2))=nreg+2
	xl=[x;x]
	yl=[y;y-sz(2)]
	prt(k,2)=ncout
      end
    end
    
    lk = scicos_link(xx=xl,yy=yl,ct=[prt(k,4),typ],from=from,to=to)
    reg.objs(nreg+1)=sp
    reg.objs(nreg+2)=lk
    reg.objs(k1)=o1
  end
  reg = do_purge(reg)

endfunction
