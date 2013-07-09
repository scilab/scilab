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

function [Ts,bllst,corinv,indout,ok,scs_m,flgcdgen,freof]=sample_clk(MAT,Ts,bllst,corinv,scs_m,indout,flgcdgen)

    // This function takes care of the synchronize clocks.
    // Two ways are used for that calculation.
    // the first one uses a fast clock connected to a counter which is connected to an event select.
    // This method used the two functions clock_major and update_diag. It is used for the moment in the code generation
    // and it may be used in the future in the atomics blocks.
    // The second method generates an event at a specific time using the multifrequency block.
    // It uses the s_clk2 function. it is used in the compilation of all the diagram.
    // The compilation using the second method is faster than using the first one but the first method generates periodic events.
    // Fady NASSIF 2008. Copyright INRIA.

    //input: -MAT: a M*5 matrix. This matrix is the sco_mat given by the scicos_flat function.
    //             When the fifth column is equal to '4' then the corresponding block is the SampleCLK block.
    //       -Ts: the links_table given by th scicos_flat function.
    //	 -bllst: block list.same definition as in c_pass1.
    //	 -corinv: same definition as in c_pass1.
    //	 -scs_m: diagram
    //	 -indout: index used in c_pass1 to separate modelica blocks from regular blocks.
    //	 -flgcdgen: -1 when the function is not called by the code generation
    //	 	    it takes the number of event input of the superblock when it is called under the code generation.
    //
    // This part can be used in the next version to choose between the two methods.
    // ------------
    //   [num]=x_choose(['event select';'multiple frequency'],..
    //                    ["You have to choose a method for the sample time computation:";..
    //                     "The first method is a periodic synchronize system";..
    //                     "it uses the event select block to generate events"; "at every clock time (regular period)";..
    //                     "The number of outputs is equal to the least common"; "multiple of the input frequencies ";..
    //                     "The second method uses the multi-frequency block";..
    //                     "it generates an event only on the used time";..
    //                     "The default value is select method"])
    //num=scs_m.props.tol(8)
    //the test will be done on num and not on the flgcdgen
    //---------------------------------------------------------------------

    if flgcdgen==-1 then
        [Ts,bllst,corinv,indout,ok]=s_clk2(MAT,Ts,bllst,corinv,scs_m,indout)
    else
        [frequ,offset,freqdiv,den,flg,ok]=clock_major(MAT)
        if ok then
            [Ts,corinv,bllst,indout,ok,scs_m,flgcdgen,freof]=update_diag(scs_m,corinv,Ts,frequ,offset,freqdiv,bllst,den,flg,indout,flgcdgen)
        end
    end
endfunction

function [frequ,offset,freqdiv,den,flg,ok]=clock_major(MAT)

    // this function calculates the frequency and the offset of the fastest clock
    //modification to support double
    // output parameters:	-frequ:   the frequency of the major clock.
    //		    	-offset:  the offset of the major clock.
    //			-freqdiv: A vector containing the frequencies of all the SampleClk multiplied by the common denominator.
    //			-den: The common denominator.
    //			-flg: 0 if there is one frequency and one offset
    //			      1 else case.
    //			-ok: flag for error.

    ok=%t;
    flg=1;
    index=find(MAT(:,5)==string(4));
    if index<>[] then
        freq1=evstr(MAT(index,3));
        offset1=evstr(MAT(index,4));
    end
    freqdiv=unique(freq1);
    if size(unique(offset1),"*")==1 then  // case of one offset
        if size(freqdiv,"*")==1 then flg=0;end // case of one offset and one frequency.
        v=freq1;
        v=v(find(v<>0));
        min_v=min(v);max_v=max(v);
        if (max_v/min_v)>1e5 then messagebox(["The difference between the frequencies is very large";..
            "the clocks could not be synchronized"],"modal");
            ok=%f;Ts=[];bllst=[];corinv=list();indout=[];
            return;
        end
        [frequ,den]=fixedpointgcd(v); //computing the pgcd of the frequencies
        offset=offset1(1);
    else // case of multiple offset
        v=[freq1;offset1]
        v=v(find(v<>0));
        min_v=min(v);max_v=max(v);
        if (max_v/min_v)>1e5 then messagebox(["The difference between the frequencies is very large";..
            "the clocks could not be synchronized"],"modal");
            ok=%f;Ts=[];bllst=[];corinv=list();indout=[];
            return;
        end
        [frequ,den]=fixedpointgcd(v);//computes the pgcd of the frequencies and the offsets.
        offset=0;
    end
    freqdiv=uint32(freqdiv*double(den));
    frequ=double(frequ)/double(den);
    if frequ==[] then frequ=0;end
    if offset==[] then offset=0; end
    if (offset > frequ) then
        offset=modulo(offset,frequ)
        if (offset~=0) then ok=%f; end
    end
