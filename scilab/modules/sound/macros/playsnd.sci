// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab 
// Copyright (C) ???? - ENPC
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function []=playsnd(y,rate,bits,aplay)
// play signal y at sample rate rate 
// bits is unused 
  [lhs,rhs]=argn(0);
  if rhs <= 3 aplay='aplay'; end 
  if rhs <= 2 bits=16; end 
  if rhs <= 1 rate=22050; end 
  if MSDOS  then
    savewave(TMPDIR+'/_playsnd_.wav',y,rate);
    PlaySound(TMPDIR+'/_playsnd_.wav');      
    return 
  else
    if aplay<>'/dev/audio' then 
      // should send aplay to PlaySound 
      savewave(TMPDIR+'/_playsnd_.wav',y,rate);
      PlaySound(TMPDIR+'/_playsnd_.wav',aplay);  
    else
      [fp,www] = mopen('/dev/audio','wb',0);
      if www<0 then 
	fp = -1;
      end
      if fp==(-1) then
	warning(gettext("Audio capabilities not available."));
	return
      end
      mput(lin2mu(y),'uc',fp);
      mclose(fp);
    end
  end
endfunction
