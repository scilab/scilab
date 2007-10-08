

//code2str 

if code2str(10:35)<>'abcdefghijklmnopqrstuvwxyz' then pause,end
if code2str(-(10:35))<>'ABCDEFGHIJKLMNOPQRSTUVWXYZ' then pause,end
if code2str(0:9)<>'0123456789' then pause,end
if code2str([])<>emptystr() then pause,end
