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

function Copy_()

  if size(Select,1)>1 then
    //** ... more than a single object
    [pt1,win1,scs_m_sel] = get_selection2(Select,%pt,%win)
    if size(scs_m_sel.objs)==1 then
       Clipboard = scs_m_sel.objs(1)
    else
    	Clipboard = scs_m_sel
    end
    Cmenu=[];
    // BUG Fix : Direct Paste after Select.
    Select=[];
  elseif size(Select,1)==1
    //** single object
    [pt1,win1,o] = get_selection(Select,%pt,%win)
    Clipboard = o ;
    Cmenu = [];
    // BUG Fix : Direct Paste after Select.
    Select=[];
  else
    //** no object
    message(['No block is selected';
	     'click on a block or select a region first.'])
    Cmenu=[]; %pt=[]; %ppt=[]
  end
endfunction

//**-----------------------------------------------------------------------

function [%pt,%win,reg] = get_selection2(Select,%pt,%win)
 win=Select(1,2)  // all items in the second column are identical
  kc=find(win==windows(:,2))
  if kc==[] then
    reg=[];return // window no longer active
  elseif windows(kc,1)<0 then //palette
    scs_m=palettes(-windows(kc,1))
  elseif win==curwin then //selected object in current window
    // scs_m is fine
  elseif slevel>1 then
    execstr('scs_m=scs_m_'+string(windows(kc,1)))
  end

  del=setdiff(1:size(scs_m.objs),Select(:,1))
  [reg,DEL,DELL]=do_delete1(scs_m,del,%f)
  reg=do_purge(reg)

  o=reg.objs(1)

  if typeof(o)=='Block' then
    o=disconnect_ports(o)
    [orig,sz]=(o.graphics.orig,o.graphics.sz)
    %pt=orig(:)+sz(:)/2
  elseif typeof(o)=='Text'  then
    [orig,sz]=(o.graphics.orig,o.graphics.sz)
    %pt=orig(:)+sz(:)/2
  elseif typeof(o)=='Link' then
    %pt=[(o.xx(1)+o.xx(2))/2,(o.yy(1)+o.yy(2))/2] //middle of first
                                                  //segment
  else
    o=[]  // perhaps deleted
  end
  %win=win
endfunction


