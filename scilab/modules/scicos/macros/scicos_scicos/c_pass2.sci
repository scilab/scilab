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

function cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv,flag)
    // cor    ; correspondence table with initial block ordering
    //
    // bllst: list with nblk elts where nblk denotes number of blocks.
    //        Each element must be a list with 16 elements:
    //          1- a list containing function name and function type
    //          2- vector of number of inputs
    //          3- vector of number of outputs
    //          4- vector of number of clock inputs
    //          5- vector of number of clock outputs
    //          6- vector (column) of continuous initial condition
    //          7- vector (column) of discrete initial condition
    //          8- vector (column) of real parameters
    //          9- vector (column) of integer parameters
    //          10- string: 'l' for synchro (ifthenelse,eselect) or 'm'
    //          (memo) or 'x' for blocks that need to be called during
    //          integration even in the absence of state (only if with workspace)
    //          11- vector of size <number of clock outputs> including
    //              preprogrammed event firing times (<0 if no firing)
    //          12- boolean vector (column): 1:nin entry for dependence on u,
    //          last on t
    //          13- block label
    //          14- number of zero crossings
    //          15- number of modes
    //          16- empty list (equation for modelica blocks)
    //
    // connectmat: nx4 matrix. Each row contains, in order, the block
    //             number and the port number of an outgoing scicopath,
    //             and the block number and the port number of the target
    //             ingoing scicopath.
    //
    // clkconnect: same as connectmat but for clock scicopaths.
    //
    // define some constants
    if argn(2) <6 then flag="verbose",end
    show_trace=%f
    if show_trace then mprintf("c_pass1:\t%f\n", timer()),end

    show_pause=%f;
    show_comment=%f;

    if bllst==list() then
        messagebox(_("No block can be activated"),"modal","error")
        cpr=list()
        ok=%f;
        return
    end

    //correction of clkconnect.. Must be done before
    clkconnect(find(clkconnect(:,2)==0),2)=1;




    if exists("%scicos_solver")==0 then %scicos_solver=0,end

    clkptr=1,cliptr=1,typ_l=[],
    nblk=size(bllst)

    //take care of the heritage
    [bllst,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,dep_u,dep_uptr,dep_t,..
    typ_l,typ_r,typ_m,tblock,typ_cons,typ_zx,ok]=mini_extract_info(bllst,..
    connectmat,clkconnect)
    if show_trace then mprintf("c_pass20:\t%f\n", timer()),end
    if ~ok then
        cpr=list()
        return
    end
    [outoin,outoinptr]=conn_mat(inpptr,outptr,inplnk,outlnk)

    [critev]=critical_events(connectmat,clkconnect,dep_t,typ_r,..
    typ_l,typ_zx,outoin,outoinptr,clkptr)
    [clkconnect,exe_cons]=pak_ersi(connectmat,clkconnect,typ_r,..
    typ_l,outoin,outoinptr,tblock,typ_cons,clkptr)


    [ordclk,ordptr,cord,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,..
    dep_uptr,corinv,clkptr,cliptr,critev,ok]=paksazi2(typ_l,clkconnect,..
    connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)

    if ~ok then
        cpr=list()
        return
    end

    if show_pause then
        mprintf("fin de paksazi")
        pause
    end

    if show_trace then mprintf("c_pass31:\t%f\n", timer()),end

    //extract various info from bllst
    [lnksz,lnktyp,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,xptr,zptr,..
    ozptr,typ_mod,rpptr,ipptr,opptr,xc0,xcd0,xd0,oxd0,rpar,..
    ipar,opar,typ_z,typ_x,typ_m,funs,funtyp,initexe,labels,uids,..
    bexe,boptr,blnk,blptr,ok]=extract_info(bllst,connectmat,clkconnect,typ_l);
    typ_z0=typ_z;



    if ~ok then
        messagebox(_("Problem in port size or type."),"modal","error");
        cpr=list()
        return,
    end

    if show_trace then mprintf("c_pass41:\t%f\n", timer()),end

    //form a matrix which gives destinations of each block
    [outoin,outoinptr]=conn_mat(inpptr,outptr,inplnk,outlnk)
    [evoutoin,evoutoinptr]=synch_clkconnect(typ_l,clkconnect)
    //
    if show_trace then mprintf("c_pass50:\t%f\n", timer()),end

    [execlk_cons]=discard(clkptr,cliptr,clkconnect,exe_cons)

    clkconnect=[];exe_cons=[]

    if show_trace then mprintf("c_pass501:\t%f\n", timer()),end

    // Set execution scheduling tables
    [ordclk,iord,oord,zord,typ_z,ok]=scheduler(inpptr,outptr,clkptr,execlk_cons,..
    outoin,outoinptr,evoutoin,evoutoinptr,typ_z,typ_x,typ_l,bexe,boptr,blnk,blptr,..
    ordclk,ordptr,cord,dep_t,dep_u,dep_uptr);

    if ~ok then
        cpr=list()
        return,
    end

    if show_trace then mprintf("c_pass51:\t%f\n", timer()),end
    //form scicos arguments

    nb=size(typ_z,"*");
    zcptr=ones(nb+1,1);
    modptr=ones(nb+1,1);

    for i=1:nb
        zcptr(i+1)=zcptr(i)+typ_z(i)
        modptr(i+1)=modptr(i)+sign(typ_z(i))*typ_mod(i);
    end

    ztyp=sign(typ_z0)  //completement inutile pour simulation
    // utiliser pour la generation de code

    if xptr($)==1 & zcptr($)>1 then
        mess=msprintf(_("No continuous-time state. Thresholds are ignored; this \nmay be OK if you don''t generate external events with them.\nIf you want to reactivate the thresholds, then you need\n\nto include a block with continuous-time state in your diagram.\n   You can for example include DUMMY CLSS block (linear palette)."))
        messagebox(mess,"modal","error");
    end

    subscr=[]
    ncblk=0;nxblk=0;ndblk=0;ndcblk=0;
    sim=scicos_sim(funs=funs,xptr=xptr,zptr=zptr,ozptr=ozptr,..
    zcptr=zcptr,inpptr=inpptr,outptr=outptr,..
    inplnk=inplnk,outlnk=outlnk,rpar=rpar,rpptr=rpptr,..
    ipar=ipar,ipptr=ipptr,opar=opar,opptr=opptr,..
    clkptr=clkptr,ordptr=ordptr,execlk=ordclk,..
    ordclk=ordclk,cord=cord,oord=oord,zord=zord,..
    critev=critev(:),nb=nb,ztyp=ztyp,nblk=nblk,..
    ndcblk=ndcblk,subscr=subscr,funtyp=funtyp,..
    iord=iord,labels=labels,uids=uids,modptr=modptr);

    //initialize agenda
    [tevts,evtspt,pointi]=init_agenda(initexe,clkptr)
    if show_trace then mprintf("c_pass61:\t%f\n", timer()),end

    //mod=0*ones(modptr($)-1,1)

    outtb=list();
    outtb=buildouttb(lnksz,lnktyp);

    iz0=zeros(nb,1);

    if max(funtyp)>10000 &%scicos_solver==0 then
        warning(_("Diagram contains implicit blocks, compiling for implicit Solver."))
        %scicos_solver=100
    end
    if or(%scicos_solver==[100, 101, 102]) then xc0=[xc0;xcd0],end
    state=scicos_state()
    state.x=xc0
    state.z=xd0
    state.oz=oxd0
    state.iz=iz0
    state.tevts=tevts
    state.evtspt=evtspt
    state.pointi=pointi
    state.outtb=outtb
    //  state.mod=mod

    cpr=scicos_cpr(state=state,sim=sim,cor=cor,corinv=corinv);

    if show_trace then mprintf("c_pass71:\t%f\n", timer()),end

endfunction


//donne les sources d'activation du sch�ma
function [vec_clk]=get_clocks(clkconnect,clkptr)
    vec_clk=[]
    if (find(clkconnect(:,1)==0) ~=[]) then
        //activation continue
        vec_clk=[vec_clk;0 0];
    end
    for blk=1:size(clkptr,1)-1
        if ~typ_l(blk) then
            for port=1:clkptr(blk+1)-clkptr(blk)
                vec_clk=[vec_clk; blk port];
            end
        end
    end
endfunction



//insere le vecteur primary dans vec_clk apr�s la ligne comptenant le bloc i
function vec_clk0=set_primary_clk(vec_clk,primary,i)

    if vec_clk~=[] then
        n_vc0=find(vec_clk==i)
        n_vc0=n_vc0($)
        vec_clk0=vec_clk(1:n_vc0)
        vec_clk1=vec_clk(n_vc0+1:size(vec_clk,1))
        for k=1:size(primary,1)
            if find(primary(k)==vec_clk0)==[] then
                vec_clk0($+1)=primary(k)
            end
        end
        for k=1:size(vec_clk1,1)
            if find(vec_clk1(k)==vec_clk0)==[] then
                vec_clk0($+1)=vec_clk1(k)
            end
        end
    else
        vec_clk0=primary
    end

endfunction

//insere la sous-matrice primary dans vec_clk apr�s la ligne k
function vec_clk0=set_primary_clkport(vec_clk,primary,i)

    if vec_clk~=[] then
        vec_clk0=vec_clk(1:i,:)
        vec_clk1=vec_clk(i+1:size(vec_clk,1),:)
        for k=1:size(primary,1)
            f=find(primary(k,1)==vec_clk0(:,1))
            if f==[] then
                vec_clk0=[vec_clk0;primary(k,:)]
            end
        end
        n_vc1=size(vec_clk1,1)
        if n_vc1>0 then
            for k=1:n_vc1
                f=find(vec_clk1(k,1)==vec_clk0(:,1))
                if f==[] then
                    vec_clk0=[vec_clk0;vec_clk1(k,:)]
                end
            end
        end
    else
        vec_clk0=primary
    end

endfunction

//insere la sous-matrice ordoclk0 dans ordclk apr�s le block k
function [ordptr,ordclk,blocs_traites]=set_ordclk(ordclk,..
    ordoclk0,k,j,ordptr,blocs_traites)
    if ordoclk0~=[] then
        if ordclk==[] then
            ordclk=[ordclk;ordoclk0];
            ordptr($+1)=ordptr($)+size(ordoclk0,1);
            blocs_traites=[blocs_traites;k] //k
        else
            m=max(find(blocs_traites==k))
            if j>1 & m~=[] then
                ordclk=[ordclk(1:ordptr(m+1)-1,:);ordoclk0;ordclk(ordptr(m+1):$,:)]
                ordptr=[ordptr(1:m+1);ordptr(m+1:$)+size(ordoclk0,1)]
                blocs_traites=[blocs_traites(1:m);k;blocs_traites(m+1:$)]
            else
                ind=find(blocs_traites <k)
                m=max(ind)
                if m==size(blocs_traites,1) then
                    ordclk=[ordclk;ordoclk0]
                    ordptr($+1)=ordptr($)+size(ordoclk0,1);
                    blocs_traites=[blocs_traites;k]
                else
                    if m~=[] then
                        ordclk=[ordclk(1:ordptr(m+1)-1,:);ordoclk0;ordclk(ordptr(m+1):$,:)]
                        ordptr=[ordptr(1:m+1);ordptr(m+1:$)+size(ordoclk0,1)]
                        blocs_traites=[blocs_traites(1:m);k;blocs_traites(m+1:$)]
                    else
                        ordclk=[ordoclk0;ordclk]
                        ordptr=[1;ordptr+size(ordoclk0,1)]
                        blocs_traites=[k;blocs_traites]
                    end
                end
            end
        end
    else
        if j>1 & find((blocs_traites==k))~=[] then
            m=max(find(blocs_traites==k))
            ordptr=[ordptr(1:m+1);ordptr(m+1:$)+size(ordoclk0,1)]
            blocs_traites=[blocs_traites(1:m);k;blocs_traites(m+1:$)]
        else
            ind=find(blocs_traites <k)
            m=max(ind)
            if m==size(blocs_traites,1) then
                ordptr($+1)=ordptr($)+size(ordoclk0,1);
                blocs_traites=[blocs_traites;k]
            else
                if m~=[] then
                    ordptr=[ordptr(1:m+1);ordptr(m+1:$)+size(ordoclk0,1)]
                    blocs_traites=[blocs_traites(1:m);k;blocs_traites(m+1:$)]
                else
                    ordptr=[1;ordptr+size(ordoclk0,1)]
                    blocs_traites=[k;blocs_traites]
                end
            end
        end
    end
endfunction


//donne les blocs activant blk
function [parents]=get_parents(parents,blk)
    f=find(clkconnect(:,3)==blk)
    n_f=size(f,2)
    if n_f>0 then
        for i=1:n_f
            clki1=clkconnect(f(i),1)
            clki2=clkconnect(f(i),2)
            g=find(clki1==parents(:,1))
            if g==[] | (g~=[] & parents(g,2)~=clki2) then
                parents=[parents;clki1,clki2]
            end
        end
    end
