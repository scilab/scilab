function integr=logr(k,var)
// integr=logr(k,var)
// calcul theorique de l'exposant de Liapunov y
//!
// Copyright INRIA
deff('y=fff(x)',...
   'y=log(abs('+string(k)+'+x))*exp(-x**2/(2*'+string(var)'+'))');
integr=intg(-100,100,fff)
endfunction
