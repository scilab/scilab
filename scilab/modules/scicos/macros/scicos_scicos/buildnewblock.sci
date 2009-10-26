//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>   - 2007
//                      - Allan CORNET - 2008
//                      - Rachid Djenidi
//                      - Simone Mannori
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

function [ok] = buildnewblock(blknam, files, filestan, libs, rpat, ldflags, cflags)

//** buildnewblock : generates Makefiles for
//                   the generated C code of a Scicos block,
//                   compile and link it in Scilab
//
// Input :
//         blknam : a prefix
//
//         files : files to be compiled
//
//         filestan : files to be compiled and included in
//                    the standalone code
//
//         libs : a vector of string of object files
//                to include in the building process
//
//         rpat     : a target directory
//
//         ldflags  : linker flags
//
//         cflags   : C compiler flags
//
// Output :  ok : a flag to say if build is ok
//

  //** check rhs paramaters
  [lhs,rhs] = argn(0);

  if rhs <= 1 then files    = blknam, end
  if rhs <= 2 then filestan = '', end
  if rhs <= 3 then libs     = '', end
  if rhs <= 4 then rpat     = TMPDIR, end
  if rhs <= 5 then ldflags  = '', end
  if rhs <= 6 then cflags   = '', end

  
  if MSDOS then
    //**------------ Windows ----------------------------------------------
    //** on Windows we keep the old way : "hard coded" makefile (.mak)

    //** Try to open a file 
    [fd,ierr] = mopen(rpat+'/'+blknam+'f.f','r') 
  
    if ierr==0 then
      mclose(fd); 
      files = [files,blknam+'f']
    end

    //** adjust path and name of object files
    //** to include in the building process
    if (libs ~= emptystr()) then
    
      // MODELICAC on windows does not support '\' at the end of path
      // TO DO: Fix MODELICAC
      libs = pathconvert(libs,%f,%t);
    end

    //** def make file name
    Makename = rpat+'/'+blknam+'_Makefile';

    //** otherlibs treatment
    [ok, libs, for_link] = link_olibs(libs,rpat)
    if ~ok then
      ok = %f;
      message(["Sorry compiling problem";lasterror()]);
      return;
    end

    //** generate txt of makefile and get right name
    //** of the Makefile file
    [Makename2, txt] = gen_make(blknam,files,filestan,libs,Makename,ldflags,cflags);

    //** write text of the Makefile in the file called Makename
    ierr = execstr("mputl(txt,Makename2)",'errcatch')
    if ierr<>0 then
      message(["Can''t write "+Makename2;lasterror()])
      ok = %f
      return
    end

    //** unlink if necessary
    [a,b] = c_link(blknam);
    while a
      ulink(b);
      [a,b]=c_link(blknam);
    end

    //** save path in case of error in ilib_compile
    oldpath=pwd();

    //** compile Makefile
    ierr=execstr('libn=ilib_compile(''lib''+blknam,Makename)','errcatch')
    if ierr<>0 then
      ok = %f;
      chdir(oldpath);
      message(["sorry compiling problem";lasterror()]);
      return;
    end

    //** link scicos generated code in scilab
    // MODELICAC on windows does not support '\' at the end of path
    // TO DO: Fix MODELICAC
    libn = pathconvert(libn,%f,%t)
    ierr = execstr('libnumber=link(libn)','errcatch')
    ierr = execstr('link(libnumber,blknam,''c'')','errcatch')
    if ierr<>0 then
      ok = %f;
      message(["sorry link problem";lasterror()]);
      return;
    end

    //** generate text of the loader file
    [txt] = gen_loader(blknam,for_link)

    //** write text of the loader in file
    ierr = execstr('mputl(txt,rpat+''/''+blknam+''_loader.sce'')','errcatch')
    if ierr<>0 then
      message(["Can''t write "+blknam+"_loader.sce";lasterror()])
      ok = %f ;
      return
    end
  
  else
    //**------------ Linux / Unix  -------------------------------------------
    //** on Linux Scilab 5 uses an "autotools" based mechanism

    //** the input parameters are : "blknam", "files", "filestan", "libs", "rpat", "ldflags", cflags
    //** "blknam" : a prefix (the name 
    //** "files" : files to be compiled
    //** "filestan" : files to be compiled and included in the standalone code
    //** "libs" : a vector of string of object files "exteral lib(s)" to include in the building process
    //** "rpat" : a target directory
    //** "ldflags"  : linker flags
    //** "cflags"  : C compiler flags

    //** save path in case of error
    oldpath = pwd(); //** current working path 

    //** For this revision (22/03/2008) Scicos Super Block and Modelica are handled in a slight different 
    //** ways, so the code is splitted in two branches. 
    //** We plan to remove these differences - in a second phase - modifing the upper levels routines. 
     
    //** the switch is done looking inside at "filestan" : it is empty for Modelica ;) 
    //** BEWARE: "ilib_for_link" does NOT provide support for "standalone" applications !

    if filestan=="" then
      //** --------------------------- MODELICA ---------------------------------------
      //** Special note for Modelica : this code override some settings 
      //**
      // 
       //** change dir to the TEMPDIR  
       chdir(TMPDIR);

       entry_names = blknam ;
       file_names = files + ".c"  ; //** the new ilib_for_link accept .c extension 
       libs = libs ;
       flag = "c" ; //** "C" entry point / "C" source code  
       makename = "Makelib" ;
       loadername = "loader.sce" ;
       libname = "";
       ldflags = ldflags ;
       //** BEWARE: default directory for source and binary versions are different ! 
       //** if this dir exist means that someone has installed a Scilab binary version 
       if isdir(SCI+"/../../include/scilab/scicos_blocks/") then
          //** Binary version
	  cflags = "-I"+SCI+"/../../include/scilab/scicos_blocks/"; //** look for standard Scicos include 
       else	  
          //** it is a source version 
          cflags = "-I"+SCI+"/modules/scicos_blocks/includes/"; //** look for standard Scicos include
       end

       fflags = ""; //** no additional Fortran flags 
       cc = "";     //** default "C" compiler 
  
       disp("Compile and link Modelica generated C code"); 

       libn = ilib_for_link(entry_names, file_names, libs, flag, makename, loadername, libname, ldflags, cflags, fflags, cc); 

       exec("loader.sce"); //** load the shared library 

    else
     //** -------------- Scicos Super Block Compilation ---------------------------------------
     //** 
       //** change dir to the TEMPDIR  
       chdir(TMPDIR);
 
       //** Copy all the source generated file in the TMPDIR because 
       //** TMPDIR is the "ilib_for_link" default 
       cmd_line = "cp "+rpat+"/*"+" "+"." ;
       unix(cmd_line); 

       //** prepare the parameters 
       //** BEWARE : some settings are "hard coded" 

       entry_names = [files(1), files(1)+"_actuator", files(1)+"_sensor"]; 
       
       file_names = files + ".c"  ;
       libs = libs ;
       flag = "c" ; //** "C" entry point / "C" source code  
       makename = "Makelib" ;
       loadername = "loader.sce" ;
       libname = "";
       ldflags = ldflags ;
       //** BEWARE: default directory for source and binary versions are different ! 
       //** if this dir exist means that someone has installed a binary version 
       if isdir(SCI+"/../../include/scilab/scicos_blocks/") then
          //** Binary version
          //** NB you must add "/../../" because the default path is "<scilab>/share/scilab/" so you need
          //**    to remount two positions :) 
	      //** disp(".bin");
          cflags = "-I"+SCI+"/../../include/scilab/scicos_blocks/"+"  "+..
                   "-I"+SCI+"/../../include/scilab/dynamic_link/"+"  "+..
                   "-I"+SCI+"/../../include/scilab/scicos/"; 
       else	  
          //** it is a source version 
          //** disp(".source");
          cflags = "-I"+SCI+"/modules/scicos_blocks/includes/"+"  "+..
                   "-I"+SCI+"/modules/dynamic_link/includes/"+"  "+..
                   "-I"+SCI+"/modules/scicos/includes/"; 
       end

       fflags = ""; //** no additional Fortran flags 
       cc = "";     //** default "C" compiler 
    
      message(["BEWARE: if you want to reload this simulation,", ...
               "you need to MANUALLY reload - for each Compiled Super Block - ",...
               "the shared library using ''exec loader.sce'' AND",...
               "the interfacing function ''*.sci''              ",...
               "in the corresponding directory ", ...
               "   _BEFORE_ ",...
               "running ''scicos();'' "]);  

      disp("Compile and link Scicos generated C code"); 

       libn = ilib_for_link(entry_names, file_names, libs, flag, makename, loadername, libname, ldflags, cflags, fflags, cc); 
     
      //** This is just a patch because "ilib_for_link" has some issues outside "TMPDIR"
      //** copy the shared library and the loader where the Scicos code generated is 
      //** 
      //** change dir to the permanent directory   
      chdir(rpat);

      cmd_line = "cp "+TMPDIR+"/"+"lib"+files(1)+".*"+" "+"." ;
      unix(cmd_line); 
  
      cmd_line = "cp "+TMPDIR+"/"+"loader.sce"+" "+"." ;
      unix(cmd_line);
      
      exec("loader.sce"); //** load the shared library 

      //**--------------------------------------------------------------------- 
 
  end 


  chdir(oldpath); //** restore the old current working directory 
  
  end 

