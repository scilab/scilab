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

function XcosMenuRun()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

    nc_save = 4  ; 
    Cmenu   = [] ;
    ok = %t      ;
    %mm = getfield(1, %scicos_context) ;
    
    //** Context evaluation 
    for %mi=%mm(3:$)
      ierr = execstr(%mi+'=%scicos_context(%mi)','errcatch')
      if ierr<>0 then
	break
      end
    end

    //** Real Simulation is here 
    [ok,%tcur,%cpr,alreadyran,needcompile,%state0, %scicos_solver] = do_run(%cpr) ;

    scs_m.props.tol(6) = %scicos_solver ;

    if ok then
      newparameters = list()
    end

endfunction
