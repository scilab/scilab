files=['intex12c.o'];
ilib_build('gw_ex12c',['ex12c','intex12c'],files,[]);

exec loader.sce;

deff('p=mypoly(x)','p=poly(x,''z'',''coeffs'')')
c=[1,2,3];
P1=ex12c(2,c);