endfunction


function [blks,blksptr]=depend_on(connectmat,dep_u,dep_uptr)
    // returns the blocks on which depend directly  a block
    blks=[];blksptr=1
    for i=1:size(dep_uptr,"*")-1
        indport=find(dep_u(dep_uptr(i):dep_uptr(i+1)-1));
        f=[]
        for j=indport
            f=[f,find(connectmat(:,3)==i&connectmat(:,4)==j)]
        end
        blkis=unique(connectmat(f,1))
        blks=[blks;blkis]
        blksptr=[blksptr;blksptr($)+size(blkis,"*")]
    end
endfunction

function [eblks,eblksptr]=update_event_depend_on(eblks,eblksptr,clkconnect,cliptr,bls)
    // updates the blocks activating a block
    nb1=size(cliptr,"*")
    eblksptr($:nb1)=eblksptr($)
    for i=bls
        f=find(clkconnect(:,3)==i)
        blkis=unique(clkconnect(f,1))
        dblkis=size(blkis,"*")-(eblksptr(i+1)-eblksptr(i))
        Ibef=eblksptr(1):eblksptr(i)-1;
        Iaft=eblksptr(i+1):eblksptr($)-1;
        eblks=[eblks(Ibef);blkis;eblks(Iaft)]
        eblksptr(i+1:$)=eblksptr(i+1:$)+dblkis
    end
endfunction


function [fblks,fblksptr]=update_event_activates(fblks,fblksptr,clkconnect,clkptr,bls)
    // returns the blocks activated by a block (assumed typ_l)
    nb1=size(cliptr,"*")
    fblksptr($:nb1)=fblksptr($)
    for i=bls
        f=find(clkconnect(:,1)==i)
        blkis=unique(clkconnect(f,3))
        dblkis=size(blkis,"*")-(fblksptr(i+1)-fblksptr(i))
        Ibef=fblksptr(1):fblksptr(i)-1;
        Iaft=fblksptr(i+1):fblksptr($)-1;
        fblks=[fblks(Ibef);blkis;fblks(Iaft)]
        fblksptr(i+1:$)=fblksptr(i+1:$)+dblkis
    end
endfunction


function [eblks,eblksptr]=event_depend_on(clkconnect,cliptr)
    // returns the blocks activating a block
    eblks=[];eblksptr=1
    for i=1:size(cliptr,"*")-1
        f=find(clkconnect(:,3)==i)
        blkis=unique(clkconnect(f,1))
        eblks=[eblks;blkis]
        eblksptr=[eblksptr;eblksptr($)+size(blkis,"*")]
    end
endfunction


function [fblks,fblksptr]=event_activates(clkconnect,clkptr)
    // returns the blocks activated by a block of type typ_l
    fblks=[];fblksptr=1
    for i=1:size(typ_l,"*")
        if typ_l(i) then
            f=find(clkconnect(:,1)==i)
            blkis=unique(clkconnect(f,3))
            fblks=[fblks;blkis]
        else
            blkis=[]
        end
        fblksptr=[fblksptr;fblksptr($)+size(blkis,"*")]
    end
endfunction


function uni=merge_mat(m1,m2)
    // for  m1 and m2 with two columns containing >=0 values
    if isempty(m1) & isempty(m2) then
        uni=[];
        return;
    end
    uni=[m1;m2]
    n=max(uni(:,2))+1;
    [j,ind]=unique(uni(:,1)*n+uni(:,2))
    uni=uni(-gsort(-ind),:)
endfunction

function [typ_l,clkconnect,connectmat,vbllst,dep_t,dep_u,dep_uptr,..
    corinv,clkptr,cliptr,critev]=duplicate_block(bl,typ_l,clkconnect,..
    connectmat,vbllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)

    nblock=size(typ_l,1)+1
    g=find(clkconnect(:,1)==bl)
    if g<> [] then
        xx=clkconnect(g,:)
        xx(:,1)=nblock
        clkconnect=[clkconnect;xx]
    end

    g=find(connectmat(:,3)==bl)
    xx=connectmat(g,:)
    xx(:,3)=nblock*ones(size(xx,1),1)
    connectmat=[connectmat;xx]

    typ_l($+1)=%t
    critev=[critev;critev(clkptr(bl):clkptr(bl+1)-1)]
    vbllst(nblock)=vbllst(bl)
    dep_t(nblock)=dep_t(bl)
    dep_u=[dep_u;dep_u(dep_uptr(bl))]
    dep_uptr($+1)=dep_uptr($)+1

    corinv(nblock)=corinv(bl)
    clkptr(nblock+1)=clkptr(nblock)+clkptr(bl+1)-clkptr(bl)
    cliptr(nblock+1)=cliptr(nblock)+cliptr(bl+1)-cliptr(bl)

endfunction

function [childs]=get_allchilds(primary,fblks,fblksptr,typ_l)

    oldtaille=0
    childs=primary(:)'
    taille=size(childs,"*")

    while oldtaille<>taille
        oldtaille=taille
        for i=childs(typ_l(childs))
            bb=fblks(fblksptr(i):fblksptr(i+1)-1)
            childs=union(childs,bb)
        end
        taille=size(childs,"*")
    end
endfunction


function ok=is_alg_event_loop(typ_l,clkconnect)
    clkconnect(find(clkconnect(:,1)==0),:)=[]
    lclkconnect=clkconnect(typ_l(clkconnect(:,1))&typ_l(clkconnect(:,3)),[1,3])
    n=size(typ_l,"*")
    oldvec=zeros(n,1)
    vec(lclkconnect(:,1))=1
    i=0
    ok=%f
    while i<=n
        i=i+1
        oldvec=vec  // not optimal to use large v
        if isempty(vec(lclkconnect(:,1))) then
            vec(lclkconnect(:,2))=1
        else
            vec(lclkconnect(:,2))=vec(lclkconnect(:,1))+1
        end
        if and(vec==oldvec) then
            ok=%t
            return
        end
    end
endfunction

