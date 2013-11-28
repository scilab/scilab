//  Scicos
//
//  Copyright (C) INRIA - Author : EADS-CCR
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

function t1=cformatlinedp(t ,l,sep)
    //utilitary fonction used to format long C instruction
    //t : a string containing a C instruction
    //l : max line length allowed

    l1=l-2
    t1=[]
    kw=strindex(t," ")
    nw=0
    if kw<>[] then
        if kw(1)==1 then // there is leading blanks
            k1=find(kw(2:$)-kw(1:$-1)<>1)
            if k1==[] then // there is a single blank
                nw=1
            else
                nw=kw(k1(1))
            end
        end
    end
    t=part(t,nw+1:length(t));
    bl=part(" ",ones(1,nw))
    l1=l-nw;first=%t
    while %t
        if length(t)<=l then
            t1=[t1;bl+t],
            return,
        end
        k=strindex(t,sep);
        if k==[] then
            t1=[t1;bl+t],
            return,
        end
        k($+1)=length(t)+1 // positions of the commas
        i=find(k(1:$-1)<=l&k(2:$)>l) //nearest left comma (reltively to l)
        if i==[] then
            i=1,
        end
        t1=[t1;bl+part(t,1:k(i))]
        t=part(t,k(i)+1:length(t))
        if first then
            l1=l1-2;
            bl=bl+"  ";
            first=%f;
        end
    end
endfunction