endfunction
//**------------------------------------------------------------------------------------------------

//** convpathforwin : convert path that only include
//                    a single '\' in a '\\'
//
// Input : path : the path name to be converted
//                a single string.
//
// Output : txt : the path converted
//                a single string.
//
// Author : Alan Layec, 30 Juin 2007
//
function txt = convpathforwin(path)
  txt=''
  for j=1:length(path)
    if part(path,j)=='\' then
      if j==1 then
        if part(path,j+1)<>'\' then
          txt=txt+'\';
        end
      elseif j==length(path) then
        if part(path,j-1)<>'\' then
          txt=txt+'\';
        end
      else
        if part(path,j-1)<>'\' & part(path,j+1)<>'\' then
          txt=txt+'\';
        end
      end
    end
    txt=txt+part(path,j);
  end
endfunction
//**------------------------------------------------------------------------------------------

//** exportlibforlcc : export a lcc.lib file from
//                     an existing dll
//
// Input : libs  : a single string containing path+name
//                 of the (dll) library
//                 for export a lcc.lib file
//
//         rpat  : a target directory for temporary generated files
//
// Output : ok : a boolean variable to say if job has succed
//
// Rmk : the lcc.lib file is generated close to the dll file,
//       in the same directory (<> rpat if it is not informed)
//
// Author : Alan Layec, 1 Jul 2007
//
function [ok] = exportlibforlcc(libs,rpat)

  //** get lhs,rhs nb paramaters
  [lhs,rhs]=argn(0);

  //** extract path, name and extension of libs
  [path,fname,extension]=fileparts(libs);

  //** convert path of libs if needed
  Elibs=convpathforwin(libs)

  //** check rhs paramaters
  if rhs <= 1 then
    rpat = path
    [Erpat,fname,extension]=fileparts(Elibs);
  else
    //** convert path of rpat if needed
    Erpat=convpathforwin(rpat)
  end

  //** run pedumd to extract info from dll file
  //** .lcc file is generated in rpat directory
  ierr=execstr('unix(''pedump /exp """"''+..
               Elibs+''.dll"""" >""""''+..
               Erpat+''\''+fname+''.lcc""""'')','errcatch');

  if ierr<>0 then
    ok=%f
    //add a message here please
    return
  end

  //** generate an .exp file for buildlib of lcc
  //** .exp file is generated in rpat directory
  fw=mopen(rpat+'\'+fname+'.exp',"w");
  fr=mopen(rpat+'\'+fname+'.lcc',"r");
  if (meof(fr) == 0) then
    line=mfscanf(1,fr,"%s");
    mfprintf(fw,"%s\n",line);
   //printf('.');
  end
  while ( meof(fr) == 0)
    line=mfscanf(1,fr,"%s");
    if (line ~= []) then
      mfprintf(fw,"_%s\n",line);
    end
  end
  mclose(fw);
  mclose(fr);

  //** run buildlib and then generate lcc.lib file
  //** .exp is read from rpat
  //** lcc.lib is generated in path (of the dll file)
  command='buildLib ""'+Erpat+'\'+fname+'.exp""'+...
          ' ""'+Elibs+'lcc.lib""'+' ""'+Elibs+'.dll""';
  ierr=execstr('unix(command)','errcatch')
  if ierr<>0 then
    ok=%f
    //add a message here please
    return
  end

endfunction
//**-----------------------------------------------------------------------------------

//** gen_loader : generates the Scilab script for defining the
//                newly created block into Scilab.
//
// input : blknam    : the name of the block to compile
//
// output : SCode : the text of the loader file
//
// Authors : Rachid Djenidi
//           Alan Layec, 28 Juin 2007
//                 "cosmetic comments" (looks like
//                                      Simone's comments)
//                 change calling sequence of gen_loader
//                 rewritte
//
function SCode=gen_loader(blknam,for_link)

  //** check rhs paramaters
  [lhs,rhs]=argn(0);

  if rhs <= 1 then for_link=[], end

  SCode=['//** Exec file used to load the ""compiled"" block into Scilab'
         'blknam='+sci2exp(blknam)+';';
         '//** Get the absolute path of this loader file'
         'DIR=get_absolute_file_path(blknam+''_loader.sce'');'
         '//** Define Makefile name'
         'Makename = DIR+blknam+''_Makefile'';'
         '//** Unlink if necessary'
         '[a,b]=c_link(blknam);'
         'while a'
         '  ulink(b);'
         '  [a,b]=c_link(blknam);'
         'end';
         '//** Run Makefile'
         'libn=ilib_compile('+sci2exp('lib'+blknam)+',Makename);'
         '//** Adjust path name of object files'
         'if MSDOS then'
         '  fileso=strsubst(libn,''/'',''\'');'
         'else'
         '  fileso=strsubst(libn,''\'',''/'');'
         'end';]

  if for_link<>[] then
    SCode=[SCode
           '//** Link otherlibs'
           for_link]
  end

  SCode=[SCode
         ''
         '//** Link block routine in scilab'
         'link(fileso,blknam,''c'');'
         '//** Load the gui function';
         'if fileinfo(DIR+blknam+''_c.sci'')<>[] then'
         '  exec(DIR+blknam+''_c.sci'');'
         'end']

  SCode=[SCode;
         'clear blknam'
         'clear Makename'
         'clear DIR'
         'clear fileso'
         'clear libn'
         '']

endfunction
//**-------------------------------------------------------------------------------------

//** gen_make_lccwin32 : generate text of the Makefile
//              for scicos code generator for
//              lccwin32 compiler
//
// Input : blknam     : name of the library
//         files    : files to be compiled
//         filestan : files to be compiled and included in
//                    the standalone code
//         libs     :  a vector of object files
//                    to include in the building process
//
// Output : T : the text of the makefile
//
// Alan Layec, 29 Juin 2007
// Alan , 29 Juin 2007, common for generation of Scicos
//                      blocks and THE modelica Scicos block
//
function T=gen_make_lccwin32(blknam,files,filestan,libs,ldflags,cflags)

  WSCI=strsubst(SCI,'/','\')

  T=["# generated by builder.sce: Please do not edit this file"
     "# ------------------------------------------------------"
     "SCIDIR            = "+SCI
     "SCIDIR1           = "+WSCI
     "DUMPEXTS          = """+WSCI+"\bin\dumpexts"""
     "SCIIMPLIB         = """+SCIHOME+"\lcclib\LibScilab.lib"""
     "SCILIBS           = """+SCIHOME+"\lcclib\LibScilab.lib"""
     "SCICOSCLIB        = """+SCIHOME+"\lcclib\scicos.lib"""
     "SCICOSFLIB        = """+SCIHOME+"\lcclib\scicos_f.lib"""
     "SCICOS_BLOCKSCLIB = """+SCIHOME+"\lcclib\scicos_blocks.lib"""
     "SCICOS_BLOCKSFLIB = """+SCIHOME+"\lcclib\scicos_blocks_f.lib"""
     "LIBRARY      = lib"+blknam
     "CC           = lcc"
     "LINKER       = lcclnk"
     "OTHERLIBS    = "+libs
     "LINKER_FLAGS = -dll -nounderscores"
     "INCLUDES     = -I"""+WSCI+"\libs\f2c""" 
     "CC_COMMON    = -DWIN32 -DSTRICT -DFORDLL -D__STDC__ $(INCLUDES)"
     "CC_OPTIONS   = $(CC_COMMON)"
     "CFLAGS       = $(CC_OPTIONS) -I"""+WSCI+"\modules\core\includes"" " + .. 
                     "-I"""+WSCI+"\modules\scicos\includes"" " + ..
                     "-I"""+WSCI+"\modules\scicos_blocks\includes"" " + ..
                     "-I"""+WSCI+"\modules\dynamic_link\includes"" " + cflags
     "FFLAGS       = $(FC_OPTIONS) -I"""+WSCI+"\modules\core\includes"" " + ..
                     "-I"""+WSCI+"\modules\scicos\includes"" " + ..
                     "-I"""+WSCI+"\modules\dynamic_link\includes"" " + ..
                     "-I"""+WSCI+"\modules\scicos_blocks\includes"" "
     ""
     "OBJS         = "+strcat(files+'.obj',' ')]

  if filestan<>'' then
    T=[T;
       "OBJSSTAN     = "+strcat(filestan+'.obj',' ')]
  end

  T=[T;
     ""
     "all :: $(LIBRARY).dll"
     ""
     "$(LIBRARY).dll: $(OBJS)"
     ascii(9)+"@echo Creation of dll $(LIBRARY).dll and import lib from ..."
     ascii(9)+"@echo $(OBJS)"
     ascii(9)+"@$(DUMPEXTS) -o ""$(LIBRARY).def"" ""$*"" $(OBJS)"
     ascii(9)+"@$(LINKER) $(LINKER_FLAGS) $(OBJS) $(SCIIMPLIB) $(OTHERLIBS) "+ ..
              " $(SCICOSCLIB) $(SCICOSFLIB) $(SCICOS_BLOCKSCLIB) $(SCICOS_BLOCKSFLIB) " + ..
              " $(SCILAB_LIBS) $*.def -o "+ ..
              " $(LIBRARY).dll"
     ".c.obj:"
     ascii(9)+"@echo ------------- Compile file $< --------------"
     ascii(9)+"$(CC) $(CFLAGS) $< "
     ".f.obj:"
     ascii(9)+"@echo ----------- Compile file $*.f (using f2c) -------------"
     ascii(9)+"@"""+WSCI+"\bin\f2c.exe"" $(FFLAGS) $*.f "
     ascii(9)+"@$(CC) $(CFLAGS) $*.c "
     ascii(9)+"@del $*.c "
     "clean::"
     ascii(9)+"@del *.CKP"
     ascii(9)+"@del *.ln"
     ascii(9)+"@del *.BAK"
     ascii(9)+"@del *.bak"
     ascii(9)+"@del *.def"
     ascii(9)+"@del *.dll"
     ascii(9)+"@del *.exp"
     ascii(9)+"@del *.lib"
     ascii(9)+"@del errs"
     ascii(9)+"@del *~"
     ascii(9)+"@del *.obj"
     ascii(9)+"@del .emacs_*"
     ascii(9)+"@del tags"
     ascii(9)+"@del TAGS"
     ascii(9)+"@del make.log "
     ""
     "distclean:: clean "
     ""]

  if filestan<>'' then
    T=[T;
       "standalone: $(OBJSSTAN) "
        ascii(9)+"$(LINKER) $(LINKER_FLAGS) $(OBJSSTAN)"+...
                 "$(OTHERLIBS) $(SCILIBS) "+ .. 
                 "$(SCICOSCLIB) $(SCICOSFLIB) $(SCICOS_BLOCKSCLIB) $(SCICOS_BLOCKSFLIB) " + ..
                 "/out:standalone.exe "];
  end
