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

function [blklstr,cmatr,ccmat,cor,corinv,ok]=extract_implicit(blklst,cmat,ccmat)

    imp=[];blklstr=[];cmatr=[];ok=%t

    nb=size(blklst);
    for k=1:nb
        if or(getfield(1,blklst(k))=="equations") then
            if blklst(k).equations<>list() then imp=[imp,k];end
        end
    end
    if imp==[] then [blklstr,cmatr]=(blklst,cmat), return,end

    if size(ccmat,1)>0 then
        if or(dsearch(ccmat(:,1),imp,"d")<>0) then
            messagebox("An implicit block has an event input","modal","error");
            ok=%f
            return
        end
        if  or(dsearch(ccmat(:,3),imp,"d")<>0) then
            messagebox("An implicit block has an event output","modal","error");
            ok=%f
            return
        end
    end
    indfrom=dsearch(cmat(:,1),imp,"d")
    indto=dsearch(cmat(:,4),imp,"d")

    I=find(indfrom<>0&indto<>0) //links between two implicit blocs
    Iin=find(indfrom==0&indto<>0)
    Iout=find(indfrom<>0&indto==0)

    blklsti=list(blklst(imp)); //the implicit blocks list
    cmati=cmat(I,:);

    //add blocks for input and outputs
    nbi=size(blklsti);
    N=size(Iin,"*");
    if N>0
        mo=modelica();mo.model="InPutPort";mo.outputs="vo";mo.inputs="vi";
        for k=1:N
            blklsti($+1)=scicos_model(equations=mo);
        end
        ci=cmat(Iin,:);ci(:,1)=-(nbi+(1:N))';ci(:,2)=1;
        cmati=[cmati;ci];
        nbi=nbi+N;
    end
    N=size(Iout,"*")
    if N>0
        mo=modelica();mo.model="OutPutPort";mo.outputs="vo";mo.inputs="vi";
        for k=1:N
            blklsti($+1)=scicos_model(equations=mo);
        end
        co=cmat(Iout,:);co(:,4)=-(nbi+(1:N)');co(:,5)=1;
        cmati=[cmati;co];
        nbi=nbi+N;
    end

    //renumber links connecting implicit blocs
    for k=1:size(cmati,1)
        l=find(cmati(k,1)==imp);if l<>[] then cmati(k,1)=l;end
        l=find(cmati(k,4)==imp);if l<>[] then cmati(k,4)=l;end
    end
    cmati=abs(cmati)

    name="imppart_"+stripblanks(scs_m.props.title(1));
    txt=create_modelica( blklsti,cmati,name);
    mputl(txt,scs_m.props.title(2)+name+".mo");
    [ok,name1,nx,ng]=compile_modelica(scs_m.props.title(2)+name+".mo");
    if ~ok then return,end
    reg=1:nb;reg(imp)=[];
    blklstr=list(blklst(reg)); //the regular blocks list

    //add the block equivalent to the implicit part
    nx=nx*2//should be read from generated C code
    blklstr($+1)=scicos_model(sim=list(name,10002),..
    in=ones(Iin'),out=ones(Iout'),state=zeros(nx,1),..
    dep_ut=[%f %t],nzcross=ng)
    nbr=size(blklstr)
    cmatr=cmat;cmatr([I,Iin,Iout],:)=[];
    //renumber links connecting regular blocs
    N=size(Iin,"*");
    if N>0
        ci=cmat(Iin,:);ci(:,4)=-nbr;ci(:,5)=(1:N)';
        cmatr=[cmatr;ci];
    end
    N=size(Iout,"*")
    if N>0
        co=cmat(Iout,:);co(:,1)=-nbr;co(:,2)=(1:N)';
        cmatr=[cmatr;co];
    end
    for k=1:size(cmatr,1)
        l=find(cmatr(k,1)==reg);if l<>[] then cmatr(k,1)=l;end
        l=find(cmatr(k,4)==reg);if l<>[] then cmatr(k,4)=l;end
    end
    cmatr=abs(cmatr)

    //renumber ccmat
    for k=1:size(ccmat,1)
        l=find(ccmat(k,1)==reg);if l<>[] then ccmat(k,1)=l;end
        l=find(ccmat(k,3)==reg);if l<>[] then ccmat(k,3)=l;end
    end
    //update corinv, we choose imp(1) to represent all implicits blocs!
    corinv=list(corinv([reg,imp(1)]))
    //update cor

    cor=update_cor(cor,reg)
endfunction

function cor=update_cor(cor,reg)
    n=size(cor)
    for k=1:n
        if type(cor(k))==15 then
            cor(k)=update_cor(cor(k),reg)
        else
            p=find(cor(k)==reg)
            if p<>[] then
                cor(k)=p
            elseif cor(k)<>0 then
                cor(k)=size(reg,"*")+1
            end
        end
    end
endfunction