function [ordclk,ordptr,cord,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev,ok]=paksazi2(typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)

    ordclk=[];ordptr=1;cord=[];
    lordclk=list()

    vbllst=1:length(bllst)

    zz=get_clocks(clkconnect,clkptr)
    //testing event algebraic loops
    ok=is_alg_event_loop(typ_l,clkconnect)
    if ~ok then
        disp(msprintf("%s: alg_event_loop failed", "c_pass2"));
        messagebox(_("Algebraic loop on events."),"modal","error");
        return
    end


    ok=%t
    [fblks,fblksptr]=event_activates(clkconnect,clkptr)
    [blks,blksptr]=depend_on(connectmat,dep_u,dep_uptr)
    [eblks,eblksptr]=event_depend_on(clkconnect,cliptr)


    for i=1:size(zz,1)

        nblock=size(typ_l,1);  // number of blocks in diagram

        todo=zz(i,:)
        while todo<>[]

            blk=todo(1,1);port=todo(1,2);
            if blk==0 |find(definedfields(lordclk)==clkptr(blk)+port-1)==[] then

                f=find(clkconnect(:,1)==blk&clkconnect(:,2)==port);
                if f<>[] then
                    primary0=clkconnect(f,3);prt0=clkconnect(f,4)
                else
                    primary0=[]
                end

                if  show_comment then mprintf("Processing blk " + string(blk) + " port "+...
                + string(port) + "\n"),end

                if primary0<>[] then  // delete redundant links
                    [jj,k]=unique(primary0*(1+max(prt0))+prt0)
                    index=setdiff(1:size(primary0,1),k)
                    clkconnect(f(index),:)=[]  // does not affect e(f)blks

                    primary=unique(primary0)

                    [balg,vec]=ini_ordo3(primary)
                    if balg then
                        disp(msprintf("%s: ini_ordo (3) failed", "c_pass2"));
                        messagebox(_("Algebraic loop."),"modal","error"),
                        ok=%f
                        return
                    end

                    pvec=vec(primary)+.5*typ_l(primary) // same order typ_l to the right
                    [mi,In]=gsort(-pvec)
                    primary=primary(In)
                    lp=find(typ_l(primary))
                    lprimary=primary(lp)

                    bouclalg=%f
                    lp_backw=lp($:-1:1)
                    for i=lp_backw
                        for J=primary(i+1:$)'
                            if intersect(get_allchilds(primary(i)),get_allchilds(J))<>[] then
                                bouclalg=%t
                                break
                            end
                        end
                        if bouclalg then break,end
                    end

                    if show_comment&bouclalg then mprintf("found intersect \n"),end

                    if ~bouclalg then
                        [bouclalg,Vec,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,..
                        dep_uptr,corinv,clkptr,cliptr,critev]=ini_ordo2(primary,..
                        clkconnect,connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,..
                        corinv,clkptr,cliptr,critev)
                        if bouclalg then
                            if show_comment then mprintf("found non convergence\n"),pause,end
                            i=lp(1)  // first typ_l
                            if i==[] then
                                disp(msprintf("%s: ini_ordo (2) failed", "c_pass2"));
                                messagebox(_("Algebraic loop."),"modal","error")
                                ok=%f
                                return
                            end
                            J=primary(i+1:$)
                        end
                    end

                    if bouclalg then
                        modif=%f
                        bl=primary(i)

                        nout=clkptr(bl+1)-clkptr(bl)
                        //duplicate bl if it receives other links
                        f=find(clkconnect(:,1)==blk&clkconnect(:,2)==port&clkconnect(:,3)==bl)
                        h=find(clkconnect(:,3)==bl)  // this is always one
                        if size(f,2)<>size(h,2) then
                            nblock=nblock+1
                            clkconnect(f,3)=nblock
                            if  show_comment then
                                mprintf("duplicating pivot"+string(bl)+" to obtain "+string(nblock) + "\n"),
                            end
                            [typ_l,clkconnect,connectmat,vbllst,dep_t,dep_u,dep_uptr,..
                            corinv,clkptr,cliptr,critev]=duplicate_block(bl,typ_l,clkconnect,..
                            connectmat,vbllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)
                            vec(nblock)=vec(bl)
                            //update pointer
                            blks_bl=blks(blksptr(bl):blksptr(bl+1)-1)
                            blks=[blks;blks_bl]
                            blksptr($+1)=blksptr($)+size(blks_bl,"*")
                            //
                            bls=[bl,nblock,fblks(fblksptr(bl):fblksptr(bl+1)-1)']
                            [eblks,eblksptr]=..
                            update_event_depend_on(eblks,eblksptr,clkconnect,cliptr,bls)
                            bl=nblock  // use new block as pivot
                            lprimary(i)=bl
                        end

                        for bli=J(:)'
                            modif=%t
                            f=find(clkconnect(:,1)==blk&clkconnect(:,2)==port&clkconnect(:,3)==bli)
                            clkconnect(f,1)=bl
                            clkconnect(f,2)=1
                            xx= clkconnect(f,:)
                            for jj=2:nout
                                if  show_comment then
                                    mprintf("No block duplication but link between "+string(blk)+..
                                    " and "+string(bli)+" replaced with links from "+..
                                    string(bl)+" to "+string(bli) + "\n"),
                                end
                                xx(:,2)=jj;
                                clkconnect=[clkconnect;xx]
                            end
                        end

                        [eblks,eblksptr]=update_event_depend_on(eblks,eblksptr,clkconnect,cliptr,J(:)')
                        bls=[blk,bl];bls(find(bls==0))=[];bls=bls(typ_l(bls)); //exclude non typ_l and 0
                        [fblks,fblksptr]=update_event_activates(fblks,fblksptr,clkconnect,clkptr,bls)

                        // can (must) be done outside the for loop
                        //  [fblks2,fblksptr2]=event_activates(clkconnect,clkptr)
                        //  [eblks2,eblksptr2]=event_depend_on(clkconnect,cliptr)
                        //but then it must be updated
                        //  if norm(eblksptr-eblksptr2)>0 then mprintf('roro2');pause,end
                        //  if norm(fblksptr-fblksptr2)>0 then mprintf('soso2');pause,end
                        //  if norm(eblks-eblks2)>0 then mprintf('roro');pause,end
                        //  if norm(fblks-fblks2)>0 then mprintf('soso');pause,end


                        if ~modif then messagebox(_("Algebraic loop."),"modal","error"),ok=%f,return,end
                    else
                        primary0=primary0(k);
                        prt0=prt0(k)
                        [primary0,prt0]=aggregate(primary0,prt0)
                        [mi,In]=gsort(-Vec(primary0))
                        if blk<>0 then
                            lordclk(clkptr(blk)+port-1)=[primary0(In),prt0(In)]

                            if  show_comment then
                                mprintf("for blk port "+string(blk)+" "+string(port)+..
                                " ordclk is :\n"), disp(lordclk(clkptr(blk)+port-1)),
                            end
                        else
                            cord=[primary0(In),prt0(In)]
                        end

                        todo(1,:)=[]
                        L=[];
                        for Bi=lprimary'
                            C=[1:clkptr(Bi+1)-clkptr(Bi)]'
                            L=[L;Bi*ones(C),C]
                        end
                        todo=merge_mat(todo,L)
                    end
                else
                    if blk<>0 then
                        J=clkptr(blk)+port-1
                        lordclk(J)=[]
                        if  show_comment then
                            mprintf("for blk port "+string(blk)+" "+string(port)+..
                            " ordclk is"), mprintf(lordclk(J) + "\n"),
                        end
                    else
                        cord=[]
                    end

                    todo(1,:)=[]
                end
            else
                todo(1,:)=[]
            end
        end
    end

    for J=1:clkptr($)-1
        ordclk=[ordclk;lordclk(J)]
        ordptr=[ordptr;ordptr($)+size(lordclk(J),1)]
    end

    bllst=list(bllst(vbllst))

endfunction

function  [primary0,port0]=aggregate(primary0,port0)
    primary=discardprimary([primary0,port0]) // must rewrite
    primary0=primary(:,1);port0=primary(:,2)
endfunction


function [parents]=get_parents2(parents,blk,port)
    k=size(parents,1)+1;
    f=find(clkconnect(:,3)==blk)
    n_f=size(f,2)
    if n_f>0 then
        for i=1:n_f
            if clkconnect(f(i),4)==port then
                clki1=clkconnect(f(i),1)
                clki2=clkconnect(f(i),2)
                g=find(clki1==parents(:,1))
                if g==[] | (g~=[] & parents(g,2)~=clki2) then
                    parents(k,1)=clki1
                    parents(k,2)=clki2
                    k=k+1
                end
            end
        end
    end
endfunction

//suppression des liens inutiles
function [clkconnect,amaj]=find_del_inutile(clkconnect,vec_plus,typ_l)
    amaj=[]
    for i=1:size(vec_plus,1)
        blk=vec_plus(i,1)
        port=vec_plus(i,2)
        parents=get_parents2([],blk,port)
        j=1
        done=%f
        n_p=size(parents,1)
        while j<=n_p & ~done
            par1=parents(j,1)
            if par1~=0 & typ_l(par1) then
                n_out=clkptr(par1+1)-clkptr(par1)
                f=find(par1==parents(:,1))
                if size(f,2)==n_out then
                    if show_comment then
                        mprintf("find_del_inutile:")
                        mprintf("link between blocks "+string(par1)+" and "+string(blk)+..
                        " are deleted\n")
                        pause
                    end
                    [clkconnect]=del_inutile(clkconnect,par1,n_out,blk,port)
                    done=%t
                    amaj=[amaj;par1;blk]
                end
            end
            j=j+1
        end
    end
endfunction

function [clkconnect]=del_inutile(clkconnect,blk0,n_out,blk,port)
    f=find(clkconnect(:,1)==blk0)
    del=[]
    for i=1:size(f,2)
        if clkconnect(f(i),3)==blk & clkconnect(f(i),4)==port then
            del=[del;f(i)]
        end
    end
    clkconnect(del,:)=[]
    p=get_parents([],blk0)
    n_p=size(p,1)
    for i=1:n_p
        clkconnect($+1,:)=[p(i,1),p(i,2),blk,port]
    end
endfunction


function  blk0=get_father(blk1,vectmp)
    gg=find(clkconnect(:,3)==blk1) // parents of blk1
    del=find(clkconnect(gg,1)==0)
    gg(del)=[]  // remove continuous-time source
    // it is inactive
    hh=find(vectmp(clkconnect(gg,1))>0) // keep active ones
    blk0=unique(clkconnect(gg(hh),1)) // active parent(s)
    blk0=blk0(typ_l(blk0)) // can be a vector
endfunction

function [bouclalg,vec,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,..
    dep_uptr,corinv,clkptr,cliptr,critev]=ini_ordo2(primary,..
    clkconnect,connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,corinv,clkptr,..
    cliptr,critev)

    n_p=size(primary,1)

    nblock=size(typ_l,1);
    //initialisation de vec
    //on initialise vec � -1
    vec=-ones(nblock,1)
    vec(primary(:,1))=1

    bouclalg=%f
    fromfixe=%f
    oldvec2=[]
    counter2=0

    [wprim,wprimptr,g0prim,g0primptr,gprim,gprimptr]=prim_calc(primary)

    while ~fromfixe
        vec=update_vec3(vec,wprim,wprimptr,g0prim,g0primptr,gprim,gprimptr)
        //
        if and(vec==oldvec2) then
            fromfixe=%t
        else
            oldvec2=vec
        end
        counter2=counter2+1
        if (counter2>n_p+1) then
            bouclalg=%t
            //      mprintf('Algebraic Loop detection.')
            break
        end
    end
endfunction

function [bouclalg,vec]=ini_ordo3(primary)

    n_p=size(primary,1)

    nblock=size(typ_l,1);
    //initialisation de vec
    //on initialise vec � -1
    vec=-ones(nblock,1)
    vec(primary(:,1))=0

    bouclalg=%f
    fromfixe=%f
    oldvec2=[]
    counter2=0

    [wprim,wprimptr,g0prim,g0primptr,gprim,gprimptr]=prim_calc(primary)

    while ~fromfixe
        vec=update_vec4(vec,wprim,wprimptr,g0prim,g0primptr,gprim,gprimptr)

        if and(vec==oldvec2) then
            fromfixe=%t
        else
            oldvec2=vec
        end
        counter2=counter2+1
        if (counter2>n_p+1) then
            bouclalg=%t
            //      mprintf('Algebraic Loop detection.')
            break
        end
    end
endfunction


function  vec=update_vec3(vec,wprim,wprimptr,g0prim,g0primptr,gprim,gprimptr)
    for i=1:size(wprimptr,"*")-1
        w=wprim(wprimptr(i):wprimptr(i+1)-1)
        g0=g0prim(g0primptr(i):g0primptr(i+1)-1)
        g=gprim(gprimptr(i):gprimptr(i+1)-1)
        // detecting algebraic loops within a single cluster
        //mprintf([px,py,size(blks,'*')])
        vec(w)=max(vec(w))
        if (g0 ~= []) then
            vec(w(1))=max(vec(w(1)),max(vec(g0))+1)
        end
        //g=g(find(vec(g)>-1))  //not optimal  test max(vec(g))
        if g~=[] then
            vec(w)=vec(w)-1
            vec(w)=max(vec(w),max(vec(g)))+1
        end
    end
endfunction

function  vec=update_vec4(vec,wprim,wprimptr,g0prim,g0primptr,gprim,gprimptr)
    for i=1:size(wprimptr,"*")-1
        w=wprim(wprimptr(i):wprimptr(i+1)-1)
        g0=g0prim(g0primptr(i):g0primptr(i+1)-1)
        if g0<>[] then vec(w(1))=max(vec(w(1)),max(vec(g0))), end
        vec(w(2:$))=vec(w(1))+1
    end
endfunction

function [wprim,wprimptr,g0prim,g0primptr,gprim,gprimptr]=prim_calc(primary)
    wprim=[];wprimptr=1
    gprim=[];gprimptr=1
    g0prim=[];g0primptr=1


    for i=1:size(primary,"*")
        w0=primary(i)
        if typ_l(w0) then
            w=get_allchilds(w0,fblks,fblksptr,typ_l)
            j=find(w==w0)
            w([1,j])=w([j,1])  // put w0 on top
        else
            w=w0;
        end
        wprim=[wprim;w(:)]
        wprimptr($+1)=wprimptr($)+size(w,"*")

        px=blksptr(w(1));py=blksptr(w(1)+1)-1
        //mprintf([px,py,size(blks,'*')])
        g0prim=[g0prim;blks(px:py)]
        g0primptr($+1)=g0primptr($)+size(px:py,2)

        g=[]
        for i1=2:size(w,"*") // 1 is done already
            px=blksptr(w(i1));py=blksptr(w(i1)+1)-1
            g=[g;blks(px:py)]
        end
        gprim=[gprim;g]
        gprimptr($+1)=gprimptr($)+size(g,"*")
    end
endfunction

function primary=discardprimary(primary)
    // discard
    mma=max(primary(:,2))+1
    con=mma*primary(:,1)+primary(:,2)
    [junk,ind]=gsort(-con);con=-junk
    primary=primary(ind,:)
    // discard duplicate calls to the same block port
    if size(con,"*")>=2 then
        primary(find(con(2:$)-con(1:$-1)==0),:)=[]
    end
    // group calls to different ports of the same block.
    primary=[primary(:,1),2^(primary(:,2)-ones(primary(:,2)))]
    primary=int(primary)
    con=primary(:,1)
    clkconnectjj=[]
    if size(con,"*")>=2 then
        iini=[find(con(2:$)-con(1:$-1)<>0),size(primary,1)]
    else
        iini=1
    end
    for ii=iini
        clkconnectjj=[clkconnectjj;[primary(ii,1),..
        mysum(primary(find(primary(:,1)==primary(ii,1)),2))]]
    end
    primary=clkconnectjj
endfunction




function [ordclk,iord,oord,zord,typ_z,ok]=scheduler(inpptr,outptr,clkptr,execlk_cons,..
    outoin,outoinptr,evoutoin,evoutoinptr,typ_z,typ_x,typ_l,bexe,boptr,..
    blnk,blptr,ordclk,ordptr,cord,dep_t,dep_u,dep_uptr);

    nblk=size(typ_x,1)
    //compute iord
    if execlk_cons<>[] then
        vec=-ones(1,nblk)
        no_tu_dep=execlk_cons(:,1)
        wec=zeros(1,nblk)
        wec(no_tu_dep')=execlk_cons(:,2)'
        vec(no_tu_dep)=0*no_tu_dep'
        [r,ok]=newc_tree2(vec,outoin,outoinptr,dep_u,dep_uptr)

        iord=[r,wec(r)']
    else
        iord=[]
    end
    //
    if ~ok then
        disp(msprintf("%s: scheduling failed", "c_pass2"));
        messagebox(_("Algebraic loop."),"modal","error");
        iord=[],oord=[],zord=[],critev=[]
        return,
    end

    n=size(cord,1)
    vec=-ones(1,nblk);
    vec(cord(:,1))=0;
    typp=zeros(typ_l);typp(typ_l)=1

    ext_cord1=cord;
    j=1
    while %t
        ii=ext_cord1(j,1)
        if typ_l(ii)
            for i=[clkptr(ii):clkptr(ii+1)-1]
                ext_cord1=[ext_cord1;ordclk([ordptr(i):ordptr(i+1)-1],:)];
            end
        end
        j=j+1
        if j>size(ext_cord1,1) then break;end
    end
    // code to replace faulty unique which reorders
    yy=ext_cord1(:,1)'
    [xx,kkn]=unique(yy);
    ext_cord=yy(-gsort(-kkn))
    //ext_cord=unique(ext_cord1(:,1)');
    //for i=ext_cord
    //  if typ_l(i) then typ_z(i)=clkptr(i+1)-clkptr(i)-1;end
    //end  // adding zero crossing surfaces to cont. time synchros

    //a supprimer

    [ext_cord_old,ok]=newc_tree3(vec,dep_u,dep_uptr,typp);

    if or(gsort(ext_cord_old)<>gsort(ext_cord)) then pause,end
    //
    //pour mettre a zero les typ_z qui ne sont pas dans ext_cord
    //noter que typ_z contient les tailles des nzcross (peut etre >1)
    typ_z(ext_cord)=-typ_z(ext_cord)
    typ_z=-min(typ_z,0)

    if ~ok then mprintf("serious bug, report.");pause;end
    // ext_cord=ext_cord(n+1:$);

    typ_z_save=typ_z

    fin=0
    while ~fin
        fin=1
        for i=ext_cord($:-1:1)
            for ii=[outoin(outoinptr(i):outoinptr(i+1)-1,1)',..
                evoutoin(evoutoinptr(i):evoutoinptr(i+1)-1,1)']
                //ii est un block affecte par changement de sortie du
                //i-eme block de oord
                if typ_z(ii) then
                    if typ_z(i)==0 then typ_z(i)=1;fin=0;end
                end
                if typ_x(ii) then
                    if ~typ_x(i) then typ_x(i)=%t;fin=0;end
                end
                if typ_z(i)&typ_x(i) then break,end
            end
        end
    end
    //supprimer les blocks a supprimer
    ind=find(typ_z(cord(:,1)));
    zord=cord(ind,:)
    ind=find(typ_x(cord(:,1)));
    oord=cord(ind,:)
    typ_z=typ_z_save
    //critev: vecteur indiquant si evenement est important pour tcrit
    //Donc les blocks indiques sont des blocks susceptibles de produire
    //des discontinuites quand l'evenement se produit
    if isempty(ext_cord1) then
        if isempty(ordclk) then
            maX=1;
        else
            maX=max(ordclk(:,2))+1;
        end
        cordX = [];
    else
        maX=max([ext_cord1(:,2);ordclk(:,2)])+1;
        cordX=ext_cord1(:,1)*maX+ext_cord1(:,2);
    end

    // 1: important; 0:non
    //n=clkptr(nblk+1)-1 //nb d'evenement
    n=size(ordptr,"*")-1 //nb d'evenement

    //a priori tous les evenements sont non-importants
    //critev=zeros(n,1)
    for i=1:n
        for hh=ordptr(i):ordptr(i+1)-1
            jj= ordclk(hh,1) //block excite par evenement i
            //Following line is not correct because of ever active synchros
            if or(jj*maX+ordclk(hh,2)==cordX) then
                ordclk(hh,2)=-ordclk(hh,2)
            end
        end
    end
endfunction

function [ord,ok]=tree3(vec,dep_ut,typ_l)
    //compute blocks execution tree
    ok=%t
    nb=size(vec,"*")
    for j=1:nb+2
        fini=%t
        for i=1:nb
            if vec(i)==j-1&typ_l(i)<>-1 then
                if j==nb+2 then
                    disp(msprintf("%s: tree (3) failed", "c_pass2"));
                    messagebox(_("Algebraic loop."),"modal","error");ok=%f;ord=[];return;
                end
                if typ_l(i)==1 then
                    fini=%f;
                    kk=bexe(boptr(i):boptr(i+1)-1)';
                else
                    kk=[];
                    for ii=blnk(blptr(i):blptr(i+1)-1)'
                        if vec(ii)>-1 & (dep_ut(ii,1) | (typ_l(ii)==1)) then
                            fini=%f;
                            kk=[kk ii];
                        end
                    end
                end
                vec(kk)=j*ones(kk) ;   //mprintf(vec)
            end
        end
        if fini then break;end
    end
    [k,ord]=gsort(-vec);
    ord(find(k==1))=[];
endfunction

function [clkconnectj_cons]=discard(clkptr,cliptr,clkconnect,exe_cons)

    if exe_cons<>[] then
        clkconnectj=exe_cons
        mma=max(clkconnectj(:,2))+1
        con=mma*(clkconnectj(:,1))+clkconnectj(:,2)
        [junk,ind]=gsort(-con);con=-junk
        clkconnectj=clkconnectj(ind,:)
        // discard duplicate calls to the same block port
        if size(con,"*")>=2 then
            clkconnectj(find(con(2:$)-con(1:$-1)==0),:)=[]
        end
        // group calls to different ports of the same block.
        clkconnectj=[clkconnectj(:,1),2^(clkconnectj(:,2)-ones(clkconnectj(:,2)))]
        clkconnectj=int(clkconnectj)
        con=clkconnectj(:,1)
        clkconnectj_cons=[]
        if size(con,"*")>=2 then
            iini=[find(con(2:$)-con(1:$-1)<>0),size(clkconnectj,1)]
        else
            iini=1
        end
        for ii=iini
            clkconnectj_cons=[clkconnectj_cons;[clkconnectj(ii,1),..
            mysum(clkconnectj(find(clkconnectj(:,1)==clkconnectj(ii,1)),2))]]
        end
    else
        clkconnectj_cons=[]
    end
endfunction

function a=mysum(b)
    if b<>[] then a=sum(b), else a=[], end
endfunction

function [lnksz,lnktyp,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,xptr,zptr,..
    ozptr,typ_mod,rpptr,ipptr,opptr,xc0,xcd0,xd0,oxd0,rpar,..
    ipar,opar,typ_z,typ_x,typ_m,funs,funtyp,initexe,labels,uids,..
    bexe,boptr,blnk,blptr,ok]=extract_info(bllst,connectmat,clkconnect,typ_l)

    ok=%t
    nbl=length(bllst)
    clkptr=zeros(nbl+1,1);clkptr(1)=1
    cliptr=clkptr;inpptr=cliptr;outptr=inpptr;
    xptr=1;zptr=1;ozptr=1;
    rpptr=clkptr;ipptr=clkptr;opptr=clkptr;
    //
    xc0=[];xcd0=[];xd0=[];
    oxd0=list()
    rpar=[];ipar=[];
    opar=list();

    fff=ones(nbl,1)==1
    typ_z=zeros(nbl,1);typ_x=fff;typ_m=fff;typ_mod=zeros(nbl,1);

    initexe=[];
    funs=list();
    funtyp=zeros(typ_z)
    labels=[]
    uids=[]
    [ok,bllst]=adjust_inout(bllst,connectmat)
    if ok then
        [ok,bllst]=adjust_typ(bllst,connectmat)
    end

    // placed here to make sure nzcross and nmode correctly updated
    if ~ok then
        lnksz=[],lnktyp=[],inplnk=[],outlnk=[],clkptr=[],cliptr=[],inpptr=[],outptr=[],..
        xptr=[],zptr=[],ozptr=[],rpptr=[],ipptr=[],opptr=[],xc0=[],..
        xcd0=[],xd0=[],oxd0=list(),rpar=[],ipar=[],opar=list(),..
        typ_z=[],typ_x=[],typ_m=[],funs=[],funtyp=[],initexe=[],..
        labels=[],uids=[],bexe=[],boptr=[],blnk=[],blptr=[]
        return;
    end
    for i=1:nbl
        ll=bllst(i)

        if type(ll.sim)==15 then
            funs(i)=ll.sim(1)
            funtyp(i,1)=ll.sim(2)
        else
            funs(i)=ll.sim;
            funtyp(i,1)=0;
        end
        if funtyp(i,1)>999&funtyp(i,1)<10000 then
            if ~c_link(funs(i)) then
                msg = _("A C or Fortran block is used but not linked.\nYou can save your compiled diagram and load it.\nThis will automatically link the C or Fortran function.")
                messagebox(msprintf(msg), "modal", "error")
            end
        end
        inpnum=ll.in;outnum=ll.out;cinpnum=ll.evtin;coutnum=ll.evtout;
        //
        inpptr(i+1)=inpptr(i)+size(inpnum,"*")
        outptr(i+1)=outptr(i)+size(outnum,"*")
        cliptr(i+1)=cliptr(i)+size(cinpnum,"*")
        clkptr(i+1)=clkptr(i)+size(coutnum,"*")
        //
        X0=ll.state(:)
        if funtyp(i,1)<10000 then
            xcd0=[xcd0;0*X0]
            xc0=[xc0;X0]
            xptr(i+1)=xptr(i)+size(ll.state,"*")
        else
            xcd0=[xcd0;X0($/2+1:$)]
            xc0=[xc0;X0(1:$/2)]
            xptr(i+1)=xptr(i)+size(ll.state,"*")/2
        end

        //dstate
        if (funtyp(i,1)==3 | funtyp(i,1)==5 | funtyp(i,1)==10005) then //sciblocks
            if ll.dstate==[] then xd0k=[]; else xd0k=var2vec(ll.dstate);end
        else
            xd0k=ll.dstate(:)
        end
        xd0=[xd0;xd0k]
        zptr(i+1)=zptr(i)+size(xd0k,"*")

        //odstate
        if type(ll.odstate)==15 then
            if ((funtyp(i,1)==5) | (funtyp(i,1)==10005)) then //sciblocks : don't extract
                if lstsize(ll.odstate)>0 then
                    oxd0($+1)=ll.odstate
                    ozptr(i+1)=ozptr(i)+1;
                else
                    ozptr(i+1)=ozptr(i);
                end
            elseif ((funtyp(i,1)==4)    | (funtyp(i,1)==10004) |...
                (funtyp(i,1)==2004) | (funtyp(i,1)==12004))  //C blocks : extract
                ozsz=lstsize(ll.odstate);
                if ozsz>0 then
                    for j=1:ozsz, oxd0($+1)=ll.odstate(j), end;
                    ozptr(i+1)=ozptr(i)+ozsz;
                else
                    ozptr(i+1)=ozptr(i);
                end
            else
                ozptr(i+1)=ozptr(i);
            end
        else
            //add an error message here please !
            ozptr(i+1)=ozptr(i);
        end

        //mod
        typ_mod(i)=ll.nmode;
        if typ_mod(i)<0 then
            messagebox(msprintf(_("Number of modes in block #%d cannot be determined."),i),"modal","error")
            ok=%f
        end

        //real paramaters
        rpark=ll.rpar(:)
        rpar=[rpar;rpark]
        rpptr(i+1)=rpptr(i)+size(rpark,"*")

        //integer parameters
        if type(ll.ipar)==1 then
            ipar=[ipar;ll.ipar(:)]
            ipptr(i+1)=ipptr(i)+size(ll.ipar,"*")
        else
            ipptr(i+1)=ipptr(i)
        end

        //object parameters
        if type(ll.opar)==15 then
            if ((funtyp(i,1)==5) | (funtyp(i,1)==10005)) then //sciblocks : don't extract
                if lstsize(ll.opar)>0 then
                    opar($+1)=ll.opar
                    opptr(i+1)=opptr(i)+1;
                else
                    opptr(i+1)=opptr(i);
                end
            elseif ((funtyp(i,1)==4)    | (funtyp(i,1)==10004) |...
                (funtyp(i,1)==2004) | (funtyp(i,1)==12004)) then //C blocks : extract
                oparsz=lstsize(ll.opar);
                if oparsz>0 then
                    for j=1:oparsz, opar($+1)=ll.opar(j), end;
                    opptr(i+1)=opptr(i)+oparsz;
                else
                    opptr(i+1)=opptr(i);
                end
            else
                opptr(i+1)=opptr(i);
            end
        else
            //add an error message here please !
            opptr(i+1)=opptr(i);
        end

        //    typ_z(i)=ll.blocktype=='z'
        typ_z(i)=ll.nzcross
        if typ_z(i)<0 then
            messagebox(msprintf(_("Number of zero crossings in block #%d cannot be determined."),i),"modal","error")
            ok=%f
        end
        typ_x(i)=ll.state<>[]|ll.blocktype=="x" // some blocks like delay
        // need to be in oord even
        // without state

        typ_m(i)=ll.blocktype=="m"
        //
        if ll.evtout<>[] then
            ll11=ll.firing
            prt=find(ll11>=zeros(ll11))
            nprt=prod(size(prt))
            initexe=[initexe;[i*ones(nprt,1),matrix(prt,nprt,1),matrix(ll11(prt),nprt,1)]];
        end

        if type(ll.label)==10 then
            labels=[labels;ll.label(1)]
        else
            labels=[labels;" "]
        end

        if type(ll.uid)==10 then
            uids=[uids;ll.uid(1)]
        else
            uids=[uids;" "]
        end
    end

    clkconnect=clkconnect(find(clkconnect(:,1)<>0),:);

    if isempty(clkconnect) then
        con=-1;
    else
        con=clkptr(clkconnect(:,1))+clkconnect(:,2)-1;
    end
    [junk,ind]=gsort(-con);con=-junk;
    clkconnect=clkconnect(ind,:);
    //
    bclkconnect=clkconnect(:,[1 3]);
    boptr=1;
    bexe=[];
    for i=1:nbl
        r=bclkconnect(find(bclkconnect(:,1)==i),2);
        bexe=[bexe;r];
        boptr=[boptr;boptr($)+size(r,1)];
    end
    //

    blptr=1;
    blnk=[];

    for i=1:nbl
        r=connectmat(find(connectmat(:,1)==i),3:4);
        blnk=[blnk;r];
        blptr=[blptr;blptr($)+size(r,1)];
    end
    //

    nlnk=size(connectmat,1)
    inplnk=zeros(inpptr($)-1,1);outlnk=zeros(outptr($)-1,1);ptlnk=1;
    lnksz=[];lnktyp=[];
    for jj=1:nlnk
        ko=outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)
        ki=inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)
        if ko<>0 & ki<>0 then
            if ko>ki then
                outlnk(outlnk>ko)=outlnk(outlnk>ko)-1
                outlnk(outlnk==ko)=ki
                inplnk(inplnk>ko)=inplnk(inplnk>ko)-1
                inplnk(inplnk==ko)=ki
                ptlnk=-1+ptlnk
                lnksz(ko,1)=[];
                lnksz(ko,2)=[];
                lnktyp(ko) =[];
            elseif ki>ko
                outlnk(outlnk>ki)=outlnk(outlnk>ki)-1
                outlnk(outlnk==ki)=ko
                inplnk(inplnk>ki)=inplnk(inplnk>ki)-1
                inplnk(inplnk==ki)=ko
                ptlnk=-1+ptlnk
                lnksz(ki,1)=[];
                lnksz(ki,2)=[];
                lnktyp(ki) =[];
            end
        elseif ko<>0 then
            inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)=ko
        elseif ki<>0 then
            outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)=ki
        else
            outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)=ptlnk
            inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)=ptlnk
            lnksz(ptlnk,1)=bllst(connectmat(jj,1)).out(connectmat(jj,2));
            lnksz(ptlnk,2)=bllst(connectmat(jj,1)).out2(connectmat(jj,2));
            lnktyp(ptlnk)=bllst(connectmat(jj,1)).outtyp(connectmat(jj,2));
            ptlnk=1+ptlnk
        end
    end

    //store unconnected outputs, if any, at the end of outtb
    unco=find(outlnk==0);
    for j=unco
        m=max(find(outptr<=j))
        n=j-outptr(m)+1
        nm=bllst(m).out(n)
        if nm<1 then
            under_connection(corinv(m),n,nm,-1,0,0,1),ok=%f,return,
        end
        nm2=bllst(m).out2(n)
        if nm2<1 then
            under_connection(corinv(m),n,nm2,-1,0,0,1),ok=%f,return,
        end
        lnksz($+1,1)=bllst(m).out(n);
        lnksz($,2)=bllst(m).out2(n);
        lnktyp($+1)=bllst(m).outtyp(n);
        outlnk(j)=max(outlnk)+1
    end

    //store unconnected inputs, if any, at the end of outtb
    unco=find(inplnk==0);
    for j=unco
        m=max(find(inpptr<=j))
        n=j-inpptr(m)+1
        nm=bllst(m).in(n)
        if nm<1 then
            under_connection(corinv(m),n,nm,-2,0,0,1),ok=%f,return,
        end
        nm2=bllst(m).in2(n)
        if nm2<1 then
            under_connection(corinv(m),n,nm2,-2,0,0,1),ok=%f,return,
        end
        lnksz($+1,1)=bllst(m).in(n);
        lnksz($,2)=bllst(m).in2(n);
        lnktyp($+1)=bllst(m).intyp(n);
        inplnk(j)=max([inplnk;max(outlnk)])+1
    end

