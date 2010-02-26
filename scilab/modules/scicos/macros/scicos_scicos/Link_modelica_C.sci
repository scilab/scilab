//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//
// Copyright INRIA

function  [ok]=Link_modelica_C(Cfile)

  mlibs=pathconvert(modelica_libs,%f,%t)  
  Cfile=pathconvert(Cfile,%f,%t)
  name=basename(Cfile)
  path=strsubst(stripblanks(Cfile),name+'.c','')
  if MSDOS then Ofile=path+name+'.obj', else Ofile=path+name+'.o', end

  //below newest(Cfile,Ofile) is used instead of  updateC in case where
  //Cfile has been manually modified (debug,...)
  if newest(Cfile,Ofile)==1 then

    //  build the list of external functions libraries
    // remove repreated directories from mlibs
    rep=[];
    for k=1:size(mlibs,'*')
      for j=k+1:size(mlibs,'*')
        if stripblanks(mlibs(k))==stripblanks(mlibs(j)) then rep=[rep,j]; end
      end
    end
    mlibs(rep)=[];
    //--------------------------------
    libs=[];
    if MSDOS then ext='\*.lib',else ext='/*.a',end 
    // removing .a or .ilib sufixs
    for k=1:size(mlibs,'*')
      aa=listfiles(mlibs(k)+ext);
      for j=1:size(aa,'*')
        [pathx,fnamex,extensionx]=fileparts(aa(j));
        libsname= fullfile(pathx,fnamex);
        libs=[libs;libsname];
      end
    end

    // add modelica_libs to the list of directories to be searched for *.h
    //if MSDOS then ext='\*.h',else ext='/*.h',end
    EIncludes=''
    for k=1:size(mlibs,'*')
      EIncludes=EIncludes+'  -I""'+ mlibs(k)+'""';
    end

    E2='';
    for i=1:length(EIncludes)
      if (part(EIncludes,i)=='\') then
        E2=E2+'\';
      end
      E2=E2+part(EIncludes,i);
    end

    //** build shared library with the C code
    files = name;

    // [ok,XX,gui_path,flgcdgen,szclkINTemp,freof,c_atomic_code]=do_compile_superblock42(all_scs_m,numk,atomicflag)

    //## buildnewblock(blknam,files,filestan,filesint,libs,rpat,ldflags,cflags)
    ok = buildnewblock(name,files,'','',libs,TMPDIR,'',E2);
    if ~ok then return, end

  end
endfunction 
 