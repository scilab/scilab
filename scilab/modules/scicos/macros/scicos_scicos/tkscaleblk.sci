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

function block=tkscaleblk(block,flag)
  blknb=string(curblock())
  if flag==4 then
    cur=%cpr.corinv(curblock())
    if size(cur,'*')==1 then // open widget only if the block 
                             // is in main Scicos editor window
      o=scs_m.objs(cur).graphics.orig;
      sz=scs_m.objs(cur).graphics.sz
      pos=point2pixel(1000,o)
      pos(1)=pos(1)+width2pixel(1000,sz(1)) // widget position 
      geom='wm geometry $w +'+string(pos(1))+'+'+ string(pos(2));
      titled=block.label
      if titled==[] then titled="TK source",end
      tit='wm title $w Scale'+blknb // write block label
      bounds=block.rpar(1:2)
      bnds='-from '+string(bounds(1))+' -to '+string(bounds(2))
      cmd='-command ""f'+blknb+' $w.frame.scale""'
      lab='-label ""'+titled+'""';
      L='-length 100'
      I='-tickinterval '+string((bounds(2)-bounds(1))/4)
      scale=strcat(['scale $w.frame.scale -orient vertical',..
                    lab,bnds,cmd,L,I],' ')
      initial=mean(bounds) // initial value is the mean
      txt=['set w .vscale'+blknb;
           'set y'+blknb+' 0';
           'catch {destroy $w}';
           'toplevel $w';
           tit
           geom
           'frame $w.frame -borderwidth 10';
           'pack $w.frame';
           scale
           'frame $w.frame.right -borderwidth 15';
           'pack $w.frame.scale -side left -anchor ne';
           '$w.frame.scale set '+string(initial);
           'proc f'+blknb+' {w height} {global y'+blknb+';set y'+blknb+' $height}'
          ];
      TCL_EvalStr(txt) // call TCL interpretor to create widget
      block.outptr(1)=mean(block.rpar(1:2))/block.rpar(3);
    end
  elseif flag==1 then // evaluate output during simulation
    block.outptr(1)=evstr(TCL_GetVar('y'+blknb))/block.rpar(3);
  end
endfunction
///\withPrompt{}
