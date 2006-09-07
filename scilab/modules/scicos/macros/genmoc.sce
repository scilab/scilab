mode(-1)
//this script is used to compile Modelica models of scicos implicit
//blocks
//mprintf('Compiling Modelica models in '+pwd()')

load('SCI/modules/core/macros/lib')
load('SCI/modules/overloading/macros/lib')
load('SCI/modules/integer/macros/lib')
load('SCI/modules/elementaries_functions/macros/lib')
load('SCI/modules/polynomials/macros/lib')

if ~isdef('genmoc_verbose') then 
  genmoc_verbose=%f
end

SCI=getenv('SCI')  
MSDOS = getenv('WIN32','NO')=='OK' & ..
	or(COMPILER==['VC++' 'gcc'])

TMPDIR=getenv('TMPDIR');

if ~with_modelica_compiler() then quit,end
models=stripblanks(mgetl('models'))
if MSDOS then
  compilerpath=pathconvert(SCI+'/modules/scicos/bin/modelicac.exe',%f,%t)
else
  compilerpath=pathconvert(SCI+'/modules/scicos/bin/modelicac',%f,%t)
end

for k=1:size(models,1)
  M=models(k)
  n=newest(M,M+'c',compilerpath)
  if n<>2 then
    write(%io(2),'Processing file '+M)
    if genmoc_verbose then 
      ierr=unix(compilerpath+' -c '+M+' -o '+M+'c')
      if ierr<>0 then 
	mprintf('------------------------- '+M+..
		' Compilation error detected')
	break,
      end
    else
      // error will stop unix_s in case of error
      unix_s(compilerpath+' -c '+M+' -o '+M+'c')
    end
  end
end


