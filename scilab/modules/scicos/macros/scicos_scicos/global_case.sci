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

function [links_table,sco_mat,ok]=global_case(links_table,sco_mat)
    [s,k]=gsort(sco_mat(:,2))
    sco_mat=sco_mat(k,:);
    index=find((sco_mat(:,2)=="1")&(sco_mat(:,4)=="3"))
    if index<> [] then
        for i=1:size(index,"*")
            indexx=find((sco_mat(index(:),3)==sco_mat(index(i),3))&(sco_mat(index(:),5)==sco_mat(index(i),5)))
            if size(indexx,"*")>1 then
                messagebox(["Error In Compilation";"You cannot have multiple global GOTO with the same tag"],"modal")
                ok=%f
                return
            end
        end
        for i=index
            index1=find((sco_mat(:,2)=="-1")&(sco_mat(:,3)==sco_mat(i,3))&(sco_mat(:,5)==sco_mat(i,5)))
            if index1<>[] then
                for j=index1
                    index2=find(links_table(:,1)==-evstr(sco_mat(j,1)))
                    if index2<>[] then
                        links_table(index2',1)=-evstr(sco_mat(i,1))
                    end
                end
                sco_mat(index1,:)=[]
            end
        end
    end
endfunction
