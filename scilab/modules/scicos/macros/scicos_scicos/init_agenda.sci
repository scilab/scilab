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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [tevts,evtspt,pointi]=init_agenda(initexe,clkptr)

    // order initial firing events in chronological order.
    nblk=size(clkptr,1)-1
    timevec=initexe(:,3)
    if timevec<>[] then
        [timevec,indtime]=gsort(-timevec)
        initexe=initexe(indtime,:)
    else
        initexe=[]
    end
    timevec=[]
    //initialize agenda
    ninit=size(initexe,1)
    pointi=0
    nevts=clkptr(nblk+1)-1 //time events agenda size
    tevts=0*ones(nevts,1)
    if initexe<>[] then
        tevts(clkptr(initexe(:,1))+initexe(:,2)-1)=initexe(:,3)
    end
    evtspt=-ones(nevts,1)

    if ninit>0 then
        pointi=clkptr(initexe(1,1))+initexe(1,2)-1;
        evtspt(pointi)=0
    end
    if ninit>1 then
        evtspt(clkptr(initexe(1:ninit-1,1))+initexe(1:ninit-1,2)-1)=..
        clkptr(initexe(2:ninit,1))+initexe(2:ninit,2)-1;
        evtspt(clkptr(initexe(ninit,1))+initexe(ninit,2)-1)=0;
    end
endfunction
