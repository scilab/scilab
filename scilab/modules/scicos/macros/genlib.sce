mode(-1)
//this script is used to compile Modelica models of scicos implicit
//blocks
//mprintf('Compiling Modelica models in '+pwd()')
load SCI/macros/util/lib


load('SCI/modules/elementary_functions/macros/lib')
load('SCI/modules/differential_equations/macros/lib')
load('SCI/modules/polynomials/macros/lib')
load('SCI/modules/overloading/macros/lib')

names=stripblanks(mgetl('names'));
args=sciargs()
genlib(args($-1),args($),names=names,verbose=%t)
quit