endfunction

function [outoin,outoinptr]=conn_mat(inpptr,outptr,inplnk,outlnk)
    outoin=[];outoinptr=1
    nblk=size(inpptr,1)-1
    for i=1:nblk
        k=outptr(i):outptr(i+1)-1
        ii=[]
        for j=outlnk(k)'
            ii=[ii,find(inplnk==j)]
        end
        outoini=[];jj=0
        for j=ii
            m=max(find(inpptr<=j))
            n=j-inpptr(m)+1
            outoini=[outoini;[m,n]]
            jj=jj+1
        end
        outoinptr=[outoinptr;outoinptr($)+jj]
        outoin=[outoin;outoini]
    end
endfunction

function [ord,ok]=tree2(vec,outoin,outoinptr,dep_ut)
    //compute blocks execution tree
    ok=%t;

    nb=size(vec,"*");
    for j=1:nb+2
        fini=%t
        for i=1:nb
            if vec(i)==j-1 then
                if j==nb+2 then
                    disp(msprintf("%s: tree (2) failed", "c_pass2"));
                    messagebox(_("Algebraic loop."),"modal","error");ok=%f;ord=[];return;
                end
                for k=outoinptr(i):outoinptr(i+1)-1
                    ii=outoin(k,1);
                    //indport=find(dep_u(dep_uptr(ii):dep_uptr(ii+1)-1)==1);
                    //if (indport ~=[] &vec(ii)>-1) then
                    if (dep_ut(ii) &vec(ii)>-1) then
                        fini=%f;
                        vec(ii)=j;
                    end
                end
            end
        end
        if fini then break;end
    end

    [k,ord]=gsort(-vec);
    ord(find(k==1))=[];
    ord=ord(:)
