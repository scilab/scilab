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

function Eval_()
    Cmenu='Replot'

    %now_win=xget('window')

    [%scicos_context,ierr]=script2var(scs_m.props.context,%scicos_context)
    //for backward compatibility for scifunc
    if ierr==0 then
      %mm=getfield(1,%scicos_context)
      for %mi=%mm(3:$)
	ierr=execstr(%mi+'=%scicos_context(%mi)','errcatch')
	if ierr<>0 then
	  break
	end
      end
    end
    //end of for backward compatibility for scifunc
    
    xset('window',%now_win)
    if ierr==0 then 
      do_terminate()
      [scs_m,%cpr,needcompile,ok]=do_eval(scs_m,%cpr)
      if needcompile<>4&size(%cpr)>0 then %state0=%cpr.state,end
      alreadyran=%f
    else
      message(['Incorrect context definition, ';lasterror()] )
    end

endfunction
