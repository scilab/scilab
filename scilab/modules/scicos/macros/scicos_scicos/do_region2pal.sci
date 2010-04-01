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
function [%pt,scs_m] = do_region2pal(%pt,scs_m)
// Copyright INRIA
  win = %win;
  xc = %pt(1); yc = %pt(2);
  %pt=[] ;
  scs_m_save = scs_m
  nc_save    = needcompile;
  //** block select function
  [scs_mb, rect, prt] = get_region22(xc,yc,win);
  if rect==[] then //** if no rectangle
    return
  end
  if lstsize(scs_mb.objs)==0 then //** if no object selected
    return
  end
  //superblock should not inherit the context nor the name
  scs_mb.props.context=' ' 
  scs_mb.props.title(1)='Palette'
  [scs_mb,edited,ok] = do_rename(scs_mb,%t)
  if ~ok then scs_m=scs_m_save;%pt=[];return;end
  ox=rect(1); oy=rect(2)+rect(4); w=rect(3), h=rect(4)
  n=0
  W=max(600,rect(3))
  H=max(400,rect(4))
  sup = PAL_f('define')
  sup.graphics.orig   = [rect(1)+rect(3)/2-20, rect(2)+rect(4)/2-20]
  sup.graphics.sz     = [40 40]
  sup.model.rpar      = scs_mb
  sup.graphics.id     = scs_mb.props.title(1)
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
  drawlater();
  [scs_m,DEL] = do_delete2(scs_m,del,%t) ; //** VERY dangerous here !
  // add super block
  drawobj(sup)
  scs_m.objs($+1) = sup
  [scs_m_save,nc_save,enable_undo,edited,needcompile,..
   needreplay] = resume(scs_m_save,nc_save,%t,%t,4,needreplay)
endfunction

function [reg, rect, prt] = get_region22(xc, yc, win)
  wins = curwin ; //** save the semi-global current window variable
  scf(win); //** set the current wind. from calling arg.
  reg = list(); rect = []
  kc = find (win==windows(:,2) )
  if kc==[] then
    messagebox('This window is not an active palette','modal') ;
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
    messagebox('This window is not an active palette','modal')
    return ;
  end
  drawnow(); //** this calling look unuseful 
  [rect,button] = rubberbox([xc; yc; 0; 0], %t) ;
  if or(button == [2 5 12 -100]) then // right button exit OR active window has been closed
    prt  = [] ;
    rect = [] ;
    return ; //** ---> Exit point
  end
  ox=rect(1), oy=rect(2), w=rect(3), h=rect(4);
  clear rect ; //** just for safety because the variable is reused later
  [keep, del] = get_blocks_in_rect(scs_m,ox,oy,w,h); //** see file "get_blocks_in_rect.sci"
  prt = splitted_links(scs_m,keep,del) //** the 'Text' does not have 'Links' ;)
  [reg,DEL] = do_delete2(scs_m,del,%f) //** potentialy dangerous.....
  rect = [ox,oy-h,w,h] ;
  scf(wins);
  reg = do_purge(reg)
endfunction 
 