endfunction


//adjust_inout : it resolves positive, negative and null size
//               of in/out port dimensions of connected block.
//               If it's not done in a first pass, the second
//               pass try to resolve negative or null port
//               dimensions by asking user to informed dimensions
//               with underconnection function.
//               It is a fixed point algorithm.
//
//in parameters  : bllst : list of blocks
//
//                 connectmat : matrix of connection
//                              connectmat(lnk,1) : source block
//                              connectmat(lnk,2) : source port
//                              connectmat(lnk,3) : target block
//                              connectmat(lnk,4) : target port
//
//out parameters : ok : a boolean flag to known if adjust_inout have
//                      succeeded to resolve the in/out port size
//                      - ok = %t : all size have been resolved in bllst
//                      - ok = %f : problem in size adjustement
//
//                 bllst : modified list of blocks
//
//18/05/06, Alan  : improvement in order to take into
//                  account two dimensional port size.
//
//28/12/06, Alan : type for source port and target port must
//                 be the same.
//
//29/12/06, Fady : type for source and target can be different
//                 in one condition that they are double and complex.
//                 the result on the link will be complex.
//
//04/01/07, Fady : Can test the case of negatives equals target's dimensions.
//
//19/01/07, Alan : - Return correct information for user in editor
//                   with preceding test of Fady in the first pass
//                 - Second pass reviewed : under_connection returns two dimensions now
//
//10/05/07, Alan : - if-then-else event-select case

