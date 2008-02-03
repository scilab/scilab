
files=['intex9c.o','as2osc.o'];
ilib_build('gw_ex9c',['ex9c','intex9c'],files,[]);

exec loader.sce;

//a's to o's
x=ex9c('gaad');
if x<>'good' then pause,end 
