deff('foo()','a=1,b=2,c=3;[x,y]=resume(a,b)')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','a=1,b=2,c=3;[x,y,z]=resume(a,b,c)')
foo(); if x<>1|y<>2|z<>3 then pause,end
clear foo x y z
deff('foo()','if %t then a=1,b=2,c=3;[x,y]=resume(a,b);end')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','if %t then a=1,b=2,c=3;[x,y,z]=resume(a,b,c);end')
foo(); if x<>1|y<>2|z<>3 then pause,end

clear foo x y z
deff('foo()','for k=1:3,if k==3 then a=1,b=2,c=3;[x,y]=resume(a,b);end;end')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','for k=1:3,if k==3 then a=1,b=2,c=3;[x,y,z]=resume(a,b,c);end;end')
foo(); if x<>1|y<>2|z<>3 then pause,end

clear foo x y z
deff('foo()','a=1,b=2,c=3;[x,y]=resume(a,b)','n')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','a=1,b=2,c=3;[x,y,z]=resume(a,b,c)','n')
foo(); if x<>1|y<>2|z<>3 then pause,end
clear foo x y z
deff('foo()','if %t then a=1,b=2,c=3;[x,y]=resume(a,b);end','n')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','if %t then a=1,b=2,c=3;[x,y,z]=resume(a,b,c);end','n')
foo(); if x<>1|y<>2|z<>3 then pause,end

clear foo x y z
deff('foo()','for k=1:3,if k==3 then a=1,b=2,c=3;[x,y]=resume(a,b);end;end','n')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','for k=1:3,if k==3 then a=1,b=2,c=3;[x,y,z]=resume(a,b,c);end;end','n')
foo(); if x<>1|y<>2|z<>3 then pause,end

//======================================================================
deff('foo()','a=1,b=2,c=3;execstr(''[x,y]=resume(a,b)'')')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','a=1,b=2,c=3;execstr(''[x,y,z]=resume(a,b,c)'')')
foo(); if x<>1|y<>2|z<>3 then pause,end
clear foo x y z
deff('foo()','if %t then a=1,b=2,c=3;execstr(''[x,y]=resume(a,b)'');end')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','if %t then a=1,b=2,c=3;execstr(''[x,y,z]=resume(a,b,c)'');end')
foo(); if x<>1|y<>2|z<>3 then pause,end

clear foo x y z
deff('foo()','for k=1:3,if k==3 then a=1,b=2,c=3;execstr(''[x,y]=resume(a,b)'');end;end')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','for k=1:3,if k==3 then a=1,b=2,c=3;execstr(''[x,y,z]=resume(a,b,c)'');end;end')
foo(); if x<>1|y<>2|z<>3 then pause,end

clear foo x y z
deff('foo()','a=1,b=2,c=3;execstr(''[x,y]=resume(a,b)'')','n')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','a=1,b=2,c=3;execstr(''[x,y,z]=resume(a,b,c)'')','n')
foo(); if x<>1|y<>2|z<>3 then pause,end
clear foo x y z
deff('foo()','if %t then a=1,b=2,c=3;execstr(''[x,y]=resume(a,b)'');end','n')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','if %t then a=1,b=2,c=3;execstr(''[x,y,z]=resume(a,b,c)'');end','n')
foo(); if x<>1|y<>2|z<>3 then pause,end

clear foo x y z
deff('foo()','for k=1:3,if k==3 then a=1,b=2,c=3;execstr(''[x,y]=resume(a,b)'');end;end','n')
foo(); if x<>1|y<>2 then pause,end
clear foo x y z
deff('foo()','for k=1:3,if k==3 then a=1,b=2,c=3;execstr(''[x,y,z]=resume(a,b,c)'');end;end','n')
foo(); if x<>1|y<>2|z<>3 then pause,end

