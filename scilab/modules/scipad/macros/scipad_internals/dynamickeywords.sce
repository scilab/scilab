//  Scipad - programmer's editor and debugger for Scilab
//
//  Copyright (C) 2002 -      INRIA, Matthieu Philippe
//  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
//  Copyright (C) 2004-2008 - Francois Vogel
//
//  Localization files ( in tcl/msg_files/) are copyright of the 
//  individual authors, listed in the header of each file
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
// See the file scipad/license.txt
//
function dynamickeywords()
// wrapped as function to have all variables local
  function cset=lineform(keywordlist)
     keywordlist=sort(keywordlist)
     initial=gsort(unique(part(keywordlist,1)),"r","i")
     cset=[]
     for i=1:size(initial,1)
         cset(i)=strcat(keywordlist(part(keywordlist,1)==initial(i))," ")
     end
  endfunction


  function setscipadwords(wset,wtype)
    // checks that scipad interp exists
    if TCL_ExistInterp('scipad') then
      lp=lineform(wset);
      TCL_EvalStr("set chset(scilab."+wtype+") {}","scipad")
      for i=1:size(lp,1)
        initial=part(lp(i),1);
        TCL_EvalStr("append chset(scilab."+wtype+") """+..
                 initial+"""","scipad")
        TCL_EvalStr("set words(scilab."+wtype+"."+initial+") """+..
                 lp(i)+"""","scipad")
      end
    end
  endfunction

  //are we in scilab4 or scilab5?
  if listfiles(SCI+"/modules/scipad/")<>[] then scilab5=%t; else scilab5=%f; end

  //commands and primitives
  [primitives,commands]=what();
  setscipadwords(commands,"command")
  setscipadwords(primitives,"intfun")

  //predefined variables
  names=who("get"); p=names(($-predef())+1:$);
  setscipadwords(p,"predef")

  //library functions
  libfun=[]; libvar=[];
  for i=1:size(names,1)
    if type(eval(names(i)))==14 then
       libvar=[libvar;names(i)];
       libstring=string(eval(names(i)));
       if or(libstring=="") then
         warning(" suspect empty function name found in "+names(i)+" (BUG #2338)")
         libstring=libstring(libstring<>"")
       end
       libfun=[libfun;libstring(2:$)];
    end
  end

  setscipadwords(libfun,"libfun")

  if exists("scicos") then  // was once %scicos==%t -- which test is stabler?
    if scilab5 then
      scicosdir=SCI+"/modules/scicos/macros/scicos_scicos";
      //scicos basic functions: read the lib
      [l,s,b]=listvarinfile(scicosdir+"/lib");
      load(scicosdir+"/lib");
    else 
      scicosdir=SCI+"/macros/";
      //scicos basic functions: read the lib
      [l,s,b]=listvarinfile(scicosdir+"scicos/lib");
      load(scicosdir+"scicos/lib");
    end
    
    //scicos basic functions: read the lib
    n=string(eval(l)); scicosfun=(n(2:$));
    execstr("clear "+l);


    //scicos palettes: read each lib
    scicosblocks=[];
    if scilab5 then
      scicos_blocksdir = SCI+"/modules/scicos_blocks/macros";
      subdirs=listfiles(scicos_blocksdir);
      for i=1:size(subdirs,"r")
        blocklib=scicos_blocksdir+"/"+subdirs(i)+"/lib";
        if fileinfo(blocklib)<>[] then
          [l,s,b]=listvarinfile(blocklib);
          load(blocklib);
          n=string(eval(l)); scicosblocks=[scicosblocks;(n(2:$))];
          execstr("clear "+l);
        end
      end
    else
      subdirs=listfiles(scicosdir+"/scicos_blocks");
        for i=1:size(subdirs,"r")
          blocklib=scicosdir+"/scicos_blocks/"+subdirs(i)+"/lib";
          if fileinfo(blocklib)<>[] then
            [l,s,b]=listvarinfile(blocklib);
            load(blocklib);
            n=string(eval(l)); scicosblocks=[scicosblocks;(n(2:$))];
            execstr("clear "+l);
          end
      end
    end

    setscipadwords([scicosfun;scicosblocks],"scicos")
  end

  //TCL_EvalStr("tk_messageBox -message $words(scilab.predef.%)","scipad")

endfunction

dynamickeywords()

clear dynamickeywords
