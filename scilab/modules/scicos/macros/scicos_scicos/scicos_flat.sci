//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Ramine Nikoukhah <ramine.nikoukhah@inria.fr> - 2003
//                      - Serge Steer <serge.steer@inria.fr>           - 2003
//                      - Fady Nassif <fady.nassif@inria.fr>           - 2007
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

function  [cor,corinv,links_table,cur_fictitious,sco_mat,ok, IN, OUT, EIN, EOUT]=scicos_flat(scs_m,ksup,MaxBlock)
    //This function takes a hierarchical Scicos diagram and computes the
    //"flat" equivalent, removing "non computational" blocs like splits.
    //S. Steer, R. Nikoukhah 2003. Copyright INRIA

    //NB::--------------------------------------------------------------
    // Modifying this function to take care of the GOTO FROM SampleCLK and VirtualCLK0 blocks.
    // A negatif number in cor and in links_table is refer to a GOTO,FROM
    // GotoTagVisibility, SampleCLK, VirtualCLK0. in other words the blocks that are virtual.
    // These blocks are removed in the compilation part.
    // Modification of shiftcors. It will not shift the negatives numbers.
    // A sco_mat is a string matrix composed by the :
    // For the blocks GOTO, FROM, VirtualCLK0
    //	- the first column : the negatif of the number of the virtual block in the cor.
    //	- the second column: 1 if it is a GOTO; -1 if it is a FROM.
    //                           The VirtualCLK0 is considered as a GOTO.
    //	- the third column : the tag value. 'scicostimeclk0' is used in the case of the VirtualCLK0
    //	- the forth column : the tag visibility
    //                           For the GOTO: + 2: scoped
    //                                         + 3: global
    //                           For the FROM: + 1
    //                           For the VirtualCLK0: 2
    //	- the fifth column : 1=regular 2=event 3=modelica 10=VirtualCLK0
    // For the SampleCLK:
    //	- the first column : the negatif of the number of the virtual block in the cor.
    //	- the second column: the value 1.
    //	- the third column : The frequency value.
    //	- the forth column : The offset value.
    //	- the fifth column : the value 4.
    // Another two string matrices are used for the GOTO/FROM blocks. The loc_mat it is used when the GOTO
    // block is local. The from_mat to match the local from with the local goto.
    // A tag_exprs matrices is used for the GotoTagVisibility and the VirtualCLK0:
    // it is composed by:
    //       - the first column: The tag value. 'scicostimeclk0' in the VirtualCLK0 case.
    //       - the second column: 1=regular 2=event 3=modelica 10=VirtualCLK0
    // The local and scoped cases are studied in this function.
    // The global case is studied in the function global_case in c_pass1.
    // A Modification of update_cor in c_pass1. For the negatives numbers
    // the cor will be set to 0. (In this case the blocks are considered as IN_f ...)
    // Fady NASSIF 2007. INRIA.
    //-------------------------------------------------------------------

    if argn(2)<=1 then ksup=0;end //used for recursion
    if ksup==0 then   // main scheme
        MaxBlock=countblocks(scs_m);
        //last created fictitious block (clock split,clock sum,super_blocks, superbloc))
        cur_fictitious=MaxBlock
        path=[];       // for delete_unconnected
        scs_m_s=scs_m ;// for delete_unconnected
    end
    //-------------- suppress blocks with an unconnected regular port --------------
    scs_m=delete_unconnected(scs_m);

    //list of blocks with are not retained in the final block list
    blocks_to_remove=["CLKSPLIT_f" "SPLIT_f" "IMPSPLIT_f" "CLKSOM_f" "CLKSOMV_f" "NRMSOM_f" "PAL_f" "xcos_block"]
    port_blocks=["IN_f","INIMPL_f","OUT_f","OUTIMPL_f","CLKIN_f","CLKINV_f","CLKOUT_f","CLKOUTV_f"]
    block_goto=["GOTO","CLKGOTO","GOTOMO"]
    block_from=["FROM","CLKFROM","FROMMO"]
    block_tag=["GotoTagVisibility","CLKGotoTagVisibility","GotoTagVisibilityMO"]
    n=lstsize(scs_m.objs) //number of "objects" in the data structure
    //-------------- initialize outputs --------------
    nb=0;
    links_table=[]; //
    corinv=list();
    cor=list();for k=1:n, cor(k)=0;end

    ok=%t;
    IN=[];OUT=[];EIN=[];EOUT=[];
    Links=[] //to memorize links position in the data structure
    mod_blk_exist=%f;
    //-------------- Analyse blocks --------------
    loc_mat=[];from_mat=[];tag_exprs=[];sco_mat=[];
    for k=1:n //loop on all objects
        o=scs_m.objs(k);
        x=getfield(1,o);
        cor(k)=0
        if x(1)=="Block" then
            if or(o.gui==block_goto) then
                cur_fictitious=cur_fictitious+1;
                cor(k)=-cur_fictitious;
                if (o.graphics.exprs(2)=="1") then
                    loc_mat=[loc_mat;[string(cur_fictitious),string(1),(o.graphics.exprs(1)),string(find(block_goto(:)==o.gui))]]
                    locomat=[];
                    for i=1:size(loc_mat,1)
                        locomat=[locomat;strcat([loc_mat(i,3) loc_mat(i,4)])]
                    end
                    vec=unique(locomat)
                    if size(vec,1)<>size(loc_mat,1) then
                        if flgcdgen<>-1 then path=[numk path];scs_m=all_scs_m; end
                        if (ksup==0)|flgcdgen<>-1  then
                            hilite_path([path,k],"There is another local GOTO in this diagram with the same tag ''"+loc_mat($,3)+"''",%t);
                        else
                            hilite_path([path,k], "There is another local GOTO in this diagram with the same tag ''"+loc_mat($,3)+"''",%t);
                        end
                        disp(msprintf("%s: goto tag not unique", "scicos_flat"));
                        ok=%f;return
                    end
                else
                    sco_mat=[sco_mat;[string(cur_fictitious),string(1),o.graphics.exprs(1),o.graphics.exprs(2),string(find(block_goto(:)==o.gui))]]
                end
            elseif or(o.gui==block_from) then
                cur_fictitious=cur_fictitious+1;
                cor(k)=-cur_fictitious
                sco_mat=[sco_mat;[string(cur_fictitious),string(-1),o.graphics.exprs(1),string(1),string(find(block_from(:)==o.gui))]]
                from_mat=[from_mat;[string(cur_fictitious),string(-1),o.graphics.exprs(1),string(find(block_from(:)==o.gui))]]
            elseif or(o.gui==block_tag) then
                tag_exprs=[tag_exprs;[o.graphics.exprs(1),string(find(block_tag(:)==o.gui))]]
                cur_fictitious=cur_fictitious+1;
                cor(k)=-cur_fictitious
            elseif o.gui=="SampleCLK" then
                if o.graphics.peout<>0 then
                    cur_fictitious=cur_fictitious+1;
                    cor(k)=-cur_fictitious
                    [Nf,Df]=rat(o.model.rpar(1),1d-9);
                    [No,Do]=rat(o.model.rpar(2),1d-9);
                    sco_mat=[sco_mat;[string(cur_fictitious),string(1),string(Nf)+"/"+string(Df),..
                    string(No)+"/"+string(Do),string(4)]]
                end
                //Adding the VirtualCLK0. Fady 18/11/2007
            elseif o.gui=="VirtualCLK0" then
                cur_fictitious=cur_fictitious+1;
                cor(k)=-cur_fictitious
                sco_mat=[sco_mat;[string(cur_fictitious),string(1),"scicostimeclk0",..
                string(2),string(10)]]
                tag_exprs=[tag_exprs;["scicostimeclk0",string(10)]]
            elseif or(o.gui==blocks_to_remove) then
                cur_fictitious=cur_fictitious+1;
                cor(k)=cur_fictitious
            elseif o.gui=="SUM_f"|o.gui=="SOM_f" then
                nb=nb+1;
                corinv(nb)=k;
                cor(k)=nb
                //scs_m=adjust_sum(scs_m,k)
            elseif or(o.gui==port_blocks) then
                //here we suppose to be inside a superblock
                //may be we can handle this blocks just as blocks_to_remove
                if ksup==0 then
                    scs_m=scs_m_s
                    hilite_path([path,k],gettext("I/O blocks must be only used in a Super Block"),%f)
                    disp(msprintf("%s: Port out of hierarchy", "scicos_flat"));
                    ok=%f;return
                end
                connected=get_connected(scs_m,k)
                if connected==[] then
                    scs_m=scs_m_s
                    hilite_path([path,k],gettext("This Super block input port is not connected."),%t)
                    disp(msprintf("%s: Not connected super block input", "scicos_flat"));
                    ok=%f;return
                end
                if or(o.gui==["IN_f","INIMPL_f"]) then
                    pind=Pind(1)
                    IN=[IN o.model.ipar]
                elseif or(o.gui==["OUT_f","OUTIMPL_f"]) then
                    pind=Pind(2)
                    OUT=[OUT o.model.ipar]
                elseif or(o.gui==["CLKIN_f","CLKINV_f"]) then
                    pind=Pind(3)
                    EIN=[EIN o.model.ipar]
                elseif or(o.gui==["CLKOUT_f","CLKOUTV_f"]) then
                    pind=Pind(4)
                    EOUT=[EOUT o.model.ipar]
                end
                //connect the link to the fictitious bloc replacing the superblock
                if scs_m.objs(connected).from(1)==k then
                    scs_m.objs(connected).from(1)=-(pind+o.model.ipar)
                end
                if scs_m.objs(connected).to(1)==k then
                    scs_m.objs(connected).to(1)=-(pind+o.model.ipar)
                end
            elseif o.model.sim(1)=="asuper" then
                nb=nb+1
                corinv(nb)=k
                cor(k)=nb
                if o.graphics.exprs(3).dep_ut($)==%t then
                    sco_mat=[sco_mat;[string(nb) "-1" "scicostimeclk0" "1" "10"]]
                end
            elseif o.model.sim=="super"|o.model.sim=="csuper" then
                path=[path k] //superbloc path in the hierarchy
                //replace superbloc by a set of fictitious blocks (one per port)
                //and reconnect links connected to the superblock to these
                //ficitious blocks
                Pinds=[];if exists("Pind") then Pinds=Pind,end
                Pind=[] //base of ports numbering
                //msprintf("entering superblock at level "+string(size(path,'*'))+"\r\n")
                nb_pin=size(scs_m.objs(k).graphics("pin"),1);
                nb_pein=size(scs_m.objs(k).graphics("pein"),1);
                for port_type=["pin","pout","pein","peout"]
                    Pind=[Pind cur_fictitious]
                    ip=scs_m.objs(k).graphics(port_type);
                    ki=find(ip>0)
                    for kk=ki
                        kc=ip(kk)
                        //**  a link is connected to the same sblock on both ends
                        if scs_m.objs(kc).to(1)==scs_m.objs(kc).from(1) then
                            //** regular input port
                            if port_type=="pin" then
                                scs_m.objs(kc).to(1)=-(cur_fictitious+scs_m.objs(kc).to(2));
                                scs_m.objs(kc).to(2)=1

                                if scs_m.objs(kc).from(3)==0 then //** in connected to out
                                    scs_m.objs(kc).from(1)=-(cur_fictitious+scs_m.objs(kc).from(2)+nb_pin);
                                    scs_m.objs(kc).from(2)=1
                                else //** in connected to in
                                    scs_m.objs(kc).from(1)=-(cur_fictitious+scs_m.objs(kc).from(2));
                                    scs_m.objs(kc).from(2)=1
                                end

                                //** regular output port
                            elseif port_type=="pout" then
                                scs_m.objs(kc).from(1)=-(cur_fictitious+scs_m.objs(kc).from(2));
                                scs_m.objs(kc).from(2)=1

                                if scs_m.objs(kc).to(3)==0 then //** out connected to out
                                    scs_m.objs(kc).to(1)=-(cur_fictitious+scs_m.objs(kc).to(2));
                                    scs_m.objs(kc).to(2)=1
                                end

                                //** event input port
                            elseif port_type=="pein" then
                                scs_m.objs(kc).to(1)=-(cur_fictitious+scs_m.objs(kc).to(2));
                                scs_m.objs(kc).to(2)=1

                                scs_m.objs(kc).from(1)=-(cur_fictitious+scs_m.objs(kc).from(2)+nb_pein);
                                scs_m.objs(kc).from(2)=1

                                //** peout and pein are never connected to themselves
                            end

                        elseif scs_m.objs(kc).to(1)==k then  // a link going to the superblock
                            scs_m.objs(kc).to(1)=-(cur_fictitious+scs_m.objs(kc).to(2));
                            scs_m.objs(kc).to(2)=1

                        elseif scs_m.objs(kc).from(1)==k then  // a link coming from the superblock
                            scs_m.objs(kc).from(1)=-(cur_fictitious+scs_m.objs(kc).from(2));
                            scs_m.objs(kc).from(2)=1
                        end
                    end
                    cur_fictitious=cur_fictitious+size(ip,"*")
                end


                //Analyze the superblock contents
                [cors,corinvs,lt,cur_fictitious,scop_mat,ok, localIN, localOUT, localEIN, localEOUT]=scicos_flat(o.model.rpar,cur_fictitious,MaxBlock)
                if ~ok then
                    disp(msprintf("%s: Invalid super block at %d", "scicos_flat", k));
                    return
                end

                // check if the ports has the right order
                localIN=-gsort(-localIN);
                if or(localIN<>[1:size(localIN,"*")]) then
                    ok=%f;
                    msg=gettext("Input ports are not numbered properly.")
                    hilite_path(path,msg,%t)
                    disp(msprintf("%s: Input ports are not numbered properly.", "scicos_flat"))
                    return
                end
                localOUT=-gsort(-localOUT);
                if or(localOUT<>[1:size(localOUT,"*")]) then
                    ok=%f;
                    msg=gettext("Output ports are not numbered properly.")
                    hilite_path(path,msg,%t)
                    disp(msprintf("%s: Output ports are not numbered properly.", "scicos_flat"))
                    return
                end
                localEIN=-gsort(-localEIN);
                if or(localEIN<>[1:size(localEIN,"*")]) then
                    ok=%f;
                    msg=gettext("Event input ports are not numbered properly.")
                    hilite_path(path,msg,%t)
                    disp(msprintf("%s: Event input ports are not numbered properly.", "scicos_flat"))
                    return
                end
                localEOUT=-gsort(-localEOUT);
                if or(localEOUT<>[1:size(localEOUT,"*")]) then
                    ok=%f;
                    msg=gettext("Event output ports are not numbered properly.")
                    hilite_path(path,msg,%t)
                    disp(msprintf("%s: Event output ports are not numbered properly.", "scicos_flat"))
                    return
                end

                //shifting the scop_mat for regular blocks. Fady 08/11/2007
                if scop_mat<>[] then
                    v_mat=find(eval(scop_mat(:,1))<MaxBlock)
                    v_mat=v_mat(:)
                    for j=v_mat
                        scop_mat(j,1)=string(eval(scop_mat(j,1))+nb)
                    end
                end
                //Adding the scop_mat to the old sco_mat.
                sco_mat=[sco_mat;scop_mat]
                nbs=size(corinvs)

                //catenate superbloc data with current data

                f=find(lt(:,1)>0&lt(:,1)<=nbs);if f<>[] then lt(f,1)=lt(f,1)+nb,end
                links_table=[links_table;lt]

                for kk=1:nbs, corinv(nb+kk)=[k,corinvs(kk)];end
                cors=shiftcors(cors,nb)
                //	cur_fictitious=cur_fictitious+nb
                cor(k)=cors
                nb=nb+nbs
                Pind=Pinds
                path($)=[]

            else //standard blocks
                nb=nb+1
                corinv(nb)=k
                //[model,ok]=build_block(o.model)
                cor(k)=nb
                //Adding the always activated blocks to the sco_mat to take care of the enabling if exists.
                //Fady 18/11/2007
                if ~is_modelica_block(o) then
                    if o.model.dep_ut($) then
                        sco_mat=[sco_mat;[string(nb) "-1" "scicostimeclk0" "1" "10"]]
                    end
                else
                    mod_blk_exist=%t // Flag for the existance of modelica's blocks
                end
            end
        elseif x(1)=="Deleted"|x(1)=="Text" then
            //this objects are ignored
        else //links
            Links=[Links k] // memorize their position for use during links analysis
        end
    end //end of loop on objects

    if ksup==0&nb==0 then
        messagebox(msprintf(gettext("%s: Empty diagram"), "Xcos"),"modal")
        ok=%f
        disp(msprintf("%s: Empty diagram", "scicos_flat"));
        return
    end
    //-------------- Analyse  links --------------
    for k=Links
        o=scs_m.objs(k);
        f=0
        if o.from(1)<0|o.from(1)>MaxBlock then //Link coming from a superblock input port
        else
            o.from(1)=cor(o.from(1));
        end
        if o.to(1)<0 |o.to(1)>MaxBlock then //Link going to a superblock output port
        else
            o.to(1)=cor(o.to(1)),
        end

        if o.ct(2)==2 //implicit links
            //if abs(o.from(1))==125|abs(o.to(1))==125 then pause,end
            links_table=[links_table
            o.from(1:3)    o.ct(2)
            o.to(1:3)      o.ct(2) ]
        else //regular or event links
            links_table=[links_table
            o.from(1:2)  -1  o.ct(2) //outputs are tagged with -1
            o.to(1:2)    1   o.ct(2) ] //inputs are tagged with 1
        end
    end
    // Warning in case of modelica's blocks in an enabled diagram.
    // Fady 18/11/2007
    tof=find((sco_mat(:,2)=="1")& (sco_mat(:,5)=="10"))
    if tof<>[] then
        if mod_blk_exist then
            messagebox("Warning the enable does not consider the modelica blocks","modal")
        end
    end
    //----------------------Goto From Analyses--------------------------
    // Local case
    if loc_mat<>[] then
        for i=1:size(loc_mat,1)
            index1=find((from_mat(:,2)=="-1")&(from_mat(:,3)==loc_mat(i,3))&(from_mat(:,4)==loc_mat(i,4)))
            for j=index1
                index2=find(links_table(:,1)==-evstr(from_mat(j,1)))
                // 	     for k=index2
                // 		  links_table(k,1)=-evstr(loc_mat(i,1))
                // 	     end
                if index2<>[] then
                    links_table(index2',1)=-evstr(loc_mat(i,1))
                end
                index2=find(sco_mat(:,1)==from_mat(j,1))
                sco_mat(index2',:)=[]
            end
        end
    end
    //scoped case
    if tag_exprs<>[] then
        //to be modified !!!!!
        //      vec=unique(tag_exprs)
        for i=1:size(tag_exprs,1)
            index=find((tag_exprs(:,1)==tag_exprs(i,1))&(tag_exprs(:,2)==tag_exprs(i,2)))
            if size(index,"*") > 1  then
                messagebox(["Error In Compilation. You cannot have multiple GotoTagVisibility";..
                " with the same tag value in the same scs_m"],"modal")
                ok=%f;
                disp(msprintf("%s: Multiple GotoTagVisibility at the same level", "scicos_flat"));
                return
            end
        end

        for i=1:size(tag_exprs,1)
            index=find((sco_mat(:,2)=="1")&(sco_mat(:,3)==tag_exprs(i,1))&(sco_mat(:,4)=="2")&(sco_mat(:,5)==tag_exprs(i,2)))
            if index<>[] then
                if size(index,"*")>1 then
                    messagebox(["Error in compilation";"Multiple GOTO are taged by the same GotoTagVisibility"],"modal")
                    ok=%f
                    disp(msprintf("%s: Shared GotoTagVisibility across GOTO", "scicos_flat"));
                    return
                end
                index1=find((sco_mat(:,2)=="-1")&(sco_mat(:,3)==tag_exprs(i,1))&(sco_mat(:,5)==tag_exprs(i,2)))
                if index1<>[] then
                    for j=index1
                        index2=find(links_table(:,1)==-evstr(sco_mat(j,1)))
                        if index2<>[] then
                            links_table(index2',1)=-evstr(sco_mat(index,1))
                        end
                        //linking the always active blocks to the VirtualCLK0 if exists.
                        if sco_mat(j,5)=="10" then
                            links_table($+1,:)=[-evstr(sco_mat(index,1)) 1 -1 -1]
                            links_table($+1,:)=[evstr(sco_mat(j,1)) 0 1 -1]
                        end
                    end
                end
                sco_mat([index1';index'],:)=[]
                //sco_mat(index,:)=[]
            end
        end
    end
    //global case
    // function global_case in c_pass1
    //------------------------------------------------------------------------
endfunction
