// =============================================================================
// Bernard HUGUENEY - DIGITEO - 2010
// =============================================================================
files=['sci_apply.cpp'];
ilib_build('apply',['apply','sci_apply'],files,[]);
// =============================================================================
exec loader.sce;
// =============================================================================
function r = scilabfoo(x)
  r = x(1)+x(2)+x(3);
endfunction
v = apply([1,2,3;3,4,5; 555,666,777; 5,6,7;8,9,10]',scilabfoo,1);
disp('result : ' + string(v));
v = apply([10,20,30;30,40,50]',scilabfoo, 1);
disp('result : ' + string(v));


   fun_name='test_fun';
   c_prog=['#include  <math.h>'
   'void '+fun_name+'(double* arg, double* res) {'
   '*res= 2.*(*arg);'
   '}'];
   disp(c_prog);
   mputl(c_prog,fun_name+'.c');
   ilib_for_link(fun_name,fun_name+'.c',[],"c");
   exec loader.sce;
   apply([1,2],fun_name,1)

//ulink();
