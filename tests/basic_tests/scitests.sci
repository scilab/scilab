function scitests(tstfile)
mydisp('------------------- File '+tstfile+'--------------------')
//Reset standard globals
rand('seed',0);rand('uniform');

diafile=strsubst(tstfile,'.tst','.dia')
// Do some modification in  tst file 
// ---------------------------------
txt=mgetl(tstfile)
txt=strsubst(txt,'pause,end','bugmes();return;end')
txt=strsubst(txt,'-->','@#>') //to avoid suppression of input --> with prompts
txt=strsubst(txt,'halt()','')
// and save it in a temporary file
u=file('open',TMPDIR+'/tst','unknown');write(u,txt,'(a)');file('close',u)

// exec the modified tst file
deff('[]=bugmes()','write(%io(2),''error on test'')');

myexec()
//  Do some modification in  dia file 
// ----------------------------------
dia=mgetl(TMPDIR+'/dia')
dia(grep(dia,'exec('))=[];

//suppress the prompts
dia=strsubst(strsubst(dia,'-->',''),'@#>','-->')
//standardise  number display   
dia=strsubst(strsubst(strsubst(strsubst(dia,' .','0.'),..
    'E+','D+'),'E-','D-'),'-.','-0.')

dia=strsubst(dia,'bugmes();return','bugmes();quit'); //not to change the ref files
// write down the resulting dia file
u=file('open',diafile,'unknown');write(u,dia,'(a)');file('close',u)

//Check for execution errors
// -------------------------
if grep(dia,'error on test')<>[] then
  mydisp("Test failed ERROR DETECTED  while executing "+tstfile)
  return
end
//Check for diff with the .ref file
// --------------------------------
[u,ierr]=mopen(diafile+'.ref','r')
if ierr== 0 then //ref file exists
  ref=mgetl(u);mclose(u)
  // suppress blank (diff -nw)
  dia=strsubst(dia,' ','')
  ref=strsubst(ref,' ','')
  
  if or(ref<>dia) then 
    mydisp('Test Failed SEE : diff -w  '+diafile+' '+diafile+'.ref ')
  else
    mydisp('Test passed')
  end
end
mydisp('----------------------------------------------------------')

function mydisp(str)
write(result,str,'(a)')
write(0,str,'(a)')

function myexec()
if MSDOS then
  fil=TMPDIR+'\tst'
  dia=TMPDIR+'\dia'
  gr=TMPDIR+'\gr'
  unix_s('del '+dia)
else
  fil=TMPDIR+'/tst'
  dia=TMPDIR+'/dia'
  gr=TMPDIR+'/gr'
  unix_s('rm -f '+dia)
end
lines(0);diary(dia);driver('Pos');xinit(gr)
exec(fil,3)
diary(0);xend()

function maketests
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
path=SCI+'/tests/'
if MSDOS then
  path=strsubst(path,'/','\')
  lst=unix_g('dir /B /OD '+path+'* ')
  lst=lst($:-1:1)
else
  lst=unix_g('ls  -t1 '+path+'*.*') 
end
result=file('open','test.log','unknown')
lst=strsubst(lst,path,'')
for k1=1:size(tests,'*')  // loop on .tst files
  k=find(tests(k1)==lst);
  if MSDOS then
    fl=path+lst(k)
    fnam=lst(k)
  else
    fl=lst(k)
    ks=strindex(fl,'/')
    if ks==[] then fnam=fl;else fnam=part(fl,ks($)+1:length(fl));end
  end
  dia=find(strsubst(lst(k),'.tst','.dia')==lst)
  if dia==[]| dia>k then
    scitests(lst(k))
  end
end

file('close',result)
