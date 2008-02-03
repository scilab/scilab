
files=['intex4f.o'];
ilib_build('gw_ex4f',['ex4f','C2F(intex4f)'],files,[]);

exec loader.sce;

ex4f();
if Str<> "Pilgrim said he, where can it be this land of Eldorado" then pause;end

