mode(-1)
//this script is used to compile Modelica models of scicos implicit
//blocks
//mprintf('Compiling Modelica models in '+pwd()')
names=stripblanks(mgetl('names'));
args=sciargs()
genlib(args($-1),args($),names=names,verbose=%t)
quit