endfunction
//**----------------------------------------------------------------------------------------------------

//** gen_make : generate text of the Makefile
//              for scicos code generator
//              That's a wrapper for
//                 gen_make_lccwin32
//                 gen_make_msvc
//                 gen_make_unix
//
// Input : blknam   : name of the Scicos block to compile
//         files    : files to be compiled
//         filestan : files to be compiled and included in
//                    the standalone code
//         libs     :  a vector of object files
//                    to include in the building process
//         Makename : the name of Makefile file
//         ldflags  : linker flags
//         cflags   : C compiler flags
//
// Output : Makename : the name of Makefile file (modified
//                     for the case of win32)
//
//          txt      : the text of the Makefile
//
// Alan Layec, 28 Juin 2007
//
function [Makename,txt]=gen_make(blknam,files,filestan,libs,Makename,ldflags,cflags)

  //** check rhs paramaters
  [lhs,rhs]=argn(0);

  if rhs <= 1 then files    = blknam, end
  if rhs <= 2 then filestan = '', end
  if rhs <= 3 then libs     = '', end
  if rhs <= 4 then Makename = blknam+'_Makefile', end
  if rhs <= 5 then ldflags  = '', end
  if rhs <= 6 then cflags   = '', end

  //** generate Makefile for LCC compilator
 
   if MSDOS then
     if findmsvccompiler() <> 'unknown' then 
       txt=gen_make_msvc(blknam,files,filestan,libs,ldflags,cflags);
       Makename = strsubst(Makename,'/','\')+'.mak';
     else
       if findlcccompiler() <> %F then 
         txt=gen_make_lccwin32(blknam,files,filestan,libs,ldflags,cflags);
         Makename = strsubst(Makename,'/','\')+'.lcc';
       end
     end
   else
   	txt=gen_make_unix(blknam,files,filestan,libs,ldflags,cflags);
   end
    
endfunction
//**---------------------------------------------------------------------------------------------


//** gen_make_unix : generate text of the Makefile
//              for scicos code generator for cc/gcc
//              unix compiler
//
// Input : blknam     : name of the library
//         files    : files to be compiled
//         filestan : files to be compiled and included in
//                    the standalone code
//         libs     : a vector of object files
//                    to include in the building process
//
// Output : T : the text of the makefile
//
// Alan Layec, 28 Juin 2007
// Alan , 29 Juin 2007, common for generation of Scicos
//                      blocks and THE modelica Scicos block
//
function T=gen_make_unix(blknam,files,filestan,libs,ldflags,cflags)

  T=["# generated by builder.sce: Please do not edit this file"
     "# ------------------------------------------------------"
     "SCIDIR       = "+SCI
     "SCILIBS      = $(SCIDIR)/libs/scicos.a $(SCIDIR)/libs/lapack.a "+..
       "$(SCIDIR)/libs/poly.a $(SCIDIR)/libs/calelm.a "+..
       "$(SCIDIR)/libs/blas.a $(SCIDIR)/libs/lapack.a $(SCIDIR)/libs/os_specific.a"
     "LIBRARY      =  lib"+blknam
     "OTHERLIBS    = "+libs
     ""
     "OBJS         = "+strcat(files+'.o',' ')]

  if filestan<>'' then
    T=[T;
       "OBJSSTAN     = "+strcat(filestan+'.o',' ')]
  end

  T=[T;
     "include $(SCIDIR)/Makefile.incl";
     "CFLAGS    = $(CC_OPTIONS) -I$(SCIDIR)/routines/ "+cflags
     "FFLAGS    = $(FC_OPTIONS) -I$(SCIDIR)/routines/"
     "include $(SCIDIR)/config/Makeso.incl"]

  if filestan<>'' then
    T=[T;
       "standalone: $(OBJSSTAN) "
        ascii(9)+"f77 $(FFLAGS) -o $@  $(OBJSSTAN) $(OTHERLIBS) $(SCILIBS)"]
  end

endfunction
//**--------------------------------------------------------------------------------------------

//** gen_make_msvc : generate text of the Makefile
//              for scicos code generator for
//              Ms VS compilers
//
// Input : blknam     : name of the library
//         files    : files to be compiled
//         filestan : files to be compiled and included in
//                    the standalone code
//         libs     :  a vector of object files
//                    to include in the building process
//
// Output : T : the text of the makefile
//
// Alan Layec, 28 Juin 2007
// Alan , 29 Juin 2007, common for generation of Scicos
//                      blocks and THE modelica Scicos block
//
// Allan CORNET, Juillet 2008, DIGITEO
// support build on x64 plateforms with VS
function T=gen_make_msvc(blknam,files,filestan,libs,ldflags,cflags)

  WSCI=strsubst(SCI,'/','\');
  if win64() then
    LINKER_FLAGS_MACHINE =  "/NOLOGO /machine:x64";
    CC_COMMON = "-D__MSC__ -DWIN32 -c -DSTRICT -nologo $(INCLUDES)";
    CC_OPTIONS = "$(CC_COMMON) -Od -Gd -W3";
  else
    LINKER_FLAGS_MACHINE =  "/NOLOGO /machine:ix86";
    CC_COMMON = "-D__MSC__ -DWIN32 -c -DSTRICT -nologo $(INCLUDES)";
    CC_OPTIONS = "$(CC_COMMON) -Od  -GB -Gd -W3";
  end
  

  T=["# generated by builder.sce: Please do not edit this file"
     "# ------------------------------------------------------"
     "SCIDIR       = "+SCI
     "SCIDIR1      = "+WSCI
     "DUMPEXTS     = """+WSCI+"/bin/dumpexts"""
     "SCIIMPLIB    = """+WSCI+"/bin/LibScilab.lib"""
     "SCILIBS      = """+WSCI+"/bin/LibScilab.lib"""
     "SCICOSCLIB      = """+WSCI+"/bin/scicos.lib"""
     "SCICOSFLIB      = """+WSCI+"/bin/scicos_f.lib"""
     "SCICOS_BLOCKSCLIB      = """+WSCI+"/bin/scicos_blocks.lib"""
     "SCICOS_BLOCKSFLIB      = """+WSCI+"/bin/scicos_blocks_f.lib"""
     "LIBRARY      = lib"+blknam
     "CC           = cl"
     "LINKER       = link"
     "OTHERLIBS    = "+libs
     "LINKER_FLAGS = "+LINKER_FLAGS_MACHINE
     "INCLUDES     = -I"""+WSCI+"/libs/f2c"""
     "CC_COMMON    = "+CC_COMMON
     "CC_OPTIONS   = "+CC_OPTIONS
     "CFLAGS       = $(CC_OPTIONS) -I"""+WSCI+"/modules/core/includes"" " + .. 
                     "-I"""+WSCI+"/modules/scicos/includes"" " + ..
                     "-I"""+WSCI+"/modules/scicos_blocks/includes"" " + ..
                     "-I"""+WSCI+"/modules/dynamic_link/includes"" " + cflags
                     
                     
     "FFLAGS       = $(FC_OPTIONS) -I"""+WSCI+"/modules/core/includes"" " + ..
                     "-I"""+WSCI+"/modules/scicos/includes"" " + ..
                     "-I"""+WSCI+"/modules/dynamic_link/includes"" " + ..
                     "-I"""+WSCI+"/modules/scicos_blocks/includes"" "
     ""
     "OBJS         = "+strcat(files+'.obj',' ')]

  if filestan<>'' then
    T=[T;
       "OBJSSTAN     = "+strcat(filestan+'.obj',' ')]
  end

  T=[T;
     ""
     "all :: $(LIBRARY).dll"
     ""
     "$(LIBRARY).dll: $(OBJS)"
     ascii(9)+"@echo Creation of dll $(LIBRARY).dll and import lib from ..."
     ascii(9)+"@echo $(OBJS)"
     ascii(9)+"@$(DUMPEXTS) -o ""$*.def"" ""$*.dll"" $**"
     ascii(9)+"@$(LINKER) $(LINKER_FLAGS) $(OBJS) $(SCIIMPLIB) " + .. 
              "$(SCICOSCLIB) $(SCICOSFLIB) $(SCICOS_BLOCKSCLIB) $(SCICOS_BLOCKSFLIB) $(OTHERLIBS) "+ ..
              "$(SCILAB_LIBS) /nologo /dll /out:""$*.dll"""+...
              " /implib:""$*.lib"" /def:""$*.def"""
     ".c.obj:"
     ascii(9)+"@echo ------------- Compile file $< --------------"
     ascii(9)+"$(CC) $(CFLAGS) $< "
     ".f.obj:"
     ascii(9)+"@echo ----------- Compile file $*.f (using f2c) -------------"
     ascii(9)+"@"""+WSCI+"/bin/f2c.exe"" $(FFLAGS) $*.f "
     ascii(9)+"@$(CC) $(CFLAGS) $*.c "
     ascii(9)+"@del $*.c "
     "clean::"
     ascii(9)+"@del *.CKP"
     ascii(9)+"@del *.ln"
     ascii(9)+"@del *.BAK"
     ascii(9)+"@del *.bak"
     ascii(9)+"@del *.def"
     ascii(9)+"@del *.dll"
     ascii(9)+"@del *.exp"
     ascii(9)+"@del *.lib"
     ascii(9)+"@del errs"
     ascii(9)+"@del *~"
     ascii(9)+"@del *.obj"
     ascii(9)+"@del .emacs_*"
     ascii(9)+"@del tags"
     ascii(9)+"@del TAGS "
     ascii(9)+"@del make.log "
     ""
     "distclean:: clean "
     ""]

  if filestan<>'' then
    T=[T;
       "standalone: $(OBJSSTAN) "
        ascii(9)+"$(LINKER) $(LINKER_FLAGS)  $(OBJSSTAN)"+ ..
                 " $(OTHERLIBS) $(SCILIBS) " + ..
                 "$(SCICOSCLIB) $(SCICOSFLIB) $(SCICOS_BLOCKSCLIB) $(SCICOS_BLOCKSFLIB) " + ..
                 "/out:standalone.exe "];
  end

endfunction
