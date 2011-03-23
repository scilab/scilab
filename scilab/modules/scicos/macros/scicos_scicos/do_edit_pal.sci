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

function [scicos_palnew]=do_edit_pal(scicos_pal)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

scicos_palnew=[];
txt=scicos_pal(:,2);
txtnew=txt

//## set param of scstxtedit
ptxtedit= scicos_txtedit(clos = 0,...
           typ  = "palette",...
           head = 'Edit the list of palettes below');

while %t

  [txtnew,Quit] = scstxtedit(txtnew,ptxtedit);

  if ptxtedit.clos==1 then
     break;
  end

  if txtnew==[]|Quit==1 then
    scicos_palnew=[];
    break;
  end

  scicos_palnew=[]

  for i=1:size(txtnew,1)
    txtnew(i)=stripblanks(txtnew(i))
    l=length(txtnew(i))
    if l<>0 then
      k=strindex(txtnew(i),'/');if k==[] then k=0;end
      h=strindex(txtnew(i),'\');if h==[] then h=0;end
      m=max([k,h]); 
      n=strindex(txtnew(i),'.cosf')
      if n==[] then n=strindex(txtnew(i),'.cos');end
      if n==[] then
        messagebox('All files must end with .cos or .cosf','modal')
        scicos_palnew=[]
        break
      end
      a=part(txtnew(i),m+1:n-1);
      scicos_palnew=[scicos_palnew;[a,txtnew(i)]];
      ptxtedit.clos = 1
    end
  end
end
endfunction
