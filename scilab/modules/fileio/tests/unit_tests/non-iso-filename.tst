// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yung-Jang Lee <yjlee123@gmail.com>
// Copyright (C) 2008 - Digiteo - Sylvestre Ledru
// Copyright (C) 2009 - Digiteo - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

//
// Script to thest file name, path name of Scilab command
// set specialName with system encoding characters > 127

olddir=pwd();

// 中文
// =============================================================================

    specialName = '中文';
    cd TMPDIR;

    // for CP950, BIG5with character code > 127
    // and western charset

    dirName     = 'dir_'+specialName;
    fileName    = 'file_'+specialName;
    fileWithExt = 'file_'+specialName+'.ext';

    // 1. test mkdir, createdir,cd
    // =========================================================================

    mkdir(dirName);
    if isdir(dirName) == %f then pause, end
    createdir(dirName+'_other');
    if isdir(dirName+'_other') == %f then pause, end
    cd(dirName);


    // 2. Fix bug in  sci_pwd, for pwd
    // =========================================================================

    if ~(length(pwd()) > 0) then pause, end


    // 3. test mopen
    // =========================================================================

    fd=mopen(fileName,'w');
    if (fileinfo(fileName)==[]) then pause, end
    mclose(fd);// ok


    // 4. copyfile (host,unix)
    // =========================================================================

    copyfile(fileName,fileWithExt);
    if (fileinfo(fileWithExt)==[]) then pause, end
    deletefile(fileWithExt);
    copyfile(fileName,'../'+dirName+'_other');
    if (fileinfo('../'+dirName+'_other/'+fileName)==[]) then pause, end
    deletefile('../'+dirName+'_other/'+fileName);


    // 5. mputl, mfprintf
    // =========================================================================

    mputl([specialName+'1';specialName+'2'],fileName+'_mputl');
    if (fileinfo(fileName+'_mputl')==[]) then pause, end
    fd=mopen(fileName+"_"+specialName,'w');
    mfprintf(fd,'%s %f',specialName+'3',%pi);
    filestuff=fileinfo(fileName+"_"+specialName);
    if (filestuff==[] & filestuff(1)>0) then pause, end
    mclose(fd);


    // 6. Fortran style open/close
    // filename fixed, but string in fortran file still in UTF encoding
    // =========================================================================
    fd = mopen(fileName+'_open','w');
    if (fileinfo(fileName+'_open')==[]) then pause, end
    mfprintf(fd,'%s %f',specialName+'3',%pi);
    filestuff=fileinfo(fileName+'_open');
    if (filestuff==[] & filestuff(1)>0) then pause, end
    mclose(fd);

    fd = mopen(fileName+'_fprintf','wt');
    mfprintf(fd,'%s %s',specialName+'3',string(%pi));
    mclose(fd);
    filestuff=fileinfo(fileName+'_fprintf');
    if (filestuff==[] & filestuff(1)>0) then pause, end

    fprintfMat(fileName+"_Mat_"+specialName,[11,12;21,22],'%5.2f',specialName+'_fprintfMat Test');
    filestuff=fileinfo(fileName+"_Mat_"+specialName);
    if (filestuff==[] & filestuff(1)>0) then pause, end


    // 7. findfiles.c
    // =========================================================================

    A = [ 'file_中文_fprintf'; ..
          'file_中文_mputl'; ..
          'file_中文_open'; ..
          'file_中文_Mat_中文'; ..
          'file_中文_中文'; ..
          'file_中文'];

    B = ls();
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = listfiles();
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = dir();
    if(or(gsort(A) <> gsort(B(2)))) then pause,end;

    B = findfiles('.','*');
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = ls('../'+dirName);
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = listfiles('../'+dirName);
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = dir('../'+dirName);
    if(or(gsort(A) <> gsort(B(2)))) then pause,end;

    B = findfiles('.','*'+specialName+'*');
    if(or(gsort(A) <> gsort(B))) then pause,end;


    // 8. test getshortpathname,getlongpathname
    // =========================================================================

    sname=getshortpathname(fileName);
    if ~(length(sname) > 0) then pause, end
    lname=getlongpathname(sname);
    if ~(length(lname) > 0) then pause, end


    // 9. test deletefile, removedir
    // =========================================================================

    removedir('../'+dirName+'_other');
    chdir("..");
    removedir(dirName);



