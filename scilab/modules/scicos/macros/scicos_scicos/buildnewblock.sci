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

function ok = buildnewblock(blknam,files,filestan,filesint,libs,rpat,ldflags,cflags)
//** buildnewblock : generates Makefiles for
//                   the generated C code of a Scicos block,
//                   compile and link it in Scilab
//
// Input :
//         blknam : a prefix
//
//         files : files to be compiled
//
//         filesint : files to be compiled and included in
//                    the interfacing of standalone code
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
//* 29/06/2009: Serge Steer, use of standard ilib_for_link and
//  tbx_build_gateway instead of specific codes
  
//** check rhs paramaters
  [lhs,rhs] = argn(0);

  //The tests below are unuseful (buildnewblock is always called with 8 args
  if rhs <= 1 then files    = blknam, end
  if rhs <= 2 then filestan = '', end
  if rhs <= 3 then filesint = '', end 
  if rhs <= 4 then libs     = '', end
  if rhs <= 5 then rpat     = TMPDIR, end
  if rhs <= 6 then ldflags  = '', end
  if rhs <= 7 then cflags   = '', end


  //## define a variable to know if with use
  //## a scilab interfacing function for the standalone
  with_int = %f;

  //** adjust path and name of object files
  //   to include in the building process
  if (libs ~= emptystr())&libs ~= [] then
    libs=pathconvert(libs,%f,%t)

  end
  //** otherlibs treatment 
  [ok,libs,for_link]=link_olibs(libs,rpat)
  if ~ok then return,end
  
 
  //** Building the dynamic library for the generated block simulation function
  //** save path 
  oldpath = getcwd();
  chdir(rpat)
  cflags = strcat("-I"+scicos_include_paths(),' ')+' '+cflags
  mprintf(_('Creating dynamic lib for the block simulation function.\n'))
  instr = 'ilib_for_link(blknam,files,libs,''c'',''Makefile'',''loader.sce'',blknam,ldflags,cflags)'
  ierr = execstr(instr,'errcatch');
  chdir(oldpath);
  if ierr<>0 then
    messagebox([_('Sorry problem encountered\n'+..
	       '   when trying to build the block simulation function\n'+..
	       '   dynamic library\n\n');
	     lasterror()],"modal","error")
    return;
  end
  //** rename the loader file
  txt=strsubst(mgetl(rpat+'/loader.sce'),'loader.sce','block_loader.sce')
  mputl(txt,rpat+'/block_loader.sce')
  
  //** unlink if necessary (it is now  done in the loader)
  if %f then
  [a,b]=c_link(blknam);
  while a
    ulink(b);
    [a,b]=c_link(blknam);
  end
  end
  //** Loading the dynamic library for the generated block simulation function
  try
  mprintf(_('Loading dynamic lib for block simulation function.\n'));
  if execstr('exec(rpat+''/block_loader.sce'',-1)','errcatch')<>0 then
    ok=%f;
    messagebox([_('sorry problem encountered\n'+..
	       '   when trying to load the block simulation function\n'+..
	       '   dynamic library\n\n');
	     lasterror()],"modal","error");
    return;
  end

  
  if filesint<>'' then
     //## generate interfacing function of the standalone simulator and it's
     //building an loading tools
     blknamint=filesint(3) //see filesint definition in CodeGeneration_.sci
     mprintf('Creating dynamic lib for standalone simulator gateway\n')
     files=unique([files filesint])
     instr='tbx_build_gateway(blknamint,[blknam,blknamint],files,rpat,[],ldflags,cflags)'

     ierr = execstr(instr,'errcatch');
     if ierr<>0 then
       messagebox([_('sorry problem encountered\n'+..
		  '   when trying to build the standalone simulator gateway\n'+..
		  '   dynamic library\n\n');
		lasterror()],"modal","error");
       return;
     end
     //** rename the loader file
     txt=strsubst(mgetl(rpat+'/loader.sce'),'loader.sce','standalone_loader.sce')
     mputl(txt,rpat+'/standalone_loader.sce')

  end
  //## if succes then with_int becomes true
  with_int=%t;
  
  end
endfunction
