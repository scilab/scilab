function tests=testslist()
  tests = ['parse.tst'
	   'break.tst';
	   'clause.tst';
	   'global.tst';
	   'listextraction.tst';
	   'listinsertion.tst';
	   'scilab.tst';
	   'resume.tst';
	   'function.tst';
	   'maxi.tst';
	   'matsys.tst';
	   'macro.tst';
	   'matelm.tst';
	   'lstops.tst';
	   'strelm.tst';
	   'algebra.tst'
	   'saveload.tst';
	   'arma.tst';
	   
	   'basiclink.tst';
	   'binary.tst';
	   'breakpoints.tst';
	   'bvode.tst';
	   'call.tst';
	   'calpol.tst';
	   'cmp.tst';
	   'dassldasrt.tst';
	   'dcd.tst';
	   'display.tst';
	   'errcatch.tst';
	   'feval.tst';
	   'fscanfMat.tst';
	   'grandb.tst';
	   'grand.tst';
	   'gsort.tst';
	   'intops.tst';
	   'links.tst';
	   'lsparse.tst';
	   'lstops.tst';
	   'matimp.tst';
	   'matode.tst';
	   'matops.tst';
	   'matopt.tst';
	   'metanet.tst';
	   'mfprintf.tst';
	   'mfscanf.tst';
	   'msprintf.tst';
	   'msscanf.tst';
	   'norm.tst';
	   'odedc.tst';
	   'odedi.tst';
	   'pause.tst';
	   'polelm.tst';
	   'polops.tst';
	   'pvm.tst';	   
	   'sound.tst';
	   'sparse.tst';
	   'sprintf.tst';
	   'sscanf.tst';
	   'slicot.tst';
	   'fptr.tst';
	   'scicos.tst';
	   'auto.tst'
	   ];

if MSDOS then
  comp_target = getenv('COMPILER','NO');
  if comp_target == 'gcc ' then 
    tests(find(tests=='links.tst'))=[]
  end
tests(find(tests=='pvm.tst'))=[]
end
endfunction

function maketests(job)
lines(0) // to disable line control
if argn(2)<1 then job='tests',end
if job=='clean' then //remove .dia files
  cleantests()
else
  tests=testslist()
  for k1=1:size(tests,'*')  // loop on .tst files
    scitest(tests(k1))
  end
end
endfunction

function cleantests()
if MSDOS then
  path=SCI+'\tests\'
  file_to_clean=path+strsubst(testslist(),'.tst','.dia')
  unix_s('del '+strcat(file_to_clean,' '))
else
  path=SCI+'/tests/'
  file_to_clean=path+strsubst(testslist(),'.tst','.dia')
  unix_s('rm -f '+strcat(file_to_clean,' '))
end
endfunction