// æøå
// =============================================================================

    specialName = 'æøå';
    cd TMPDIR;

    // for CP950, BIG5with character code > 127
    // and western charset

    dirName     = 'dir_'+specialName;
    fileName    = 'file_'+specialName;
    fileWithExt = 'file_'+specialName+'.ext';

    // 1. test mkdir, createdir,cd
    // =========================================================================

    mkdir(dirName);
    if isdir(dirName) == %f then pause, end
    createdir(dirName+'_other');
    if isdir(dirName+'_other') == %f then pause, end
    cd(dirName);


    // 2. Fix bug in  sci_pwd, for pwd
    // =========================================================================

    if ~(length(pwd()) > 0) then pause, end


    // 3. test mopen
    // =========================================================================

    fd=mopen(fileName,'w');
    if (fileinfo(fileName)==[]) then pause, end
    mclose(fd);// ok


    // 4. copyfile (host,unix)
    // =========================================================================

    copyfile(fileName,fileWithExt);
    if (fileinfo(fileWithExt)==[]) then pause, end
    deletefile(fileWithExt);
    copyfile(fileName,'../'+dirName+'_other');
    if (fileinfo('../'+dirName+'_other/'+fileName)==[]) then pause, end
    deletefile('../'+dirName+'_other/'+fileName);


    // 5. mputl, mfprintf
    // =========================================================================

    mputl([specialName+'1';specialName+'2'],fileName+'_mputl');
    if (fileinfo(fileName+'_mputl')==[]) then pause, end
    fd=mopen(fileName+"_"+specialName,'w');
    mfprintf(fd,'%s %f',specialName+'3',%pi);
    filestuff=fileinfo(fileName+"_"+specialName);
    if (filestuff==[] & filestuff(1)>0) then pause, end
    mclose(fd);


    // 6. Fortran style open/close
    // filename fixed, but string in fortran file still in UTF encoding
    // =========================================================================

    fd = mopen(fileName+'_open','w');
    if (fileinfo(fileName+'_open')==[]) then pause, end
    mfprintf(fd,'%s %s',specialName+'3',string(%pi));
    filestuff=fileinfo(fileName+'_open');
    if (filestuff==[] & filestuff(1)>0) then pause, end
    mclose(fd);

    fd = mopen(fileName+'_fprintf','wt');
    mfprintf(fd,'%s %s',specialName+'3',string(%pi));
    mclose(fd);
    filestuff=fileinfo(fileName+'_fprintf');
    if (filestuff==[] & filestuff(1)>0) then pause, end

    fprintfMat(fileName+"_Mat_"+specialName,[11,12;21,22],'%5.2f',specialName+'_fprintfMat Test');
    filestuff=fileinfo(fileName+"_Mat_"+specialName);
    if (filestuff==[] & filestuff(1)>0) then pause, end


    // 7. findfiles.c
    // =========================================================================

    A = [ 'file_æøå_fprintf'; ..
          'file_æøå_mputl'; ..
          'file_æøå_open'; ..
          'file_æøå_Mat_æøå'; ..
          'file_æøå_æøå'; ..
          'file_æøå'];

    B = ls();
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = listfiles();
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = dir();
    if(or(gsort(A) <> gsort(B(2)))) then pause,end;

    B = findfiles('.','*');
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = ls('../'+dirName);
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = listfiles('../'+dirName);
    if(or(gsort(A) <> gsort(B))) then pause,end;

    B = dir('../'+dirName);
    if(or(gsort(A) <> gsort(B(2)))) then pause,end;

    B = findfiles('.','*'+specialName+'*');
    if(or(gsort(A) <> gsort(B))) then pause,end;


    // 8. test getshortpathname,getlongpathname
    // =========================================================================

    sname=getshortpathname(fileName);
    if ~(length(sname) > 0) then pause, end
    lname=getlongpathname(sname);
    if ~(length(lname) > 0) then pause, end


    // 9. test deletefile, removedir
    // =========================================================================

    removedir('../'+dirName+'_other');
    chdir("..");
    removedir(dirName);

