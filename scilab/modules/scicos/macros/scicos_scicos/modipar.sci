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

function [%state0,state,sim,ok]=modipar(newparameters,%state0,state,sim,scs_m,cor)
    //store modified parameters in compiled structure state,sim
    //newparameters gives modified blocks numbers in original structure scs_m
    //cor is the correspondence table from original structure to compiled one
    xptr=sim.xptr
    zptr=sim.zptr
    ozptr=sim.ozptr
    rpptr=sim.rpptr
    ipptr=sim.ipptr
    opptr=sim.opptr
    ipar=sim.ipar
    rpar=sim.rpar
    opar=sim.opar
    ztyp=sim.ztyp
    labels=sim.labels
    x=state.x
    z=state.z
    oz=state.oz
    x0=%state0.x
    z0=%state0.z
    oz0=%state0.oz

    Impl=%f
    if xptr($)-1 < size(x,"*") then
        xd=x($/2+1:$)
        x=x(1:$/2)
        xd0=x0($/2+1:$)
        x0=x0(1:$/2)
        Impl=%t
    end

    nb=prod(size(rpptr))-1
    for k=newparameters
        if prod(size(k))==1 then //parameter of a simple block
            kc=cor(k) //index of modified block in compiled structure
            o=scs_m.objs(k)
        else
            kc=get_tree_elt(cor,k);
            o=scs_m(get_subobj_path(k))
        end
        if is_modelica_block(o) then
            parameters=o.model.equations.parameters
            rpark=[];for p=parameters(2),rpark=[rpark;p(:)];end
            ipark=ipar(ipptr(kc):ipptr(kc+1)-1)
            corinvm=%cpr.corinv(kc);
            J=[];
            if findinlistcmd(%cpr.corinv,size(cor),">")<>list() then ok=%f;return; end
            //if %cpr.corinv(kc-1)>size(cor) then ok=%f;return;end
            for jj=1:size(corinvm)
                if and(corinvm(jj)==k)  then J=jj;break,end
            end
            nek=size(rpark,"*")-(ipark(J+1)-ipark(J))
            if nek<>0 then
                warning("variying number of parameter for modelica block is not yet implemented")
            end
            rpar(rpptr(kc)+ipark(J)-1:rpptr(kc)+ipark(J+1)-2)=rpark
        else
            statekd=[]
            om=o.model
            [fun,statek,dstatek,odstatek,rpark,ipark,opark]=...
            (om.sim,om.state,om.dstate,om.odstate,om.rpar,om.ipar,om.opar);
            if type(fun)==15 then
                if (fun(2)==3 | fun(2)==5 |  fun(2)==10005) then
                    if rpark<>[] then rpark=var2vec(rpark); end
                    if dstatek<>[] then dstatek=var2vec(dstatek),end
                end
                if fun(2)>10000 then
                    statekd=statek($/2+1:$)
                    statek=statek(1:$/2)
                end
            end

            if kc>0 then
                //Change continuous state
                nek=prod(size(statek))-(xptr(kc+1)-xptr(kc))
                sel=xptr(kc+1):xptr($)-1
                if nek<>0&sel<>[] then
                    x(nek+sel)=x(sel)
                    x0(nek+sel)=x0(sel)
                    if Impl then
                        xd(nek+sel)=xd(sel)
                        xd0(nek+sel)=xd0(sel)
                    end
                end

                if nek<0 then
                    x($+nek+1:$)=[],x0($+nek+1:$)=[],
                    if Impl then xd($+nek+1:$)=[],xd0($+nek+1:$)=[],end
                end

                xptr(kc+1:$)=xptr(kc+1:$)+nek
                x(xptr(kc):xptr(kc+1)-1)=statek(:),
                x0(xptr(kc):xptr(kc+1)-1)=statek(:),
                if Impl then
                    if statekd==[] then statekd=0*statek,end
                    xd(xptr(kc):xptr(kc+1)-1)=statekd(:),
                    xd0(xptr(kc):xptr(kc+1)-1)=statekd(:),
                end

                //Change discrete state
                nek=prod(size(dstatek))-(zptr(kc+1)-zptr(kc))
                sel=zptr(kc+1):zptr($)-1
                if nek<>0&sel<>[] then
                    z(nek+sel)=z(sel)
                    z0(nek+sel)=z0(sel)
                end

                if nek<0 then
                    z($+nek+1:$)=[],z0($+nek+1:$)=[],
                end

                zptr(kc+1:$)=zptr(kc+1:$)+nek
                z(zptr(kc):zptr(kc+1)-1)=dstatek(:)
                z0(zptr(kc):zptr(kc+1)-1)=dstatek(:)

                //Change objects discrete state
                if ((type(odstatek)<>15) | ...
                    (type(fun)<>15)) then //old sci blocks or odstatek not a list
                    nek=-(ozptr(kc+1)-ozptr(kc))
                elseif ((fun(2)==5) | (fun(2)==10005)) then // sciblocks type 5 | 10005
                    if lstsize(odstatek)>0 then
                        nek=1-(ozptr(kc+1)-ozptr(kc)) //nombre d'états supplémentaires
                    else
                        nek=-(ozptr(kc+1)-ozptr(kc))
                    end
                elseif ((fun(2)==4) | (fun(2)==10004) | (fun(2)==2004)) then // C blocks type 4 | 10004
                    nek=lstsize(odstatek)-(ozptr(kc+1)-ozptr(kc))
                else // other C and sci blocks
                    nek=-(ozptr(kc+1)-ozptr(kc))
                end
                sel=ozptr(kc+1):ozptr($)-1

                if nek<>0&sel<>[] then
                    while lstsize(oz)<max(nek+sel), oz($+1)=[], end
                    while lstsize(oz0)<max(nek+sel), oz0($+1)=[], end
                    if nek>0 then sel=gsort(sel), end
                    for j=sel
                        oz(j+nek)=oz(j)
                        oz0(j+nek)=oz0(j)
                    end
                end
                ozptr(kc+1:$)=ozptr(kc+1:$)+nek;

                if ((type(odstatek)==15) & (type(fun)==15)) then
                    if ((fun(2)==5) | (fun(2)==10005)) then // sciblocks
                        if lstsize(odstatek)>0 then
                            oz(ozptr(kc))=odstatek;
                            oz0(ozptr(kc))=odstatek;
                        end
                    elseif ((fun(2)==4) | (fun(2)==10004) | (fun(2)==2004)) then  // C blocks
                        for j=1:lstsize(odstatek)
                            oz(ozptr(kc)+j-1)=odstatek(j);
                            oz0(ozptr(kc)+j-1)=odstatek(j);
                        end
                    end
                end

                //## rebuild oz list if nek < 0
                if nek < 0 then
                    n_oz = list(); n_oz0 = list();
                    for j=1:max(ozptr)-1
                        n_oz(j)=oz(j);
                        n_oz0(j)=oz0(j);
                    end
                    oz = n_oz; oz0 = n_oz0;
                    clear n_oz; clear n_oz0;
                end

                //Change real parameters
                nek=prod(size(rpark))-(rpptr(kc+1)-rpptr(kc))
                sel=rpptr(kc+1):rpptr($)-1
                if nek<>0&sel<>[] then
                    rpar(nek+sel)=rpar(sel)
                end

                if nek<0 then
                    rpar($+nek+1:$)=[]
                end

                rpptr(kc+1:$)=rpptr(kc+1:$)+nek
                rpar(rpptr(kc):rpptr(kc+1)-1)=rpark(:),

                //Change integer parameters
                if type(ipark)==1 then   //scifunc
                    nek=prod(size(ipark))-(ipptr(kc+1)-ipptr(kc))
                    sel=ipptr(kc+1):ipptr($)-1
                    if nek<>0&sel<>[] then
                        ipar(nek+sel)=ipar(sel)
                    end

                    if nek<0 then
                        ipar($+nek+1:$)=[]
                    end

                    ipptr(kc+1:$)=ipptr(kc+1:$)+nek
                    ipar(ipptr(kc):ipptr(kc+1)-1)=ipark(:),
                end
                //Change objects parameters
                if ((type(opark)<>15) | ...
                    (type(fun)<>15)) then //old sci blocks or odstatek not a list
                    nek=-(opptr(kc+1)-opptr(kc))
                elseif ((fun(2)==5) | (fun(2)==10005)) then // sciblocks
                    if lstsize(opark)>0 then
                        nek=1-(opptr(kc+1)-opptr(kc)) //nombre de paramï¿½tres supplï¿½mentaires
                    else
                        nek=-(opptr(kc+1)-opptr(kc))
                    end
                elseif ((fun(2)==4) | (fun(2)==10004) | (fun(2)==2004)) then //C blocks
                    nek=lstsize(opark)-(opptr(kc+1)-opptr(kc))
                else // other C and sci blocks
                    nek=-(opptr(kc+1)-opptr(kc))
                end
                sel=opptr(kc+1):opptr($)-1
                if nek<>0&sel<>[] then
                    while lstsize(opar)<max(nek+sel), opar($+1)=[], end
                    if nek>0 then sel=gsort(sel), end
                    for j=sel, opar(j+nek)=opar(j); end
                end
                opptr(kc+1:$)=opptr(kc+1:$)+nek;
                if ((type(opark)==15) & (type(fun)==15)) then
                    if ((fun(2)==5) | (fun(2)==10005)) then // sciblocks
                        if lstsize(opark)>0 then
                            opar(opptr(kc))=opark;
                        end
                    elseif ((fun(2)==4) | (fun(2)==10004) | (fun(2)==2004)) then //C blocks
                        for j=1:lstsize(opark), opar(opptr(kc)+j-1)=opark(j), end
                    end
                end
                //## rebuild opar list if nek < 0
                if nek < 0 then
                    n_opar = list();
                    for j=1:max(opptr)-1
                        n_opar(j)=opar(j);
                    end
                    opar = n_opar;
                    clear n_opar;
                end


                //Change simulation routine
                if type(sim("funs")(kc))<>13 then   //scifunc
                    sim("funs")(kc)=fun(1);
                    if prod(size(fun))> 1 then
                        sim("funtyp")(kc)=fun(2);
                    else
                        sim("funtyp")(kc)==0;
                    end
                end

                //Change label
                labels(kc)=o.model.label
            end
        end
    end

    sim.xptr=xptr
    sim.zptr=zptr
    sim.ozptr=ozptr
    sim.rpar=rpar
    sim.rpptr=rpptr
    sim.ipar=ipar
    sim.ipptr=ipptr
    sim.opar=opar
    sim.opptr=opptr
    sim.labels=labels

    if Impl then
        state.x=[x;xd]
    else
        state.x=x
    end

    state.z=z
    state.oz=oz
    if Impl then
        %state0.x=[x0;xd0]
    else
        %state0.x=x0
    end
    %state0.z=z0
    %state0.oz=oz0

endfunction
