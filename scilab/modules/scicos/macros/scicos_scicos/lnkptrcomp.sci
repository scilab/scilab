//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
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

function [lnksz,lnktyp]=lnkptrcomp(bllst,inpptr,outptr,inplnk,outlnk)

    lnkbsz=[];ptlnk=[];lnksz=[];lnktyp=[];
    for blkout=1:length(bllst)
        for portout=1:outptr(blkout+1)-outptr(blkout)
            ptlnk=outlnk(outptr(blkout)+portout-1)
            lnkbsz(outlnk(outptr(blkout)+portout-1))=bllst(blkout).out(portout);
            lnksz(ptlnk,1)=bllst(blkout).out(portout);
            lnksz(ptlnk,2)=bllst(blkout).out2(portout);
            lnktyp(ptlnk)=bllst(blkout).outtyp(portout);
        end
    end

    //27/09/07 Alan's patch : do test here for unconnected inputs
    if inplnk<>[] | outlnk<>[] then
        unco=find(inplnk>max(outlnk))
        for j=unco
            m=max(find(inpptr<=j))
            n=j-inpptr(m)+1
            lnksz($+1,1)=bllst(m).in(n);
            lnksz($,2)=bllst(m).in2(n);
            lnktyp($+1)=bllst(m).intyp(n);
        end
    end

    //lnkptr=cumsum([1;lnkbsz])
endfunction

