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

function wdm=do_focus(scs_m)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

[btn,%pt,win,Cmenu]=cosclick()
if Cmenu<>[] then
  Cmenu=resume(Cmenu)
end
xc=%pt(1);yc=%pt(2);
wpar=scs_m.props;wdm=wpar.wpar
if size(wdm,'*')<6 then wdm(3)=0;wdm(4)=0;wdm(5)=wdm(1);wdm(6)=wdm(2);end
Xshift=wdm(3)
Yshift=wdm(4)
[ox,oy,w,h,ok]=get_rectangle(xc,yc)
if ~ok then return;end

Xshift=xc
Yshift=yc-h
wdm(5)=w;wdm(6)=h



wdm(3)=Xshift;wdm(4)=Yshift;
endfunction
