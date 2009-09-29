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

function XcosMenuCopy()

  if size(Select,1)>1 then
    //** ... more than a single object
    [pt1,win1,scs_m_sel] = get_multiple_selection(Select,%pt,%win)
    if size(scs_m_sel.objs)==1 then
       XcosClipboard = scs_m_sel.objs(1)
    else
    	XcosClipboard = scs_m_sel
    end
    Cmenu=[];
    // BUG Fix : Direct Paste after Select.
    Select=[];
  elseif size(Select,1)==1
    //** single object
    [pt1,win1,o] = get_selection(Select,%pt,%win)
    XcosClipboard = o ;
    Cmenu = [];
    // BUG Fix : Direct Paste after Select.
    Select=[];
  else
    //** no object
    messagebox(['No block is selected';'click on a block or select a region first.'],'modal')
    Cmenu=[]; %pt=[]; %ppt=[]
  end
endfunction

//**-----------------------------------------------------------------------