function [ok,bllst]=adjust_inout(bllst,connectmat)

    //Adjust in2/out2, inttyp/outtyp
    //in accordance to in/out in bllst
    [ko,bllst]=adjust_in2out2(bllst);
    if ~ko then ok=%f,return, end //if adjust_in2out2 failed then exit
    //adjust_inout with flag ok=%f

    nlnk=size(connectmat,1) //nlnk is the number of link

    //loop on number of block (pass 1 and pass 2)
    for hhjj=1:length(bllst)+1
        //%%%%% pass 1 %%%%%//
        for hh=1:length(bllst)+1 //second loop on number of block
            ok=%t
            for jj=1:nlnk //loop on number of link

                //intyp/outtyp are the type of the
                //target port and the source port of the observed link
                outtyp = bllst(connectmat(jj,1)).outtyp(connectmat(jj,2))
                intyp = bllst(connectmat(jj,3)).intyp(connectmat(jj,4))
                //nnin/nnout are the size (two dimensions) of the
                //target port and the source port of the observed link
                //before adjust
                nnout(1,1)=bllst(connectmat(jj,1)).out(connectmat(jj,2))
                nnout(1,2)=bllst(connectmat(jj,1)).out2(connectmat(jj,2))
                nnin(1,1)=bllst(connectmat(jj,3)).in(connectmat(jj,4))
                nnin(1,2)=bllst(connectmat(jj,3)).in2(connectmat(jj,4))

                //This Part is done in adjust_typ

                //check intyp/outtyp
                //            if intyp<>outtyp then
                //              if (intyp==1 & outtyp==2) then
                //                bllst(connectmat(jj,3)).intyp(connectmat(jj,4))=2;
                //              elseif (intyp==2 & outtyp==1) then
                //                bllst(connectmat(jj,1)).outtyp(connectmat(jj,2))=2;
                //              else
                //                if bllst(connectmat(jj,3)).sim(2)<0 //if-then-else/eselect case
                //                  bllst(connectmat(jj,3)).intyp(connectmat(jj,4))=...
                //                    bllst(connectmat(jj,1)).outtyp(connectmat(jj,2))
                //                else
                //                  bad_connection(corinv(connectmat(jj,1)),connectmat(jj,2),..
                //                                 nnout,outtyp,..
                //                                 corinv(connectmat(jj,3)),connectmat(jj,4),..
                //                                 nnin,intyp,1)
                //                  ok=%f;
                //                  return
                //                end
                //              end
                //            end

                //loop on the two dimensions of source/target port
                for ndim=1:2
                    //check test source/target sizes
                    //in case of negatif equal target dimensions
                    //nin/nout are the size (two dimensions) of the
                    //target port and the source port of the observed link
                    nout(1,1)=bllst(connectmat(jj,1)).out(connectmat(jj,2))
                    nout(1,2)=bllst(connectmat(jj,1)).out2(connectmat(jj,2))
                    nin(1,1)=bllst(connectmat(jj,3)).in(connectmat(jj,4))
                    nin(1,2)=bllst(connectmat(jj,3)).in2(connectmat(jj,4))

                    //first case : dimension of source and
                    //             target ports are explicitly informed
                    //             informed with positive size
                    if(nout(1,ndim)>0&nin(1,ndim)>0) then
                        //if dimension of source and target port doesn't match
                        //then call bad_connection, set flag ok to false and exit
                        if nin(1,ndim)<>nout(1,ndim) then
                            bad_connection(corinv(connectmat(jj,1)),connectmat(jj,2),..
                            nnout,outtyp,..
                            corinv(connectmat(jj,3)),connectmat(jj,4),..
                            nnin,intyp)
                            ok=%f;return
                        end

                        //second case : dimension of source port is
                        //              positive and dimension of
                        //              target port is negative
                    elseif(nout(1,ndim)>0&nin(1,ndim)<0) then
                        //find vector of input ports of target block with
                        //first/second dimension equal to size nin(1,ndim)
                        //and assign it to nout(1,ndim)
                        ww=find(bllst(connectmat(jj,3)).in==nin(1,ndim))
                        bllst(connectmat(jj,3)).in(ww)=nout(1,ndim)
                        ww=find(bllst(connectmat(jj,3)).in2==nin(1,ndim))
                        bllst(connectmat(jj,3)).in2(ww)=nout(1,ndim)

                        //find vector of output ports of target block with
                        //first/second dimension equal to size nin(1,ndim)
                        //and assign it to nout(1,ndim)
                        ww=find(bllst(connectmat(jj,3)).out==nin(1,ndim))
                        bllst(connectmat(jj,3)).out(ww)=nout(1,ndim)
                        ww=find(bllst(connectmat(jj,3)).out2==nin(1,ndim))
                        bllst(connectmat(jj,3)).out2(ww)=nout(1,ndim)

                        //find vector of output ports of target block with
                        //ndim dimension equal to zero and sum the ndim
                        //dimension of all input ports of target block
                        //to be the new dimension of the ndim dimension
                        //of the output ports of the target block
                        if ndim==1 then
                            ww=find(bllst(connectmat(jj,3)).out==0)
                            if (ww<>[]&min(bllst(connectmat(jj,3)).in(:))>0) then
                                bllst(connectmat(jj,3)).out(ww)=sum(bllst(connectmat(jj,3)).in(:))
                            end
                        elseif ndim==2 then
                            ww=find(bllst(connectmat(jj,3)).out2==0)
                            if (ww<>[]&min(bllst(connectmat(jj,3)).in2(:))>0) then
                                bllst(connectmat(jj,3)).out2(ww)=sum(bllst(connectmat(jj,3)).in2(:))
                            end
                        end

                        //if nzcross of the target block match with
                        //the negative dimension nin(1,ndim) then
                        //adjust it to nout(1,ndim)
                        if bllst(connectmat(jj,3)).nzcross==nin(1,ndim) then
                            bllst(connectmat(jj,3)).nzcross=nout(1,ndim)
                        end
                        //if nmode of the target block match with
                        //the negative dimension nin(1,ndim) then
                        //adjust it to nout(1,ndim)
                        if bllst(connectmat(jj,3)).nmode==nin(1,ndim) then
                            bllst(connectmat(jj,3)).nmode=nout(1,ndim)
                        end

                        //third case : dimension of source port is
                        //             negative and dimension of
                        //             target port is positive
                    elseif(nout(1,ndim)<0&nin(1,ndim)>0) then
                        //find vector of output ports of source block with
                        //first/second dimension equal to size nout(1,ndim)
                        //and assign it to nin(1,ndim)
                        ww=find(bllst(connectmat(jj,1)).out==nout(1,ndim))
                        bllst(connectmat(jj,1)).out(ww)=nin(1,ndim)
                        ww=find(bllst(connectmat(jj,1)).out2==nout(1,ndim))
                        bllst(connectmat(jj,1)).out2(ww)=nin(1,ndim)

                        //find vector of input ports of source block with
                        //first/second dimension equal to size nout(1,ndim)
                        //and assign it to nin(1,ndim)
                        ww=find(bllst(connectmat(jj,1)).in==nout(1,ndim))
                        bllst(connectmat(jj,1)).in(ww)=nin(1,ndim)
                        ww=find(bllst(connectmat(jj,1)).in2==nout(1,ndim))
                        bllst(connectmat(jj,1)).in2(ww)=nin(1,ndim)

                        //find vector of input ports of source block with
                        //ndim dimension equal to zero and sum the ndim
                        //dimension of all output ports of source block
                        //to be the new dimension of the ndim dimension
                        //of the input ports of the source block
                        if ndim==1 then
                            ww=find(bllst(connectmat(jj,1)).in==0)
                            if (ww<>[]&min(bllst(connectmat(jj,1)).out(:))>0) then
                                bllst(connectmat(jj,1)).in(ww)=sum(bllst(connectmat(jj,1)).out(:))
                            end
                        elseif ndim==2 then
                            ww=find(bllst(connectmat(jj,1)).in2==0)
                            if (ww<>[]&min(bllst(connectmat(jj,1)).out2(:))>0) then
                                bllst(connectmat(jj,1)).in2(ww)=sum(bllst(connectmat(jj,1)).out2(:))
                            end
                        end

                        //if nzcross of the source block match with
                        //the negative dimension nout(1,ndim) then
                        //adjust it to nin(1,ndim)
                        if bllst(connectmat(jj,1)).nzcross==nout(1,ndim) then
                            bllst(connectmat(jj,1)).nzcross=nin(1,ndim)
                        end
                        //if nmode of the source block match with
                        //the negative dimension nout(1,ndim) then
                        //adjust it to nin(1,ndim)
                        if bllst(connectmat(jj,1)).nmode==nout(1,ndim) then
                            bllst(connectmat(jj,1)).nmode=nin(1,ndim)
                        end

                        //fourth case : a dimension of source port is
                        //              null
                    elseif(nout(1,ndim)==0) then
                        //set ww to be the vector of size of the ndim
                        //dimension of input port of the source block
                        if ndim==1 then
                            ww=bllst(connectmat(jj,1)).in(:)
                        elseif ndim==2 then
                            ww=bllst(connectmat(jj,1)).in2(:)
                        end

                        //test if all size of the ndim dimension of input
                        //port of the source block is positive
                        if min(ww)>0 then
                            //test if the dimension of the target port
                            //is positive
                            if nin(1,ndim)>0 then

                                //if the sum of the size of the ndim dimension of the input
                                //port of the source block is equal to the size of the ndim dimension
                                //of the target port, then the size of the ndim dimension of the source
                                //port is equal to nin(1,ndim)
                                if sum(ww)==nin(1,ndim) then
                                    if ndim==1 then
                                        bllst(connectmat(jj,1)).out(connectmat(jj,2))=nin(1,ndim)
                                    elseif ndim==2 then
                                        bllst(connectmat(jj,1)).out2(connectmat(jj,2))=nin(1,ndim)
                                    end
                                    //else call bad_connection, set flag ok to false and exit
                                else
                                    bad_connection(corinv(connectmat(jj,1)),0,0,1,-1,0,0,1)
                                    ok=%f;return
                                end

                                //if the ndim dimension of the target port is negative
                                //then the size of the ndim dimension of the source port
                                //is equal to the sum of the size of the ndim dimension
                                //of input ports of source block, and flag ok is set to false
                            else
                                if ndim==1 then
                                    bllst(connectmat(jj,1)).out(connectmat(jj,2))=sum(ww)
                                elseif ndim==2 then
                                    bllst(connectmat(jj,1)).out2(connectmat(jj,2))=sum(ww)
                                end
                                ok=%f
                            end

                        else
                            //set nww to be the vector of all negative size of input ports
                            //of the source block
                            nww=ww(find(ww<0))

                            //if all negative size have same size and if size of the
                            //ndim dimension of the target port is positive then assign
                            //size of the ndim dimension of the source port to nin(1,ndim)
                            if norm(nww-nww(1),1)==0 & nin(1,ndim)>0 then
                                if ndim==1 then
                                    bllst(connectmat(jj,1)).out(connectmat(jj,2))=nin(1,ndim)
                                elseif ndim==2 then
                                    bllst(connectmat(jj,1)).out2(connectmat(jj,2))=nin(1,ndim)
                                end

                                //compute a size to be the difference between the size
                                //of the ndim dimension of the target block and sum of positive
                                //size of input ports of the source block divided by the number
                                //of input ports of source block with same negative size
                                k=(nin(1,ndim)-sum(ww(find(ww>0))))/size(nww,"*")

                                //if this size is a positive integer then assign it
                                //to the size of the ndim dimension of input ports of the
                                //source block which have negative size
                                if k==int(k)&k>0 then
                                    if ndim==1 then
                                        bllst(connectmat(jj,1)).in(find(ww<0))=k
                                    elseif ndim==2 then
                                        bllst(connectmat(jj,1)).in2(find(ww<0))=k
                                    end
                                    //else call bad_connection, set flag ok to false and exit
                                else
                                    bad_connection(corinv(connectmat(jj,1)),0,0,1,-1,0,0,1)
                                    ok=%f;return
                                end

                                //set flag ok to false
                            else
                                ok=%f
                            end

                        end

                        //fifth case : a dimension of target port is
                        //             null
                    elseif(nin(1,ndim)==0) then
                        //set ww to be the vector of size of the ndim
                        //dimension of output port of the target block
                        if ndim==1 then
                            ww=bllst(connectmat(jj,3)).out(:)
                        elseif ndim==2 then
                            ww=bllst(connectmat(jj,3)).out2(:)
                        end

                        //test if all size of the ndim dimension of output
                        //port of the target block is positive
                        if min(ww)>0 then
                            //test if the dimension of the source port
                            //is positive
                            if nout(1,ndim)>0 then

                                //if the sum of the size of the ndim dimension of the output
                                //port of the target block is equal to the size of the ndim dimension
                                //of the source port, then the size of the ndim dimension of the target
                                //port is equal to nout(1,ndim)
                                if sum(ww)==nout(1,ndim) then
                                    if ndim==1 then
                                        bllst(connectmat(jj,3)).in(connectmat(jj,4))=nout(1,ndim)
                                    elseif ndim==2 then
                                        bllst(connectmat(jj,3)).in2(connectmat(jj,4))=nout(1,ndim)
                                    end
                                    //else call bad_connection, set flag ok to false and exit
                                else
                                    bad_connection(corinv(connectmat(jj,3)),0,0,1,-1,0,0,1)
                                    ok=%f;return
                                end

                                //if the ndim dimension of the source port is negative
                                //then the size of the ndim dimension of the target port
                                //is equal to the sum of the size of the ndim dimension
                                //of output ports of target block, and flag ok is set to false
                            else
                                if ndim==1 then
                                    bllst(connectmat(jj,3)).in(connectmat(jj,4))=sum(ww)
                                elseif ndim==2 then
                                    bllst(connectmat(jj,3)).in2(connectmat(jj,4))=sum(ww)
                                end
                                ok=%f
                            end

                        else
                            //set nww to be the vector of all negative size of output ports
                            //of the target block
                            nww=ww(find(ww<0))

                            //if all negative size have same size and if size of the
                            //ndim dimension of the source port is positive then assign
                            //size of the ndim dimension of the target port to nout(1,ndim)
                            if norm(nww-nww(1),1)==0 & nout(1,ndim)>0 then
                                if ndim==1 then
                                    bllst(connectmat(jj,3)).in(connectmat(jj,4))=nout(1,ndim)
                                elseif ndim==2 then
                                    bllst(connectmat(jj,3)).in2(connectmat(jj,4))=nout(1,ndim)
                                end

                                //compute a size to be the difference between the size
                                //of the ndim dimension of the source block and sum of positive
                                //size of output ports of the target block divided by the number
                                //of output ports of target block with same negative size
                                k=(nout(1,ndim)-sum(ww(find(ww>0))))/size(nww,"*")

                                //if this size is a positive integer then assign it
                                //to the size of the ndim dimension of output ports of the
                                //target block which have negative size
                                if k==int(k)&k>0 then
                                    if ndim==1 then
                                        bllst(connectmat(jj,3)).out(find(ww<0))=k
                                    elseif ndim==2 then
                                        bllst(connectmat(jj,3)).out2(find(ww<0))=k
                                    end
                                    //else call bad_connection, set flag ok to false and exit
                                else
                                    bad_connection(corinv(connectmat(jj,3)),0,0,1,-1,0,0,1)
                                    ok=%f;return
                                end

                                //set flag ok to false
                            else
                                ok=%f
                            end

                        end

                        //sixth (& last) case : dimension of both source
                        //                      and target port are negatives
                    else
                        ok=%f //set flag ok to false
                    end
                end
            end
            if ok then return, end //if ok is set true then exit adjust_inout
        end
        //if failed then display message
        msg = _("Not enough information to find port sizes.\nI try to find the problem.")
        messagebox(msprintf(msg),"modal","info");

        //%%%%% pass 2 %%%%%//
        //Alan 19/01/07 : Warning  : Behavior have changed, To Be more Tested
        findflag=%f //set findflag to false

        for jj=1:nlnk //loop on number of block
            //nin/nout are the size (two dimensions) of the
            //target port and the source port of the observed link
            nout(1,1)=bllst(connectmat(jj,1)).out(connectmat(jj,2))
            nout(1,2)=bllst(connectmat(jj,1)).out2(connectmat(jj,2))
            nin(1,1)=bllst(connectmat(jj,3)).in(connectmat(jj,4))
            nin(1,2)=bllst(connectmat(jj,3)).in2(connectmat(jj,4))

            //loop on the two dimensions of source/target port
            //only case : target and source ports are both
            //            negatives or null
            if nout(1,1)<=0&nin(1,1)<=0 | nout(1,2)<=0&nin(1,2)<=0 then
                findflag=%t;
                //
                ninnout=under_connection(corinv(connectmat(jj,1)),connectmat(jj,2),nout(1,ndim),..
                corinv(connectmat(jj,3)),connectmat(jj,4),nin(1,ndim),1)
                //
                if size(ninnout,2) <> 2 then ok=%f;return;end
                if ninnout==[] then ok=%f;return;end
                if ninnout(1,1)<=0 | ninnout(1,2)<=0 then ok=%f;return;end
                //
                ww=find(bllst(connectmat(jj,1)).out==nout(1,1))
                bllst(connectmat(jj,1)).out(ww)=ninnout(1,1)
                ww=find(bllst(connectmat(jj,1)).out2==nout(1,1))
                bllst(connectmat(jj,1)).out2(ww)=ninnout(1,1)

                ww=find(bllst(connectmat(jj,1)).out==nout(1,2))
                bllst(connectmat(jj,1)).out(ww)=ninnout(1,2)
                ww=find(bllst(connectmat(jj,1)).out2==nout(1,2))
                bllst(connectmat(jj,1)).out2(ww)=ninnout(1,2)
                //

                if bllst(connectmat(jj,1)).nzcross==nout(1,1) then
                    bllst(connectmat(jj,1)).nzcross=ninnout(1,1)
                end
                if bllst(connectmat(jj,1)).nzcross==nout(1,2) then
                    bllst(connectmat(jj,1)).nzcross=ninnout(1,2)
                end
                //
                if bllst(connectmat(jj,1)).nmode==nout(1,1) then
                    bllst(connectmat(jj,1)).nmode=ninnout(1,1)
                end
                if bllst(connectmat(jj,1)).nmode==nout(1,2) then
                    bllst(connectmat(jj,1)).nmode=ninnout(1,2)
                end
                //
                ww=find(bllst(connectmat(jj,1)).in==nout(1,1))
                bllst(connectmat(jj,1)).in(ww)=ninnout(1,1)
                ww=find(bllst(connectmat(jj,1)).in2==nout(1,1))
                bllst(connectmat(jj,1)).in2(ww)=ninnout(1,1)

                ww=find(bllst(connectmat(jj,1)).in==nout(1,2))
                bllst(connectmat(jj,1)).in(ww)=ninnout(1,2)
                ww=find(bllst(connectmat(jj,1)).in2==nout(1,2))
                bllst(connectmat(jj,1)).in2(ww)=ninnout(1,2)
                //
                ww=find(bllst(connectmat(jj,1)).in==0)
                if (ww<>[]&min(bllst(connectmat(jj,1)).out(:))>0) then
                    bllst(connectmat(jj,1)).in(ww)=sum(bllst(connectmat(jj,1)).out)
                end

                ww=find(bllst(connectmat(jj,1)).in2==0)
                if (ww<>[]&min(bllst(connectmat(jj,1)).out2(:))>0) then
                    bllst(connectmat(jj,1)).in2(ww)=sum(bllst(connectmat(jj,1)).out2)
                end
                //
                ww=find(bllst(connectmat(jj,3)).in==nin(1,1))
                bllst(connectmat(jj,3)).in(ww)=ninnout(1,1)
                ww=find(bllst(connectmat(jj,3)).in2==nin(1,1))
                bllst(connectmat(jj,3)).in2(ww)=ninnout(1,1)

                ww=find(bllst(connectmat(jj,3)).in==nin(1,2))
                bllst(connectmat(jj,3)).in(ww)=ninnout(1,2)
                ww=find(bllst(connectmat(jj,3)).in2==nin(1,2))
                bllst(connectmat(jj,3)).in2(ww)=ninnout(1,2)
                //
                if bllst(connectmat(jj,3)).nzcross==nin(1,1) then
                    bllst(connectmat(jj,3)).nzcross=ninnout(1,1)
                end
                if bllst(connectmat(jj,3)).nzcross==nin(1,2) then
                    bllst(connectmat(jj,3)).nzcross=ninnout(1,2)
                end
                if bllst(connectmat(jj,3)).nmode==nin(1,1) then
                    bllst(connectmat(jj,3)).nmode=ninnout(1,1)
                end
                if bllst(connectmat(jj,3)).nmode==nin(1,2) then
                    bllst(connectmat(jj,3)).nmode=ninnout(1,2)
                end
                //
                ww=find(bllst(connectmat(jj,3)).out==nin(1,1))
                bllst(connectmat(jj,3)).out(ww)=ninnout(1,1)
                ww=find(bllst(connectmat(jj,3)).out2==nin(1,1))
                bllst(connectmat(jj,3)).out2(ww)=ninnout(1,1)

                ww=find(bllst(connectmat(jj,3)).out==nin(1,2))
                bllst(connectmat(jj,3)).out(ww)=ninnout(1,2)
                ww=find(bllst(connectmat(jj,3)).out2==nin(1,2))
                bllst(connectmat(jj,3)).out2(ww)=ninnout(1,2)
                //
                ww=find(bllst(connectmat(jj,3)).out==0)
                if (ww<>[]&min(bllst(connectmat(jj,3)).in(:))>0) then
                    bllst(connectmat(jj,3)).out(ww)=sum(bllst(connectmat(jj,3)).in(:))
                end
                ww=find(bllst(connectmat(jj,3)).out2==0)
                if (ww<>[]&min(bllst(connectmat(jj,3)).in2(:))>0) then
                    bllst(connectmat(jj,3)).out2(ww)=sum(bllst(connectmat(jj,3)).in2(:))
                end
            end
        end

        //if failed then display message
        if ~findflag then
            msg = _("I cannot find a link with undetermined size.\nMy guess is that you have a block with unconnected \nundetermined output ports.")
            messagebox(msprintf(msg), "modal", "error")
            ok = %f
            return
        end
    end
