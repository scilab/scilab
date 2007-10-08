
//part
if part('abc',1)<>'a' then pause,end
if part('abc',[1 1])<>'aa' then pause,end
if part('abc',[1;1])<>'aa' then pause,end
if part('abc',[])<>emptystr() then pause,end
if part('abc',5)<>' ' then pause,end
if part('abc',5:6)<>'  ' then pause,end
if or(part(['abc';'x'],1)<>['a';'x']) then pause,end
if or(part(['abc';'x'],[1 1])<>['aa';'xx']) then pause,end
//if or(part(['abc';'x'],[1 2])<>['aa';'x ']) then pause,end

