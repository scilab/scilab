//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Serge Steer <serge.steer@inria.fr> - 2003
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

function [ok, name, nx, nin, nout, ng, nm, nz] = compile_modelica(fil)

  //++ Check that modelica compiler is available
  //++ Otherwise, give some feedback and quit
  if ~with_modelica_compiler() then
    messagebox(sprintf(gettext("%s: Error: Modelica compiler (MODELICAC) is unavailable."), "compile_modelica"),"modal","error");
    name = ''
    ok = %f, nx = 0, nin = 0, nout = 0, ng = 0, nm = 0, nz = 0; return
  end

  ng    = 0
  fil   = pathconvert(fil, %f, %t)
  // MODELICAC on windows does not support '\' at the end of path
  // TO DO: Fix MODELICAC
  if MSDOS then
    mlibs = pathconvert(modelica_libs, %f, %t);
  else
    mlibs = pathconvert(modelica_libs, %t, %t);
  end

  name = basename(fil)
  path = strsubst(stripblanks(fil), name + '.mo', '')

  JacobianEnabled = %t
  if JacobianEnabled then
    JAC = ' -jac '
  else
    JAC = ' '
  end

  // do not update C code if needcompile == 0
  // this allows C code modifications for debugging purposes
  updateC = needcompile <> 0 | fileinfo(path + name + '.c') == []

  if updateC then

    //++ Define some platform-dependent variables and filenames
    if MSDOS
      modelicac      = getmodelicacpath() + 'modelicac.exe'
      translator_bin = getmodelicacpath() + 'translator.exe'
      modelicac_err  = 'Wmodelicac.err'
      translator_err = 'Wtranslator.err'
      unix_err       = 'Wunix.err'
    else
      modelicac      = getmodelicacpath() + 'modelicac'
      translator_bin = getmodelicacpath() + 'translator'
      modelicac_err  = 'Lmodelicac.err'
      translator_err = 'Ltranslator.err'
      unix_err       = 'Lunix.err'
    end

    FlatName = fil

    if MSDOS then
      //++ Windows platforms: enclose paths in double quotes when they contain spaces
      if strindex(modelicac, ' ') <> [] then
        modelicac = '""' + modelicac + '""'
      end
      modelicac = modelicac + strcat(' -L ""' + mlibs + '""')
    else
      modelicac = modelicac + strcat(' -L '+ mlibs)
    end

    instr = modelicac + ' ' + FlatName + ' -o ' + path + name + '.c ' + JAC + ' > ' + TMPDIR + filesep() + modelicac_err
    if MSDOS
      //++ WINDOWS PLATFORMS: Put the instruction in a batch file instead of
      //++ executing it directly
      mputl(instr, path + 'genc.bat');
      instr = path + 'genc.bat'
    end

    if fileinfo(SCI + '/bin/translator') <> [] then
      OUTM = unix(instr) <> 0; // in order to mask the message in the Scilab windows
    else
      OUTM = execstr('unix_s(instr);', 'errcatch') <> 0
    end

    if OUTM then // if_modelicac_fails_then_use_translator
      MSG1 = mgetl(TMPDIR + filesep() + modelicac_err)
      if fileinfo(fullfile(SCI, 'bin', translator_bin)) <> [] then // if_translator_exists
        translator = pathconvert(SCI + filesep() + 'bin' + filesep() + translator_bin, %t, %t)
        ext = filesep() + '*.mo'
        molibs = []
        for k = 1:size(mlibs,'*')
          molibs = [molibs ; listfiles(mlibs(k) + ext)]
        end
        txt = []
        for k = 1:size(molibs, '*')
          [pathx, fnamex, extensionx] = fileparts(molibs(k))
          if (fnamex <> 'MYMOPACKAGE') then
            txt=[txt ; mgetl(molibs(k))]
          end
        end
        mputl(txt, TMPDIR + '/MYMOPACKAGE.mo');
        // all Modelica files found in "modelica_libs" directories are
        // merged into the temporary file  "MYMOPACKAGE.mo". This is done
        // because in Windows the length of the command line is limited.
        FlatName   = path + name + '_flattened.mo'
        translator = translator + strcat(' -lib '+ TMPDIR + '/MYMOPACKAGE.mo')
        //translator = translator + strcat(' -lib ' + molibs)
        instr = translator + ' -lib ' + fil + ' -o ' + FlatName + " -command ""' + name + ' x;"" > ' + TMPDIR + filesep() + translator_err
        if MSDOS
          mputl(instr, path + 'gent.bat');
          instr = path + 'gent.bat'
        end
        if execstr('unix_s(instr)', 'errcatch') <> 0 then
          MSG2 = mgetl(TMPDIR + filesep() + translator_err)
          messagebox(['-------Modelica compiler error:-------'; ..
                     MSG1; ..
                     '-------Modelica translator error:-------'; ..
                     MSG2; ..
                     'Please read the error message in the Scilab window'],"modal","error");
          ok = %f, nx = 0, nin = 0, nout = 0, ng = 0, nm = 0, nz = 0; return
        end
        instr = modelicac + ' ' + FlatName + ' -o ' + path + name + '.c ' + JAC + ' > ' + TMPDIR + filesep() + unix_err
        if execstr('unix_s(instr)', 'errcatch') <> 0 then
          MSG3 = mgetl(TMPDIR + filesep() + unix_err)
          messagebox(['-------Modelica compiler error (without the translator):-------'; ..
                     MSG1; ..
                     '-------Modelica compiler error (with the translator):-------'; ..
                     MSG3; ..
                     'Please read the error message in the Scilab window'],"modal","error");
          ok = %f, nx = 0, nin = 0, nout = 0, ng = 0, nm = 0, nz = 0; return
        else
          mprintf('   Flat modelica code generated at ' + FlatName + '\n')
        end
      else // if_translator_exists
        messagebox(['-------Modelica compiler error (without the translator):-------'; ..
                   MSG1; ..
                   'Please read the error message in the Scilab window'; ..
                   ' '; ..
                   'Please install the Modelica translator (available at www.scicos.org) in ""SCI' + filesep() + 'bin"" and try again'],"modal","error");
        ok = %f, nx = 0, nin = 0, nout = 0, ng = 0, nm = 0, nz = 0; return
      end // if_translator_exists
    end // if_modelicac_fails_then_use_translator

    mprintf('   C code generated at ' + path + name + '.c\n')
  end

  Cfile = path + name + '.c'

  if MSDOS then
    Ofile = path + name + '.obj'
  else
    Ofile = path + name + '.o'
  end

  // get the generated block properties
  [nx, nin, nout, ng, nm, nz] = analyze_c_code(mgetl(Cfile))

  mprintf('\n\r Modelica blocks are reduced to a block with:')
  mprintf('\n\r Number of continuous time states: %d',nx)
  mprintf('\n\r Number of discrete time states  : %d',nz)
  mprintf('\n\r Number of zero-crossing surfaces: %d',ng)
  mprintf('\n\r Number of modes  : %d',nm)
  mprintf('\n\r Number of inputs : %d',nin)
  mprintf('\n\r Number of outputs: %d',nout)
  mprintf('\n\r ')

  // below newest(Cfile, Ofile) is used instead of  updateC in case where
  // Cfile has been manually modified (debug, ...)
  if newest(Cfile, Ofile) == 1 then
    //unlink if necessary
    [a,b] = c_link(name); while a; ulink(b); [a,b] = c_link(name); end
    //  build the list of external functions libraries

    // remove repreated directories from mlibs
    rep = []
    for k = 1:size(mlibs,'*')
      for j = k+1:size(mlibs,'*')
        if stripblanks(mlibs(k)) == stripblanks(mlibs(j)) then rep = [rep,j]; end
      end
    end
    mlibs(rep) = []
    //--------------------------------
    libs = []
    if MSDOS then ext = '\*.lib', else ext='/*.a', end
    // removing .a or .lib sufixs
    for k = 1:size(mlibs,'*')
      aa = listfiles(mlibs(k) + ext)
      for j = 1:size(aa,'*')
        [pathx, fnamex, extensionx] = fileparts(aa(j))
        libsname = fullfile(pathx, fnamex)
        libs = [libs; libsname]
      end
    end
    // add modelica_libs to the list of directories to be searched for *.h
    // if MSDOS then ext='\*.h',else ext='/*.h',end
    EIncludes = ''
    for k = 1:size(mlibs,'*')
      EIncludes = EIncludes + '  -I""' + mlibs(k)+ '""'
    end
    E2 = ''
    for i = 1:length(EIncludes)
      if (part(EIncludes, i) == '\') then
        E2 = E2 + '\'
      end
      E2 = E2 + part(EIncludes, i)
    end

    //** build shared library with the C code

    // files=name+'.o';Make=path+'Make'+name;loader=path+name+'.sce'
    // ierr=execstr('libn=ilib_for_link(name,files,libs,''c'',Make,loader,'''','''',E2)','errcatch')
    // if ierr<>0 then
    //   ok=%f;messagebox(['sorry compilation problem';lasterror()],"modal","error");
    //   return
    // end

    // executing loader file
    // if execstr('exec(loader); ','errcatch')<>0 then
    //   ok=%f;
    //   messagebox(['Problem while linking generated code';lasterror()],"modal","error");
    //   return
    // end

    files = name

    ok = buildnewblock(name, files, '', libs, TMPDIR, '', E2)

    if ~ok then
      return
    end
  end
endfunction

//------------------------------------------------------------------------------

function [nx, nin, nout, ng, nm, nz] = analyze_c_code(txt)

  match = 'number of variables = '
  T     = txt(grep(txt(1:10), match)) // look for match in the first 10 lines
  nx    = evstr(strsubst(T, match, ''))

  match = 'number of inputs = '
  T     = txt(grep(txt(1:10), match)) // look for match in the first 10 lines
  nin   = evstr(strsubst(T, match, ''))

  match = 'number of outputs = '
  T     = txt(grep(txt(1:10), match)) // look for match in the first 10 lines
  nout  = evstr(strsubst(T, match, ''))

  match = 'number of zero-crossings = '
  T     = txt(grep(txt(1:10), match)) // look for match in the first 10 lines
  ng    = evstr(strsubst(T, match ,''))

  match = 'number of modes = '
  T     = txt(grep(txt(1:10), match)) // look for match in the first 10 lines
  nm    = evstr(strsubst(T, match, ''))

  match = 'number of discrete variables = '
  T     = txt(grep(txt(1:10), match)) // look for match in the first 10 lines
  nz    = evstr(strsubst(T, match, ''))

endfunction

