//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function scicos_demonstration(scs_m)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


  //** -------------------- Check the recurring calling level of scicos ----------------------
  [%ljunk, %mac] = where() ; //** where I am ?
  slevel = prod ( size ( find ( %mac=='scicos') ) ) ; //** "slevel" is the superblock level
  Scicos_is_active = slevel > 0 ; //** ... means that SCICOS is already active 

   if Scicos_is_active then

      messagebox(["You can run ONLY one Scicos demo at time.";..
               "Please close any previous Scicos diagram";..
               "before choose a new demo. "],"modal");
      return
   else
      //** with no Scicos previous active, you can start Scicos 
      scicos(scs_m); 
   end

endfunction
