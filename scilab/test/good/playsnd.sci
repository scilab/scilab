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
	disp('Audio capabilities not available');
	return
      end
      mput(lin2mu(y),'uc',fp);
      mclose(fp);
    end
  end
endfunction
