// load the shared library 

exec loader.sce ;

// run tests 

[u,v]=mexf15(2,1:3);
if u~=2 then pause,end






