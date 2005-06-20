mode(-1)
//this script is used to compile Modelica models of scicos implicit
//blocks
//mprintf('Compiling Modelica models in '+pwd()')
load SCI/macros/util/lib
load('SCI/macros/elem/lib')
load('SCI/macros/int/lib')
load('SCI/macros/calpol/lib')
load('SCI/macros/percent/lib')

SCI=getenv('SCI')  
MSDOS = getenv('WIN32','NO')=='OK' & ..
	or(COMPILER==['VC++' 'gcc'])

names=stripblanks(mgetl('names'));
args=sciargs()
genlib(args($-1),args($),names=names,verbose=%t)
quit
