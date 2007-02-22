// Allan CORNET 
// INRIA 2007

exec(SCI+'/modules/mexlib/examples/fmex/builder.sce');
exec(SCI+'/modules/mexlib/examples/fmex/loader.sce');


[a,b]=mexf1(1,2);

if (b~=2) then 
  disp('problem with this example');
else
  disp('ok');
end
