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
//**
//------------------------------------------------------------------------
//** link_olibs   : links otherlibs in scilab
//                  for scicos C generated block
//
// Input : libs   : a matrix of string containing path+name
//                 of the libraries
//
//         rpat   : a target directory for temporary generated files
//
// Output : ok    : a boolean variable to say if job has succed
//          libs  : a matrix of string containing path+name
//                  of the libraries
//          for_link : a vector of strings with link cmd
//                     for exec or for loader.sce
//
// Author : Alan Layec, 1 Jul 2007
//
function [ok,libs,for_link] = link_olibs(libs,rpat)

  //** get lhs,rhs nb paramaters
  [lhs,rhs]=argn(0);

  //** decl and set local variables
  ok=%t
  x=''
  xlibs=[]
  for_link=[]

  //** get out from this function if
  //   there is nothing to do
  if libs=='' | libs==[] then return, end

  //** LCC
  if with_lcc()==%T then
    //** add lcc.lib
    //   for compatibility with dll of
    //   msvc
    libs=libs(:)';
    for x=libs
      //** extract path, name and extension of libs
      [path,fname,extension]=fileparts(x);
      if rhs <= 1 then
        rpat = path
      end
      if (extension == '') then
        //** search dll
        if fileinfo(x+'.dll')<>[] then
          if fileinfo(x+'lcc.lib')==[] then
            //** export lcc.lib
            x_message(['I will try to export a '+x+'lcc.lib']);
            ok=exportlibforlcc(x,rpat)
            if ~ok then
              x_message(['Can''t export a '+path+fname+'lcc.lib';
                         'Please try to do your own lcc.lib file with';
                         'the xx scilab function or change the path';
                         'of your library '+x+'.dll']);
              ok=%f;
              return
            end
          end
          for_link=[for_link;x+'.dll']
          link(for_link($));
          xlibs=[xlibs;x+'lcc.lib']

        //** search DLL
        elseif fileinfo(x+'.DLL')<>[] then
          if fileinfo(x+'lcc.lib')==[] then
            //** export lcc.lib
            x_message(['I will try to export a '+x+'lcc.lib']);
            ok=exportlibforlcc(x,rpat)
            if ~ok then
              x_message(['Can''t export a '+path+fname+'lcc.lib';
                         'Please try to do your own lcc.lib file with';
                         'the xx scilab function or change the path';
                         'of your library '+x+'.dll']);
              ok=%f;
              return
            end
          end
          for_link=[for_link;x+'.DLL']
          link(for_link($));
          xlibs=[xlibs;x+'lcc.lib']

        else
          //** no extension
          //   no .dll exists
          //   do something here please ?
          ok=%f
          pause
        end
      elseif fileinfo(x)==[] then
        x_message(['Can''t include '+x;
                   'That file doesn''t exist';
                   lasterror()])
        ok=%f
        return
      //** extension assume that user know what he does
      else
        //** compiled object (.obj)
        //** compiled object doesn't need to be linked
        if extension=='.obj' | extension=='.OBJ'  then
          xlibs=[xlibs;x]
        //** library (.dll)
        elseif extension=='.dll' | extension=='.DLL' then
          for_link=[for_link;x]
          link(for_link($));
          if fileinfo(x+'lcc.lib')==[] then
            //** export lcc.lib
            x_message(['I will try to export a '+x+'lcc.lib']);
            ok=exportlibforlcc(path+fname,rpat)
            if ~ok then
              x_message(['Can''t export a '+path+fname+'lcc.lib';
                         'Please try to do your own lcc.lib file with';
                         'the xx scilab function or change the path';
                         'of your library '+x+'.dll']);
              ok=%f;
              return
            end
          end
          xlibs=[xlibs;path+fname+'lcc.lib']

        //** library (.lib)
        elseif extension=='.lib' | extension=='.ilib' then
          if fileinfo(path+fname+'.dll')<>[] then
            for_link=[for_link;path+fname+'.dll']
            link(for_link($));
          elseif fileinfo(path+fname+'.DLL')<>[] then
            for_link=[for_link;path+fname+'.DLL']
            link(for_link($));
          else
            //link(x);
            x_message(['I don''t know what to do !';
                      'Please report to alan.layec@inria.fr';])
            ok=%f
            pause
          end
          xlibs=[xlibs;x]
        else
          //link(x);
          x_message(['I don''t know what to do !';
                     'Please report to alan.layec@inria.fr';])
          ok=%f
          pause
        end
      end
    end

  //** MSVC
  elseif getenv('WIN32','NO')=='OK' then
    //** add .lib or .ilib
    libs=libs(:)';
    for x=libs
      [path,fname,extension]=fileparts(x);
      if (extension == '') then
        //** search ilib
        if fileinfo(x+'.ilib')<>[] then
          //** search dll
          if fileinfo(x+'.dll')<>[] then
            for_link=[for_link;x+'.dll']
            link(for_link($));
          //** search DLL
          elseif fileinfo(x+'.DLL')<>[] then
            for_link=[for_link;x+'.DLL']
            link(for_link($));
          //** no .dll, .DLL
          else
            x_message(['I cant''t find a dll !';
                       'Please report to alan.layec@inria.fr';])
            ok=%f
            pause
          end
          xlibs=[xlibs;x+'.ilib']
        //** search lib
        elseif fileinfo(x+'.lib')<>[] then
          //** search dll
          if fileinfo(x+'.dll')<>[] then
            for_link=[for_link;x+'.dll']
            link(for_link($));
          //** search DLL
          elseif fileinfo(x+'.DLL')<>[] then
            for_link=[for_link;x+'.DLL']
            link(for_link($));
          //** no .dll, .DLL
          else
            x_message(['I cant''t find a dll !';
                       'Please report to alan.layec@inria.fr';])
            ok=%f
            pause
          end
          xlibs=[xlibs;x+'.lib']
        else
          //** no extension
          //   no .lib, no .ilib exists
          //   do something here please ?
          x_message(['I don''t know what to do !';
                     'Please report to alan.layec@inria.fr';])
          ok=%f
          pause
        end
      elseif fileinfo(x)==[] then
        x_message(['Can''t include '+x;
                   'That file doesn''t exist';
                   lasterror()])
        ok=%f
        return
      //** extension assume that user know what he does
      else
        //** compiled object (.obj)
        //** compiled object doesn't need to be linked
        if extension=='.obj' | extension=='.OBJ'  then
          xlibs=[xlibs;x]
        //** library (.dll)
        elseif extension=='.dll' | extension=='.DLL' then
          for_link=[for_link;x]
          link(for_link($));
          if fileinfo(path+fname+'.ilib')<> [] then
            xlibs=[xlibs;path+fname+'.ilib']
          elseif fileinfo(path+fname+'.lib')<> [] then
            xlibs=[xlibs;path+fname+'.lib']
          else
            //link(x);
            x_message(['I don''t know what to do !';
                      'Please report to alan.layec@inria.fr';])
            ok=%f
            pause
          end
        //** library (.lib)
        elseif extension=='.lib' | extension=='.ilib' then
          if fileinfo(path+fname+'.dll')<>[] then
            for_link=[for_link;path+fname+'.dll']
            link(for_link($));
          elseif fileinfo(path+fname+'.DLL')<>[] then
            for_link=[for_link;path+fname+'.DLL']
            link(for_link($));
          else
            //link(x);
            x_message(['I don''t know what to do !';
                      'Please report to alan.layec@inria.fr';])
            ok=%f
            pause
          end
          xlibs=[xlibs;x]
        else
          //link(x);
          x_message(['I don''t know what to do !';
                     'Please report to alan.layec@inria.fr';])
          ok=%f
          pause
        end
      end
    end

  //** Unix
  else
    //** add .a
    //   for compatibility test if we have already a .a
    libs=libs(:)';
    for x=libs
      [path,fname,extension]=fileparts(x);
      //** no extension. Assume that's a so library
      if (extension == '') then
       if fileinfo(path+fname+'.so')<>[] then
        for_link=[for_link;x+'.so']
        link(for_link($));
       elseif fileinfo(path+fname+'.SO')<>[] then
        for_link=[for_link;x+'.SO']
        link(for_link($));
       else
         //link(x);
         x_message(['I don''t know what to do !';
                    'Please report to alan.layec@inria.fr';])
         ok=%f
         pause
       end
       if fileinfo(x+'.a')<>[] then
         xlibs=[xlibs;x+'.a']
       elseif fileinfo(x+'.A')<>[] then
         xlibs=[xlibs;x+'.A']
       else
         //link(x);
         x_message(['I don''t know what to do !';
                    'Please report to alan.layec@inria.fr';])
         ok=%f
         pause
       end
      elseif fileinfo(x)==[] then
        x_message(['Can''t include '+x;
                   'That file doesn''t exist';
                   lasterror()])
        ok=%f
        return
      //** extension assume that user know what he does
      else
        //** compiled object (.o)
        //** compiled object doesn't need to be linked
        if extension=='.o' | extension=='.O'  then
          xlibs=[xlibs;x]
        //** library (.so)
        elseif extension=='.so' | extension=='.SO' then
          for_link=[for_link;x]
          link(for_link($));
          if fileinfo(path+fname+'.a')<> [] then
            xlibs=[xlibs;path+fname+'.a']
          elseif fileinfo(path+fname+'.A')<> [] then
            xlibs=[xlibs;path+fname+'.A']
          else
            //link(x);
            x_message(['I don''t know what to do !';
                      'Please report to alan.layec@inria.fr';])
            ok=%f
            pause
          end
        //** library (.a)
        elseif extension=='.a' | extension=='.A' then
          if fileinfo(path+fname+'.so')<>[] then
            for_link=[for_link;path+fname+'.so']
            link(for_link($));
          elseif fileinfo(path+fname+'.SO')<>[] then
            for_link=[for_link;path+fname+'.SO']
            link(for_link($));
          else
            //link(x);
            x_message(['I don''t know what to do !';
                      'Please report to alan.layec@inria.fr';])
            ok=%f
            pause
          end
          xlibs=[xlibs;x]
        else
          //link(x);
          x_message(['I don''t know what to do !';
                    'Please report to alan.layec@inria.fr';])
          ok=%f
          pause
        end
      end
    end
  end

  //** add double quote for include in
  //   Makefile
  libs=xlibs
  if MSDOS then
      libs='""'+libs+'""'
   else
     libs=''''+libs+''''
   end

  //** return link cmd for for_link
  if for_link <> [] then
    for_link = 'link(""'+for_link+'"");';
  end

  //** concatenate libs for Makefile
  if size(libs,1)<>1 then
    libs = strcat(libs,' ')
  end

endfunction