endfunction

function id = getBlockIds(path)
    // Return a block id path from a block index path
    //
    // path: the path in the index form
    // id: th path in the uid form

    scs_m; // check scs_m access
    id=[];

    k = path(:);
    for i = k
        b = scs_m.objs(i);
        if typeof(b) == "Block" &  length(scs_m.objs(i).model.uid) >= 1 then
            id($ + 1) = scs_m.objs(i).model.uid;
        end
        if typeof(b.model.rpar) == "diagram" then
            scs_m = b.model.rpar;
        end
    end
endfunction

//19/01/07, Alan : under_connection show bad link and returns two dimensions now
function ninnout=under_connection(path_out,prt_out,nout,path_in,prt_in,nin,flagg)
    // alert for badly connected blocks
    // path_out : Path of the "from block" in scs_m
    // path_in  : Path of the "to block" in scs_m
    //!

    if path_in==-1 then
        msg = "<html><body>";
        msg = msg + gettext("One of this block output has negative size.<br />Please check.");
        msg = msg + "</body></html>";
        hilite_path(path_out, msg);
        ninnout=0
        return
    end

    if path_in==-2 then
        msg = "<html><body>";
        msg = msg + gettext("Block input has negative size:");
        msg = msg + "<ul>";
        msg = msg + "<li>" + msprintf(gettext("Input port %s size is: %s"), string(prt_out), sci2exp(nout)) + "</li>";
        msg = msg + "</ul>";
        msg = msg + "</body></html>";
        hilite_path(path_out, msg);
        ninnout=0
        return
    end

    // different use case (Unable to report on a non opened diagram)
    if isdef("Code_gene_run") then
        messagebox([gettext("Unable to report an error into a SuperBlock"); gettext("Please compile the diagram to report the error.")], "Compilation error", "error", "modal");
        ninnout=0
        return
    end

    msg = "<html><body>";
    if flagg==1 then
        msg = msg + gettext("<em>Please update the diagram to avoid this warning.</em><br />Block output port has a non-determined size:");
        msg = msg + "<ul>";
        msg = msg + "<li>" + msprintf(gettext("Output port %s size is: %s"), string(prt_out), sci2exp(nout)) + "</li>";
        msg = msg + "<li>" + msprintf(gettext("Input port %s size is: %s"), string(prt_in), sci2exp(nin)) + "</li>";
    else
        msg = msg + gettext("<em>Please update the diagram to avoid this warning.</em><br />Block output port has a non-determined type:");
        msg = msg + "<ul>";
        msg = msg + "<li>" + msprintf(gettext("Output port %s type."), string(prt_out)) + "</li>";
        msg = msg + "<li>" + msprintf(gettext("Input port %s type."), string(prt_in)) + "</li>";
    end
    msg = msg + "</ul>";
    msg = msg + "</body></html>";
    hilite_path(path_out, msg)

    if or(path_in<>path_out) then
        msg = "<html><body>";
        if flagg==1 then
            msg = msg + gettext("<em>Please update the diagram to avoid this warning.</em><br />Block input port has a non-determined size:");
            msg = msg + "<ul>";
            msg = msg + "<li>" + msprintf(gettext("Output port %s size is: %s"), string(prt_out), sci2exp(nout)) + "</li>";
            msg = msg + "<li>" + msprintf(gettext("Input port %s size is: %s"), string(prt_in), sci2exp(nin)) + "</li>";
        else
            msg = msg + gettext("<em>Please update the diagram to avoid this warning.</em><br />Block input port has a non-determined type:");
            msg = msg + "<ul>";
            msg = msg + "<li>" + msprintf(gettext("Output port %s type."), string(prt_out)) + "</li>";
            msg = msg + "<li>" + msprintf(gettext("Input port %s type."), string(prt_in)) + "</li>";
        end
        msg = msg + "</ul>";
        msg = msg + "</body></html>";
        hilite_path(path_in, msg)
    end

    mess=msprintf(_("Highlighted block(s) have connected ports \nwith  sizes that cannot be determined by the context.\nWhat is the size of this link?"))

    if flagg==1 then
        ninnout=evstr(dialog(mess,"[1,1]"))
    else
        ninnout=evstr(dialog(mess,"1"))
    end
endfunction

function [clkconnect,exe_cons]=pak_ersi(connectmat,clkconnect,..
    typ_r,typ_l,outoin,outoinptr,tblock,typ_cons,clkptr)

    //add every event to the time event because time includes all events
    all_out=[]
    for k=1:size(clkptr,1)-1
        if ~typ_l(k) then
            kk=[1:(clkptr(k+1)-clkptr(k))]'
            all_out=[all_out;[k*ones(kk),kk]]
        end
    end
    all_out=[all_out;[0,0]]

    //add time event if needed
    ind=find(tblock)
    ind=ind(:)
    for k=ind'
        clkconnect=[clkconnect;[all_out,ones(all_out)*[k,0;0,0]]]
    end
    if show_trace then mprintf("c_pass4444:\t%f\n", timer()),end
    ind1=find(typ_cons)
    ind=[ind;ind1(:)]
    exe_cons=[ind,zeros(ind)]

    vec=-ones(1,nblk);
    vec(ind)=0
    [r,ok]=newc_tree4(vec,outoin,outoinptr,typ_r)

    exe_cons=[exe_cons;r]

    if show_trace then mprintf("c_pass4445:\t%f\n", timer()),end

    [clkr,clkc]=size(clkconnect);
    if isempty(clkconnect) then
        cll=[];
    else
        mm=max(clkconnect(:,2))+1;
        cll=clkconnect(:,1)*mm+clkconnect(:,2);
    end
    [cll,ind]=gsort(-cll);
    clkconnect=clkconnect(ind,:);
    if cll<>[] then mcll=max(-cll)+1, else mcll=1;end
    cll=[-1;-cll;mcll];
    ii=find(cll(2:$)-cll(1:$-1)<>0)

    for k=1:size(ii,"*")-1
        oo=[ii(k):ii(k+1)-1]
        vec=-ones(1,nblk);
        vec(clkconnect(oo,3))=0
        [r,ok]=newc_tree4(vec,outoin,outoinptr,typ_r)
        m=size(r,1)
        r=[clkconnect(ii(k),1)*ones(m,1),clkconnect(ii(k),2)*ones(m,1),r]
        clkconnect=[clkconnect;r]
    end
    // temporary fix to take care of conditional blocks inherting from
    // constants: make these blocks always active

    ind=setdiff(find(typ_l),clkconnect(:,3))
    ind=ind(:)
    for k=ind'
        clkconnect=[clkconnect;[all_out,ones(all_out)*[k,0;0,0]]]
    end
    // end of  temoprary fix
    if show_trace then mprintf("c_pass4446:\t%f\n", timer()),end
endfunction

function [r,ok]=tree4(vec,outoin,outoinptr,typ_r)
    //compute blocks which inherit
    ok=%t;
    nb=size(vec,"*");
    r=[];
    for j=1:nb-1
        fini=%t
        for i=1:nb
            if vec(i)==j-1 then
                for k=outoinptr(i):outoinptr(i+1)-1
                    ii=outoin(k,1);
                    if (vec(ii)>-1)|typ_r(ii) then
                        fini=%f;
                        vec(ii)=j;
                    end
                    if typ_r(ii) then
                        r=[r;outoin(k,:)]
                    end
                end
            end
        end
        if fini then break;end
    end
endfunction

function [bllst,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,dep_u,dep_uptr,dep_t,..
    typ_l,typ_r,typ_m,tblock,typ_cons,typ_zx,ok]=mini_extract_info(bllst,..
    connectmat,clkconnect)
    ok=%t
    nbl=length(bllst)
    clkptr=zeros(nbl+1,1);clkptr(1)=1
    cliptr=clkptr;inpptr=cliptr;outptr=inpptr;
    fff=ones(nbl,1)==1
    typ_l=fff;typ_r=fff;typ_cons=fff;typ_m=fff;typ_zx=fff;
    dep_t=ones(nbl,1)==1;
    dep_u=[];dep_uptr=1;
    tblock=fff
    //tblock=[]  // specifies blocks that must be connected to time event.
    //
    for i=1:nbl
        ll=bllst(i)
        if (ll.state==[]&ll.nzcross==0) then typ_zx(i)=%f;end
        inpnum=ll.in;outnum=ll.out;cinpnum=ll.evtin;coutnum=ll.evtout;
        //
        if cinpnum==[] then
            // this block inherits
            //ok=%f

            typ_r(i)=~ll.dep_ut($)
            tblock(i)=ll.dep_ut($)
            //if block depends on time but has no event input port
            if ~ll.dep_ut($) then
                //inherits from the inputs
                cinpnum=ones(inpnum)
                bllst(i).evtin=cinpnum  //XXXXXXXXXXXXXXXXXXXXX
            end
            //
        else
            tblock(i)=ll.dep_ut($);typ_r(i)=%f
        end
        inpptr(i+1)=inpptr(i)+size(inpnum,"*")
        outptr(i+1)=outptr(i)+size(outnum,"*")
        cliptr(i+1)=cliptr(i)+size(cinpnum,"*")
        clkptr(i+1)=clkptr(i)+size(coutnum,"*")
        //

        typ_l(i)=ll.blocktype=="l";typ_m(i)=ll.blocktype=="m";
        typ_cons(i)=cinpnum==[]&inpnum==[]&~ll.dep_ut($)
        //test of the dep_ut size
        sizenin=size(ll.in,"*");
        if (size(ll.dep_ut,"*") <> 2) then
            if ( size(ll.dep_ut(1:$-1),"*") <> sizenin) then
                msg = _("the dep_ut size of the %s block is not correct.\nIt should be a colon vector of size %d.")
                messagebox(msprintf(msg, ll.sim(1), sizenin+1), "modal", "error")
                ok=%f;
            end
        end

        dep_t(i)=ll.dep_ut($);

        if (size(ll.dep_ut,"*") == 2) then
            if (sizenin == 1) then
                dep_u($+1)=ll.dep_ut(1);
                dep_uptr($+1)=dep_uptr($)+1;
            elseif (sizenin > 1) then
                dep_u=[dep_u;ones(sizenin,1)==1*ll.dep_ut(1)];
                dep_uptr($+1)=dep_uptr($)+sizenin;
            else
                dep_uptr($+1)=dep_uptr($);
            end
        else
            dep_u_i=ll.dep_ut(1:$-1);
            dep_u=[dep_u;dep_u_i(:)];
            dep_uptr($+1)=dep_uptr($)+sizenin;
        end

        //
    end
    if show_trace then mprintf("c_pass22222222:\t%f\n", timer()),end //'
    nlnk=size(connectmat,1)
    inplnk=zeros(inpptr($)-1,1);outlnk=zeros(outptr($)-1,1);ptlnk=1;

    for jj=1:nlnk
        ko=outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)
        ki=inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)
        if ko<>0 & ki<>0 then
            if ko>ki then
                outlnk(outlnk>ko)=outlnk(outlnk>ko)-1
                outlnk(outlnk==ko)=ki
                inplnk(inplnk>ko)=inplnk(inplnk>ko)-1
                inplnk(inplnk==ko)=ki
                ptlnk=-1+ptlnk
            elseif ki>ko
                outlnk(outlnk>ki)=outlnk(outlnk>ki)-1
                outlnk(outlnk==ki)=ko
                inplnk(inplnk>ki)=inplnk(inplnk>ki)-1
                inplnk(inplnk==ki)=ko
                ptlnk=-1+ptlnk
            end
        elseif ko<>0 then
            inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)=ko
        elseif ki<>0 then
            outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)=ki
        else
            outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)=ptlnk
            inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)=ptlnk
            ptlnk=1+ptlnk
        end
    end

    //store unconnected outputs, if any, at the end of outtb
    for unco=find(outlnk==0);
        outlnk(unco)=max(outlnk)+1
    end

    //store unconnected inputs, if any, at the end of outtb
    for unco=find(inplnk==0);
        inplnk(unco)=max([inplnk;max(outlnk)])+1
    end
