currentdir=pwd();
cd SCI+'/modules/mexlib/examples/interface/lib'

exec('builder.sce');

cd SCI+'/modules/mexlib/examples/interface'
exec('builder.sce');

exec(SCI+'/modules/mexlib/examples/interface/loader.sce');


if f1(89)<>89+2 then pause,end
if f2(89)<>89+3 then pause,end
if f3(89)<>89 then pause,end
if f4(89)<>89+5 then pause,end

cd currentdir

