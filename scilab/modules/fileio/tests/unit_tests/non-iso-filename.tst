// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yung-Jang Lee <yjlee123@gmail.com>
// Copyright (C) 2008 - Digiteo - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

//
// Script to thest file name, path name of Scilab command
//
// set specialName with system encoding characters > 127
//
olddir=pwd();
cd TMPDIR;
specialNameA=['中文' 'æøå' ]; // UTF 8 stuff 

for specialName=specialNameA
// for CP950, BIG5with character code > 127
// and western charset
dirName    ='dir_'+specialName;
fileName   ='file_'+specialName;
fileWithExt='file_'+specialName+'.ext';

// 1.
// test mkdir, createdir,cd
//
mkdir(dirName);
if isdir(dirName) == %f then pause, end
createdir(dirName+'_other');
if isdir(dirName+'_other') == %f then pause, end
cd(dirName);

// 2.
//fix bug in  sci_getcwd, for pwd and getcwd
//
if ~(length(pwd()) > 0) then pause, end
if ~(length(getcwd()) > 0) then pause, end
// 3.
// test mopen
//
fd=mopen(fileName,'w');
if (fileinfo(fileName)==[]) then pause, end
mclose(fd);// ok 

//4.
// copyfile (host,unix)
copyfile(fileName,fileWithExt)
if (fileinfo(fileWithExt)==[]) then pause, end
deletefile(fileWithExt); 
copyfile(fileName,'../'+dirName+'_other')
if (fileinfo('../'+dirName+'_other/'+fileName)==[]) then pause, end
deletefile('../'+dirName+'_other/'+fileName)
//5
// mputl, mfprintf
//
mputl([specialName+'1';specialName+'2'],fileName+'_mputl');
if (fileinfo(fileName+'_mputl')==[]) then pause, end
fd=mopen(fileName+"_"+specialName,'w');
mfprintf(fd,'%s %f',specialName+'3',%pi); 
filestuff=fileinfo(fileName+"_"+specialName);
if (filestuff==[] & filestuff(1)>0) then pause, end
mclose(fd);
//6
// Fortran style open/close 
// filename fixed, but string in fortran file still in UTF encoding
//
fd=file('open',fileName+'_open','new');
if (fileinfo(fileName+'_open')==[]) then pause, end
fprintf(fd,'%s %f',specialName+'3',%pi);
filestuff=fileinfo(fileName+'_open');
if (filestuff==[] & filestuff(1)>0) then pause, end
file('close',fd);

fprintf(fileName+'_fprintf','%s %f',specialName+'3',%pi); 
filestuff=fileinfo(fileName+'_fprintf');
if (filestuff==[] & filestuff(1)>0) then pause, end

fprintfMat(fileName+"_Mat_"+specialName,[11,12;21,22],'%5.2f',specialName+'_fprintfMat Test');
filestuff=fileinfo(fileName+"_Mat_"+specialName);
if (filestuff==[] & filestuff(1)>0) then pause, end
//7
//  findfiles.c
//
ls()//
listfiles() //
dir() //
findfiles('.','*') 
ls('../'+dirName) //
listfiles('../'+dirName) //
dir('../'+dirName) //
findfiles('.','*'+specialName+'.*')
//8
// test getshortpathname,getlongpathname
//
sname=getshortpathname(fileName);
if ~(length(sname) > 0) then pause, end
lname=getlongpathname(sname);
if ~(length(lname) > 0) then pause, end
//9
// test deletefile, removedir
//
removedir('../'+dirName+'_other');
chdir("..");
removedir(dirName);
end //for specialName=specialNameA
chdir(olddir);

