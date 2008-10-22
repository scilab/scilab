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
function ext_ex = ext_exists(varargin)
// Extend the exists instruction so as to be able to check whether a matrix
// or list element exists.
// This is required for the Scipad debugger since if a matrix d exists in Scilab,
// exists(d) is true but exists(d(1)) is false
// Author: François Vogel, 2005
//
// For Scilab 4, there is an underlying problem:
// the variable exists both in 'all' and in 'local' scopes
// since 'all' includes 'local', there is no way to know if
// the variable exists only outside of function ext_exists
// this is the case for the varargin variable for instance
// consequence is that this variable cannot be watched.
// The limitation has been overcome after trunk commit 10635,
// with the introduction of exists("nolocal").

// The scilab 4 workaround is put here for easing
// backporting, it will be removed once Scilab 5 is stabilized.
// We use whereis(scipad)=="utillib"|"scipadlib" as a fast way 
// of discriminating the two cases

  if argn(2) == 0 then error(39); else var = varargin(1); end

////code which works in Scilab 5:
  if whereis(scipad)=="scipadlib" then
    if exists(var,"nolocal") then   //Scilab5
      ext_ex = %t;                  //Scilab5  
    else
      var = string(var);
      if prod(size(var)) > 1 then
        ext_ex = %f;
      else
        var_tok = tokens(var,["(",")"]);
        if prod(size(var_tok)) <> 2 then
          ext_ex = %f;
        else
          if ~exists(var_tok(1),"nolocal") then  //Scilab5
            ext_ex = %f;
          else
            accesstryresult = execstr(var_tok(1) + "(" + var_tok(2) + ")","errcatch");
            if accesstryresult == 0 then
              // this element can be addressed, therefore it exists
              ext_ex = %t;
            else
              // element does not exist. This includes:
              // error   4: undefined variable - can happen when trying to access
              //            d(i), if i is not defined
              // error  21: invalid index (for a matrix or a list)
              // error 117: undefined list element
              ext_ex = %f;
              // the error that occurred in the execstr above has been recorded
              // and must be cleared, otherwise the debugger reports it in the
              // watchwindow (proc checkexecutionerror_bp)
              // this was not needed before revision 13662 of the svn trunk,
              // because the errcatch mode was (erroneously) lost during pause
              lasterror(%t);
            end
          end
        end
      end
    end
  else
  ///for Scilab 4, use this workaround:
    if exists(var) then              //Scilab4
      if ~exists(var,'local') then   //Scilab4
        ext_ex = %t;                 //Scilab4
      else                           //Scilab4
        ext_ex = %f;                 //Scilab4
      end                            //Scilab4  
    else
      var = string(var);
      if prod(size(var)) > 1 then
        ext_ex = %f;
      else
        var_tok = tokens(var,["(",")"]);
        if prod(size(var_tok)) <> 2 then
          ext_ex = %f;
        else
          if ~exists(var_tok(1)) then           //Scilab4
            ext_ex = %f;
          else
            accesstryresult = execstr(var_tok(1) + "(" + var_tok(2) + ")","errcatch");
            if accesstryresult == 0 then
              // this element can be addressed, therefore it exists
              ext_ex = %t;
            else
              // element does not exist. This includes:
              // error   4: undefined variable - can happen when trying to access
              //            d(i), if i is not defined
              // error  21: invalid index (for a matrix or a list)
              // error 117: undefined list element
              ext_ex = %f;
              // the error that occurred in the execstr above has been recorded
              // and must be cleared, otherwise the debugger reports it in the
              // watchwindow (proc checkexecutionerror_bp)
              // this was not needed before revision 13662 of the svn trunk,
              // because the errcatch mode was (erroneously) lost during pause
              lasterror(%t);
            end
          end
        end
      end
    end
  end
endfunction

