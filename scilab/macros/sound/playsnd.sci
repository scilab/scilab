function []=playsnd(y,fs,bits)
//Unix sound player facility 
if ~MSDOS then
  [fp,www] = mopen('/dev/audio','wb',0);
  if www<0 then fp = -1;end
  if fp==(-1) then
    disp('Audio capabilities not available');
    return
   end
  mput(lin2mu(y),'uc',fp);
  mclose(fp);
end