endfunction

function [Ts,corinv,bllst,indout,ok,scs_m,flgcdgen,freof]=update_diag(scs_m,corinv,Ts,frequ,offset,freqdiv,bllst,den,flg,indout,flgcdgen)
    //modification to support double
    ok=%t
    n=lstsize(scs_m.objs)

    // when the function is called by the code generator we add an input event to the diagram
    // the major clock will be put outside the superblock. it will be explicitly drawn.
    // In the other case the major clock will be implicitly used in the diagram.

    if flgcdgen<>-1 then // when the function is called by the codegeneration.
        flgcdgen=flgcdgen+1 	// the flgcdgen contains the number of event input.
        // we incremented to be able to add the sampleclk to the diagram at the end
        // Adding the event input block.
        bllst($+1)=scicos_model(sim=list("bidon",0),in=[],in2=[],intyp=1,out=[],out2=[],..
        outtyp=1,evtin=[],evtout=1,state=[],dstate=[],odstate=list(),..
        rpar=[],ipar=flgcdgen,opar=list(),blocktype="d",firing=-1,..
        dep_ut=[%f,%f],label="",nzcross=0,nmode=0,equations=list());
        freof=[frequ;offset];
        // Drawing the block in the scs_m
        blk=CLKINV_f("define");
        blk.gui="EVTGEN_f";
        blk.model.ipar=flgcdgen;
        blk.model.sim(1)="bidon";
        blk.graphics.exprs=sci2exp(flgcdgen);
        blk.graphics.sz=[20 30];
        blk.graphics.peout=20
        scs_m.objs($+1)=blk
    else
        bllst($+1)=scicos_model(sim=list("evtdly4",4),in=[],in2=[],intyp=1,out=[],out2=[],..
        outtyp=1,evtin=1,evtout=1,state=[],dstate=[],odstate=list(),..
        rpar=[frequ;offset],ipar=[],opar=list(),blocktype="d",firing=offset,..
        dep_ut=[%f,%f],label="",nzcross=0,nmode=0,equations=list());
        scs_m.objs(n+1)=EVTDLY_c("define");
        scs_m.objs(n+1).model.rpar=[frequ;offset];
        scs_m.objs(n+1).model.firing=offset;
        freof=[frequ;offset];
    end
    corinv($+1)=n+1
    nb=size(corinv)
    nc=size(bllst);
    // nb and nc defers in the diagrams that contains modelicas blocks.
    indout($+1)=nc;
    if flgcdgen==-1 then
        // linking the output of the evtdly to its input.
        Ts($+1:$+2,:)=[nb 1 -1 -1;..
        nb 1 1  -1]
    end
    if flg then //more then one frequency or offset

        nn=lcm(freqdiv);
        nn=double(nn)/(frequ*(double(den)));// the number of output of the event select block
        //Adding the counter to the block list.
        bllst($+1)=scicos_model(sim=list("counter",4),in=[],in2=[],intyp=1,out=1,out2=1,..
        outtyp=1,evtin=1,evtout=[],state=[],dstate=0,odstate=list(),..
        rpar=[],ipar=[1;double(nn);1],opar=list(),blocktype="c",firing=[],..
        dep_ut=[%f,%f],label="",nzcross=0,nmode=0,equations=list());
        // Adding the event select to the block list.
        bllst($+1)=scicos_model(sim=list("eselect",-2),in=1,in2=1,intyp=-1,out=[],out2=[],..
        outtyp=1,evtin=[],evtout=ones(nn,1),state=[],dstate=[],odstate=list(),..
        rpar=[],ipar=[],opar=list(),blocktype="l",firing=-ones(nn,1),..
        dep_ut=[%t,%f],label="",nzcross=0,nmode=0,equations=list());

        corinv($+1)=n+2
        //we have to add the counter and the eventselect block to the scs_m
        //to be compatible with the code generation
        scs_m.objs(n+2)=Counter("define");
        scs_m.objs(n+2).model.ipar=[1;double(nn);1];
        corinv($+1)=n+3
        scs_m.objs(n+3)=ESELECT_f("define");
        scs_m.objs(n+3).model.evtout=ones(nn,1);
        scs_m.objs(n+3).model.firing=-ones(nn,1);
        nc=size(bllst)
        nb=size(corinv)
        indout($+1)=nc-1;
        indout($+1)=nc;
        // linking the event output of the evntdly or the bidon to the counter.
        // and linking the regular output of the counter to the event select.
        Ts($+1:$+4,:)=[nb-2 1 -1 -1;..
        nb-1,1,1,-1;..
        nb-1,1,-1,1;..
        nb,1,1,1]
        // replacing the SampleClk by the output of the event select
        index=find(MAT(:,5)==string(4))
        for i=index
            num=-evstr(MAT(i,1))
            Ts(find(Ts(:,1)==num),1)=-num
            K=0:nn-1;
            M=find(modulo(int(K*frequ*den),int(evstr(MAT(i,3))*double(den)))==0)';
            ON=ones(size(M,"*"),1)
            Ts($+1:2:$+2*size(M,"*"),:)=[nb*ON M+ON*(evstr(MAT(i,4))-offset)*double(den) -ON -ON]
            N=[1:size(M,"*")]';
            Ts($+1-(2*size(M,"*")-2):2:$+1,:)=[-num*ON N ON -ON]
        end
    else
        nb=size(corinv)
        index=find(MAT(:,5)==string(4))
        ON=ones(size(index,"*"),1)
        Ts($+1:2:$+2*size(index,"*"),:)=[nb*ON ON -ON -ON]
        num=-evstr(MAT(index,1))
        //   N=[1:size(index,'*')]';
        Ts($+1-(2*size(index,"*")-2):2:$+1,:)=[-num ON ON -ON]
        for i=1:size(index,"*")
            Ts(find(Ts(:,1)==num(i)),1)=-num(i)
        end
    end
