function   transorder=translatepaths(Paths,res_path)
// Copyright INRIA

// Perform translation of Matlab M-files to Scilab for all M-files found in Paths

// Output :
//  -transorder : a vector which contains the files names with a determinated order
// Input :
//  -Paths : a vector of strings (paths of Matlab M-files to translate)
//  -res_paths : a string (resolution path of translated files)

// M2SCI kernel functions called:
//  -lst_funcall
//  -translateorder
//  - mfile2sci

// Get default arguments

[lhs,rhs]=argn(0)
if rhs<2 then res_path="./",end
if rhs<1 then m2sci_gui();return;end

// Loads libraries related to m2sci
if exists("m2skernellib")==0 then load("SCI/macros/m2sci/kernel/lib"),end
if exists("m2spercentlib")==0 then load("SCI/macros/m2sci/percent/lib"),end
if exists("m2ssci_fileslib")==0 then load("SCI/macros/m2sci/sci_files/lib"),end

// Convert paths so that they can be used according to the platform

if MSDOS then 
  sep='\'
  Paths=strsubst(Paths,'/',sep)
  res_path=strsubst(res_path,'/',sep)
else
  sep='/'
  Paths=strsubst(Paths,'\',sep)
  res_path=strsubst(res_path,'\',sep)
end

// Close resolution path with a / or a \
res_path=stripblanks(res_path)
if part(res_path,length(res_path))<>sep then
  res_path=res_path+sep
end

// Create a logfile and a whatis file
Paths=stripblanks(Paths)
[tempfd,ierr]=file('open',pathconvert(TMPDIR)+"unitfile.dat","old");

if ierr==0 then
  load(pathconvert(TMPDIR)+"unitfile.dat")
  file('close',whsfil_unit);
  file('close',tempfd);
  mdelete(pathconvert(TMPDIR)+"unitfile.dat")
end

whsfil_unit=file('open',res_path+'whatis','unknown')
save(pathconvert(TMPDIR)+"unitfile.dat",whsfil_unit)
// Close paths with a / or a \
for k=1:size(Paths,'*')
  if part(Paths(k),length(Paths(k)))<>sep then 
    Paths(k)=Paths(k)+sep,
  end
end

// Find names of files to translate
// mfiles is a vector which contains the names (and the paths) of files to translate
mfiles=[]
for k=1:size(Paths,'*')
  path=Paths(k)
  if MSDOS then 
    mfiles=[mfiles;path + unix_g('dir /b '+ """" + path + """" + '*.m')]
  else
    mfiles=[mfiles;unix_g('ls '+ """" + path + """" + '*.m')]
  end
end

// fnamvect is a vector which contains all M-files names (just the names) found in Paths
// filefuncallname is a list of vectors. The first component of each vector is the name of a M-file (found in the Path(s)) to translate, followed by the called functions by this M-file) 
fnamvect=[]
filefuncallname=list()
for k1=1:size(mfiles,1)
  kk=strindex(mfiles(k1),sep)
  fnamvect=[fnamvect;part(mfiles(k1),kk($)+1:length(mfiles(k1))-2)]
end

for k1=1:size(mfiles,1)
    mpath=mfiles(k1)
    disp("********************lst_funcall**********************")
   disp(mpath)
 filefuncallname($+1)=lst_funcall(mpath,fnamvect)
end

// Determination of a translation order for the M-files found in the Paths
// transorder is a vector containing the M-files names which are arranged to order to respect an priority order of translation
transorder=[]
for i=1:size(filefuncallname)
  execstr(filefuncallname(i)(1)+"vect=filefuncallname(i)")
end

overfunname=[] 
for i=1:size(filefuncallname)
  execstr("transorder=translateorder(transorder,"+filefuncallname(i)(1)+"vect,overfunname)")
end

// funpath is a vector which contains the M-files names (ands their paths) to translate
funpath=[]
for i=1:size(transorder,1)
  ind=find(transorder(i)==fnamvect)
  if ind<>[] then
    funpath=[funpath;mfiles(ind)]
  end
end   

// Translation is done only if M-file has changed
logtxt=[]
resumelogtxt=[]

for i=1:size(funpath,1)
  kk=strindex(funpath(i),sep)
  mpath=funpath(i) 
  fnam=part(funpath(i),kk($)+1:length(funpath(i))-2)
  scipath=res_path+fnam+".sci"  
  scepath=res_path+fnam+".sce"

  if newest(mpath,scipath,scepath)==1 then
  [fd,ierr]=file('open',pathconvert(TMPDIR)+fnam+".m",'old');
    if ierr==0 & strindex(mpath,TMPDIR)==[] then
      mfile2sci(pathconvert(TMPDIR)+fnam+".m",res_path)
      file('close',fd)
      mdelete(pathconvert(TMPDIR)+fnam+".m")  
      rmdir(pathconvert(TMPDIR)+pathconvert(fnam),'s')
    else
     mfile2sci(funpath(i),res_path)
    end
    
    tmp_sci_file=pathconvert(TMPDIR)+"tmp_"+fnam+".sci"
    ierr=execstr("getf(tmp_sci_file)","errcatch");errclear();
    if ierr==0 & strindex(mpath,TMPDIR)==[] then
      txt=[]
      txt=mgetl(scipath)
      txt=[txt;" ";mgetl(tmp_sci_file)]
      mputl(txt,scipath) 
      mdelete(tmp_sci_file)
    end
    
    // LOG
    tmp_m2sci_file=pathconvert(TMPDIR)+"tmp_m2sci_"+fnam+".log"
    m2scipath=res_path+"m2sci_"+fnam+".log" 
    logtxt=[logtxt;" ";" ";mgetl(m2scipath)]
    mdelete(m2scipath)
    
    [fd,ierr]=file('open',tmp_m2sci_file,'old');
    if ierr==0 & strindex(mpath,TMPDIR)==[] then
      logtxt=[logtxt;" ";mgetl(tmp_m2sci_file)]
      file('close',fd)
      mdelete(tmp_m2sci_file)
    end
    
    // RESUMELOG
    tmp_resume_m2sci_file=pathconvert(TMPDIR)+"tmp_resume_m2sci_"+fnam+".log"
    resumem2scipath=res_path+"resume_m2sci_"+fnam+".log" 
    if fileinfo(resumem2scipath)<>[] then
      resumelogtxt=[resumelogtxt;" ";" ";mgetl(resumem2scipath)]
      mdelete(resumem2scipath)
    end
    
    [fd,ierr]=file('open',tmp_resume_m2sci_file,'old');
    if ierr==0 & strindex(mpath,TMPDIR)==[] then
      resumelogtxt=[resumelogtxt;" ";mgetl(tmp_resume_m2sci_file)]
      file('close',fd)
      mdelete(tmp_resume_m2sci_file)
    end
  end
  mputl(logtxt,res_path+'log')
  mputl(resumelogtxt,res_path+'resumelog')
end

// File closing
file('close',whsfil_unit);
mdelete(pathconvert(TMPDIR)+"unitfile.dat")

// create builder.sce and loader.sce files
// get the directory name where the Scilab functions are written 
if res_path=="./" then
  current_path=pathconvert(unix_g('pwd'))
  index_slash=strindex(current_path,'/')
  if size(index_slash,'*')==1 then index_slash=[0 index_slash],end
  namelib=part(current_path,index_slash($-1)+1:index_slash($)-1)
else
  index_slash=strindex(res_path,'/')
  if size(index_slash,'*')==1 then index_slash=[0 index_slash],end
  namelib=part(res_path,index_slash($-1)+1:index_slash($)-1)
end

//builder.sce
buildertxt=[]
buildertxt($+1)="path=get_absolute_file_path(""builder.sce"")"
buildertxt($+1)="genlib("""+namelib+"lib"",path)"
builderfile=res_path+"builder.sce"
mputl(buildertxt,builderfile)
//loader.sce
loadertxt=[]
loadertxt($+1)="path=get_absolute_file_path(""loader.sce"")"
loadertxt($+1)="load(path+"+"""lib"")"
loaderfile=res_path+"loader.sce"
mputl(loadertxt,loaderfile)

endfunction
