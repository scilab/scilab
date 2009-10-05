//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function xcos_simulate(scs_m)
   //** extract tolerances from scs_m.props.tol
  tolerances = scs_m.props.tol ;
  //** extract solver type from tolerances
  solver = tolerances(6) ; 

  //Add environment variable so that scicos is not lost
  // {
  %state0 = list();
  needcompile = 4;
  curwin = 1000;
  %cpr = list();
  %tcur = 0;
  %cpr.state = %state0;
  tf=scs_m.props.tf;
// }
//** update parameters or compilation results
  [%cpr,%state0_n,needcompile,alreadyran,ok] = do_update(%cpr, %state0,needcompile);
  [state,t]=scicosim(%cpr.state,%tcur,tf,%cpr.sim,"start", tolerances);
  
endfunction
