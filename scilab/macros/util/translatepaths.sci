function translatepaths(Paths,res_path)
// Copyright INRIA

// Perform translation of Matlab M-files to Scilab for all M-files found in Paths

// M2SCI kernel functions called:
//  - mfile2sci

// Get default arguments
[lhs,rhs]=argn(0)
if rhs<2 then res_path="./",end
if rhs<1 then m2sci_gui();return;end

// Loads libraries related to m2sci
if exists("m2skernellib")==0 then load("SCI/macros/m2sci/kernel/lib"),end
if exists('m2spercentlib')==0 then load("SCI/macros/m2sci/percent/lib"),end
if exists("m2ssci_fileslib")==0 then load("SCI/macros/m2sci/sci_files/lib"),end

// Convert paths so that they can be used accoerding to the platform
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
logfile=file('open',res_path+'log','unknown')
whsfil_unit=file('open',res_path+'whatis','unknown')

// Close paths with a / or a \
for k=1:size(Paths,'*')
  if part(Paths(k),length(Paths(k)))<>sep then 
    Paths(k)=Paths(k)+sep,
  end
end

// Find names of files to translate
mfiles=[]
for k=1:size(Paths,'*')
  path=Paths(k)
  if MSDOS then 
    mfiles=[mfiles;unix_g('dir /b '+path+'*.m')]
  else
    mfiles=[mfiles;unix_g('ls '+path+'*.m')]
  end
end

// Translate all files
for k1=1:size(mfiles,1)
  if MSDOS then
    fnam=part(mfiles(k1),1:length(mfiles(k1))-2)
    mpath=path+mfiles(k1)
  else   
    kk=strindex(mfiles(k1),sep)
    fnam=part(mfiles(k1),kk($)+1:length(mfiles(k1))-2)
    mpath=mfiles(k1)    
  end
  scipath=res_path+fnam+'.sci'
  scepath=res_path+fnam+'.sce'
  // Translation is done only if M-file has changed
  if newest(mpath,scipath,scepath)==1 then
    mfile2sci(mpath,res_path,%F,%F,3,%F)
  end
end
endfunction
