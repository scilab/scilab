
//sort
[s]=sort(['abc','abd','aa','bxe']);
if or(s<>['aa','abc','abd','bxe']) then pause,end

[s,k]=sort(['abc','abd','aa','bxe']);
if or(s<>['aa','abc','abd','bxe']) then pause,end
if or(k<>[3 1 2 4])  then pause,end

if sort('abc')<>'abc' then pause,end
