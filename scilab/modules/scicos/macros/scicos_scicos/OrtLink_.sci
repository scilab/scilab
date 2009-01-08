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

function OrtLink_()
//** 28 Nov 2008 : First attempt to implement a menu option switch 
//**               for SL mode editor 

  Cmenu = [] ;

  //** TODO : we will use the "scs_m.props.options" later
  //** [edited,options] = do_options(scs_m.props.options,'OrtLink')
  //** scs_m.props.options = options ;

  global SL_mode ; //** "0" standard scicos oblique link ; "1" SL orthogonanal links 

  if SL_mode==1 then 
    tt = "Yes"
  else
    tt = "No"
  end

  rep1 = message(["Use orthogonal links?" ; "current choice is " + tt], ["Yes" ; "No"])

  if rep1==1 then
     SL_mode = 1;
  else
     SL_mode = 0;
  end 

endfunction
