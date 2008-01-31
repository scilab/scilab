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

function AboutScicos_()


  Cmenu=[] ; %pt=[];

  if MSDOS & evstr(TCL_EvalStr('file exists $env(COMSPEC)'))     then
    num=message([strsubst(get_scicos_version(),"scicos","Scicos -")+"-";...
             "For more information visit:";...
	     "     www.scicos.org      "],['Open URL','Cancel']);

    if num==1 then
      TCL_EvalStr('exec $env(COMSPEC) /c start http://www.scicos.org &')
    end
    
  else
    message([strsubst(get_scicos_version(),"scicos","Scicos -")+"-";...
             "For more information visit:";...
	     "     www.scicos.org      "]);
  end

endfunction
