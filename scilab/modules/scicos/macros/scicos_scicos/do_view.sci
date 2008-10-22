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

function wdm=do_view(scs_m)
  wpar=scs_m.props;wdm=wpar.wpar
  if size(wdm,'*')<6 then wdm(3)=0;wdm(4)=0;wdm(5)=wdm(1);wdm(6)=wdm(2);end
  Xshift=wdm(3)
  Yshift=wdm(4)
  oxc=Xshift+(wdm(5))/2
  oyc=Yshift+(wdm(6))/2
  plot2d(oxc,oyc,-1,'000')


  [btn,xc,yc]=xclick(0) //get center of new view
  Xshift=Xshift+(xc-oxc)
  Yshift=Yshift+(yc-oyc)


  wdm(3)=Xshift;wdm(4)=Yshift;
endfunction
