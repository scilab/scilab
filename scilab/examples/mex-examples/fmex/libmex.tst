// load the shared library 
exec loader.sce ;
// run tests 
[a,b]=mexf1(1,2);
if b~=2 then pause,end

[a,b,c,d,e]=mexf2(1,2);
if part(e,1:6) ~='scimex' then pause;end

a=mexf3(2);
if b~=2 then pause,end






