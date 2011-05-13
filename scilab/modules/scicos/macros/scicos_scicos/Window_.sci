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

function Window_()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

    Cmenu=[]
    disablemenus()

    params=scs_m.props;
    params=do_window(params)
    edited=or(params<>scs_m.props)
    
    if or(scs_m.props.wpar<>params.wpar) then
      xset('alufunction',3);clf();show_window();xset('alufunction',6);
      window_set_size()

      scs_m.props.wpar=params.wpar
      %wdm=scs_m.props.wpar
      %wdm(5:6)=(params.wpar(1:2)./scs_m.props.wpar(1:2)).*%wdm(5:6)
      scs_m.props.wpar(5)=%wdm(5);scs_m.props.wpar(6)=%wdm(6);

      drawobjs(scs_m),
      if pixmap then xset('wshow'),end
    end
    enablemenus()
endfunction
