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

function CtrlSelect_()
  Cmenu=[];
  if windows(find(%win==windows(:,2)),1)==100000 then
    %pt=[]
    return
  end
  kc=find(%win==windows(:,2))
  if kc==[] then
    message('This window is not an active scicos window')
    %pt=[];return
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    palette=palettes(kpal)
    k=getobj(palette,%pt)
  elseif %win==curwin then //click dans la fenetre courante
    k=getobj(scs_m,%pt)  
  elseif slevel>1 then
    execstr('k=getobj(scs_m_'+string(windows(kc,1))+',%pt)')
   else
    message('This window is not an active scicos window')
    %pt=[];return
  end   
 
  if k<>[] then
    ki=find(k==Select(:,1)&%win==Select(:,2))
    if Select<>[] & Select(1,2)<>%win then
      Select=[]  
    end
    if ki==[] then
      Select=[Select;[k,%win]];
      %pt=[];return
    else 
      Select(ki,:)=[];
      %pt=[];return
    end
  else
    %pt=[];return
  end
endfunction


