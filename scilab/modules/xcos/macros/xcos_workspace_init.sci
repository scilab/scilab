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
// See the file ./license.txt
//
//
function xcos_workspace_init()
//preserve all variables of type struct having a "time" and a "value"
//field in temporary binary files 
  path=TMPDIR+"/Workspace/"
  mkdir(path);
  
  
  //prepare the fromworkspace stuff We need to store all variables of
  //type struct which have 2 fields "values" and "time". these variables
  //are stored into Scilab binary files whose name are the variable names
  //and which contain 2 variables "x" and "t"
  //
  prt=funcprot(),funcprot(0)
  varnames = who("get")   ;
  varnames = varnames(1:$-predef()+1);  //** exclude protected variables
  varnames(varnames=='ans')=[];
  for var=varnames'
    v=evstr(var);
    if typeof(v)=='st' then
      fn=fieldnames(v)
      if and(fn==["values";"time"] | fn==["time";"values"]) then
        if execstr('x=v.values','errcatch')==0 then
          if execstr('t=v.time','errcatch') ==0 then
            execstr('save(""'+path+var+''",x,t)')
          end
        end
      end
    end
  end
  funcprot(prt)
endfunction