endfunction

function [Ts,bllst,corinv,indout,ok]=s_clk2(MAT,Ts,bllst,corinv,scs_m,indout)
    ok=%t
    index=find(MAT(:,5)==string(4)) //index of SampleCLK blocks
    MAT1=MAT(index,:);
    frequ=evstr(MAT1(:,3));  // frequencies of the sampleCLK
    offset=evstr(MAT1(:,4)); // offsets of the SampleCLK
    offset=offset(:);frequ=frequ(:);
    [m,den,off,count,m1,fir,frequ,offset,ok]=mfrequ_clk(frequ,offset);
    if ~ok then return;end
    mn=(2**size(m1,"*"))-1;//number of event outputs.
    n=lstsize(scs_m.objs);
    bllst($+1)=scicos_model(sim=list("m_frequ",4),in=[],in2=[],intyp=1,out=[],out2=[],outtyp=1,..
    evtin=1,evtout=ones(mn,1),state=[],dstate=[],odstate=list(),rpar=[],ipar=[],..
    opar=list(m,double(den),off,count),blocktype="d",firing=fir,dep_ut=[%f,%f],..
    label="",nzcross=0,nmode=0,equations=list());
    corinv($+1)=n+1;
    nb=size(corinv);
    nc=size(bllst);// nc differs from nb if the diagram contains modelica's block
    indout($+1)=nc;
    k=1:mn;
    //connecting all the event outputs to the event input of the M_Frequ block
    Ts($+1:2:$+2*mn,:)=[nb*ones(mn,1) k' -ones(mn,2)]
    Ts($+1-(2*mn-2):2:$+1,:)=[nb*ones(mn,1) ones(mn,2) -ones(mn,1)]
    //replacing the SampleCLK by the outputs of the M_frequ
    for i=1:size(frequ,"*")
        num=evstr(MAT1(find((evstr(MAT1(:,3))==frequ(i))&(evstr(MAT1(:,4))==offset(i))),1))
        for ii=num'
            Ts(find(Ts(:,1)==-ii),1)=ii;
            j=2**(i-1):2**i:mn;
            v=j;
            for k=1:2**(i-1)-1;
                v=[v,j+k]
            end
            v=(unique(v))
            ON=ones(size(v,"*"),1)
            N=[1:size(v,"*")]';
            Ts($+1:2:$+2*size(v,"*"),:)=[nb*ON v' -ON -ON]
            Ts($+1-(2*size(v,"*")-2):2:$+1,:)=[ii*ON N ON -ON]
        end
    end
endfunction

function [m,k]=uni(fr,of)
    k=[];
    m=[];
    ot=[];
    for i=1:size(fr,"*")
        istreated=%f;
        ind=find(m==fr(i));
        if ind==[] then
            m=[m;fr(i)];
            ot=[ot;of(i)];
            k=[k;i];
        else
            for j=ind
                if of(i)==ot(j) then
                    istreated=%t
                end
            end
            if ~istreated then
                m=[m;fr(i)];
                ot=[ot;of(i)]
                k=[k;i];
            end
        end
    end
endfunction
