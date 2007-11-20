//===============================
// unit tests code2str
// INRIA 2007
//===============================
if code2str(10:35)<>'abcdefghijklmnopqrstuvwxyz' then pause,end
if code2str(-(10:35))<>'ABCDEFGHIJKLMNOPQRSTUVWXYZ' then pause,end
if code2str(0:9)<>'0123456789' then pause,end
if code2str([])<>emptystr() then pause,end
//===============================
s = code2str([-28 12 18 21 10 11]);
if (s <>'Scilab') then pause,end
//===============================