endfunction

function [evoutoin,evoutoinptr]=synch_clkconnect(typ_l,clkconnect)
    nblk=size(typ_l,"*")
    evoutoin=[];evoutoinptr=1
    for i=1:nblk
        if typ_l(i) then
            dd=clkconnect(clkconnect(:,1)==i,3)
        else
            dd=[]
        end
        evoutoin=[evoutoin;dd]
        evoutoinptr=[evoutoinptr;evoutoinptr($)+size(dd,"*")]
    end
endfunction

function   clkconnect=cleanup(clkconnect)
    mm=max(clkconnect)+1
    cc=clkconnect(:,4)+mm*clkconnect(:,3)+clkconnect(:,2)*mm^2+..
    clkconnect(:,1)*mm^3
    [cc1,ind]=gsort(-cc)
    clkconnect=clkconnect(ind,:)
    ind=find(cc1(2:$)-cc1(1:$-1)==0)
    clkconnect(ind,:)=[]
endfunction

//function mat=cleanup1(mat)
//  mm=max(mat)+1
//  cc=mat(:,1)*mm
//  [cc1,ind]=gsort(-cc)
//  mat=mat(ind,:)
//  ind=find(cc1(2:$)-cc1(1:$-1)==0)
//  mat(ind,:)=[]
//endfunction

function vec=intersection(vec1,vec2)
    vec=[]
    for i=1:size(vec1,1)
        if find(vec1(i)==vec2)~=[] then
            vec=[vec;vec1(i)]
        end
    end
endfunction

function  [r,ok]=newc_tree2(vec,outoin,outoinptr,dep_u,dep_uptr)
    dd=zeros(dep_u);dd(dep_u)=1;
    [r,ok2]=ctree2(vec,outoin,outoinptr,dd,dep_uptr)
    ok=ok2==1
endfunction

function  [r,ok]=new_tree2(vec,outoin,outoinptr,dep_u,dep_uptr)
    dd=zeros(dep_u);dd(dep_u)=1;
    [r,ok2]=sci_tree2(vec,outoin,outoinptr,dd)
    ok=ok2==1
endfunction

function  [r,ok]=new_tree3(vec,dep_ut,typ_l)
    dd=zeros(dep_ut);dd(dep_ut)=1;
    [r2,ok2]=sci_tree3(vec,dd,typ_l,bexe,boptr,blnk,blptr)
    r=r2'
    ok=ok2==1
endfunction

function  [r,ok]=newc_tree3(vec,dep_u,dep_uptr,typ_l)
    dd=zeros(dep_u);dd(dep_u)=1;
    [r2,ok2]=ctree3(vec,dd,dep_uptr,typ_l,bexe,boptr,blnk,blptr)
    r=r2'
    ok=ok2==1
endfunction

function  [r,ok]=new_tree4(vec,outoin,outoinptr,typ_r)
    if isempty(outoin) then
        nd=zeros(size(vec,"*"),1);
    else
        nd=zeros(size(vec,"*"),(max(outoin(:,2))+1));
    end
    ddd=zeros(typ_r);ddd(typ_r)=1;
    [r1,r2]=sci_tree4(vec,outoin,outoinptr,nd,ddd)
    r=[r1',r2']
    ok=%t
endfunction

function  [r,ok]=newc_tree4(vec,outoin,outoinptr,typ_r)
    if isempty(outoin) then
        nd=zeros(size(vec,"*"),1);
    else
        nd=zeros(size(vec,"*"),(max(outoin(:,2))+1));
    end
    ddd=zeros(typ_r);ddd(typ_r)=1;
    [r1,r2]=ctree4(vec,outoin,outoinptr,nd,ddd)
    r=[r1',r2']
    ok=%t
endfunction

function [critev]=critical_events(connectmat,clkconnect,dep_t,typ_r,..
    typ_l,typ_zx,outoin,outoinptr,clkptr)

    if isempty(clkconnect) then
        critev = [];
        return
    end

    typ_c=typ_l<>typ_l;
    typ_r=typ_r|dep_t

    done1=%f
    while ~done1
        done1=%t
        [clkr,clkc]=size(clkconnect);
        mm=max(clkconnect)+1;

        cll=clkconnect(:,1)*mm+clkconnect(:,2);
        [cll,ind]=gsort(-cll);
        clkconnect=clkconnect(ind,:);
        cll=[-1;-cll;mm];
        ii=find(cll(2:$)-cll(1:$-1)<>0)

        for k=1:size(ii,"*")-1
            oo=[ii(k):ii(k+1)-1]
            vec=-ones(1,nblk);
            vec(clkconnect(oo,3))=0
            [r,ok]=newc_tree4(vec,outoin,outoinptr,typ_r)

            m=size(r,1)
            r=[clkconnect(ii(k),1)*ones(m,1),clkconnect(ii(k),2)*ones(m,1),r]
            clkconnect=[clkconnect;r]
        end

        done=%f;
        while ~done
            done=%t;
            for jj=find(typ_l&(~typ_c));
                if ~or(jj==clkconnect(:,3)) then
                    typ_r(clkconnect(find(jj==clkconnect(:,1)),3))=%t
                    clkconnect(find(jj==clkconnect(:,1)),:)=[];
                    typ_c(jj)=%t;
                    done1=%f
                    done=%f
                end
            end
        end
    end
    critev=zeros(clkptr($)-1,1);
    for bb=1:size(clkptr,1)-1
        for i=clkptr(bb):clkptr(bb+1)-1
            if or(typ_zx(clkconnect(find((clkconnect(:,1)==bb)&..
                (clkconnect(:,2)==i-clkptr(bb)+1)),3))) then
                critev(i)=1
            end
        end
    end
endfunction

// adjust_typ: It resolves positive and negative port types.
//		   Its Algorithm is based on the algorithm of adjust_inout
// Fady NASSIF: 14/06/2007

function [ok,bllst]=adjust_typ(bllst,connectmat)

    for i=1:length(bllst)
        if size(bllst(i).in,"*")<>size(bllst(i).intyp,"*") then
            bllst(i).intyp=bllst(i).intyp(1)*ones(bllst(i).in);
        end
        if size(bllst(i).out,"*")<>size(bllst(i).outtyp,"*") then
            bllst(i).outtyp=bllst(i).outtyp(1)*ones(bllst(i).out);
        end
    end
    nlnk=size(connectmat,1)
    for hhjj=1:length(bllst)+1
        for hh=1:length(bllst)+1
            ok=%t
            for jj=1:nlnk
                nnout(1,1)=bllst(connectmat(jj,1)).out(connectmat(jj,2))
                nnout(1,2)=bllst(connectmat(jj,1)).out2(connectmat(jj,2))
                nnin(1,1)=bllst(connectmat(jj,3)).in(connectmat(jj,4))
                nnin(1,2)=bllst(connectmat(jj,3)).in2(connectmat(jj,4))
                outtyp = bllst(connectmat(jj,1)).outtyp(connectmat(jj,2))
                intyp = bllst(connectmat(jj,3)).intyp(connectmat(jj,4))

                //first case : types of source and
                //             target ports are explicitly informed
                //             with positive types
                if (intyp>0 & outtyp>0) then
                    //if types of source and target port don't match and aren't double and complex
                    //then call bad_connection, set flag ok to false and exit

                    if intyp<>outtyp then
                        if (intyp==1 & outtyp==2) then
                            bllst(connectmat(jj,3)).intyp(connectmat(jj,4))=2;
                        elseif (intyp==2 & outtyp==1) then
                            bllst(connectmat(jj,1)).outtyp(connectmat(jj,2))=2;
                        else
                            bad_connection(corinv(connectmat(jj,1)),connectmat(jj,2),..
                            nnout,outtyp,..
                            corinv(connectmat(jj,3)),connectmat(jj,4),..
                            nnin,intyp,1)
                            ok=%f;
                            return
                        end
                    end

                    //second case : type of source port is
                    //              positive and type of
                    //              target port is negative
                elseif(outtyp>0&intyp<0) then
                    //find vector of input ports of target block with
                    //type equal to intyp
                    //and assign it to outtyp
                    ww=find(bllst(connectmat(jj,3)).intyp==intyp)
                    bllst(connectmat(jj,3)).intyp(ww)=outtyp

                    //find vector of output ports of target block with
                    //type equal to intyp
                    //and assign it to outtyp
                    ww=find(bllst(connectmat(jj,3)).outtyp==intyp)
                    bllst(connectmat(jj,3)).outtyp(ww)=outtyp

                    //third case : type of source port is
                    //             negative and type of
                    //             target port is positive
                elseif(outtyp<0&intyp>0) then
                    //find vector of output ports of source block with
                    //type equal to outtyp
                    //and assign it to intyp
                    ww=find(bllst(connectmat(jj,1)).outtyp==outtyp)
                    bllst(connectmat(jj,1)).outtyp(ww)=intyp

                    //find vector of input ports of source block with
                    //type equal to size outtyp
                    //and assign it to intyp
                    ww=find(bllst(connectmat(jj,1)).intyp==outtyp)
                    bllst(connectmat(jj,1)).intyp(ww)=intyp


                    //fourth (& last) case : type of both source
                    //                      and target port are negatives
                else
                    ok=%f //set flag ok to false
                end
            end
            if ok then return, end //if ok is set true then exit adjust_typ
        end
        //if failed then display message
        tmp = _("Not enough information to find port type.\nI will try to find the problem.")
        messagebox(msprintf(tmp),"modal","info");
        findflag=%f
        for jj=1:nlnk
            nouttyp=bllst(connectmat(jj,1)).outtyp(connectmat(jj,2))
            nintyp=bllst(connectmat(jj,3)).intyp(connectmat(jj,4))

            //loop on the two dimensions of source/target port
            //only case : target and source ports are both
            //            negative or null
            if nouttyp<=0 & nintyp<=0 then
                findflag=%t;
                //
                inouttyp=under_connection(corinv(connectmat(jj,1)),connectmat(jj,2),nouttyp,..
                corinv(connectmat(jj,3)),connectmat(jj,4),nintyp,2)
                //
                if inouttyp<1|inouttyp>8 then ok=%f;return;end
                //
                ww=find(bllst(connectmat(jj,1)).outtyp==nouttyp)
                bllst(connectmat(jj,1)).outtyp(ww)=inouttyp

                //
                ww=find(bllst(connectmat(jj,1)).intyp==nouttyp)
                bllst(connectmat(jj,1)).intyp(ww)=inouttyp

                ww=find(bllst(connectmat(jj,3)).intyp==nintyp)
                bllst(connectmat(jj,3)).intyp(ww)=inouttyp
                //
                ww=find(bllst(connectmat(jj,3)).outtyp==nintyp)
                bllst(connectmat(jj,3)).outtyp(ww)=inouttyp

                //
            end
        end
        //if failed then display message
        if ~findflag then
            msg = _("I cannot find a link with undetermined size.\nMy guess is that you have a block with unconnected \nundetermined types.")
            messagebox(msprintf(msg), "modal","error")
            ok = %f
            return
        end
    end
endfunction

