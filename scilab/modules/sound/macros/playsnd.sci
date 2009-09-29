// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab 
// Copyright (C) ???? - ENPC
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [] = playsnd(y, rate, bits, aplay)
  // play signal y at sample rate rate 
  // bits is unused 
  [lhs,rhs] = argn(0);
  
  // default values
  if (rhs <= 2) then
    bits = 16; 
  end 
  
  if (rhs <= 1) then
    rate = 22050; 
  end 
  
  if rhs > 2 then
  
    if type(aplay) <> 10 then
      error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),'playsnd',3));
    end
    
    if size(aplay,'*') <> 1 then
      error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),'playsnd',3));
    end
   
  else
    if rhs <= 3 then
      aplay = 'aplay'; 
    end
  end

  if MSDOS then
    savewave(TMPDIR+'/_playsnd_.wav', y, rate);
    PlaySound(TMPDIR+'/_playsnd_.wav');      
    return 
  else
    // We should use a external C library here
    if aplay <> '/dev/audio' then
      savewave(TMPDIR+'/_playsnd_.wav', y, rate);
      cmd = msprintf("%s  %s > /dev/null 2>&1", aplay, TMPDIR + '/_playsnd_.wav');
      [res, stat] = unix_g(cmd);
      if (stat <> 0) then
        error(strcat(err, ascii(10)));
      end
    else
      [fp, www] = mopen('/dev/audio','wb', 0);
      if www < 0 then 
        fp = -1;
      end
      
      if fp == (-1) then
        warning(msprintf(_("%s: Audio capabilities not available.\n"),'playsnd'));
        return
      end
      
      mput(lin2mu(y),'uc',fp);
      mclose(fp);
      
    end
  end
  
endfunction
