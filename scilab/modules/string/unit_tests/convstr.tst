
//convstr
if convstr('ABC')<>'abc' then pause,end
if convstr('ABC','l')<>'abc' then pause,end
if convstr('ABC','u')<>'ABC' then pause,end
if convstr(['ABC';'x'])<>['abc';'x'] then pause,end
if convstr(['ABC';'x'],'l')<>['abc';'x'] then pause,end
if convstr(['ABC';'x'],'u')<>['ABC';'X'] then pause,end
if or(convstr(['';''],'u')<>['';'']) then pause,end

if convstr('ABC')<>'abc' then pause,end
if convstr('ABC','l')<>'abc' then pause,end
if convstr('ABC','u')<>'ABC' then pause,end
if convstr(['ABC','x'])<>['abc','x'] then pause,end
if convstr(['ABC','x'],'l')<>['abc','x'] then pause,end
if convstr(['ABC','x'],'u')<>['ABC','X'] then pause,end
if convstr(emptystr())<>emptystr() then pause,end
