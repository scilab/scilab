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

function [reg,rect] = get_region(xc,yc,win)


//** alu = xget('alufunction')
wins = curwin ;

// xset('window',win)
scf(win);

//** xset('alufunction',6)

reg = list(); rect=[]

kc = find(win==windows(:,2))

if kc==[] then
  messagebox('This window is not an active palette','modal')
  //** xset('window',wins)
  scf(wins); 
  //** xset('alufunction',alu)
  return
  
elseif windows(kc,1)<0 then // click inside a palette window 
  kpal  = -windows(kc,1)
  scs_m = palettes(kpal)

elseif win==curwin then //click dans la fenetre courante
  scs_m = scs_m

elseif pal_mode&win==lastwin then 
  scs_m=scs_m_s

elseif slevel>1 then
  execstr('scs_m=scs_m_'+string(windows(kc,1)))

else
  messagebox('This window is not an active palette','modal')
  //** xset('window',wins)
  scf(wins); 
  //** xset('alufunction',alu)
  return
end

drawnow()
[ox,oy,w,h,ok] = get_rectangle(xc,yc) ; //** use the brand new function with the 
                                        //** "rubberbox" macro

if ~ok then 
  //** xset('window',wins)
  scf(wins); 
  //** xset('alufunction',alu)
  return;
end

[keep,del] = get_blocks_in_rect(scs_m,ox,oy,w,h) ; //** OK

[reg,DEL,DELL] = do_delete1(scs_m,del,%f) ; //** OK 

reg = do_purge(reg) ; //** 

rect = [ox,oy-h,w,h] ; //** 

//** xset('window',wins)
scf(wins); 

//** xset('alufunction',alu)

endfunction
