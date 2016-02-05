//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Serge Steer <serge.steer@inria.fr>
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

function  [blklst,cmat,ccmat,cor,corinv,ok,scs_m,flgcdgen,freof]=c_pass1(scs_m,flgcdgen)
    //derived from c_pass1 for implicit diagrams
    //%Purpose
    // Determine one level blocks and connections matrix
    //%Parameters
    // scs_m  :   scicos data structure
    // ksup   :
    // blklst : a list containing the "model" information structure for each block
    //
    // cmat   : nx6 matrix. Each row contains, in order, the block
    //             number and the port number and the port type of an outgoing scicopath,
    //             and the block number and the port number and the port type of the target
    //             ingoing scicopath. for regular links
    //
    // ccmat  : nx4 matrix.  Each row contains, in order, the block
    //             number and the port number  of an outgoing scicopath,
    //             and the block number and the port number  of the target
    //             ingoing scicopath for clock connections

    // cor    : is a list with same recursive structure as scs_m each leaf
    //          contains the index of associated block in blklst
    // corinv : corinv(nb) is the path of nb ith block defined in blklst
    //          in the scs_m structure
    //!
    // Serge Steer 2003, Copyright INRIA
    // flgcdgen: is a flag containing the numbre of event input of the diagram
    //           it is used only in the Codegeneration case.
    // freof   : it is a vector containing the frequency and the offset of the major clock.
    //           it is used only in the Codegeneration case.
    // Fady Nassif 2007. INRIA.
    //c_pass1;

    if argn(2)<=1 then flgcdgen=-1, end
    freof=[];
    MaxBlock=countblocks(scs_m);
    blklst=[];cmat=[],ccmat=[],cor=[],corinv=[]
    [cor,corinvt,links_table,cur_fictitious,sco_mat,ok]=scicos_flat(scs_m);
    if ~ok then
        disp(msprintf("%s: flat failed", "c_pass1"));
        return;
    end
    [links_table,sco_mat,ok]=global_case(links_table,sco_mat)
    if ~ok then
        disp(msprintf("%s: global case failed", "c_pass1"));
        return;
    end
    index1=find((sco_mat(:,2)=="-1")& (sco_mat(:,5)<>"10"))
    if index1<>[] then
        for i=index1
            [path]=findinlist(cor,-evstr(sco_mat(i,1)))
            full_path=path(1)
            if flgcdgen<>-1 then full_path=[numk full_path];scs_m=all_scs_m;end
            hilite_path(full_path,"Error in compilation, There is a FROM ''"+(sco_mat(i,3))+ "'' without a GOTO",%t)
            ok=%f;
            disp(msprintf("%s: invalid connection matrix", "c_pass1"));
            return;
        end
    end

    nb=size(corinvt);
    reg=1:nb
    //form the block lists

    blklst=list();kr=0 ; //regular  block list
    blklstm=list();km=0; //modelica block list

    //if ind(i)>0  ith block is a regular  block and stored in blklst(ind(i))
    //if ind(i)<0  ith block is a modelica block and stored in blklstm(-ind(i))

    ind=[];

    for kb=1:nb
        o=scs_m(scs_full_path(corinvt(kb)));
        if is_modelica_block(o) then
            km=km+1;blklstm(km)=o.model;
            ind(kb)=-km;
            [modelx,ok]=build_block(o); // compile modelica block type 30004
            if ~ok then
                disp(msprintf("%s: unable to build modelica block", "c_pass1"));
                return
            end
        else
            [model,ok]=build_block(o);
            if ~ok then
                disp(msprintf("%s: unable to build block", "c_pass1"));
                return,
            end

            if or(model.sim(1)==["plusblk"]) then
                [model,links_table]=adjust_sum(model,links_table,kb);
            end

            kr=kr+1;blklst(kr)=model;
            ind(kb)=kr;
        end

    end

    if (find(sco_mat(:,5)==string(4))<>[]) then
        if flgcdgen ==-1 then
            [links_table,blklst,corinvt,ind,ok]=sample_clk(sco_mat,links_table,blklst,corinvt,scs_m,ind,flgcdgen)
        else
            [links_table,blklst,corinvt,ind,ok,scs_m,flgcdgen,freof]=sample_clk(sco_mat,links_table,blklst,corinvt,scs_m,ind,flgcdgen)
        end
        if ~ok then
            disp(msprintf("%s: unable to sample the whole diagram", "c_pass1"));
            return,
        end
    end
    nb=size(corinvt)
    nl=size(links_table,1)/2

    // Check if size match !
    if(size(links_table(:,1:3), "r") == size(matrix([1;1]*(1:nl),-1,1), "r") ..
        & size(matrix([1;1]*(1:nl),-1,1), "r") == size(links_table(:,4), "r"))
        links_table=[links_table(:,1:3) matrix([1;1]*(1:nl),-1,1) ..
        links_table(:,4) ];
    else
        disp(msprintf("%s: invalid links table size", "c_pass1"));
        return
    end
    imp=find(ind<0)

    reg(imp)=[]

    if imp==[] then //no modelica block exists
        cmat=matfromT(links_table(find(links_table(:,5)==1),:),nb); //data flow links
        ccmat=cmatfromT(links_table(find(links_table(:,5)==-1),:),nb); //event links
        corinv=corinvt
    else // mixed diagram
        nm=size(imp,"*") //number of modelica blocks
        nr=nb-nm //number of regular blocks
        cmmat=mmatfromT(links_table(find(links_table(:,5)==2),:),nb); //modelica links
        cmat=matfromT(links_table(find(links_table(:,5)==1),:),nb); //data flow links
        ccmat=cmatfromT(links_table(find(links_table(:,5)==-1),:),nb);//event links

        //build connections between modelica world and regular one. These
        //links should be data flow links
        // links from modelica world to regular world
        fromM=find(dsearch(cmat(:,1),imp,"d")>0);

        // links from regular world to modelica world
        toM=find(dsearch(cmat(:,3),imp,"d")>0);

        // merge the modelica to modelica as data flow links
        [varM, kfrom, kto] = intersect(fromM, toM);
        fromM(kfrom) = [];
        toM(kto) = [];

        NoM=size(fromM,"*");
        if NoM>0 then
            //add modelica Output ports in Modelica world
            mo=modelica();mo.model="OutPutPort";mo.outputs="vo";mo.inputs="vi";
            for k=1:NoM,blklstm($+1)=scicos_model(equations=mo);end
            //add modelica connections to these Output ports, set negative
            //value to port numbers to avoid conflits with other blocks
            cmmat=[cmmat;
            cmat(fromM,1:2) zeros(NoM,1) -(nm+(1:NoM)'),ones(NoM,1),ones(NoM,1)];

            nm=nm+NoM;
            //add regular connection with regular block replacing the modelica world
            cmat(fromM,1:2)=[-(nr+1)*ones(NoM,1),(1:NoM)'];
        end

        NiM=size(toM,"*");
        if NiM>0 then
            //add modelica Input ports in Modelica world
            mo=modelica();mo.model="InPutPort";mo.outputs="vo";mo.inputs="vi";
            for k=1:NiM,blklstm($+1)=scicos_model(equations=mo);end
            //add modelica connections to these Input ports  set negative
            //value to port numbers to avoid conflits with other blocks
            cmmat=[cmmat;
            -(nm+(1:NiM)'), ones(NiM,1),zeros(NiM,1), cmat(toM,3:4), ones(NiM,1) ];

            nm=nm+NiM;
            //add regular connection with regular block replacing the modelica world
            cmat(toM,3:4)=[-(nr+1)*ones(NiM,1),(1:NiM)'];
        end

        NvM=size(varM, "*");
        if NvM>0 then
            //add modelica Values (both input and output) ports in Modelica world
            l=[cmat(varM,1:2), zeros(NvM,1), cmat(varM,3:4), ones(NvM,1)];
            cmmat=[cmmat;
            l];

            // remove the corresponding explicit link
            cmat(varM, :) = [];
        end

        // modelica blocks with events ports are not allowed yet
        if size(ccmat,1)>0 then
            if or(dsearch(ccmat(:,[1 3]),imp,"d")>0) then
                messagebox("An implicit block has an event port","modal","error");
                disp(msprintf("%s: implicit block with event port", "c_pass1"));
                ok=%f;return
            end
        end

        //renumber blocks according to their types

        corinv=list();corinvm=list();

        for kb=1:nb

            if ind(kb)<0 then // modelica block

                km=-ind(kb);

                //replace by negative value to avoid conflicts

                cmmat(find(cmmat(:,1)==kb),1)=-km ;

                cmmat(find(cmmat(:,4)==kb),4)=-km;

                corinvm(km)=corinvt(kb);

            else

                kr=ind(kb);

                cmat (find(cmat (:,1)==kb),1)=-kr;

                cmat (find(cmat (:,3)==kb),3)=-kr;

                ccmat(find(ccmat(:,1)==kb),1)=-kr;

                ccmat(find(ccmat(:,3)==kb),3)=-kr;

                corinv(kr)=corinvt(kb);

            end

        end

        //renumbering done, replace negative value by positive ones



        cmat(:,[1 3])=abs(cmat(:,[1 3])) ;

        ccmat(:,[1 3])=abs(ccmat(:,[1 3])) ;

        cmmat=abs(cmmat) ;

        //create regular block associated to all modelica blocks

        [model,ok]=build_modelica_block(blklstm,corinvm,cmmat,NiM,NoM,NvM,scs_m,TMPDIR+"/");

        if ~ok then
            disp(msprintf("%s: build the modelica meta-block failed", "c_pass1"));
            return
        end

        blklst(nr+1)=model;

        //make compiled modelica block refer to the set of corresponding

        //modelica blocks

        corinv(nr+1)=corinvm //it may be useful to adapt function making use

        //of corinv

        //adjust the numbering of regular block in sco_mat

        //if modelica's blocks exist

        //Fady 08/11/2007

        for i=1:size(sco_mat,1)

            if eval(sco_mat(i,1))<MaxBlock then

                sco_mat(i,1)=string(ind(eval(sco_mat(i,1))))

            end

        end

        sco_mat=[sco_mat;[string(size(blklst)) "-1" "scicostimeclk0" "1" "10"]]

    end

    cor=update_cor(cor,reg)



    // Taking care of the clk 0;

    //*** this part has been taken from c_pass2 modified and placed here it must be tested ***

    //Fady 08/11/2007

    nbl=size(blklst)

    fff=ones(nbl,1)==1

    clkptr=zeros(nbl+1,1);clkptr(1)=1; typ_l=fff;

    for i=1:nbl

        ll=blklst(i);

        clkptr(i+1)=clkptr(i)+size(ll.evtout,"*");

        //tblock(i)=ll.dep_ut($);

        typ_l(i)=ll.blocktype=="l";

    end

    all_out=[]

    for k=1:size(clkptr,1)-1

        if ~typ_l(k) then

            kk=[1:(clkptr(k+1)-clkptr(k))]'

            all_out=[all_out;[k*ones(kk),kk]]

        end

    end

    all_out=[all_out;[0,0]]
    //add time event if needed
    tblock=find((sco_mat(:,2)=="-1")&(sco_mat(:,5)=="10"))
    ind=sco_mat(tblock,1);
    if ind<>[] then
        ind=eval(ind(:))
        //ind=find(tblock)
        //ind=ind(:)
        for k=ind'
            ccmat=[ccmat;[all_out,ones(all_out)*[k,0;0,0]]]
        end
        for Ii=1:length(blklst)
            if type(blklst(Ii).sim(1))==10 then
                if part(blklst(Ii).sim(1),1:7)=="capteur" then
                    ccmat=[ccmat;[0 0 Ii 0]]
                end
            end
        end
    end
    //***
endfunction
//**-----------------------------------------------------------------------------------------------------------------

function [model,links_table]=adjust_sum(model,links_table,k)
    //sum blocks have variable number of input ports, adapt the associated
    //model data structure and input connection to take into account the
    //actual number of connected ports
    // Serge Steer 2003, Copyright INRIA
    in=find(links_table(:,1)==k&links_table(:,3)==1)
    nin=size(in,"*")
    model.in=-ones(nin,1)
    links_table(in,2)=(1:nin)'
endfunction


function mat=mmatfromT(Ts,nb)
    //S. Steer, R. Nikoukhah 2003. Copyright INRIA
    Ts(:,1)=abs(Ts(:,1));
    K=unique(Ts(find(Ts(:,1)>nb),1)); // identificator of blocks to be removed
    //remove superblocks port and split connections
    Ts=remove_fictitious(Ts,K)

    // from connection matrix
    Imat=[];
    for u=matrix(unique(Ts(:,4)),1,-1)
        kue=matrix(find(Ts(:,4)==u),-1,1); //identical links
        Imat=[Imat;[kue(2:$)  kue(1).*ones(kue(2:$))]];
    end
    mat=[Ts(Imat(:,1),1:3)  Ts(Imat(:,2),1:3)]
endfunction


function mat=matfromT(Ts,nb)
    //S. Steer, R. Nikoukhah 2003. Copyright INRIA

    Ts(:,1)=abs(Ts(:,1))
    K=unique(Ts(find(Ts(:,1)>nb),1)); // identificator of blocks to be removed
    //remove superblocks port and split connections
    Ts=remove_fictitious(Ts,K)

    // from connection matrix
    Imat=[];
    for u=matrix(unique(Ts(:,4)),1,-1)
        kue=matrix(find(Ts(:,4)==u&Ts(:,3)==-1),-1,1); //look for outputs
        jue=matrix(find(Ts(:,4)==u&Ts(:,3)==1),-1,1); //look for inputs
        Imat=[Imat;[ones(jue).*.kue jue.*.ones(kue)]];
    end
    mat=[Ts(Imat(:,1),1:2)  Ts(Imat(:,2),1:2)]
endfunction

function mat=cmatfromT(Ts,nb)
    //S. Steer, R. Nikoukhah 2003. Copyright INRIA
    //this function has been modified to support
    // CLKGOTO et CLKFROM
    // Fady NASSIF: 11/07/2007
    k=find(Ts(:,1)<0) //superblock ports links and CLKGOTO/CLKFROM
    K=unique(Ts(k,1));
    Ts=remove_fictitious(Ts,K)

    if Ts==[] then mat=[],return,end
    //  if size(Ts,1)<>int(size(Ts,1)/2)*2 then disp('PB'),pause,end
    [s,k]=gsort(Ts(:,[4,3]),"lr","i");Ts=Ts(k,:)
    // modified to support the CLKGOTO/CLKFROM
    //mat=[Ts(1:2:$,1:2) Ts(2:2:$,1:2)]
    //----------------------------------

    J=find(Ts(:,3)==1); //find the destination block of the link
    v=find([Ts(:,3);-1]==-1) // find the source block of the link
    // many destination blocks can be connected to one source block
    // so we have to find the number of destination blocks for each source block
    // v(2:$)-v(1:$-1)-1
    // then create the vector I that must be compatible with the vector J.
    I=duplicate(v(1:$-1),v(2:$)-v(1:$-1)-1);
    mat=[Ts(I,1:2),Ts(J,1:2)]

    //----------------------------------
    K=unique(Ts(Ts(:,1)>nb))
    Imat=[];
    for u=matrix(K,1,-1)
        jue=matrix(find(mat(:,1)==u),-1,1); //look for outputs
        kue=matrix(find(mat(:,3)==u),-1,1); //look for inputs
        Imat=[ones(jue).*.kue jue.*.ones(kue)];
        mat1=[mat(Imat(:,1),1:2), mat(Imat(:,2),3:4)];
        mat([jue;kue],:)=[];
        mat=[mat;mat1];
    end

endfunction

function Ts=remove_fictitious(Ts,K)
    //removes fictitious blocks connected links are replaced by a single one
    //S. Steer, R. Nikoukhah 2003. Copyright INRIA
    count=min(Ts(:,4))
    for i=1:size(K,"*")
        ki=K(i);
        v1=find(Ts(:,1)==ki);
        if v1<>[] then
            v=unique(Ts(v1,4));
            Ts(v1,:)=[];
            if size(v)==1 then
                ind=find(Ts(:,4)==v);
            else
                ind = find(dsearch(Ts(:,4),gsort(v,"g","i"),"d")<>0);
            end
            if size(ind,"*")>1 then
                count=count-1;
                Ts(ind,4)=count
            else
                Ts(ind,:)=[]
            end
        end
    end
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
            elseif cor(k)<0 then  // GOTO FROM cases
                cor(k)=0
            elseif cor(k)<>0 then
                cor(k)=size(reg,"*")+1
            end
        end
    end
endfunction
