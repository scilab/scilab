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
function do_analyze_diagram(cpr)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  txt=[];blks=[];
  z_size=diff(cpr.sim.zptr)
  oz_size=diff(cpr.sim.ozptr)
  with_state=find((z_size>0)|(oz_size>0))
  ordclk=cpr.sim.ordclk
  for i=with_state
    k=find(ordclk(:,1)==i)
    if max(ordclk(k,2))<0 then
      txt=[txt;
	   "   Error: Block "+string(i)+" has discrete states but is activated only by continuous activations.";
	   "   The discrete-state is not updated in this case."]
      blks=[blks,i]
    elseif min(ordclk(k,2))<0 then
      txt=[txt;
	   "   Warning: Block "+string(i)+" has discrete states but is activated by continuous activations.";
	   "   This may not be a problem, the discrete-state is updated in some cases."]
      blks=[blks,i]
    end
  end
  if txt<>[] then txt=[txt;" "],end
  
  [xx,unconnected]=setdiff(cpr.sim.outlnk,cpr.sim.inplnk ) 
  for i = unconnected'
    j=max(find(i>=cpr.sim.outptr))
    k=i-cpr.sim.outptr(j)+1
    txt=[txt;
	 "   Warning: The output "+string(k)+" of block "+string(j)+" is not connected."]
    blks=[blks,j]
  end
  
    [xx,unconnected]=setdiff(cpr.sim.inplnk,cpr.sim.outlnk ) 
  for i = unconnected'
    j=max(find(i>=cpr.sim.inpptr))
    k=i-cpr.sim.inpptr(j)+1
    txt=[txt;
	 "   Error: The input "+string(k)+" of block "+string(j)+" is not connected."]
    blks=[blks,j]
  end

  n=size(cpr.sim.funs)
  nact=setdiff(1:n,[ordclk(:,1);cpr.sim.cord(:,1);cpr.sim.iord(:,1)])
  nact=nact(:)
  if nact<>[] then
    nactxt=strcat(string(nact'),",")
    txt=[txt;"  ";"  Warning: Block(s) "+nactxt+" is(are) never activated."]
    blks=[blks,nact]
  end
  

  
  if txt==[] then 
      txt=["Analysis result for diagram "+scs_m.props.title(1)+":";
           "";"Nothing to report."]
  else
      txt=["Analysis result for diagram "+scs_m.props.title(1)+":";
           "";txt;" ";"Do a replot to remove highlight marks."]
  end
  
  ok=hilite_mult_objs(cpr.corinv,blks,txt)
endfunction
