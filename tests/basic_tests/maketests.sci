function tests=testslist()
tests=['maxi.tst'
'norm.tst'
'matops.tst'
'scilab.tst'
'macro.tst'
'calpol.tst'
'clause.tst'
'sparse.tst'
'lsparse.tst'
'lstops.tst'
'polops.tst'
'break.tst'
'breakpoints.tst'
'pause.tst'
'sprintf.tst'
'sscanf.tst'
'display.tst'
'matode.tst'
'matopt.tst'
'matelm.tst'
'polelm.tst'
'metanet.tst'
'matimp.tst'
'dassldasrt.tst'
'call.tst'
'bvode.tst'
'feval.tst'
'auto.tst'
'odedc.tst'
'sound.tst'
'listinsertion.tst'
'listextraction.tst'
'links.tst'
'strelm.tst'
'gsort.tst'
'odedi.tst'
'dcd.tst'
'grand.tst'
'grandb.tst'
'resume.tst'
'errcatch.tst'
'arma.tst'
'msscanf.tst'
'msprintf.tst'
'binary.tst'
'mfprintf.tst'
'mfscanf.tst'
'fscanfMat.tst'
'global.tst'
'matsys.tst'
'saveload.tst'
'intops.tst']
if MSDOS then
tests(find(tests=='breakpoints.tst'))=[]
tests(find(tests=='pause.tst'))=[]
tests(find(tests=='metanet.tst'))=[]
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
