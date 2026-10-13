//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//                      - Ramine Nikoukhah <ramine.nikoukhah@inria.fr>
//                      - Rachid Djenidi
//
//                      - Scilab 5 update by Simone Mannori
//
//  Copyright (C) DIGITEO - 2010 - Allan CORNET
//  Copyright (C) Scilab Enterprises - 2012 - Bruno JOFRET
//  Copyright (C) German Aerospace Center (DLR) - 2016 - Umut DURAK
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

//**---------------------------------------------------------------------------------------------------------------------------------
//
//16/06/07 Author : ?, A. Layec
//
function [Code,actt,proto]=call_actuator(i)
    nin=inpptr(i+1)-inpptr(i);  //** number of input ports
    nout=outptr(i+1)-outptr(i); //** number of output ports

    if funtyp(i)==0 then
        if nin==0 then
            uk    = 0;
            nuk_1 = 0;
            nuk_2 = 0;
            uk_t  = 1;
            //Code($+1)=+'  args[0]=(double *)outtbptr[0]);';
        else
            uk    = inplnk(inpptr(i));
            nuk_1 = size(outtb(uk),1);
            nuk_2 = size(outtb(uk),2);
            uk_t  = mat2scs_c_nb(outtb(uk));
            //Code($+1)=' args[0]=('+mat2scs_c_ptr(outtb(uk))+' *)outtbptr['+string(uk-1)+'];';
        end
    end
    //pour la fonction gui ont a : num de bloc l'adresse dans
    //nouveau  z et la taille du port
    actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]

    Code($+1)="block_"+rdnom+"["+string(i-1)+"].nevprt=nevprt;"

    Code=["/* Call of actuator (blk nb "+string(i)+") */"
    Code;
    "nport = "+string(nbact)+";";
    rdnom+"_actuator(&flag, &nport, &block_"+rdnom+"["+string(i-1)+"].nevprt, told, "+..
    "("+mat2scs_c_ptr(outtb(uk))+" *)outtbptr["+string(uk-1)+"], &nrd_"+string(nuk_1)+", &nrd_"+..
    string(nuk_2)+", &nrd_"+string(uk_t)+",bbb);"];

    proto="void "+rdnom+"_actuator("+..
    "int *, int *, int *, double *, void *, int *, int *,int *,int);"
    proto=cformatline(proto,70);
endfunction

//CallBlock : generate C calling sequence
//            of a scicos block
//
//inputs : bk   : bloc index
//         pt   : evt activation number
//         flag : flag
//
//output : txt  :
//
//16/06/07 Authors : Alan Layec
function txt=call_block42(bk,pt,flag)
    txt=[]
    //**
    if flag==2 & ((zptr(bk+1)-zptr(bk))+..
        (ozptr(bk+1)-ozptr(bk))+..
        (xptr(bk+1)-xptr(bk)+..
        with_work(bk))==0 |..
        pt<=0) & ~(stalone & or(bk==actt(:,1))) then
        return // block without state or continuously activated
    end
    if flag==0 & ((xptr(bk+1)-xptr(bk))==0) then
        return // block without continuous state
    end
    if flag==9 & ((zcptr(bk+1)-zcptr(bk))==0) then
        return // block without continuous state
    end
    if flag==3 & ((clkptr(bk+1)-clkptr(bk))==0) then
        return
    end

    //** adjust pt
    if ~(flag==3 & ((zcptr(bk+1)-zcptr(bk))<>0)) then
        pt=abs(pt)
    end

    //** add comment
    txt=[txt;
    get_comment("call_blk",list(funs(bk),funtyp(bk),bk,labels(bk)));]

    //** set nevprt and flag for called block
    txt=[txt;
    "newInstance->block_"+rdnom+"["+string(bk-1)+"].nevprt = "+string(pt)+";"
    "local_flag = "+string(flag)+";"]

    //**see if its bidon, actuator or sensor
    if funs(bk)=="bidon" then
        txt=[];
        return
    elseif funs(bk)=="bidon2" then
        txt=[];
        return
    elseif or(bk==actt(:,1)) then
        ind=find(bk==actt(:,1))
        uk=actt(ind,2)
        nuk_1=actt(ind,3)
        nuk_2=actt(ind,4)
        uk_t=actt(ind,5)
        txt = [];
//        txt = [txt;
//        "nport = "+string(ind)+";"]
//        txt = [txt;
//        rdnom+"_actuator(&local_flag, &nport, &block_"+rdnom+"["+string(bk-1)+"].nevprt, \"
//        get_blank(rdnom+"_actuator")+" &t, ("+mat2scs_c_ptr(outtb(uk))+" *)"+rdnom+"_block_outtbptr["+string(uk-1)+"], \"
//        get_blank(rdnom+"_actuator")+" &nrd_"+string(nuk_1)+", &nrd_"+string(nuk_2)+", &nrd_"+string(uk_t)+",bbb);"]
//        txt = [txt;
//        "if(local_flag < 0) return(5 - local_flag);"]
        return
    elseif or(bk==capt(:,1)) then
        ind=find(bk==capt(:,1))
        yk=capt(ind,2);
        nyk_1=capt(ind,3);
        nyk_2=capt(ind,4);
        yk_t=capt(ind,5);
        txt = [];
//        txt = [txt;
//        "nport = "+string(ind)+";"]
//        txt = [txt;
//        rdnom+"_sensor(&local_flag, &nport, &block_"+rdnom+"["+string(bk-1)+"].nevprt, \"
//        get_blank(rdnom+"_sensor")+" &t, ("+mat2scs_c_ptr(outtb(yk))+" *)"+rdnom+"_block_outtbptr["+string(yk-1)+"], \"
//        get_blank(rdnom+"_sensor")+" &nrd_"+string(nyk_1)+", &nrd_"+string(nyk_2)+", &nrd_"+string(yk_t)+",aaa);"]
//        txt = [txt;
//        "if(local_flag < 0) return(5 - local_flag);"]
        return
    end

    //**
    nx=xptr(bk+1)-xptr(bk);
    nz=zptr(bk+1)-zptr(bk);
    nrpar=rpptr(bk+1)-rpptr(bk);
    nipar=ipptr(bk+1)-ipptr(bk);
    nin=inpptr(bk+1)-inpptr(bk);  //* number of input ports */
    nout=outptr(bk+1)-outptr(bk); //* number of output ports */

    //**
    //l'adresse du pointeur de ipar
    if nipar<>0 then ipar=ipptr(bk), else ipar=1;end
    //l'adresse du pointeur de rpar
    if nrpar<>0 then rpar=rpptr(bk), else rpar=1; end
    //l'adresse du pointeur de z attention -1 pas sur
    if nz<>0 then z=zptr(bk)-1, else z=0;end
    //l'adresse du pointeur de x
    if nx<>0 then x=xptr(bk)-1, else x=0;end

    //**
    ftyp=funtyp(bk)
    if ftyp>2000 then ftyp=ftyp-2000,end
    if ftyp>1000 then ftyp=ftyp-1000,end

    //** check function type
    if ftyp < 0 then //** ifthenelse eselect blocks
        txt = [];
        return;
    else
        if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4) then
            disp("types other than 0,1,2,3 or 4 are not supported.")
            txt = [];
            return;
        end
    end

    select ftyp

    case 0 then
        //**** input/output addresses definition ****//
        if nin>1 then
            for k=1:nin
                uk=inplnk(inpptr(bk)-1+k);
                nuk=size(outtb(uk),"*");
                txt=[txt;
                "rdouttb["+string(k-1)+"]=(double *)"+rdnom+"_block_outtbptr["+string(uk-1)+"];"]
            end
            txt=[txt;
            "args[0]=&(rdouttb[0]);"]
        elseif nin==0
            uk=0;
            nuk=0;
            txt=[txt;
            "args[0]=(double *)"+rdnom+"_block_outtbptr[0];"]
        else
            uk=inplnk(inpptr(bk));
            nuk=size(outtb(uk),"*");
            txt=[txt;
            "args[0]=(double *)"+rdnom+"_block_outtbptr["+string(uk-1)+"];"]
        end

        if nout>1 then
            for k=1:nout
                yk=outlnk(outptr(bk)-1+k);
                nyk=size(outtb(yk),"*");
                txt=[txt;
                "rdouttb["+string(k+nin-1)+"]=(double *)"+rdnom+"_block_outtbptr["+string(yk-1)+"];"];
            end
            txt=[txt;
            "args[1]=&(rdouttb["+string(nin)+"]);"];
        elseif nout==0
            yk=0;
            nyk=0;
            txt=[txt;
            "args[1]=(double *)"+rdnom+"_block_outtbptr[0];"];
        else
            yk=outlnk(outptr(bk));
            nyk=size(outtb(yk),"*"),;
            txt=[txt;
            "args[1]=(double *)"+rdnom+"_block_outtbptr["+string(yk-1)+"];"];
        end
        //*******************************************//

        //*********** call seq definition ***********//
        txtc=["(&local_flag,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nevprt,&t,newInstance->block_"+rdnom+"["+string(bk-1)+"].xd, \";
        "newInstance->block_"+rdnom+"["+string(bk-1)+"].x,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nx, \";
        "newInstance->block_"+rdnom+"["+string(bk-1)+"].z,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nz,newInstance->block_"+rdnom+"["+string(bk-1)+"].evout, \";
        "&newInstance->block_"+rdnom+"["+string(bk-1)+"].nevout,newInstance->block_"+rdnom+"["+string(bk-1)+"].rpar,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nrpar, \";
        "newInstance->block_"+rdnom+"["+string(bk-1)+"].ipar,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nipar, \";
        "(double *)args[0],&nrd_"+string(nuk)+",(double *)args[1],&nrd_"+string(nyk)+");"];
        if (funtyp(bk)>2000 & funtyp(bk)<3000)
            blank = get_blank(funs(bk)+"( ");
            txtc(1) = funs(bk)+txtc(1);
        elseif (funtyp(bk)<2000)
            txtc(1) = "C2F("+funs(bk)+")"+txtc(1);
            blank = get_blank("C2F("+funs(bk)+") ");
        end
        txtc(2:$) = blank + txtc(2:$);
        txt = [txt;txtc];
        //*******************************************//


        //**
    case 1 then
        //*********** call seq definition ***********//
        txtc=["(&local_flag,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nevprt,&t,newInstance->block_"+rdnom+"["+string(bk-1)+"].xd, \";
        "newInstance->block_"+rdnom+"["+string(bk-1)+"].x,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nx, \";
        "newInstance->block_"+rdnom+"["+string(bk-1)+"].z,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nz,newInstance->block_"+rdnom+"["+string(bk-1)+"].evout, \";
        "&newInstance->block_"+rdnom+"["+string(bk-1)+"].nevout,newInstance->block_"+rdnom+"["+string(bk-1)+"].rpar,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nrpar, \";
        "newInstance->block_"+rdnom+"["+string(bk-1)+"].ipar,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nipar"];
        if (funtyp(bk)>2000 & funtyp(bk)<3000)
            blank = get_blank(funs(bk)+"( ");
            txtc(1) = funs(bk)+txtc(1);
        elseif (funtyp(bk)<2000)
            txtc(1) = "C2F("+funs(bk)+")"+txtc(1);
            blank = get_blank("C2F("+funs(bk)+") ");
        end
        if nin>=1 | nout>=1 then
            txtc($)=txtc($)+", \"
            txtc=[txtc;""]
            if nin>=1 then
                for k=1:nin
                    uk=inplnk(inpptr(bk)-1+k);
                    nuk=size(outtb(uk),"*");
                    txtc($)=txtc($)+"(double *)"+rdnom+"_block_outtbptr["+string(uk-1)+"],&nrd_"+string(nuk)+",";
                end
                txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
            end
            if nout>=1 then
                if nin>=1 then
                    txtc($)=txtc($)+", \"
                    txtc=[txtc;""]
                end
                for k=1:nout
                    yk=outlnk(outptr(bk)-1+k);
                    nyk=size(outtb(yk),"*");
                    txtc($)=txtc($)+"(double *)"+rdnom+"_block_outtbptr["+string(yk-1)+"],&nrd_"+string(nyk)+",";
                end
                txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
            end
        end

        if ztyp(bk) then
            txtc($)=txtc($)+", \"
            txtc=[txtc;
            "newInstance->block_"+rdnom+"["+string(bk-1)+"].g,&newInstance->block_"+rdnom+"["+string(bk-1)+"].ng);"]
        else
            txtc($)=txtc($)+");";
        end

        txtc(2:$) = blank + txtc(2:$);
        txt = [txt;txtc];
        //*******************************************//

        //**
    case 2 then

        //*********** call seq definition ***********//
        txtc=[funs(bk)+"(&local_flag,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nevprt,&t,newInstance->block_"+rdnom+"["+string(bk-1)+"].xd, \";
        "newInstance->block_"+rdnom+"["+string(bk-1)+"].x,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nx, \";
        "newInstance->block_"+rdnom+"["+string(bk-1)+"].z,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nz,newInstance->block_"+rdnom+"["+string(bk-1)+"].evout, \";
        "&newInstance->block_"+rdnom+"["+string(bk-1)+"].nevout,newInstance->block_"+rdnom+"["+string(bk-1)+"].rpar,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nrpar, \";
        "newInstance->block_"+rdnom+"["+string(bk-1)+"].ipar,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nipar, \";
        "(double **)newInstance->block_"+rdnom+"["+string(bk-1)+"].inptr,newInstance->block_"+rdnom+"["+string(bk-1)+"].insz,&newInstance->block_"+rdnom+"["+string(bk-1)+"].nin, \";
        "(double **)newInstance->block_"+rdnom+"["+string(bk-1)+"].outptr,newInstance->block_"+rdnom+"["+string(bk-1)+"].outsz, &newInstance->block_"+rdnom+"["+string(bk-1)+"].nout"];
        if ~ztyp(bk) then
            txtc($)=txtc($)+");";
        else
            txtc($)=txtc($)+", \";
            txtc=[txtc;
            "block_"+rdnom+"["+string(bk-1)+"].g,&block_"+rdnom+"["+string(bk-1)+"].ng);"]
        end
        blank = get_blank(funs(bk)+"( ");
        txtc(2:$) = blank + txtc(2:$);
        txt = [txt;txtc];
        //*******************************************//

        //**
    case 4 then
        txt=[txt;
        funs(bk)+"(&newInstance->block_"+rdnom+"["+string(bk-1)+"],local_flag);"];
    end
//    txt =[txt;"if(local_flag < 0) return(5 - local_flag);"]
    txt =[txt;"if(local_flag < 0) {"
          "    newInstance->functions.logger(NULL, newInstance->instanceName, fmiError, ""ERROR"","
          "    ""%s: Xcos block %s produces an internal error."", ""internalFunction"","""+funs(bk)+""");"
          "    status = fmiError;"
          "}"];

endfunction

//CallBlock : generate C calling sequence
//            of a scicos block
//
//inputs : bk   : bloc index
//         pt   :
//         flag :block_'+rdnom+'[
//
//output : txt  :
//
//16/06/07 Authors : R.Nikoukhah, A.Layec
function txt=call_block4(bk)
    //   if flag==2 & ((zptr(bk+1)-zptr(bk))+(ozptr(bk+1)-ozptr(bk))==0 | pt<=0) then
    //     return // block without discrete state or continuously activated
    //            // If work allocated in the absence of z and oz, it does not work
    //   end

    //**
    nx=xptr(bk+1)-xptr(bk);
    nz=zptr(bk+1)-zptr(bk);
    nrpar=rpptr(bk+1)-rpptr(bk);
    nipar=ipptr(bk+1)-ipptr(bk);
    nin=inpptr(bk+1)-inpptr(bk);  //* number of input ports */
    nout=outptr(bk+1)-outptr(bk); //* number of output ports */

    //**
    //l'adresse du pointeur de ipar
    if nipar<>0 then ipar=ipptr(bk), else ipar=1;end
    //l'adresse du pointeur de rpar
    if nrpar<>0 then rpar=rpptr(bk), else rpar=1; end
    //l'adresse du pointeur de z attention -1 pas sur
    if nz<>0 then z=zptr(bk)-1, else z=0;end
    //l'adresse du pointeur de x
    if nx<>0 then x=xptr(bk)-1, else x=0;end

    //**
    ftyp=funtyp(bk)
    if ftyp>2000 then ftyp=ftyp-2000,end
    if ftyp>1000 then ftyp=ftyp-1000,end

    //** check function type
    if ftyp < 0 then //** ifthenelse eselect blocks
        txt = [];
        return;
    else
        if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4) then
            disp("types other than 0,1,2,3 or 4 are not supported.")
            txt = [];
            return;
        end
    end

    //** add comment
    txt=[get_comment("call_blk",list(funs(bk),funtyp(bk),bk,labels(bk)));]

    //** write nevprt activation
    //   nclock=abs(pt);
    txt=[txt;
    "block_"+rdnom+"["+string(bk-1)+"].nevprt=nevprt;"]

    select ftyp
        //** zero funtyp
    case 0 then
        //**** input/output addresses definition ****//
        if nin>1 then
            for k=1:nin
                uk=inplnk(inpptr(bk)-1+k);
                nuk=size(outtb(uk),"*");
                TYPE=mat2scs_c_ptr(outtb(uk));//scilab index start from 1
                txt=[txt;
                "rdouttb["+string(k-1)+"]=("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(uk-1)+"];"]
            end
            txt=[txt;
            "args[0]=&(rdouttb[0]);"]
        elseif nin==0
            uk=0;
            nuk=0;
            txt=[txt;
            "args[0]=(SCSREAL_COP *)"+rdnom+"_block_outtbptr[0];"]
        else
            uk=inplnk(inpptr(bk));
            nuk=size(outtb(uk),"*");
            TYPE=mat2scs_c_ptr(outtb(uk));//scilab index start from 1
            txt=[txt;
            "args[0]=("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(uk-1)+"];"]
        end

        if nout>1 then
            for k=1:nout
                yk=outlnk(outptr(bk)-1+k);
                nyk=size(outtb(yk),"*");
                TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
                txt=[txt;
                "rdouttb["+string(k+nin-1)+"]=("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(yk-1)+"];"];
            end
            txt=[txt;
            "args[1]=&(rdouttb["+string(nin)+"]);"];
        elseif nout==0
            yk=0;
            nyk=0;
            txt=[txt;
            "args[1]=(SCSREAL_COP *)"+rdnom+"_block_outtbptr[0];"];
        else
            yk=outlnk(outptr(bk));
            nyk=size(outtb(yk),"*"),;
            TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
            txt=[txt;
            "args[1]=("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(yk-1)+"];"];
        end
        //*******************************************//

        //*********** call seq definition ***********//
        txtc=["(&flag,&block_"+rdnom+"["+string(bk-1)+"].nevprt,told,block_"+rdnom+"["+string(bk-1)+"].xd, \";
        "block_"+rdnom+"["+string(bk-1)+"].x,&block_"+rdnom+"["+string(bk-1)+"].nx, \";
        "block_"+rdnom+"["+string(bk-1)+"].z,&block_"+rdnom+"["+string(bk-1)+"].nz,block_"+rdnom+"["+string(bk-1)+"].evout, \";
        "&block_"+rdnom+"["+string(bk-1)+"].nevout,block_"+rdnom+"["+string(bk-1)+"].rpar,&block_"+rdnom+"["+string(bk-1)+"].nrpar, \";
        "block_"+rdnom+"["+string(bk-1)+"].ipar,&block_"+rdnom+"["+string(bk-1)+"].nipar, \";
        "(double *)args[0],&nrd_"+string(nuk)+",(double *)args[1],&nrd_"+string(nyk)+");"];
        if (funtyp(bk)>2000 & funtyp(bk)<3000)
            blank = get_blank(funs(bk)+"( ");
            txtc(1) = funs(bk)+txtc(1);
        elseif (funtyp(bk)<2000)
            txtc(1) = "C2F("+funs(bk)+")"+txtc(1);
            blank = get_blank("C2F("+funs(bk)+") ");
        end
        txtc(2:$) = blank + txtc(2:$);
        txt = [txt;txtc];
        //*******************************************//
        //**
    case 1 then
        //**** input/output addresses definition ****//
        //       if nin>=1 then
        //         for k=1:nin
        //           uk=inplnk(inpptr(i)-1+k);
        //           nuk=size(outtb(uk),'*');
        //         end
        //       end
        //       if nout>=1 then
        //         for k=1:nout
        //           yk=outlnk(outptr(i)-1+k);
        //           nyk=size(outtb(yk),'*');
        //         end
        //       end
        //*******************************************//

        //*********** call seq definition ***********//
        txtc=["(&flag,&block_"+rdnom+"["+string(bk-1)+"].nevprt,told,block_"+rdnom+"["+string(bk-1)+"].xd, \";
        "block_"+rdnom+"["+string(bk-1)+"].x,&block_"+rdnom+"["+string(bk-1)+"].nx, \";
        "block_"+rdnom+"["+string(bk-1)+"].z,&block_"+rdnom+"["+string(bk-1)+"].nz,block_"+rdnom+"["+string(bk-1)+"].evout, \";
        "&block_"+rdnom+"["+string(bk-1)+"].nevout,block_"+rdnom+"["+string(bk-1)+"].rpar,&block_"+rdnom+"["+string(bk-1)+"].nrpar, \";
        "block_"+rdnom+"["+string(bk-1)+"].ipar,&block_"+rdnom+"["+string(bk-1)+"].nipar"];
        if (funtyp(bk)>2000 & funtyp(bk)<3000)
            blank = get_blank(funs(bk)+"( ");
            txtc(1) = funs(bk)+txtc(1);
        elseif (funtyp(bk)<2000)
            txtc(1) = "C2F("+funs(bk)+")"+txtc(1);
            blank = get_blank("C2F("+funs(bk)+") ");
        end
        if nin>=1 | nout>=1 then
            txtc($)=txtc($)+", \"
            txtc=[txtc;""]
            if nin>=1 then
                for k=1:nin
                    uk=inplnk(inpptr(bk)-1+k);
                    nuk=size(outtb(uk),"*");
                    txtc($)=txtc($)+"(SCSREAL_COP *)"+rdnom+"_block_outtbptr["+string(uk-1)+"],&nrd_"+string(nuk)+",";
                end
                txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
            end
            if nout>=1 then
                if nin>=1 then
                    txtc($)=txtc($)+", \"
                    txtc=[txtc;""]
                end
                for k=1:nout
                    yk=outlnk(outptr(bk)-1+k);
                    nyk=size(outtb(yk),"*");
                    txtc($)=txtc($)+"(SCSREAL_COP *)"+rdnom+"_block_outtbptr["+string(yk-1)+"],&nrd_"+string(nyk)+",";
                end
                txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
            end
        end

        if ztyp(bk) then
            txtc($)=txtc($)+", \"
            txtc=[txtc;"w,&nrd_0);"];
        else
            txtc($)=txtc($)+");";
        end

        txtc(2:$) = blank + txtc(2:$);
        txt = [txt;txtc];
        //*******************************************//

        //**
    case 2 then

        //*********** call seq definition ***********//
        txtc=[funs(bk)+"(&flag,&block_"+rdnom+"["+string(bk-1)+"].nevprt,told,block_"+rdnom+"["+string(bk-1)+"].xd, \";
        "block_"+rdnom+"["+string(bk-1)+"].x,&block_"+rdnom+"["+string(bk-1)+"].nx, \";
        "block_"+rdnom+"["+string(bk-1)+"].z,&block_"+rdnom+"["+string(bk-1)+"].nz,block_"+rdnom+"["+string(bk-1)+"].evout, \";
        "&block_"+rdnom+"["+string(bk-1)+"].nevout,block_"+rdnom+"["+string(bk-1)+"].rpar,&block_"+rdnom+"["+string(bk-1)+"].nrpar, \";
        "block_"+rdnom+"["+string(bk-1)+"].ipar,&block_"+rdnom+"["+string(bk-1)+"].nipar, \";
        "(double **)block_"+rdnom+"["+string(bk-1)+"].inptr,block_"+rdnom+"["+string(bk-1)+"].insz,&block_"+rdnom+"["+string(bk-1)+"].nin, \";
        "(double **)block_"+rdnom+"["+string(bk-1)+"].outptr,block_"+rdnom+"["+string(bk-1)+"].outsz, &block_"+rdnom+"["+string(bk-1)+"].nout"];
        if ~ztyp(bk) then
            txtc($)=txtc($)+");";
        else
            txtc($)=txtc($)+", \";
            txtc=[txtc;
            "block_"+rdnom+"["+string(bk-1)+"].g,&block_"+rdnom+"["+string(bk-1)+"].ng);"]
        end
        blank = get_blank(funs(bk)+"( ");
        txtc(2:$) = blank + txtc(2:$);
        txt = [txt;txtc];
        //*******************************************//
        //**
    case 4 then
        txt=[txt;
        funs(bk)+"(&block_"+rdnom+"["+string(bk-1)+"],flag);"];

    end
endfunction

//
//16/06/07 Author : ?, A. Layec
//
function [Code,capt,proto]=call_sensor(i)
    nin=inpptr(i+1)-inpptr(i); ///* number of input ports */
    nout=outptr(i+1)-outptr(i); ///* number of output ports */

    //declaration des I/O des blocs de type 1
    if funtyp(i)==0 then
        if nout==0 then
            yk    = 0;
            nyk_1 = 0;
            nyk_2 = 0;
            yk_t  = 1;
            //Code($+1)=+'  args[1]=(double *)(outtbptr[0]);';
        else
            yk    = outlnk(outptr(i));
            nyk_1 = size(outtb(yk),1);
            nyk_2 = size(outtb(yk),2);
            yk_t  = mat2scs_c_nb(outtb(yk));
            //Code($+1)='  args[1]=('+mat2scs_c_ptr(outtb(yk))+' *)(outtbptr['+string(yk-1)+']);';
        end

    end
    capt=[i yk nyk_1 nyk_2 yk_t bllst(i).ipar]
    Code($+1)="block_"+rdnom+"["+string(i-1)+"].nevprt=nevprt;"
    Code=["/* Call of sensor (blk nb "+string(i)+") */"
    Code;
    "nport = "+string(nbcap)+";";
    rdnom+"_sensor(&flag, &nport, &block_"+rdnom+"["+string(i-1)+"].nevprt, "+..
    "told, ("+mat2scs_c_ptr(outtb(yk))+" *)(outtbptr["+string(yk-1)+"]), &nrd_"+string(nyk_1)+..
    ", &nrd_"+string(nyk_2)+", &nrd_"+string(yk_t)+",aaa);"];
    proto="void "+rdnom+"_sensor("+..
    "int *, int *, int *, double *,void *, int *, int *,int *, int);"
    proto=cformatline(proto,70);
endfunction

//
//22/11/16 Author : U.Durak
//

function curNotAllowedBlk = check_super_blk (diagram, curNotAllowedBlk)

    //***********************************************************
    //Check blocks properties and adapt them if necessary
    //***********************************************************

    //not supported blocks
    notAllowedBlk = ["ANDBLK"; "AUTOMAT"; "CMSCOPE"; "DELAY_f"; "DELAYV_f";
                     "CLOCK_c"; "CLOCK_f"; "VirtualCLK0";"ANDLOG_f";
                     "CEVENTSCOPE"; "CLKGotoTagVisibility"; "HALT_f";
                     "CLKOUTV_f"; "CLKSOMV_f"; "EDGE_TRIGGER";
                     "END_c"; "ESELECT_f"; "EVTDLY_c"; "EVTGEN_f"; "EVTVARDLY";
                     "Extract_Activation"; "IFTHEL_f"; "M_freq"; "MCLOCK_f";
                     "MFCLCK_f"; "DFLIPFLOP"; "DLATCH"; "JKFLIPFLOP"; "SRFLIPFLOP";
                     "CLKINV_f"; "INIMPL_f"; "OUTIMPL_f"; "PDE";
                     "GENERAL_f"; "POSTONEG_f"; "ZCROSS_f";
                     "FROMMO"; "GOTOMO"; "GotoTagVisibilityMO"; "TEXT_f";
                     "AFFICH_m"; "BARXY"; "CANIMXY"; "CANIMXY3D"; "CFSCOPE";
                     "CMAT3D"; "CMATVIEW"; "CSCOPE"; "CSCOPXY"; "INTRP2BLK_f";
                     "CSCOPXY3D"; "TOWS_c"; "WRITEAU_f"; "WRITEC_f"; "FROMWSB";
                     "READAU_f"; "READC_f"; "RFILE_f"; "VARIABLE_DELAY"; "PULSE_SC";
                     "NEGTOPOS_f"; "ISELECT_m"; "FMInterface"; "SimpleFMU"; "FMU_WRAPPER";
                     "TKSCALE"; "BOUNCE"; "BOUNCEXY"; "BPLATFORM"; "CBLOCK";
                     "CBLOCK4";"DEBUG"; "MBLOCK"; "c_block"; "fortran_block";
                     "generic_block3";"scifunc_block_m"; "RELAY_f"; "SELECT_m";
                     "GotoTagVisibility"; "Sigbuilder"; "TIME_DELAY";
                     "CONSTRAINT_c"; "CONSTRAINT2_c"; "DIFF_f"];

    for i=1:size(diagram.objs)
        if typeof(diagram.objs(i)) == "Block" then
            if diagram.objs(i).gui == "SUPER_f" then
                curNotAllowedBlk($+1) = "SUPER_f";
            elseif ~isempty(diagram.objs(i).model.equations) then
                ok=%f;%cpr=list();
                message("Superblock should not have any implicit blocks.");
                error(999, msprintf("Superblock should not have any implicit blocks."));
                return
            elseif diagram.objs(i).gui == "SampleCLK" then
                if ~isClock then
                    stepVl = string(diagram.objs(i).model.rpar(1));
                    stepInit = string(diagram.objs(i).model.rpar(2));
                    isClock = %t;
                else
                    ok=%f;%cpr=list();
                    message("Superblock should have only one SampleCLK block.");
                    error(999, msprintf("Superblock should have only one SampleCLK block."));
                    return
                end
            elseif diagram.objs(i).gui == "SWITCH2_m" then
                diagram.objs(i).model.nmode = 0;
                diagram.objs(i).model.nzcross = 0;
            elseif diagram.objs(i).gui == "CLINDUMMY_f" then
                diagram.objs(i).model.sim = list("cdummy",4);
            end
            curNotAllowedBlk($+1) = notAllowedBlk(notAllowedBlk == diagram.objs(i).gui);
        end
    end

endfunction


function reject = will_reject_super_blk(diagram, notAllowedBlk)
    // recursive scan for blocks rejected by the FMU generation
    reject = %f;
    for i = 1:size(diagram.objs)
        if typeof(diagram.objs(i)) == "Block" then
            if or(diagram.objs(i).gui == notAllowedBlk) | ~isempty(diagram.objs(i).model.equations) then
                reject = %t;
                return;
            end
            if diagram.objs(i).gui == "SUPER_f" & will_reject_super_blk(diagram.objs(i).model.rpar, notAllowedBlk) then
                reject = %t;
                return;
            end
        end
    end
endfunction


function  [ok,XX,alreadyran,flgcdgen,szclkINTemp,freof] = do_compile_super_fmu(XX,all_scs_m,numk,alreadyran)

    scs_m=XX.model.rpar
    //not supported blocks
    notAllowedBlk = ["ANDBLK"; "AUTOMAT"; "CMSCOPE"; "DELAY_f"; "DELAYV_f";
                     "CLOCK_c"; "CLOCK_f"; "VirtualCLK0";"ANDLOG_f";
                     "CEVENTSCOPE"; "CLKGotoTagVisibility"; "HALT_f";
                     "CLKOUTV_f"; "CLKSOMV_f"; "EDGE_TRIGGER";
                     "END_c"; "ESELECT_f"; "EVTDLY_c"; "EVTGEN_f"; "EVTVARDLY";
                     "Extract_Activation"; "IFTHEL_f"; "M_freq"; "MCLOCK_f";
                     "MFCLCK_f"; "DFLIPFLOP"; "DLATCH"; "JKFLIPFLOP"; "SRFLIPFLOP";
                     "CLKINV_f"; "INIMPL_f"; "OUTIMPL_f"; "PDE";
                     "GENERAL_f"; "POSTONEG_f"; "ZCROSS_f";
                     "FROMMO"; "GOTOMO"; "GotoTagVisibilityMO"; "TEXT_f";
                     "AFFICH_m"; "BARXY"; "CANIMXY"; "CANIMXY3D"; "CFSCOPE";
                     "CMAT3D"; "CMATVIEW"; "CSCOPE"; "CSCOPXY"; "INTRP2BLK_f";
                     "CSCOPXY3D"; "TOWS_c"; "WRITEAU_f"; "WRITEC_f"; "FROMWSB";
                     "READAU_f"; "READC_f"; "RFILE_f"; "VARIABLE_DELAY"; "PULSE_SC";
                     "NEGTOPOS_f"; "ISELECT_m"; "FMInterface"; "SimpleFMU"; "FMU_WRAPPER";
                     "TKSCALE"; "BOUNCE"; "BOUNCEXY"; "BPLATFORM"; "CBLOCK";
                     "CBLOCK4";"DEBUG"; "MBLOCK"; "c_block"; "fortran_block";
                     "generic_block3";"scifunc_block_m"; "RELAY_f"; "SELECT_m";
                     "GotoTagVisibility"; "Sigbuilder"; "TIME_DELAY";
                     "CONSTRAINT_c"; "CONSTRAINT2_c"; "DIFF_f"];
    // re-evaluate blocks through their interfaces, like scicos_simulate does with do_eval
    // skipped when the generation will be rejected: not supported blocks
    // (e.g. PDE) hang in their set interface
    if ~will_reject_super_blk(scs_m, notAllowedBlk) then
        if exists("%scicos_context") then
            ctx = %scicos_context;
        else
            ctx = struct();
        end
        needcompile = 4;
        scs_m = do_eval(scs_m, list(), ctx);
        // interface blocks lose their sizes when re-evaluated: restore them
        for i = 1:size(scs_m.objs)
            if typeof(scs_m.objs(i)) == "Block" & ...
                or(scs_m.objs(i).gui == ["IN_f" "OUT_f" "CLKIN_f" "CLKOUT_f"]) then
                scs_m.objs(i) = XX.model.rpar.objs(i);
            end
        end
    end
    par=scs_m.props;
    if alreadyran then
        //terminate current simulation
        do_terminate()
        alreadyran=%f
    end
    hname=scs_m.props.title(1) //superblock name
    //***********************************************************
    //Check blocks properties and adapt them if necessary
    //***********************************************************

    IN=[];
    OUT=[];
    clkIN=[];
    clkOUT=[];
    numa=0;
    numc=0;
    writeGlobal = [];
    writeGlobalSize = [];
    readGlobal = [];
    readGlobalTimeSize = [];
    readGlobalSize = [];
    isClock = %f;
    stepVl = "0.01";
    stepInit = "0";
    //not supported blocks
    curNotAllowedBlk = [];
    for i=1:size(scs_m.objs)
        if typeof(scs_m.objs(i))=="Block" then
            if scs_m.objs(i).gui=="CLKOUT_f" then
                ok=%f;%cpr=list();
                message("Superblock should not have any activation output port.")
                error(999, msprintf("Superblock should not have any activation output port."));
                return
            elseif scs_m.objs(i).gui=="IN_f" then
                //replace input ports by sensor blocks
                numc=numc+1
                scs_m.objs(i).gui="INPUTPORTEVTS";
                scs_m.objs(i).model.evtin=1
                scs_m.objs(i).model.sim(1)="capteur"+string(numc)
                IN=[IN scs_m.objs(i).model.ipar]
            elseif scs_m.objs(i).gui=="OUT_f" then
                //replace output ports by actuator blocks
                numa=numa+1
                scs_m.objs(i).gui="OUTPUTPORTEVTS";
                scs_m.objs(i).model.sim(1)="actionneur"+string(numa)
                OUT=[OUT  scs_m.objs(i).model.ipar]
            elseif ~isempty(scs_m.objs(i).model.equations) then
                ok=%f;%cpr=list();
                message("Superblock should not have any implicit blocks.");
                error(999, msprintf("Superblock should not have any implicit blocks."));
                return
            elseif scs_m.objs(i).gui == "SampleCLK" then
                if ~isClock then
                    stepVl = string(scs_m.objs(i).model.rpar(1));
                    stepInit = string(scs_m.objs(i).model.rpar(2));
                    isClock = %t;
                else
                    ok=%f;%cpr=list();
                    message("Superblock should have only one SampleCLK block.");
                    error(999, msprintf("Superblock should have only one SampleCLK block."));
                    return
                end
            elseif scs_m.objs(i).gui == "CLINDUMMY_f" then
                // not fixed in 5.5.0, to see the bug 12751
                scs_m.objs(i).model.sim = list("cdummy",4);
            elseif scs_m.objs(i).gui == "SWITCH2_m" then
                // workaround
                // zero crossing does not supported
                scs_m.objs(i).model.nmode = 0;
                scs_m.objs(i).model.nzcross = 0;
            elseif scs_m.objs(i).gui == "SUPER_f" then // check inside of superblock
                curNotAllowedBlk = check_super_blk(scs_m.objs(i).model.rpar, curNotAllowedBlk)
            end
            curNotAllowedBlk($+1) = notAllowedBlk(notAllowedBlk == scs_m.objs(i).gui);
        end
    end
    // error in the console
    if curNotAllowedBlk <> [] then
        ok=%f;%cpr=list();
        message("Not supported blocks are on the diagram.");
        error(999, msprintf("Block ""%s"" not supported for the FMU generation.\n", curNotAllowedBlk));
        return
    end

    //Check if input/output ports are numered properly
    IN=-gsort(-IN);
    if or(IN<>[1:size(IN,"*")]) then
        ok=%f;%cpr=list()
        message("Input ports are not numbered properly.")
        return
    end
    OUT=-gsort(-OUT);
    if or(OUT<>[1:size(OUT,"*")]) then
        ok=%f;%cpr=list()
        message("Output ports are not numbered properly.")
        return
    end
    clkIN=-gsort(-clkIN);
    if or(clkIN<>[1:size(clkIN,"*")]) then
        ok=%f;%cpr=list()
        message("Event input ports are not numbered properly.")
        return
    end
    clkOUT=-gsort(-clkOUT);
    if or(clkOUT<>[1:size(clkOUT,"*")]) then
        ok=%f;%cpr=list()
        message("Event output ports are not numbered properly.")
        return
    end

    //Check if there is more than one clock in the diagram
    szclkIN=size(clkIN,2);
    if szclkIN==0 then
        szclkIN=[]
    end
    flgcdgen=szclkIN;
    [bllst,connectmat,clkconnect,cor,corinv,ok,scs_m,flgcdgen,freof]=c_pass1(scs_m,flgcdgen);

    // Check for any To Workspace/FromWorkspace block
    for i = 1:size(bllst)
        if bllst(i).sim(1) == "tows_c" then
            // Explore scs_m to find original block
            towsObjIndex = corinv(i);
            towsObj = scs_m.objs(towsObjIndex(1));
            for j = 2:size(towsObjIndex, "*")
                towsObj = towsObj.model.rpar.objs(towsObjIndex(j));
            end
            bllst(i).sim(1) = "writeGlobal_" + ...
            towsObj.graphics.exprs(2);
            // Force state to enable case 2 call
            // on generated code
            bllst(i).dstate = 0;
            writeGlobal = [writeGlobal towsObj.graphics.exprs(2)];
            writeGlobalSize = [writeGlobalSize bllst(i).ipar(1)];
        elseif bllst(i).sim(1) == "fromws_c" then
            fromwsObjIndex = corinv(i);
            fromwsObj = scs_m.objs(fromwsObjIndex(1));
            for j = 2:size(fromwsObjIndex, "*")
                fromwsObj = fromwsObj.model.rpar.objs(fromwsObjIndex(j));
            end
            bllst(i).sim(1) = "readGlobal_" + fromwsObj.graphics.exprs(1);
            readGlobal = [readGlobal fromwsObj.graphics.exprs(1)];
            readGlobalTimeSize = [readGlobalTimeSize evstr("size(" + fromwsObj.graphics.exprs(1) + ".time, ""*"")")]
            readGlobalSize = [readGlobalSize evstr("size(" + fromwsObj.graphics.exprs(1) + ".values(1, :), ""*"")")]
        end
    end

    if flgcdgen<> szclkIN
        clkIN=[clkIN flgcdgen]
    end
    szclkINTemp=szclkIN;
    szclkIN=flgcdgen;
    // [bllst,connectmat,clkconnect,cor,corinv,ok]=c_pass1(scs_m);

    //Test for  ALWAYS_ACTIVE sblock (RN -25/06/07)
    ALWAYS_ACTIVE=%f;
    for blki=bllst
        if blki.dep_ut($) then
            ALWAYS_ACTIVE=%t;
            break;
        end
    end
    if ALWAYS_ACTIVE then
        CAPTEURS=[];
        for Ii=1:length(bllst)
            if part(bllst(Ii).sim(1),1:7)=="capteur" then
                bllst(Ii).dep_ut($)=%t
                //       elseif part(bllst(Ii).sim(1),1:10)=='actionneur' then
                //         bllst(Ii).dep_ut($)=%t
            end
        end
    end

    if ~ok then
        message("Sorry: problem in the pre-compilation step.")
        return
    end
    a=[];
    b=[];
    tt=[];
    howclk=[];
    allhowclk=[];
    allhowclk2=[];
    cap=[];
    act=[];

    ///**********************************
    for i=1:size(bllst)
        // check for a scilab function block
        if type(bllst(i).sim(1)) == 13 then
            ok=%f;
            %cpr=list();
            message(_("Superblock should not contains any Scilab function block."))
            return
        end

        for j=1:size(bllst)
            if (bllst(i).sim(1)=="actionneur"+string(j)) then
                if tt<>i then
                    act=[act;i];
                    tt=i;
                end
            elseif (bllst(i).sim(1)=="capteur"+string(j)) then
                if tt<>i then
                    cap=[cap;i];
                    tt=i;
                end
            elseif (bllst(i).sim(1)=="bidon") then
                if tt<>i then
                    allhowclk=[allhowclk;i];
                    tt=i;
                end
            elseif (bllst(i).sim(1)=="bidon2") then
                if tt<>i then
                    allhowclk2=[allhowclk2;i];
                    tt=i;
                end
            end
        end
    end
    ///**********************************
    if szclkIN>1 then
        //replace the N Event inputs by a fictious block with 2^N as many event
        //outputs
        output=ones((2^szclkIN)-1,1)
        bllst($+1)=scicos_model(sim=list("bidon",1),evtout=output,..
        blocktype="d",..
        firing=-output',dep_ut=[%f %f])
        corinv(size(bllst))=size(bllst)+1;
        howclk=size(bllst)
        // adjust the links accordingly
        for i=1:(2^szclkIN)-1
            vec=codebinaire(i,szclkIN)
            for j=1:szclkIN
                if vec(j)*allhowclk(j)>=1 then
                    for k=1:size(clkconnect,1)
                        if clkconnect(k,1)==allhowclk(j) then
                            clkconnect=[clkconnect;[howclk i clkconnect(k,3:4)]]
                        end
                    end
                end
            end
        end
    elseif szclkIN==[]&~ALWAYS_ACTIVE then
        //superblock has no Event input, add a fictious clock
        output=ones((2^(size(cap,"*")))-1,1)
        if (output == []) then
            output=0;
        end
        bllst($+1)=scicos_model(sim=list("bidon",1),evtout=output,..
        firing=-output,blocktype="d",dep_ut=[%f %f])
        corinv(size(bllst))=size(bllst)+1;
        howclk=size(bllst);
    elseif szclkIN==1  then
        howclk=allhowclk;
    end

    //mise en ordre de clkconnect
    if szclkIN>1 then
        newclkconnect=clkconnect;
        clkconnect=[];
        for i=1:size(newclkconnect,1)-1
            if or(newclkconnect(i,:)<>newclkconnect(i+1,:)) then
                clkconnect=[clkconnect;newclkconnect(i,:)]
            end
        end
        if or(newclkconnect($-1,:)<>newclkconnect($,:)) then
            clkconnect=[clkconnect;newclkconnect($,:)]
        end

        //suppression des blocs bidons
        newclkconnect=clkconnect;nkt=[];
        for i=1:szclkIN
            for k=1:size(newclkconnect,1)
                if newclkconnect(k,1)~=allhowclk(i) then
                    nkt=[nkt;newclkconnect(k,:)];
                end
            end
            newclkconnect=nkt;
            nkt=[];
        end
        clkconnect=newclkconnect;
    end

    //**************************************************
    // nouveau clkconnect avec liaisons sur les capteurs
    //**************************************************

    //
    // Generate block activation fo C code
    //
    n=size(cap,1)
    if ~(szclkIN==[]) then
        // Activate sensors on each clock pulse.
        generatedConnection = [];
        for i=1:n
            if szclkIN>1 then
                for j=1:(2^szclkIN)-1
                    generatedConnection = [generatedConnection ; [howclk j cap(i) 1]];
                end
            elseif szclkIN==1 then
                generatedConnection = [generatedConnection ; [howclk 1 cap(i) 1]];
            end
        end
        clkconnect = [clkconnect ; generatedConnection]
    elseif ~ALWAYS_ACTIVE then
        // Generate all possible activations for sensors, based on number of blocks.
        //codage de l'activation des capteurs dans le cas de l'heritage
        generatedConnection = [];
        for i=1:2^n-1
            vec=codebinaire(i,n);
            for j=1:n
                if (vec(j)==1) then
                    generatedConnection = [generatedConnection ; [howclk i cap(j) 1]];
                end
            end
        end
        clkconnect = [clkconnect ; generatedConnection]
    end

    FIRING=[]
    for i=1:size(allhowclk2,1)
        j = find(clkconnect(:,3)==allhowclk2(i))
        if j<>[] then
            FIRING=[FIRING;bllst(clkconnect(j,1)).firing(clkconnect(j,2))]
        end
    end

    Code_gene_run=[];

    //** OLD GRAPHICS
    //** %windo=xget('window')

    cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv)

    if cpr==list() then disp("DBG: c_pass2 failed"); ok=%f,return, end

    //** Alan's patch 5/07/07: try to solve
    //   which blocks use work
    funs_save=cpr.sim.funs;
    funtyp_save=cpr.sim.funtyp;
    with_work = zeros(cpr.sim.nblk,1)
    for i=1:size(cpr.sim.funs)
        if part(cpr.sim.funs(i),1:10)=="actionneur" then
            cpr.sim.funs(i) ="bidon"
            cpr.sim.funtyp(i) = 1
        elseif part(cpr.sim.funs(i),1:7)=="capteur" then
            cpr.sim.funs(i) ="bidon"
            cpr.sim.funtyp(i) = 1
        end
    end

    //**
    //** retrieve all open Scilab windows with winsid()
    //**

    BeforeCG_WinList = winsid();

    ierr=execstr("[state,t]=scicosim(cpr.state,0,0,cpr.sim,"+..
    "''start'',scs_m.props.tol)","errcatch")
    if ierr==0 then
        for i=1:cpr.sim.nblk
            if state.iz(i)<>0 then
                with_work(i)=%t
            end
        end
        ierr=execstr("[state,t]=scicosim(state,0,0,cpr.sim,"+..
        "''finish'',scs_m.props.tol)","errcatch")
    end

    //**
    //** retrieve all open Scilab windows with winsid
    //** and close the additional windows open since first
    //**

    //** This code does not cover 100% of the possible situations because the user can
    //** destroy one or more Scicos wins manually during this intermediate phase
    //** This code is 100% functional if the user does not close manually any win.
    //** TO BE updated in Scilab 5.0

    AfterCG_WinList = winsid();

    AfterCG_size = size(AfterCG_WinList); //** matrix
    AfterCG_size = AfterCG_size(2) ; //** vector length

    BeforeCG_size = size(BeforeCG_WinList); //** matrix
    BeforeCG_size = BeforeCG_size(2) ; //** vector length

    if (or(AfterCG_WinList<>BeforeCG_WinList)) & (AfterCG_size>BeforeCG_size) then
        //** means that a scope or other scicos object has created some
        //** output window

        DiffCG_Winlist = AfterCG_WinList<>BeforeCG_WinList ; //** T/F mismatch
        DiffCG_Index = find(DiffCG_Winlist); //** recover the mismatched indexes only

        for win_idx = DiffCG_Index
            delete( scf( AfterCG_WinList(win_idx) ) ) ; //** clear the spurious windows
        end

    end
    //**------------- end of windows cleaning ----------------------------------------

    cpr.sim.funs=funs_save;
    cpr.sim.funtyp=funtyp_save;

    //** OLD GRAPHICS
    //** xset('window',%windo)

    ///////////////////
    //les pointeurs de cpr :
    x=cpr.state.x;
    z=cpr.state.z;
    outtb=cpr.state.outtb;

    //RN
    zcptr=cpr.sim.zcptr;
    ozptr=cpr.sim.ozptr;
    rpptr=cpr.sim.rpptr;
    ipptr=cpr.sim.ipptr;
    opptr=cpr.sim.opptr;
    funs=cpr.sim.funs;
    xptr=cpr.sim.xptr;
    zptr=cpr.sim.zptr;
    inpptr=cpr.sim.inpptr;
    inplnk=cpr.sim.inplnk;
    outptr=cpr.sim.outptr;
    outlnk=cpr.sim.outlnk;
    //@l@n lnkptr=cpr.sim.lnkptr;
    ordclk=cpr.sim.ordclk;
    funtyp=cpr.sim.funtyp;
    cord=cpr.sim.cord;
    ncord=size(cord,1);
    nblk=cpr.sim.nb;
    ztyp=cpr.sim.ztyp;
    clkptr=cpr.sim.clkptr
    labels=cpr.sim.labels;
    //taille totale de z : nztotal
    nztotal=size(z,1);

    //*******************************
    //Checking if superblock is valid
    //*******************************
    msg=[]
    for i=1:length(funs)-1
        if funtyp(i)==3 then
            msg=[msg;"Type 3 block''s not allowed"]
            break;
        end
        if msg<>[] then message(msg),ok=%f,return,end
    end

    //********************************************************
    // Change logical units for readf and writef blocks if any ???
    //********************************************************
    lunit=0
    for d=1:length(funs)
        if funs(d)=="readf"  then
            z(zptr(d)+2)=lunit
            lunit=lunit+1;
        elseif funs(d)=="writef"
            z(zptr(d)+1)=lunit
            lunit=lunit+1;
        end
    end

    //***********************************
    // Get the name of the file
    //***********************************
    foo=3;
    okk=%f;
    rdnom="foo";
    rpat=pwd();
    [x,ierr] = fileinfo(rpat);
    S_IWRITE = 128; // mask write permission
    if (ierr <> 0 | bitand(x(2), S_IWRITE) == 0) then
        rpat = TMPDIR;
    end

    libs="";
    descr = ""; author = ""; vers = ""; typ = "me";

    label1=[hname;rpat+"/"+hname;typ;descr;author;vers];
    while %t do
        ok=%t  // to avoid infinite loop
        [okk,..
        rdnom,..
        rpat,..
        typ,..
        descr,..
        author,..
        vers,..
        label1]=scicos_getvalue("Set export FMU parameters :",..
        ["Model name :";
        "Created model path :";
        "Model type (me or cs) :";
        "Model description :";
        "Author :";
        "Version :"],..
        list("str",1,"str",1,"str",1,...
             "str",1,"str",1,"str",1),label1);
        if okk==%f then
            ok=%f
            return
        end
        typ = convstr(typ, "l");
        if and(typ <> (["me","cs"])) then
            ok=%f
            messagebox(["Answer given for ""Model type"" is incorrect:"...
                        "me or cs expected."],"modal","error");
        end
        rpat=stripblanks(rpat);

        //** Alan, I put a warning here in order to inform the user
        //** that the name of the superblock will change
        //** because the space char in name isn't allowed.
        if grep(rdnom," ")<>[] then
            message(["Superblock name cannot contains space characters.";
            "space chars will be automatically substituted by ""_"" "])
        end
        rdnom = strsubst(rdnom," ","_");

        //** Put a warning here in order to inform the user
        //** that the name of the superblock will change
        //** because the "-" char could generate GCC problems
        //** (the C functions contains the name of the superblock).
        if grep(rdnom,"-")<>[] then
            message(["For full C compiler compatibility ";
            "Superblock name cannot contains ""-"" characters";
            """-"" chars will be automatically substituted by ""_"" "])
        end
        rdnom = strsubst(rdnom,"-","_");

        dirinfo=fileinfo(rpat)

        if dirinfo==[] then
            [pathrp,fnamerp,extensionrp]=fileparts(rpat)
            ok=mkdir(pathrp,fnamerp+extensionrp)
            if ~ok then
                messagebox("Directory "+rpat+" cannot be created","modal","info");
            end
        elseif filetype(dirinfo(2))<>"Directory" then
            ok=%f;
            messagebox(rpat+" is not a directory","modal","error");
        end

        if stripblanks(rdnom)==emptystr() then
            ok=%f;
            messagebox("sorry C file name not defined","modal","error");
        end
        if ok then break,end
    end

    mprintf("\n"+gettext("   Generating ")+"%s.fmu\n", rpat+filesep()+rdnom);
    //////////////////////////////////////////////////
    maxnrpar=max(rpptr(2:$)-rpptr(1:$-1))
    maxnipar=max(ipptr(2:$)-ipptr(1:$-1))
    maxnx=max(xptr(2:$)-xptr(1:$-1))
    maxnz=max(zptr(2:$)-zptr(1:$-1))
    maxnin=max(inpptr(2:$)-inpptr(1:$-1))
    maxnout=max(outptr(2:$)-outptr(1:$-1))
    maxdim=[];
    for i=1:size(cpr.state.outtb)
        maxdim=max(size(cpr.state.outtb(i)))
    end
    maxtotal=max([maxnrpar;maxnipar;maxnx;maxnz;maxnin;maxnout;maxdim]);

    //************************************************************************
    //generate the call to the blocks and blocs simulation function prototypes
    //************************************************************************
    mprintf(gettext("   Generate calls to Scilab blocks\n"));
    wfunclist=list();
    nbcap=0;nbact=0;capt=[];actt=[];Protostalone=[];Protos=[];
    dfuns=[]
    for i=1:length(funs)
        if or(i==act) then //block is an actuator
            nbact=nbact+1;
            [Code,actti,protoi]=call_actuator(i)
            wfunclist($+1)=[Code;"if(flag < 0 ) return(5 - flag);"]
//            if nbact==1 then Protostalone=[Protostalone;"";protoi],end
            actt=[actt;actti]
        elseif or(i==cap) then //block is a sensor
            nbcap=nbcap+1;
            [Code,capti,protoi]=call_sensor(i)
            wfunclist($+1)=[Code;"if(flag < 0 ) return(5 - flag);"]
//            if nbcap==1 then Protostalone=[Protostalone;"";protoi] ,end
            capt=[capt;capti]
        elseif funs(i)=="bidon"
            wfunclist($+1)=" "
        elseif funs(i)=="bidon2"
            wfunclist($+1)=" "
        else
            ki=find(funs(i)==dfuns)
            dfuns=[dfuns;funs(i)]
            //[Code,protoi]=call_block(i)
            [Code]=call_block4(i)
            if Code<>[] then
                wfunclist($+1)=[Code;"if(flag < 0 ) return(5 - flag);"]
            else
                wfunclist($+1)=" ";
            end
            if ki==[] then
                Protos=[Protos;"";BlockProto(i)];
                Protostalone=[Protostalone;"";BlockProto(i)];
            end
        end
    end

    //**************************************
    //on prend en compte l'ordre des numerotation graphique des
    //capteur et des actionneur
    [junk,index]=gsort(-actt(:,$));
    actt=actt(index,1:$) ;
    [junk,index]=gsort(-capt(:,$));
    capt=capt(index,1:$) ;

    //***************************************************
    //Compute the initial state and outtb (links)  values
    //***************************************************
    //petite modification pour uniformiser les fichiers capteurs
    //et actionneurs ???
    rdcpr=cpr.sim.funs;
    for r=1:length(cap),rdcpr(cap(r))="bidon";end
    for r=1:length(act),rdcpr(act(r))="bidon";end
    Total_rdcpr=cpr.sim;Total_rdcpr.funs=rdcpr;
    //
    tcur=0;
    tf=scs_m.props.tf;
    tolerances=scs_m.props.tol;
    //[state,t]=scicosim(cpr.state,tcur,tf,Total_rdcpr,'start',tolerances);
    //cpr.state=state;
    z=cpr.state.z;
    outtb=cpr.state.outtb;
    //[junk_state,t]=scicosim(cpr.state,tcur,tf,Total_rdcpr,'finish',tolerances);

    //***********************************
    // Get all parameters of Xcos blocks
    //***********************************
    if ok then
        mprintf(gettext("   Generate scheduling\n"));
        script = "funcNames = make_names()";
        if exists("FMU_WRAPPER_BACKTRACE") && FMU_WRAPPER_BACKTRACE then
            ierr = 0;
            execstr(script);
        else
            ierr = execstr(script, "errcatch");
        end
        if ierr <> 0 then
            mprintf("   Scheduling has not been created: %s\n", lasterror());
            ok = %f;
        end
    end
    //***********************************
    // FMU Xml description generating
    //***********************************
    if ok then
       mprintf(gettext("   Generate FMU XML\n"));
       script = "[ok, description] = gen_fmuXML_description(funcNames)";
        if exists("FMU_WRAPPER_BACKTRACE") && FMU_WRAPPER_BACKTRACE then
            ierr = 0;
            execstr(script);
       else
            ierr = execstr(script, "errcatch");
       end
       if ierr <> 0 then
           mprintf("   XML description for FMU has not been generated: %s\n", lasterror());
           ok = %f;
       end
    end
    //***********************************
    // FMU code generating
    //***********************************
    if ok then
        mprintf(gettext("   Generate FMU API " + convstr(typ, "u") + "\n"));
        script = "make_standalone42(rpat+""/""+rdnom+""_Scilab_FMU.c"")"
        if exists("FMU_WRAPPER_BACKTRACE") && FMU_WRAPPER_BACKTRACE then
            ierr = 0;
            execstr(script);
        else
            ierr = execstr(script, "errcatch");
        end
        fmuFileName = rdnom+"_Scilab_FMU.c";
        if ierr <> 0 then
            mprintf("   FMU API has not been generated: %s\n", lasterror());
            ok = %f;
        end
    end
    if ok then
        mprintf(gettext("   Compile\n"));
        script = "ok = build_model(rdnom+""_fmiGetVersion"", fmuFileName, rpat, """", """")";
        if exists("FMU_WRAPPER_BACKTRACE") && FMU_WRAPPER_BACKTRACE then
            ierr = 0;
            execstr(script);
        else
            ierr = execstr(script, "errcatch");
        end
        if ierr <> 0 then
            mprintf("   FMU has not been compiled: %s\n", lasterror());
            ok = %f;
        end
    end
    //***********************************
    // FMU zip file creating
    //***********************************
    if ok then
        mprintf(gettext("   Create the FMU\n"));
        script = "ok = zip_model(fmuFileName)";
        if exists("FMU_WRAPPER_BACKTRACE") && FMU_WRAPPER_BACKTRACE then
            ierr = 0;
            execstr(script);
        else
            ierr = execstr(script, "errcatch");
        end
        if ierr <> 0 then
            mprintf("   FMU has not been created: %s\n", lasterror());
            ok = %f;
        end
    end

    if ok then
        mprintf("   Generating finished successfully.\n")
        message([_("FMU "+rdnom+" generated successfully.")]);
    else
        mprintf("   Generating failed.\n")
        messagebox(_("FMU "+rdnom+" has not been generated."), "modal", "error");
    end

endfunction

//BlockProto : generate prototype
//            of a scicos block
//
//inputs : bk   : bloc index
//
//output : txt  :
//
//16/06/07 Author : A.Layec
function [txt]=BlockProto(bk)

    nin=inpptr(bk+1)-inpptr(bk);  //* number of input ports */
    nout=outptr(bk+1)-outptr(bk); //* number of output ports */
    //**
    ftyp=funtyp(bk)
    if ftyp>2000 then ftyp=ftyp-2000,end
    if ftyp>1000 then ftyp=ftyp-1000,end

    //** check function type
    if ftyp < 0 then //** ifthenelse eselect blocks
        txt = [];
        return;
    else
        if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4) then
            disp("types other than 0,1,2,3 or 4 are not yet supported.")
            txt = [];
            return;
        end
    end
    //** add comment
    txt=[get_comment("proto_blk",list(funs(bk),funtyp(bk),bk,labels(bk)));]

    select ftyp
        //** zero funtyp
    case 0 then
        //*********** prototype definition ***********//
        txtp=["(int *, int *, double *, double *, double *, int *, double *, \";
        " int *, double *, int *, double *, int *,int *, int *, \";
        " double *, int *, double *, int *);"];
        if (funtyp(bk)>2000 & funtyp(bk)<3000)
            blank = get_blank("void "+funs(bk)+"(");
            txtp(1) = "void "+funs(bk)+txtp(1);
        elseif (funtyp(bk)<2000)
            txtp(1) = "void C2F("+funs(bk)+")"+txtp(1);
            blank = get_blank("void C2F("+funs(bk)+")");
        end
        txtp(2:$) = blank + txtp(2:$);
        txt = [txt;txtp];
        //*******************************************//
        //**
    case 1 then

        //*********** prototype definition ***********//
        txtp=["(int *, int *, double *, double *, double *, int *, double *, \";
        " int *, double *, int *, double *, int *,int *, int *";]
        if (funtyp(bk)>2000 & funtyp(bk)<3000)
            blank = get_blank("void "+funs(bk)+"(");
            txtp(1) = "void "+funs(bk)+txtp(1);
        elseif (funtyp(bk)<2000)
            txtp(1) = "void C2F("+funs(bk)+")"+txtp(1);
            blank = get_blank("void C2F("+funs(bk)+")");
        end
        if nin>=1 | nout>=1 then
            txtp($)=txtp($)+", \"
            txtp=[txtp;""]
            if nin>=1 then
                for k=1:nin
                    txtp($)=txtp($)+" double *, int * ,"
                end
                txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
            end
            if nout>=1 then
                if nin>=1 then
                    txtp($)=txtp($)+", \"
                    txtp=[txtp;""]
                end
                for k=1:nout
                    txtp($)=txtp($)+" double *, int * ,"
                end
                txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
            end
        end
        if ztyp(bk) then
            txtp($)=txtp($)+", \"
            txtp=[txtp;" double *,int *);"];
        else
            txtp($)=txtp($)+");";
        end
        txtp(2:$) = blank + txtp(2:$);
        txt = [txt;txtp];
        //*******************************************//
        //**
    case 2 then
        //*********** prototype definition ***********//
        txtp=["void "+funs(bk)+...
        "(int *, int *, double *, double *, double *, int *, double *, \";
        " int *, double *, int *, double *, int *, int *, int *, \"
        " double **, int *, int *, double **,int *, int *"];
        if ~ztyp(bk) then
            txtp($)=txtp($)+");";
        else
            txtp($)=txtp($)+", \";
            txtp=[txtp;
            " double *,int *);"]
        end
        blank = get_blank("void "+funs(bk));
        txtp(2:$) = blank + txtp(2:$);
        txt = [txt;txtp];
        //********************************************//
        //**
    case 4 then
        txt=[txt;
        "void "+funs(bk)+"(scicos_block *, int );"];

    end
endfunction

//Generating the routine for actuators interfacing
//
//
//Authors : R. Djenid, R. Nikoukhah, A. Layec
//
//actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]
function Code=make_actuator(standalone)
    Call=["/*"
    "**    Generated by Code_Generation toolbox of Xcos with "+getversion()
    "**    Date : "+date()
    "*/"
    ""
    "/*"+part("-",ones(1,40))+" Actuators */";
    "void "+rdnom+"_actuator(int *flag, int *nport, int *nevprt, double *t, void *u, int *nu1, int *nu2, int *ut, int flag1)"]

    comments=["     /*"
    "      * To be customized for standalone execution";
    "      * flag  : specifies the action to be done"
    "      * nport : specifies the  index of the Super Bloc"
    "      *         regular input (The input ports are numbered"
    "      *         from the top to the bottom )"
    "      * nevprt: indicates if an activation had been received"
    "      *         0 = no activation"
    "      *         1 = activation"
    "      * t     : the current time value"
    "      * u     : the vector inputs value"
    "      * nu1   : the input size 1"
    "      * nu2   : the input size 2"
    "      * ut    : the input type"
    "      * flag1 : learn mode (0 from terminal,1 from input file"
    "      */"]

    dcl=["{"
    "  int j,k,l;"];

    if standalone then
        a_actuator=["  /* skeleton to be customized */"
        "    switch (*flag) {"
        "    /* OutputUpdate (*flag==1) will be called on each iteration */"
        "    case 1 :"
        "      /*if(*nevprt>0) { get the input value */"
        "        switch (*ut) {"
        "        case 10 :"
        "          for (l=0;l<*nu2;l++) {"
        "            for (k=0;k<*nu1;k++) {"
        "              printf(""Actuator: time=%f, "+...
        "u(%d,%d) of actuator %d is %f "+...
        "\n"", \"
        "                     *t, k, l, *nport,"+...
        "*((double *) u+(k+l*(*nu1))));"
        "            }"
        "          }"
        "          break;"
        ""
        "        case 11 :"
        "          for (l=0;l<*nu2;l++) {"
        "            for (k=0;k<*nu1;k++) {"
        "              printf(""Actuator: time=%f, "+...
        "u(%d,%d) of actuator %d is %f,%f "+...
        "\n"", \"
        "                     *t, k, l, *nport,"+...
        "*((double *) u+(k+l*(*nu1))),"+...
        "*((double *) u+((*nu1)*(*nu2)+k+l*(*nu1))));"
        "            }"
        "          }"
        "          break;"
        ""
        "        case 81 :"
        "          for (l=0;l<*nu2;l++) {"
        "            for (k=0;k<*nu1;k++) {"
        "              printf(""Actuator: time=%f, "+...
        "u(%d,%d) of actuator %d is %i "+...
        "\n"", \"
        "                     *t, k, l, *nport,"+...
        "*((char *) u+(k+l*(*nu1))));"
        "            }"
        "          }"
        "          break;"
        ""
        "        case 82 :"
        "          for (l=0;l<*nu2;l++) {"
        "            for (k=0;k<*nu1;k++) {"
        "              printf(""Actuator: time=%f, "+...
        "u(%d,%d) of actuator %d is %hd "+...
        "\n"", \"
        "                     *t, k, l, *nport,"+...
        "*((short *) u+(k+l*(*nu1))));"
        "            }"
        "          }"
        "          break;"
        ""
        "        case 84 :"
        "          for (l=0;l<*nu2;l++) {"
        "            for (k=0;k<*nu1;k++) {"
        "              printf(""Actuator: time=%f, "+...
        "u(%d,%d) of actuator %d is %ld "+...
        "\n"", \"
        "                     *t, k, l, *nport,"+...
        "*((long *) u+(k+l*(*nu1))));"
        "            }"
        "          }"
        "          break;"
        ""
        "        case 811 :"
        "          for (l=0;l<*nu2;l++) {"
        "            for (k=0;k<*nu1;k++) {"
        "              printf(""Actuator: time=%f, "+...
        "u(%d,%d) of actuator %d is %d "+...
        "\n"", \"
        "                     *t, k, l, *nport,"+...
        "*((unsigned char *) u+(k+l*(*nu1))));"
        "            }"
        "          }"
        "          break;"
        ""
        "        case 812 :"
        "          for (l=0;l<*nu2;l++) {"
        "            for (k=0;k<*nu1;k++) {"
        "              printf(""Actuator: time=%f, "+...
        "u(%d,%d) of actuator %d is %hu "+...
        "\n"", \"
        "                     *t, k, l, *nport,"+...
        "*((unsigned short *) u+(k+l*(*nu1))));"
        "            }"
        "          }"
        "          break;"
        ""
        "        case 814 :"
        "          for (l=0;l<*nu2;l++) {"
        "            for (k=0;k<*nu1;k++) {"
        "              printf(""Actuator: time=%f, "+...
        "u(%d,%d) of actuator %d is %lu "+...
        "\n"", \"
        "                     *t, k, l, *nport,"+...
        "*((unsigned long *) u+(k+l*(*nu1))));"
        "            }"
        "          }"
        "          break;"
        "        }"
        "      /*} */"
        "      break;"
        "    case 4 : /* actuator initialisation */"
        "      /* do whatever you want to initialize the actuator */"
        "      break;"
        "    case 5 : /* actuator ending */"
        "      /* do whatever you want to end the actuator */"
        "      break;"
        "    }"]
    else
        a_actuator=[]
    end

    // pour fprintf
    nc=size(act,"*") // Alan : d'o? viens act ?????
    // reponse : de do_compile_superblock!
    typ=["""%f "]; //time
    for i=1:nc
        typ($)=typ($)+""" \"
        typ=[typ;""""];
        for j=1:sum(actt(i,3)*actt(i,4))
            //typ=typ+'%f ';
            typ($)=typ($)+scs_c_n2c_fmt(actt(i,5))+" ";
        end
    end
    typ($)=typ($)+"\n"", \"
    typ(1)="    fprintf(fprw,"+typ(1);
    bl    ="                 ";
    if size(typ,1) <> 1 then
        typ(2:$) = bl+typ(2:$);
    end
    //Code1='    fprintf(fprw, '"'+typ+' \n'",*t'
    Code1=[typ;bl+"*t"];
    //actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]
    for i=1:size(actt,1)
        ni=actt(i,3)*actt(i,4) // dimension of ith output
        Code1($)=Code1($)+","
        Code1=[Code1;bl];
        for j=1:ni
            if actt(i,5)<>11 then
                Code1($)=Code1($)+...
                "*((("+scs_c_n2c_typ(actt(i,5))+" *)("+...
                rdnom+"_block_outtbptr+"+string(actt(i,2)-1)+"))"+...
                "+"+string(j-1)+")";
            else //CAS CMPLX
                Code1($)=Code1($)+...
                "*((("+scs_c_n2c_typ(actt(i,5))+" *)("+...
                rdnom+"_block_outtbptr+"+string(actt(i,2)-1)+"))"+...
                "+"+string((j-1))+"), "+...
                "*((("+scs_c_n2c_typ(actt(i,5))+" *)("+...
                rdnom+"_block_outtbptr+"+string(actt(i,2)-1)+"))"+...
                "+"+string(ni+(j-1))+")";
            end
            if j<>ni then
                Code1($)=Code1($)+", ";
            end
        end
    end
    Code1($)=Code1($)+");"

    Code=[]

    if nc==1|~standalone then
        Code=[Call
        comments
        dcl
        "  if (flag1 == 0) {"
        a_actuator
        "  }"
        "  else if (flag1 == 1) {"
        "    if (*flag == 4 && *nport == 1) {"
        "      fprw=fopen(output,'"wt'");"
        "      if( fprw == NULL )"
        "        {"
        "          printf('"Error opening file: %s\n'", output);"
        "          return;"
        "        }"
        "    }else if (*flag == 2 /* && *nevprt>0 */) {"
        Code1
        "    }else if (*flag == 5 && *nport == 1) {"
        "      fclose(fprw);"
        "    }"

        "  }"
        "}"]
    elseif nc>1 then
        S="  switch (*nport) {"
        for k=1:nc
            S=[S;
            "  case "+string(k)+" :/* Port number "+string(k)+" ----------*/"
            "  "+a_actuator
            "  break;"]
        end
        S=[S;"  }"]

        Code=[Code
        Call
        comments
        dcl
        "  if (flag1 == 0){"
        S
        "  }"
        "  else if (flag1 == 1) {"
        "    if (*flag == 4 && *nport == 1) {"
        "      fprw=fopen(output,'"wt'");"
        "      if( fprw == NULL ) {"
        "        printf('"Error opening file: %s\n'", output);"
        "        return;"
        "        }"
        "    }"
        "    else if (*flag == 2 /*&& *nevprt>0*/ ) {"
        Code1
        "    }"
        "    else if (*flag == 5 && *nport == 1) {"
        "      fclose(fprw);"
        "    }"
        "  }"
        "}"]
    end
endfunction

// Generates all parameters of blocks
function funcNames = make_names(filename)
    z=cpr.state.z;
    oz=cpr.state.oz;
    outtb=cpr.state.outtb;
    tevts=cpr.state.tevts;
    evtspt=cpr.state.evtspt;
    outptr=cpr.sim.outptr;
    funtyp=cpr.sim.funtyp;
    clkptr=cpr.sim.clkptr;
    ordptr=cpr.sim.ordptr;
    pointi=cpr.state.pointi;
    ztyp=cpr.sim.ztyp;
    zcptr=cpr.sim.zcptr;
    zptr=cpr.sim.zptr;
    ozptr=cpr.sim.ozptr;
    opptr=cpr.sim.opptr;
    opar=cpr.sim.opar;
    rpptr=cpr.sim.rpptr;
    ipptr=cpr.sim.ipptr;
    inpptr=cpr.sim.inpptr;
    funs=cpr.sim.funs;
    xptr=cpr.sim.xptr;
    modptr=cpr.sim.modptr;
    inplnk=cpr.sim.inplnk;
    nblk=cpr.sim.nb;
    outlnk=cpr.sim.outlnk;
    oord=cpr.sim.oord;
    zord=cpr.sim.zord;
    iord=cpr.sim.iord;
    noord=size(cpr.sim.oord,1);
    nzord=size(cpr.sim.zord,1);
    niord=size(cpr.sim.iord,1);

    nZ=size(z,"*"); //** index of work in z
    stalone=%f

    //states, rpar, ipar, functions names, opar(8)
    funcNames = list([],[],[],[],[],[],[],list(),[],[],[]);

    for kf=1:nblk
        nin=inpptr(kf+1)-inpptr(kf);  //** number of input ports
        nout=outptr(kf+1)-outptr(kf); //** number of output ports
        nx=xptr(kf+1)-xptr(kf);
        ng=zcptr(kf+1)-zcptr(kf);
        nmode=modptr(kf+1)-modptr(kf);
         // get input positions in block_outtbptr for all value references
        for k=1:nin
            lprt=inplnk(inpptr(kf)-1+k);
        end
        for k=1:nout
            lprt=outlnk(outptr(kf)-1+k);
        end
        if part(funs(kf),1:7) == "capteur" then
            funcNames(9)(1,$+1) = "newInstance->block_"+rdnom+"["+string(kf-1)+"].outptr[0])";
            funcNames(9)(1,$+1) = rdnom+"_block_outtbptr["+string(lprt-1)+"];";
            if ~isempty(cpr.sim(32)(kf)) then
                funcNames(9)(1,$+1) = cpr.sim(32)(kf);
            else
                funcNames(9)(1,$+1) = "in_"+string(part(funs(kf),8:$));
            end
        elseif part(funs(kf),1:10) == "actionneur" then
            // get output positions in block_outtbptr for all value references
            for i=1:length(outtb(lprt))
                funcNames(10)(1,$+1) = string(i-1);
                funcNames(10)(1,$+1) = rdnom+"_block_outtbptr["+string(lprt-1)+"];";
                if ~isempty(cpr.sim(32)(kf)) then // labels have a wrong order in cpr !
                    funcNames(10)(1,$+1) = cpr.sim(32)(kf)+"_"+string(i);
                else
                    funcNames(10)(1,$+1) = "out_"+string(part(funs(kf),11:$)+"_"+string(i));
                end
                if i==length(outtb(lprt)) then
                    funcNames(10)(1,$+1) = "";
                end
            end
            funcNames(11)($+1) = type(outtb(lprt)); // type of outputs in fmu int32 or double
        end
        if nx <> 0 then
            valueIndex = xptr(kf);
            for i = 1:nx
                funcNames(1)($+1) = funs(kf)+"_"+string(kf); // names of states functions
                funcNames(2)($+1) = cpr.state.x(valueIndex); // values of states
                valueIndex = valueIndex + 1;
            end
        end
        if (part(funs(kf),1:7) <> "capteur" &...
            part(funs(kf),1:10) <> "actionneur" &...
            funs(kf) <> "bidon" &...
            funs(kf) <> "bidon2") then
            //** rpar **//
            if (rpptr(kf+1)-rpptr(kf)>0) then
                valueIndex = rpptr(kf);
                for i = 1:rpptr(kf+1)-rpptr(kf);
                    funcNames(3)($+1) = funs(kf)+"_"+string(kf); // names of real parameters functions
                    funcNames(4)($+1) = cpr.sim.rpar(valueIndex); // values of rpar
                    valueIndex = valueIndex + 1;
                end
            end
            //** ipar **//
            if (ipptr(kf+1)-ipptr(kf)>0) then
                valueIndex = ipptr(kf);
                for i = 1:ipptr(kf+1)-ipptr(kf);
                    funcNames(5)($+1) = funs(kf)+"_"+string(kf); // names of integer parameters functions
                    funcNames(6)($+1) = cpr.sim.ipar(valueIndex); // values of ipar
                    valueIndex = valueIndex + 1;
                end
            end
            //** opar **//
            if (opptr(kf+1)-opptr(kf)>0) then
                nopar = opptr(kf+1)-opptr(kf);
                valueIndex = opptr(kf);
                for k=1:nopar
                    oparLength = length(cpr.sim.opar(valueIndex))
                    for i=1:oparLength
                        funcNames(7)($+1) = funs(kf)+"_"+string(kf)+"_"+string(i); // names of opar parameters functions
                        funcNames(8)($+1) = cpr.sim.opar(valueIndex)(i); // values of opar
                    end
                    valueIndex = valueIndex + 1;
                end
            end
        end
    end
endfunction

//generates skeleton of external world events handling function
function Code=make_outevents()
    z="0"
    if szclkIN==[] then
        newszclkIN=0;
    else
        newszclkIN=szclkIN;
    end

    Code=[ "/*"+part("-",ones(1,40))+"  External events handling function */";
    "void "+rdnom+"_events(int *nevprt,double *t)";
    "{"
    "/*  set next event time and associated events ports"
    " *  nevprt has binary expression b1..b"+string(newszclkIN)+" where bi is a bit"
    " *  bi is set to 1 if an activation is received by port i. Note that"
    " *  more than one activation can be received simultaneously"
    " *  Caution: at least one bi should be equal to one */"
    ""]

    if (newszclkIN <> 0) then
        Code=[Code;
        "    int i,p,b[]={"+strcat(z(ones(1,newszclkIN)),",")+"};"
        ""
        "/* this is an example for the activation of events ports */"
        "    b[0]=1;"]

        if newszclkIN>1 then
            for bb=2:newszclkIN
                Code($+1)="    b["+string(bb-1)+"]=1;"
            end
        end
        Code=[Code;
        ""
        "/* definition of the step time  */"
        "    *t = *t + 0.1;"
        ""
        "/* External events handling process */"
        "    *nevprt=0;p=1;"
        "    for (i=0;i<"+string(newszclkIN)+";i++) {"
        "      *nevprt=*nevprt+b[i]*p;"
        "      p=p*2;"
        "    }"
        "}"]
    else
        Code=[Code;
        "";
        "/* definition of the step time  */"
        "    *t = *t + 0.1;"
        "}"]
    end
endfunction

//Generating the routine for sensors interfacing
//
//
//Author : R. Djenidi, R. Nikoukhah, A. Layec
//
function Code=make_sensor(standalone)
    Call=["/*"+part("-",ones(1,40))+" Sensor */";
    "void "+rdnom+"_sensor(int *flag, int *nport, int *nevprt, double *t, void *y, int *ny1, int *ny2, int *yt, int *flag1)"]

    comments=["     /*"
    "      * To be customized for standalone execution";
    "      * flag  : specifies the action to be done"
    "      * nport : specifies the  index of the Super Bloc"
    "      *         regular input (The input ports are numbered"
    "      *         from the top to the bottom )"
    "      * nevprt: indicates if an activation had been received"
    "      *         0 = no activation"
    "      *         1 = activation"
    "      * t     : the current time value"
    "      * y     : the vector outputs value"
    "      * ny1   : the output size 1"
    "      * ny2   : the output size 2"
    "      * yt    : the output type"
    "      * flag1 : learn mode (0 from terminal,1 from input file"
    "      */"]
    dcl=["{"
    "  int j,k,l;"
    "  double temps;"]

    if standalone then

        a_sensor=["    switch (*flag) {"
        "    case 1 : /* set the output value */"
        "      printf(""Require outputs of sensor number %d\n"", *nport);"
        "      printf(""time is: %f\n"", *t);"
        "      printf(""sizes of the sensor output is: %d,%d\n"", *ny1,*ny2);"
        "      switch (*yt) {"
        "      case 10 :"
        "        printf(""type of the sensor output is: %d (double) \n"", *yt);"
        "        puts(""Please set the sensor output values"");"
        "        for (l=0;l<*ny2;l++) {"
        "          for (k=0;k<*ny1;k++) {"
        "            printf(""y(%d,%d) : "",k,l);"
        "            scanf(""%lf"", (double *) y+(k+l*(*ny1)));"
        "          }"
        "        }"
        "        break;"
        ""
        "      case 11 :"
        "        printf(""type of the sensor output is: %d (complex) \n"", *yt);"
        "        puts(""Please set the sensor output values"");"
        "        for (l=0;l<*ny2;l++) {"
        "          for (k=0;k<*ny1;k++) {"
        "            printf(""y(%d,%d) real part : "",k,l);"
        "            scanf(""%lf"", (double *) y+(k+l*(*ny1)));"
        "            printf(""y(%d,%d) imag part : "",k,l);"
        "            scanf(""%lf"", (double *) y+((*ny1)*(*ny2)+k+l*(*ny1)));"
        "          }"
        "        }"
        "        break;"
        ""
        "      case 81 :"
        "        printf(""type of the sensor output is: %d (char) \n"", *yt);"
        "        puts(""Please set the sensor output values"");"
        "        for (l=0;l<*ny2;l++) {"
        "          for (k=0;k<*ny1;k++) {"
        "            printf(""y(%d,%d) : "",k,l);"
        "            scanf(""%i"", (char *) y+(k+l*(*ny1)));"
        "          }"
        "        }"
        "        break;"
        ""
        "      case 82 :"
        "        printf(""type of the sensor output is: %d (char) \n"", *yt);"
        "        puts(""Please set the sensor output values"");"
        "        for (l=0;l<*ny2;l++) {"
        "          for (k=0;k<*ny1;k++) {"
        "            printf(""y(%d,%d) : "",k,l);"
        "            scanf(""%hd"", (short *) y+(k+l*(*ny1)));"
        "          }"
        "        }"
        "        break;"
        ""
        "      case 84 :"
        "        printf(""type of the sensor output is: %d (long) \n"", *yt);"
        "        puts(""Please set the sensor output values"");"
        "        for (l=0;l<*ny2;l++) {"
        "          for (k=0;k<*ny1;k++) {"
        "            printf(""y(%d,%d) : "",k,l);"
        "            scanf(""%ld"", (long *) y+(k+l*(*ny1)));"
        "          }"
        "        }"
        "        break;"
        ""
        "      case 811 :"
        "        printf(""type of the sensor output is: %d (unsigned char) \n"", *yt);"
        "        puts(""Please set the sensor output values"");"
        "        for (l=0;l<*ny2;l++) {"
        "          for (k=0;k<*ny1;k++) {"
        "            printf(""y(%d,%d) : "",k,l);"
        "            scanf(""%d"", (unsigned char *) y+(k+l*(*ny1)));"
        "          }"
        "        }"
        "        break;"
        ""
        "      case 812 :"
        "        printf(""type of the sensor output is: %d (unsigned short) \n"", *yt);"
        "        puts(""Please set the sensor output values"");"
        "        for (l=0;l<*ny2;l++) {"
        "          for (k=0;k<*ny1;k++) {"
        "            printf(""y(%d,%d) : "",k,l);"
        "            scanf(""%hu"", (unsigned short *) y+(k+l*(*ny1)));"
        "          }"
        "        }"
        "        break;"
        ""
        "      case 814 :"
        "        printf(""type of the sensor output is: %d (unsigned long) \n"", *yt);"
        "        puts(""Please set the sensor output values"");"
        "        for (l=0;l<*ny2;l++) {"
        "          for (k=0;k<*ny1;k++) {"
        "            printf(""y(%d,%d) : "",k,l);"
        "            scanf(""%lu"", (unsigned long *) y+(k+l*(*ny1)));"
        "          }"
        "        }"
        "        break;"
        ""
        "      }"
        "      break;"
        "    case 4 : /* sensor initialisation */"
        "      /* do whatever you want to initialize the sensor */"
        "      break;"
        "    case 5 : /* sensor ending */"
        "      /* do whatever you want to end the sensor */"
        "      break;"
        "    }"]
    else
        a_sensor=[]

    end

    nc=size(cap,"*")

    // pour fscanf
    typ=["""%lf "]; //temps
    for i=1:nc
        typ($)=typ($)+""" \"
        typ=[typ;""""];
        for j=1:sum(capt(i,3)*capt(i,4))
            //typ=typ+'%f ';
            typ($)=typ($)+scs_c_n2c_fmt(capt(i,5))+" ";
        end
    end
    typ($)=typ($)+"\n"", \"
    typ=strsubst(typ,"%f","%lf");
    typ(1)="    fscanf(fprr,"+typ(1);
    bl    ="                ";
    if size(typ,1) <> 1 then
        typ(2:$) = bl+typ(2:$);
    end
    //Code1=['      fscanf( fprr, '"'+typ+' \n'",&temps']
    Code1=[typ;bl+"&temps"];
    for i=1:size(capt,1)
        ni=capt(i,3)*capt(i,4); // dimension of ith input
        Code1($)=Code1($)+",";
        Code1=[Code1;bl];
        for j=1:ni
            if capt(i,5)<>11 then
                Code1($)=Code1($)+...
                "("+scs_c_n2c_typ(capt(i,5))+" *)("+...
                rdnom+"_block_outtbptr+"+string(capt(i,2)-1)+")"+...
                "+"+string(j-1)+"";
            else //CAS CMPLX
                Code1($)=Code1($)+...
                "("+scs_c_n2c_typ(capt(i,5))+" *)("+...
                rdnom+"_block_outtbptr+"+string(capt(i,2)-1)+")"+...
                "+"+string((j-1))+", "+...
                "("+scs_c_n2c_typ(capt(i,5))+" *)("+...
                rdnom+"_block_outtbptr+"+string(capt(i,2)-1)+")"+...
                "+"+string(ni+(j-1))+"";
            end
            if j<>ni then
                Code1($)=Code1($)+", ";

            end
        end
    end
    Code1($)=Code1($)+");"

    Code=[]
    if nc==1|~standalone then
        Code=[Code;
        Call
        comments
        dcl
        "  if (flag1 == 0) {"
        a_sensor;
        "  } "
        "  else if (flag1 == 1) {"
        "    if (*flag == 4 && *nport == 1) {"
        "      fprr=fopen(input,'"r'");"
        "      if( fprr == NULL ) {"
        "        printf('"Error opening file: %s\n'", input);"
        "        return;"
        "      }"
        "    }"
        "    else if (*flag == 1) {"
        Code1
        "    }"
        "    else if (*flag == 5 && *nport == 1) {"
        "      fclose(fprr);"
        "    }"
        "  }"
        "}"];

    elseif nc>1 then
        S="  switch (*nport) {"
        for k=1:nc
            S=[S;
            "  case "+string(k)+" : /* Port number "+string(k)+" ----------*/"
            "  "+a_sensor
            "  break;"]
        end
        S=[S;"  }"]

        Code=[Code
        Call
        comments
        dcl
        "  if (flag1 == 0) {"
        S
        "  }"
        "  else if (flag1 == 1){"
        "    if (*flag == 4 && *nport == 1) {"
        "      fprr=fopen(input,'"r'");"
        "      if( fprr == NULL ) {"
        "        printf('"Error opening file: %s\n'", input);"
        "        return ;"
        "      }"
        "    }"
        "    else if (*flag == 1) {"
        Code1
        "    }"
        "    else if (*flag == 5 && *nport == 1) {"
        "      fclose(fprr);"
        "    }"
        "  }"
        "}"]
    end
endfunction

//generates code of the FMU
//
function make_standalone42(filename)

    x=cpr.state.x;
    modptr=cpr.sim.modptr;
    rpptr=cpr.sim.rpptr;
    ipptr=cpr.sim.ipptr;
    opptr=cpr.sim.opptr;
    rpar=cpr.sim.rpar;
    ipar=cpr.sim.ipar;
    opar=cpr.sim.opar;
    oz=cpr.state.oz;
    ordptr=cpr.sim.ordptr;
    oord=cpr.sim.oord;
    zord=cpr.sim.zord;
    iord=cpr.sim.iord;
    tevts=cpr.state.tevts;
    evtspt=cpr.state.evtspt;
    zptr=cpr.sim.zptr;
    clkptr=cpr.sim.clkptr;
    ordptr=cpr.sim.ordptr;
    pointi=cpr.state.pointi;
    funs=cpr.sim.funs;
    noord=size(cpr.sim.oord,1);
    nzord=size(cpr.sim.zord,1);
    niord=size(cpr.sim.iord,1);

    Indent="  ";
    Indent2=Indent+Indent;
    BigIndent="          ";

    work=zeros(nblk,1)
    Z=[z;zeros(size(outtb),1);work]';
    nX=size(x,"*");
    nztotal=size(z,1);

    stalone = %t;

    fd = mopen(filename, "wt");
    // the generated code includes the FMI 1.0 headers, ship them alongside
    copyfile(fullfile(fmigetPath(), "includes", "fmiModelFunctions.h"), fileparts(filename));
    copyfile(fullfile(fmigetPath(), "includes", "fmiModelTypes.h"), fileparts(filename));
    // xml description
    var = fmiModelVariables(rpat+filesep()+"modelDescription.xml");

    fmuType = "**    SCILAB FMU. Model Exchange version 1.0.";
    include = "#include ""fmiModelFunctions.h""";
    event = [];
    if typ == "cs" then
        fmuType = "**    SCILAB FMU. Co Simulation version 1.0.";
        event = "   fmiEventInfo eventInfo;";
    end
    mputl(["/*"
    fmuType;
    "**    Generated by the fmu module of Xcos with "+getversion()
    "**    Date : "+date()
    "**    Author : "+description(6)
    "**    Model description : "+description(5)
    "**    Model version : "+description(7)
    "*/"
    ""
    "/* ---- c include files ---- */"
    "#include <stdio.h>"
    "#include <stdlib.h>"
    "#include <math.h>"
    "#include <string.h>"
    "#include <memory.h>"
    "#include ""scicos_block4.h"""
    "#include ""scicos.h"""
    "#include ""machine.h"""
    "#ifndef max"
    "#define max(a,b) ((a) >= (b) ? (a) : (b))"
    "#endif"
    "#ifndef min"
    "#define min(a,b) ((a) <= (b) ? (a) : (b))"
    "#endif"
    ""
    "/* ---- Include FMI header files ---- */"
    include;
    ""
    "/* ---- Define model GUID and IDENTIFIER ---- */"
    "#define MODEL_IDENTIFIER "+description(1)
    "#define MODEL_GUID "+""""+description(2)+""""
    ""
    "/* ---- Define number of model variables and parameters ---- */"
    "#define ALL_REALS "+description(8)
    "#define ALL_INTEGERS "+description(9)
    "#define ALL_BOOLEANS "+description(10)
    "#define ALL_STRINGS "+"0"
    "#define ALL_STATES "+description(3)
    "#define ALL_EVENTS "+description(4)
    ""], fd);
    // non start value refs
    nonStartReal = find(isnan(var.Real.start));
    nonStartInt = find(isnan(var.Integer.start));
    if (nonStartReal & ~isempty(var.Real.name)) | (nonStartInt & ~isempty(var.Integer.name)) then
        mputl(["/* ---- Define non start value references ---- */"],fd);
        if nonStartReal & ~isempty(var.Real.name) then
           nonStart = var.Real.valueReference(nonStartReal);
            if size(nonStart, "*") > 1 then
                nonStart = part(sci2exp(nonStart),2:$-1);
                mputl("#define NON_START_REAL {"+nonStart+"}",fd);
            else
                nonStart = string(nonStart);
                mputl("#define NON_START_REAL {"+nonStart+"}",fd);
            end
        end
        if nonStartInt & ~isempty(var.Integer.name) then
           nonStart = var.Integer.valueReference(nonStartInt);
            if size(nonStart, "*") > 1 then
                nonStart = part(sci2exp(nonStart),2:$-1);
                mputl("#define NON_START_INT {"+nonStart+"}",fd);
            else
                nonStart = string(nonStart);
                mputl("#define NON_START_INT {"+nonStart+"}",fd);
            end
        end
    end
    // input/output
    numInp = length(capt(:,3));
    fOutReal = find(var.Real.causality=="output");
    fOutInt = find(var.Integer.causality=="output");
    numOut = length([fOutReal, fOutInt]);
    allOutput = []
    inpTyp = scs_c_nb2scs_nb(capt(:,5));
    outTyp = [ones(1:length(fOutReal)),zeros(1:length(fOutInt))];
    // find the same outputs
    finOut = [];
    for i=1:size(funcNames(10),"*")
        if find(isempty(funcNames(10)(1,i))) then
            finOut($+1) = i;
        end
    end
    newFuncNames = funcNames(10)(1, setdiff(1:size(funcNames(10)(1,:),"*"), finOut));
    mputl(["";"/* ---- Define inputs/outputs ---- */"],fd);
    inpOutPosition = [];
    j = 1; k = 3; l = 2;
    txtOutAdress = []; // using by fmiGetReal
    for i = 1:numInp
        if inpTyp(i) == 1 then
            vr = find(var.Real.name == funcNames(9)(k));
            inpOutPosition($+1) = " newInstance->real["+string(vr-1)+"];";
            inpOutPosition($+1) = funcNames(9)(l);
            mputl(["#define "+funcNames(9)(k)+" ("+part(inpOutPosition($-1),2:$-1)+")"],fd);
        else
            vr = find(var.Integer.name == funcNames(9)(k));
            inpOutPosition($+1) = " newInstance->integer["+string(vr-1)+"];";
            inpOutPosition($+1) = funcNames(9)(l);
            mputl(["#define "+funcNames(9)(k)+" ("+part(inpOutPosition($-1),2:$-1)+")"],fd);
        end
    j = j + 3; k = k + 3; l = l + 3;
    end
    j = 1; k = 3; l = 2;
    for i = 1:numOut
        vr = find(var.Real.name == newFuncNames(k))
        if length(vr) > 0 then
            inpOutPosition($+1) = " newInstance->real["+string(vr-1)+"];";
            inpOutPosition($+1) = newFuncNames(l);
            mputl(["#define "+newFuncNames(k)+" ("+part(inpOutPosition($-1),2:$-1)+")"],fd);
            nameLength = length(rdnom+"_block_outtbptr[ ");
            txtOutAdress($+1) = "    "+part(inpOutPosition($-1),2:$-1)+" = outtb_"+...
            string(strtod(part(inpOutPosition($),nameLength:$-2))+1)+"["+newFuncNames(j)+"];";
        else
            vr = find(var.Integer.name == newFuncNames(k));
            inpOutPosition($+1) = " newInstance->integer["+string(vr-1)+"];";
            inpOutPosition($+1) = newFuncNames(l);
            mputl(["#define "+newFuncNames(k)+" ("+part(inpOutPosition($-1),2:$-1)+")"],fd);
            nameLength = length(rdnom+"_block_outtbptr[ ");
            txtOutAdress($+1) = "    "+part(inpOutPosition($-1),2:$-1)+" = outtb_"+...
            string(strtod(part(inpOutPosition($),nameLength:$-2))+1)+"["+newFuncNames(j)+"];";
        end
    j = j + 3; k = k + 3; l = l + 3;
    end
    newInpOut = [];
    if ~isempty(inpOutPosition) then
        nameLength = length(rdnom+"_block_outtbptr[ ");
        inpOutPosition(2:2:$) = "outtb_"+string(strtod(part(inpOutPosition(2:2:$),nameLength:$-2))+1);
        inpOutPositionNew = inpOutPosition(2:2:$);
        t2 = [];
        for i=1:size(inpOutPositionNew,"*")
            t = (find(inpOutPositionNew(i)==inpOutPositionNew))
            if length(t) > 1 then
                t2($+1) = t(2)
            end
        end
//        if t2 <> [] then
//            t2 = unique(t2);
//            inpOutPositionNew(t2) = "//"+inpOutPositionNew(t2); // to avoid direct inputs/outputs
//            inpOutPosition(2:2:$) = inpOutPositionNew;
//        end

        newIndex = newFuncNames(1:3:$)';
        newIndexInp(1:numInp) = "0";
        newIndex = [newIndexInp; newIndex];
        newInpOut = inpOutPosition(2:2:$)+"["+newIndex+"]"+" ="+inpOutPosition(1:2:$);
    end
    // separate input/output
    newInput = newInpOut(1:numInp);
    // find continuous states
    ind = find((var.Real.variability == "continuous") &...
              (part(var.Real.name,1:3)== "st_"));
    states = var.Real.valueReference(ind);
    if length(ind) > 1 then
        states = part(sci2exp(states),2:$-1);
    else
        states = string(states);
    end
    if states <> [] then
    mputl([""
    "/* ---- Define continuous states value references ---- */"
    "#define CONTINUOUS_STATES {"+states+"}"
    "#if ALL_STATES > 0"
    "   fmiValueReference sciStates[ALL_STATES] = CONTINUOUS_STATES;"
    "#endif"],fd);
    end
    // new names for derivatives
    if states <> [] then
        newDerNames = var.Real.name(evstr(states)+2);
        newDerNames = part(newDerNames,1:3)+"_"+part(newDerNames,5:$-1);
        var.Real.name(evstr(states)+2) = newDerNames;
    end
    // define real and integer parameters
    [txt1,txt2,txt3,txt4] = make_define_param(evstr(states),var);
    mputl([""
    txt1],fd);
    // time events
    tEvents = [];
    if length(cpr.state(6)) > 1 then
        tEvents = part(sci2exp(cpr.state(6)'),2:$-1);
    elseif length(cpr.state(6)) == 1 then
        tEvents = string(cpr.state(6));
    end
    mputl([""
    "/* ---- Define model Instance strings are not supported at the moment in Xcos fmu---- */"
    "typedef struct {"
    "   fmiReal time;"
    "   fmiString GUID;"
    "   fmiString instanceName;"
    "   fmiCallbackFunctions functions;"
    "   fmiBoolean loggingOn;"
    "   fmiBoolean isInstantiated;"
    "   fmiBoolean isInitialized;"
    "   fmiBoolean isTerminated;"
    "   fmiReal *real;"
    "   fmiInteger *integer;"
    "   fmiBoolean *boolean;"
    "   scicos_block block_"+rdnom+"["+string(nblk)+"];"
    "   fmiBoolean internalEvent["+string(length(tevts)+1)+"];"
    event;
    "} fmuInstance;"
    ""
    "/* ---- Internals functions declaration ---- */"
    "static fmiStatus "+rdnom+"_sciOutputCompute(fmuInstance *, int);"
    "static fmiReal sciGetReal(fmuInstance *, fmiValueReference);"
    "static fmiInteger sciGetInteger(fmuInstance *, fmiValueReference);"
    "static fmiBoolean sciGetBoolean(fmuInstance *, fmiValueReference);"
    "static fmiReal sciGetEvent(fmuInstance *, int);"
    "void set_scicos_time(double);"
    Protostalone
    ""], fd);
    mputl(["/* ---- General static variables ---- */"
    "static double zero = 0;"
    "static void **"+rdnom+"_block_outtbptr;"
    "static void **work;"
    "static double *args[100];"
    "static int nrd_"+string(0:maxtotal)'+" = "+string(0:maxtotal)'+";"
    "static double t = 0;"
    "static int mode[] = {0,1,2,3};"],fd);
    if tEvents <> [] then
        mputl(["static double tEvents[] = {"+tEvents+"};"
               "static int tEventLast;"],fd);
    end

    mputl([""
    "/* ---- Initial values ---- */"
    "/* Note that z[]=[z_initial_condition;outtbptr;work]"
    cformatline("     z_initial_condition={"+...
    joinValues(z)+"};",70)
    cformatline("     outtbptr={"+...
    joinValues(zeros(size(outtb),1))+"};",70)
    cformatline("     work= {"+...
    joinValues(work)+"};",70)
    "  */"
    cformatline("static double z[]={"+strcat(string(Z),",")+"};",70)], fd);
    //** declaration of outtb
    Code_outtb = [];
    Code_outtb_initial = [];
    for i=1:size(outtb)
        for j=1:length(outtb(i))
            Code_outtb_initial =[Code_outtb_initial; "    outtb_"+string(i)+...
            "["+string(j-1)+"] = 0;"]
        end
        if mat2scs_c_nb(outtb(i)) <> 11 then
            Code_outtb=[Code_outtb;
            cformatline("static "+mat2c_typ(outtb(i))+...
            " outtb_"+string(i)+"[]={"+...
            strcat(string(outtb(i)(:)),",")+"};",70)]
        else //** cmplx test
            message(["Complex matrix does not supported."]);
            return
        end
    end
    if Code_outtb<>[] then
        mputl([""
        "/* ---- outtb declaration ---- */"
        Code_outtb
        ""], fd);
    end
    Code_outtbptr=[];
    for i=1:size(outtb)
        Code_outtbptr=[Code_outtbptr;
        "    "+rdnom+"_block_outtbptr["+...
        string(i-1)+"] = (void *) outtb_"+string(i)+";"];
    end
    //** declaration of oz
    Code_oz = [];
    for i=1:size(oz)
        if mat2scs_c_nb(oz(i)) <> 11 then
            Code_oz=[Code_oz;
            cformatline("  "+mat2c_typ(oz(i))+...
            " oz_"+string(i)+"[]={"+...
            strcat(string(oz(i)(:)),",")+"};",70)]
        else //** cmplx test
            Code_oz=[Code_oz;
            cformatline("  "+mat2c_typ(oz(i))+...
            " oz_"+string(i)+"[]={"+...
            strcat(string([real(oz(i)(:));
            imag(oz(i)(:))]),",")+"};",70)]
        end
    end

    if Code_oz <> [] then
        mputl(["  /* oz declaration */"
        Code_oz], fd);
    end

    mputl(["/* ---- External simulation function ---- */"
    "static fmiStatus "+rdnom+"_sciOutputCompute(fmuInstance *newInstance, int flag) {"
    "    int nevprt = 1;"
    "    int local_flag, nport, i;"
    "    fmiStatus status = fmiOK;"
    ""], fd);

    //inputs/outputs
    if newInput <> [] then
        mputl(["    /* Get inputs from the instance */"
        "    "+newInput],fd);
    end
    mputl([""
    "    /* Get simulation time */"
    "    t = newInstance->time;"],fd);

    finalTxtInstance = txt4;
    finalTxtInitialize = [];
    txtUpdtStates = []; txtEvents = [];
    states = evstr(states);
    newStates = states;
    for kf=1:nblk
        nx=xptr(kf+1)-xptr(kf);       //** number of continuous state
        nin=inpptr(kf+1)-inpptr(kf);  //** number of input ports
        nout=outptr(kf+1)-outptr(kf); //** number of output ports
        ng=zcptr(kf+1)-zcptr(kf); //** number of zero crossing
        nm=modptr(kf+1)-modptr(kf); //** number of modes
        // for fmiInstantiateModel
        txtInstance = [];
        txtInitialize = [];
        //** add comment
        if part(funs(kf),1:5) <> "bidon" then
        txtInstance($+1) =""
        txtInitialize($+1) =""
        txtInstance($+1) ="    "+get_comment("set_blk",list(funs(kf),funtyp(kf),kf,labels(kf)));
        txtInitialize($+1) = txtInstance($);
        txtInstance = [txtInstance;"    newInstance->block_"+rdnom+"["+string(kf-1)+"].type   = "+string(funtyp(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].ztyp   = "+string(ztyp(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].ng     = "+string(zcptr(kf+1)-zcptr(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].nz     = "+string(zptr(kf+1)-zptr(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].noz    = "+string(ozptr(kf+1)-ozptr(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].nrpar  = "+string(rpptr(kf+1)-rpptr(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].nopar  = "+string(opptr(kf+1)-opptr(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].nipar  = "+string(ipptr(kf+1)-ipptr(kf))+";"
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].nin    = "+string(inpptr(kf+1)-inpptr(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].nout   = "+string(outptr(kf+1)-outptr(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].nevout = "+string(clkptr(kf+1)-clkptr(kf))+";";
        "    newInstance->block_"+rdnom+"["+string(kf-1)+"].nmode  = "+string(modptr(kf+1)-modptr(kf))+";"];

        if nx <> 0 then
            txtInstance = [txtInstance; "    newInstance->block_"+rdnom+"["+string(kf-1)+"].nx = "+string(nx)+";"
                "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].x = "+...
                "malloc(sizeof(double)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nx))== NULL) return 0;";
                "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].xd = "+...
                "malloc(sizeof(double)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nx))== NULL) return 0;"];
                for i=1:nx
                    txtInstance = [txtInstance;
                    "    newInstance->block_"+rdnom+"["+string(kf-1)+"].x["+string(i-1)+"] = newInstance->real["+string(newStates(i))+"];";
                    "    newInstance->block_"+rdnom+"["+string(kf-1)+"].xd["+string(i-1)+"] = newInstance->real["+string(newStates(i)+1)+"];"]
                    txtUpdtStates =[txtUpdtStates; txtInstance($:-1:$-1)];
                end
            newStates = newStates(nx+1:$); // update states vector
            txtInitialize = [txtInitialize; txtInstance($:-1:$-2)];
        end
        txtInstance($+1) = "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].evout  = "+...
        "calloc(newInstance->block_"+rdnom+"["+string(kf-1)+"].nevout,sizeof(double)))== NULL) return 0;";
        //***************************** input port *****************************//
        //** alloc insz/inptr **//
        txtInstance = [txtInstance;"    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].inptr  = "+...
        "malloc(sizeof(double*)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nin))== NULL) return 0;";
        "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].insz   = "+...
        "malloc(3*sizeof(int)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nin))== NULL) return 0;"];
        //** inptr **//
        for k=1:nin
            lprt=inplnk(inpptr(kf)-1+k);
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].inptr["+string(k-1)+...
            "]  = "+rdnom+"_block_outtbptr["+string(lprt-1)+"];";
            txtInitialize($+1) = txtInstance($);
        end

        //** 1st dim **//
        for k=1:nin
            lprt=inplnk(inpptr(kf)-1+k);
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].insz["+string((k-1))+...
            "]   = "+string(size(outtb(lprt),1))+";";
        end
        //** 2dn dim **//
        for k=1:nin
            lprt=inplnk(inpptr(kf)-1+k);
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].insz["+string((k-1)+nin)+...
            "]   = "+string(size(outtb(lprt),2))+";";
        end
        //** typ **//
        for k=1:nin
            lprt=inplnk(inpptr(kf)-1+k);
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].insz["+string((k-1)+2*nin)+...
            "]   = "+mat2scs_c_typ(outtb(lprt))+";";
        end
        //**********************************************************************//
        // zero crossing
        if ng <> 0 then
            txtInstance($+1) = "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].g  = "+...
            "malloc(sizeof(double)*newInstance->block_"+rdnom+"["+string(kf-1)+"].ng))== NULL) return 0;";
            for i=1:ng
                txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].g["+string(i-1)+"]  = zero;";
                txtEvents($+1) = "newInstance->block_"+rdnom+"["+string(kf-1)+"].g["+string(i-1)+"]"
            end
        else
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].g  = &(zero);";
            txtInitialize($+1) = txtInstance($);
        end
        // modes
        if nm <> 0 then
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].mode  = &(mode[3]);"
        end
        //***************************** output port *****************************//
        //** alloc outsz/outptr **//
        txtInstance = [txtInstance;"    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].outsz  = "+...
        "malloc(3*sizeof(int)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nout))== NULL) return 0;";
        "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].outptr = "+...
        "malloc(sizeof(double*)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nout))== NULL) return 0;"];

        //** outptr **//
        for k=1:nout
            lprt=outlnk(outptr(kf)-1+k);
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].outptr["+string(k-1)+...
            "] = "+rdnom+"_block_outtbptr["+string(lprt-1)+"];";
            txtInitialize($+1) = txtInstance($);
        end

        //** 1st dim **//
        for k=1:nout
            lprt=outlnk(outptr(kf)-1+k);
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].outsz["+string((k-1))+...
            "]  = "+string(size(outtb(lprt),1))+";";
        end

        //** 2dn dim **//
        for k=1:nout
            lprt=outlnk(outptr(kf)-1+k);
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].outsz["+string((k-1)+nout)+...
            "]  = "+string(size(outtb(lprt),2))+";";
        end

        //** typ **//
        for k=1:nout
            lprt=outlnk(outptr(kf)-1+k);
            txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].outsz["+string((k-1)+2*nout)+...
            "]  = "+mat2scs_c_typ(outtb(lprt))+";";
        end
        //**********************************************************************//

        txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+...
        "].z = &(z["+string(zptr(kf)-1)+"]);";
        txtInitialize($+1) = txtInstance($);

        if (part(funs(kf),1:7) ~= "capteur" &...
            part(funs(kf),1:10) ~= "actionneur" &...
            funs(kf) ~= "bidon") then
            //** rpar **//
            if (rpptr(kf+1)-rpptr(kf)>0) then
                txtInstance($+1) = "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].rpar = "+...
                "malloc(sizeof(double)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nrpar))== NULL) return 0;";
                for i=1:rpptr(kf+1)-rpptr(kf)
                    ind = find(txt2 == " newInstance->block_"+rdnom+"["+string(kf-1)+...
                    "].rpar["+string(i-1)+"]");
                    txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+...
                    "].rpar["+string(i-1)+"] ="+txt2(ind+1);
                    txtInitialize($+1) = txtInstance($);
                end
            end
            //** ipar **//
            if (ipptr(kf+1)-ipptr(kf)>0) then
                txtInstance($+1) = "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].ipar = "+...
                "malloc(sizeof(int)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nipar))== NULL) return 0;";
                for i=1:ipptr(kf+1)-ipptr(kf)
                    ind = find(txt3 == " newInstance->block_"+rdnom+"["+string(kf-1)+...
                    "].ipar["+string(i-1)+"]");
                    txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+...
                    "].ipar["+string(i-1)+"] ="+txt3(ind+1);
                    txtInitialize($+1) = txtInstance($);
                end
            end
            //** opar **//
            if (opptr(kf+1)-opptr(kf)>0) then
                txtInstance = [txtInstance;"    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].oparptr = "+...
                "malloc(sizeof(void *)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nopar))== NULL ) return 0;";
                "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].oparsz  = "+...
                "malloc(2*sizeof(int)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nopar))== NULL ) return 0;";
                "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].opartyp = "+...
                "malloc(sizeof(int)*newInstance->block_"+rdnom+"["+string(kf-1)+"].nopar))== NULL ) return 0;"];
                nopar = opptr(kf+1)-opptr(kf);
                //** oparptr **//
                for k=1:nopar
                    txtInstance = [txtInstance;"    newInstance->block_"+rdnom+"["+string(kf-1)+"].oparptr["+string(k-1)+...
                    "]   = (void *) OPAR_"+string(opptr(kf)-1+k)+";"];
                end
                //** 1st dim **//
                for k=1:nopar
                    txtInstance = [txtInstance;"    newInstance->block_"+rdnom+"["+string(kf-1)+"].oparsz["+string(k-1)+...
                    "]    = "+string(size(opar(opptr(kf)-1+k),1))+";"];
                end
                //** 2dn dim **//
                for k=1:nopar
                    txtInstance = [txtInstance;"    newInstance->block_"+rdnom+"["+string(kf-1)+"].oparsz["+string(nopar+(k-1))+...
                    "]    = "+string(size(opar(opptr(kf)-1+k),2))+";"];
                end
                //** typ **//
                for k=1:nopar
                    txtInstance = [txtInstance;"    newInstance->block_"+rdnom+"["+string(kf-1)+"].opartyp["+string(k-1)+...
                    "]   = "+mat2scs_c_typ(opar(opptr(kf)-1+k))+";"];
                end
            end
            //** oz **//
            if (ozptr(kf+1)-ozptr(kf)>0) then
                noz = ozptr(kf+1)-ozptr(kf);
                txtInstance = [txtInstance;"    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].ozptr = "+...
                "malloc(sizeof(void *)*newInstance->block_"+rdnom+"["+string(kf-1)+"].noz))== NULL) return 0;";
                "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].ozsz  = "+...
                "malloc(2*sizeof(int)*newInstance->block_"+rdnom+"["+string(kf-1)+"].noz))== NULL) return 0;";
                "    if ((newInstance->block_"+rdnom+"["+string(kf-1)+"].oztyp = "+...
                "malloc(sizeof(int)*newInstance->block_"+rdnom+"["+string(kf-1)+"].noz))== NULL) return 0;"];
                //** ozptr **//
                for k=1:noz
                    txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].ozptr["+string(k-1)+...
                    "]  = (void *) oz_"+string(ozptr(kf)-1+k)+";";
                end
                //** 1st dim **//
                for k=1:noz
                    txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].ozsz["+string(k-1)+...
                    "]  = "+string(size(oz(ozptr(kf)-1+k),1))+";";
                end
                //** 2nd dim **//
                for k=1:noz
                    txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].ozsz["+string(noz+(k-1))+...
                    "]  = "+string(size(oz(ozptr(kf)-1+k),2))+";";
                end
                //** typ **//
                for k=1:noz
                    txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].oztyp["+string(k-1)+...
                    "]  = "+mat2scs_c_typ(oz(ozptr(kf)-1+k))+";";
                end
            end
        end
        txtInstance($+1) = "    newInstance->block_"+rdnom+"["+string(kf-1)+"].work = "+...
        "(void **)(((double *)work)+"+string(kf-1)+");";
        txtInitialize($+1) = txtInstance($);
        end
        finalTxtInstance = [finalTxtInstance; txtInstance]; // for fmiIntitialize
        finalTxtInitialize = [finalTxtInitialize; txtInitialize];
    end

    //** cst blocks and it's dep
    txt=write_code_idoit()
    if txt<>[] then
        mputl([""
        "    /* Initial blocks must be called with flag 1 */"
        "    if (flag == 1) {"
        txt;
        "    }"], fd);
    end
//*****************************************************
    //** find source activation number
    blks=find(funtyp>-1);
    evs=[];

    for blk=blks
        for ev=clkptr(blk):clkptr(blk+1)-1
            if funs(blk)=="bidon" then
                if ev > clkptr(howclk) -1
                    evs=[evs,ev];
                end
            end
        end
    end

    mputl("    if (flag == 2) {",fd)
    isBooleans = strtod(description(10)) > 0;
    if isBooleans then
        mputl(["    for (i=0; i<ALL_BOOLEANS; i++) {"
               "         if (newInstance->boolean[i]) nevprt = i+1;"
               "         else nevprt = 0;"],fd);
    end;
    //** flag 1,2,3
    for flag=[1,3] // flag 2 make_doit_fmu
        txt3=[]
        //** continuous time blocks must be activated
        //** for flag 1
        if flag==1 then
            txt = write_code_cdoit(flag);
            if txt <> [] then
                txt3=[""
                "    "+get_comment("ev",list(0))
                txt;
                ];
            end
        end
        //** blocks with input discrete event must be activated
        //** for flag 1, 2 and 3
        if size(evs,2)>=1 then
            txt4=[];
            //**
            for ev=evs
                txt2=write_code_doit(ev,flag);
                if txt2<>[] then
                    //** adjust event number because of bidon block
                    new_ev=ev-(clkptr(howclk)-1)
                    //**
                    txt4=[txt4;
                    Indent2+["  case "+string(new_ev)+" : "+...
                    get_comment("ev",list(new_ev))
                    txt2];
                    "      break;";""]
                end
            end
            //**
            if txt4 <> [] then
                txt3=[txt3;
                Indent+"  /* Discrete activations */"
                Indent+"  switch (nevprt) {"
                txt4
                "    }"];
            end
        end
        //**
        if txt3<>[] then
            mputl(["    "+get_comment("flag",list(flag))
            txt3], fd);
        end
    end
    txtBool = make_doit_fmu();
    if txtBool <> [] then
        mputl(txtBool,fd);
    end
    if isBooleans then
       mputl("    }",fd);
    end
    mputl("    }",fd);
    // update continuous states
    if states <> [] then
       txtUpdtStatesNew = [];
       for i=1:size(txtUpdtStates, "*")
          txtUpdtStatesNew = [txtUpdtStatesNew; tokens(txtUpdtStates(i), "=")];
       end
    end
    // flag 0 continuous states
    if txtUpdtStates <> [] then
        mputl(["    if (flag == 0) {";
          "    "+txtUpdtStates;
          indentLines("    ", write_code_odoit(0));],fd);
          if txtUpdtStates <> [] then
             mputl(["       "+part(txtUpdtStatesNew(2:2:$),1:$-1)+" = "+part(txtUpdtStatesNew(1:2:$),5:$-1)+";"],fd);
          end
        mputl("    }",fd);
    end
    txt = []
    for j=1:nzord
        bk=zord(j,1);
        pt=zord(j,2);
        if funtyp(bk)>-1 then
                    txt=[txt;
                    indentLines("    ", call_block42(bk,pt,9))];
        end
    end

        if txt <> [] then
            mputl(["    if (flag == 9) {"
                   txt
                   "    }"], fd);
        end
    // update outputs
    if txtOutAdress <> [] then
        mputl(txtOutAdress,fd);
    end
    mputl([""
    "  return status;"
    "}"
    ""
    "/*"+part("-",ones(1,40))+"  Lapack messag function */";
    "void C2F(xerbla)(char *SRNAME,int *INFO,long int L)"
    "{"
    "  printf(""** On entry to %s, parameter number %d"""
    "         ""  had an illegal value\n"",SRNAME,*INFO);"
    "}"
    ""
    "void set_block_error(int err)"
    "{"
    "  return;"
    "}"
    ""
    "int get_phase_simulation()"
    "{"
    "  return 1;"
    "}"
    ""
    "void * scicos_malloc(size_t size)"
    "{"
    "  return malloc(size);"
    "}"
    ""
    "void scicos_free(void *p)"
    "{"
    "  free(p);"
    "}"
    ""
    "double get_scicos_time()"
    "{"
    "  return t;"
    "}"
    ""
    "void do_cold_restart()"
    "{"
    "  return;"
    "}"
    ""
    "void sciprint (char *fmt)"
    "{"
    "  return;"
    "}"
    ""], fd);
//*********************FMU EXPORT*******************************************//
    // helper function for fmiGetReal
    getCase = ["      case 0 : return 0;"];
    varNameRef = [];
    varNameRef = [var.Real.name; string(var.Real.valueReference)];
    if ~isempty(varNameRef) then
        getCase = "      case "+varNameRef(2,:)'+" : " + "return "+varNameRef(1,:)'+";"
    end
    mputl(["// helper function for fmiGetReal"
    "static fmiReal sciGetReal(fmuInstance *newInstance, fmiValueReference vr) {"
    "    switch (vr) {"
    getCase
    "      default : return 0;"
    "    }"
    "}"
    ""],fd)
    // helper function for fmiGetInteger
    getCase = ["      case 0 : return 0;"];
    varNameRef = [];
    varNameRef = [var.Integer.name; string(var.Integer.valueReference)];
    if ~isempty(varNameRef) then
        getCase = "      case "+varNameRef(2,:)'+" : " + "return "+varNameRef(1,:)'+";"
    end
    mputl(["// helper function for fmiGetInteger"
    "static fmiInteger sciGetInteger(fmuInstance *newInstance, fmiValueReference vr) {"
    "    switch (vr) {"
    getCase
    "      default : return 0;"
    "    }"
    "}"
    ""],fd)
    // helper function for fmiGetEventIndicators
    getCase = ["      case 0 : return 0;"];
    if txtEvents <> [] then
        getCase = [];
        lengthTxtEvents = string([0:size(unique(part(txtEvents,1:$-5)), "*")-1]');
        lengthTxtEvents = lengthTxtEvents($:-1:1);
        for i=1:size(txtEvents,"*")
            ind = find(part(txtEvents,1:$-5) == (part(txtEvents(i),1:$-5)));
            if (length(ind) > 1) & (i <> ind(2)) then
                getCase = [getCase; "      case "+lengthTxtEvents($)+" : ";
                          "           if ("+txtEvents(i)+" >= 0) "+part(txtEvents(i),1:$-4)+"mode = &mode[1];";
                          "           else if ("+txtEvents(ind(2))+" <= 0) "+part(txtEvents(i),1:$-4)+"mode = &mode[2];";
                          "           else "+part(txtEvents(i),1:$-4)+"mode = &mode[3];";
                          "        return "+txtEvents(i)+";"];
                lengthTxtEvents = (lengthTxtEvents(1:$-1));
            elseif (length(ind) == 1) then
                getCase = [getCase; "      case "+lengthTxtEvents($)+" : ";
                   "           if ("+txtEvents(i)+" >= 0) "+part(txtEvents(i),1:$-4)+"mode = &mode[1];";
                   "           else if ("+txtEvents(i)+" <= 0) "+part(txtEvents(i),1:$-4)+"mode = &mode[2];";
                   "           else "+part(txtEvents(i),1:$-4)+"mode = &mode[3];";
                   "        return "+txtEvents(i)+";"];
                lengthTxtEvents = (lengthTxtEvents(1:$-1));
            end
        end
    end
    mputl(["// helper function for fmiGetEventIndicators"
    "static fmiReal sciGetEvent(fmuInstance *newInstance, int z) {"
    "    switch (z) {"
    getCase;
    "      default : return 0;"
    "    }"
    "}"
    ""],fd)
    getCase = ["      case 0 : return 0;"];
    varNameRef = [];
    varNameRef = [var.Boolean.name; string(var.Boolean.valueReference)];
    if ~isempty(varNameRef) then
        getCase = "      case "+varNameRef(2,:)'+" : " + "return newInstance->boolean["+varNameRef(2,:)'+"];"
    end
    mputl(["// helper function for fmiGetBoolean"
    "static fmiBoolean sciGetBoolean(fmuInstance *newInstance, fmiValueReference vr) {"
    "    switch (vr) {"
    getCase;
    "      default : return 0;"
    "    }"
    "}"
    ""],fd)
    // Model Exchange or Co-Simulation
    // default model Exchange
    platform = "const char* fmiGetModelTypesPlatform() {";
    instance = ["    fmuInstance *newInstance = (fmuInstance*) c;"];
    instantiate = ["fmiComponent fmiInstantiateModel(fmiString instanceName, fmiString GUID,"
                   "                                 fmiCallbackFunctions functions, fmiBoolean loggingOn) {"];
    freeInstance = ["void fmiFreeModelInstance(fmiComponent c) {"];
    initialize = ["fmiStatus fmiInitialize(fmiComponent c, fmiBoolean toleranceControlled,"
                  "                        fmiReal relativeTolerance, fmiEventInfo *eventInfo) {"];
    terminate = ["fmiStatus fmiTerminate(fmiComponent c) {"];
    if typ == "cs" then
        platform = "const char* fmiGetTypesPlatform() {";
        instantiate = ["fmiComponent fmiInstantiateSlave(fmiString instanceName, fmiString GUID,"
                       "                                 fmiString fmuLocation, fmiString mimeType,"
                       "                                 fmiReal timeout, fmiBoolean visible, fmiBoolean interactive,"
                       "                                 fmiCallbackFunctions functions, fmiBoolean loggingOn) {"];
        freeInstance = ["void fmiFreeSlaveInstance(fmiComponent c) {"];
        initialize = ["fmiStatus fmiInitializeSlave(fmiComponent c, fmiReal tStart,"
                      "                             fmiBoolean StopTimeDefined, fmiReal tStop) {"];
        terminate = ["fmiStatus fmiTerminateSlave(fmiComponent c) {"];
    end
    // fmiGetModelTypesPlatform
    mputl([platform;
    "    return fmiModelTypesPlatform;"
    "}"
    ""
    // fmiGetVersion
    "const char* fmiGetVersion() {"
    "    return fmiVersion;"
    "}"
    ""],fd);
    // fmiInstantiateModel
    mputl([instantiate;
    "    fmuInstance* newInstance;"
    "    unsigned int lengthName = 0;"
    "    if (!functions.logger) {"
    "        return NULL;"
    "    }"
    "    if (!functions.allocateMemory) {"
    "        functions.logger(NULL, instanceName, fmiError, ""Callback error"","
    "        ""%s: Can not find memory allocation function."", ""fmiInstantiateModel"");"
    "        return NULL;"
    "    }"
    "    if (!functions.freeMemory) {"
    "        functions.logger(NULL, instanceName, fmiError, ""Callback error"","
    "        ""%s: Can not find memory release function."", ""fmiInstantiateModel"");"
    "        return NULL;"
    "    }"
    "    if (!instanceName || strlen(instanceName)==0) {"
    "        functions.logger(NULL, instanceName, fmiError, ""Instance name error"","
    "        ""%s: Can not find instance name."", ""fmiInstantiateModel"");"
    "        return NULL;"
    "    }"
    "    if (strcmp(GUID, MODEL_GUID)) {"
    "        functions.logger(NULL, instanceName, fmiError, ""GUID error"","

    "        ""%s: Wrong model GUID : %s. Expected : %s."", ""fmiInstantiateModel"","
    "        GUID, MODEL_GUID);"
    "        return NULL;"
    "    }"
    "    newInstance = (fmuInstance*)functions.allocateMemory(1, sizeof(fmuInstance));"
    "    lengthName = strlen(instanceName);"
    "    if (newInstance) {"
    "        newInstance->real = functions.allocateMemory(ALL_REALS, sizeof(fmiReal));"
    "        newInstance->integer = functions.allocateMemory(ALL_INTEGERS, sizeof(fmiInteger));"
    "        newInstance->boolean = functions.allocateMemory(ALL_BOOLEANS, sizeof(fmiBoolean));"
    "        newInstance->instanceName = (fmiString)functions.allocateMemory(lengthName+1, sizeof(char));"
    "    }"
    "    if(!newInstance || !newInstance->real || !newInstance->integer"
    "      || !newInstance->boolean || !newInstance->instanceName) {"
    "    functions.logger(NULL, instanceName, fmiError, ""Memory error"","
    "                    ""%s: Error allocating memory."", ""fmiInstantiateModel"");"
    "        return NULL;"
    "    }"
    "    newInstance->GUID = GUID;"
    // intenal copy of instance name
    "    memcpy((char*)newInstance->instanceName, instanceName, lengthName+1);"
    "    newInstance->functions = functions;"
    "    newInstance->loggingOn = loggingOn;"
    "    newInstance->isInstantiated = fmiTrue;"
    "    newInstance->isInitialized = fmiFalse;"
    "    newInstance->isTerminated = fmiFalse;"
    "    newInstance->time = 0;"
    ""
    "    /* set start values */"],fd);
    setStartValues();
    mputl([""
    "    if (loggingOn == fmiTrue) {"
    "        functions.logger(NULL, newInstance->instanceName, fmiOK, ""OK"","
    "        ""%s: Instantiation succeeded. GUID : %s"", ""fmiInstantiateModel"", GUID);"
    "    }"
    "    return newInstance;"
    "}"
    ""],fd);
    // fmiFreeModelInstance

    mputl([freeInstance;
    instance;
    "    if (!newInstance) return;"
    "    if (newInstance->real) {"
    "        newInstance->functions.freeMemory(newInstance->real);"
    "    }"
    "    if (newInstance->integer) {"
    "        newInstance->functions.freeMemory(newInstance->integer);"
    "    }"
    "    if (newInstance->boolean) {"
    "        newInstance->functions.freeMemory(newInstance->boolean);"
    "    }"
    "    newInstance->isInstantiated = fmiFalse;"
    "    newInstance->isInitialized  = fmiFalse;"
    "    newInstance->isTerminated   = fmiFalse;"
    "    if (newInstance->loggingOn) {"
    "        newInstance->functions.logger(c, newInstance->instanceName, fmiOK, ""OK"","
    "        ""%s: Freeing succeeded."", ""fmiFreeModelInstance"");"
    "    }"
    "    if (newInstance->instanceName) {"
    "        newInstance->functions.freeMemory(newInstance->instanceName);"
    "    }"
    "    newInstance->functions.freeMemory(newInstance);"
    "}"
    ""],fd);
    // fmiSetDebugLogging
    mputl(["fmiStatus fmiSetDebugLogging(fmiComponent c, fmiBoolean loggingOn) {"
    instance;
    instantiatedInitialized("fmiSetDebugLogging", 1);
    "    newInstance->loggingOn = loggingOn;"
    "    if (newInstance->loggingOn) {"
    "        newInstance->functions.logger(c, newInstance->instanceName, fmiOK, ""OK"","
    "        ""%s: Debug logging on."", ""fmiSetDebugLogging"");"
    "    }"
    "    return fmiOK;"
    "}"
    ""],fd);
    // only for model Exchange
    if typ == "me" then
        // fmiSetTime
        mputl(["fmiStatus fmiSetTime(fmiComponent c, fmiReal time) {"
        instance;
        instantiatedInitialized("fmiSetTime", 1);
        "    set_scicos_time(time);"
        "    newInstance->time = time;"
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName, fmiOK, ""OK"","
        "        ""%s: Time = %.14g"", ""fmiSetTime"", time);"
        "    }"
        "    return fmiOK;"
        "}"
        ""],fd);
        // fmiSetContiniousStates
        mputl(["fmiStatus fmiSetContinuousStates(fmiComponent c, const fmiReal x[], size_t nx) {"
        instance],fd);
        if strtod(description(3)) then
            mputl("    int i;",fd);
        end
        mputl([instantiatedInitialized("fmiSetContinuousStates", 2);
        "    if (nx > ALL_STATES) {"
        "       newInstance->functions.logger(c, newInstance->instanceName, fmiError, ""logging"","
        "       ""%s: Incorrect number of continuous states. Expected : %d"", ""fmiSetContinuousStates"", ALL_STATES);"
        "       return fmiError;"
        "    }"
        "#if ALL_STATES > 0"
        "        for (i=0; i<nx; i++) {"
        "            fmiValueReference vr = sciStates[i];"
        "            if (newInstance->loggingOn) {"
        "                newInstance->functions.logger(c, newInstance->instanceName, fmiOK, ""OK"","
        "                ""%s: #r%d# = %.14f"", ""fmiSetContinuousStates"", vr, x[i]);"
        "            }"
        "        newInstance->real[vr] = x[i];"
        "        }"
        "#endif"
        "    return fmiOK;"
        "}"
        ""
        // fmiCompletedIntegratorStep
        "fmiStatus fmiCompletedIntegratorStep(fmiComponent c, fmiBoolean *callEventUpdate) {"
        instance;
        instantiatedInitialized("fmiCompletedIntegratorStep", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName, fmiOK,"
        "        ""OK"",""%s: Step completed."", ""fmiCompletedIntegratorStep"");"
        "    }"
        "    *callEventUpdate = fmiFalse;"
        "    return fmiOK;"
        "}"
        ""],fd);
    end
    // fmiSetReal
    mputl(["fmiStatus fmiSetReal(fmiComponent c, const fmiValueReference vr[],"
    "    size_t nvr, const fmiReal value[]) {"
    instance;
    "    int i;"
    "    fmiStatus status;"
    "    fmiBoolean nonStartPresent = fmiFalse;"
    "#ifdef NON_START_REAL"
    "    int j;"
    "    fmiBoolean nonStartRef;"
    "    fmiValueReference nonStartReal["+string(length(nonStartReal))+"] = "+...
    "NON_START_REAL;"
    "#endif"
    instantiatedInitialized("fmiSetReal", 12);
    "    for (i=0; i<nvr; i++) {"
    "#ifdef NON_START_REAL"
    "        nonStartRef = fmiFalse;"
    "#endif"
    "        if (vr[i] >= ALL_REALS) {"
    "            newInstance->functions.logger(c, newInstance->instanceName,"
    "            fmiError, ""OK"", ""%s: Wrong value reference #r%d"","
    "            ""fmiSetReal"",vr[i]);"
    "            return fmiError;"
    "        }"
    "#ifdef NON_START_REAL"
    "        if (newInstance->loggingOn) {"
    "            for (j=0; j<"+string(length(nonStartReal))+"; j++) {"
    "                if (nonStartReal[j] == vr[i]) {"
    "                    nonStartRef = fmiTrue;"
    "                    nonStartPresent = fmiTrue;"
    "                }"
    "             }"
    "                if (!nonStartRef) {"
    "                    newInstance->functions.logger(c, newInstance->instanceName,"
    "                    fmiOK, ""OK"",""%s: #r%d# = %.14f"", ""fmiSetReal"","
    "                    vr[i], value[i]);"
    "                }"
    "                else {"
    "                    newInstance->functions.logger(c, newInstance->instanceName,"
    "                    fmiWarning, ""Warning"",""%s: It is not the reference to start value : #r%d# = %.14f"","
    "                    ""fmiSetReal"", vr[i], value[i]);"
    "                }"
    "         }"
    "#endif"
    "#ifndef NON_START_REAL"
    "    if (newInstance->loggingOn) {"
    "        newInstance->functions.logger(c, newInstance->instanceName,"
    "        fmiOK, ""OK"",""%s: #r%d# = %.14f"", ""fmiSetReal"", vr[i], value[i]);"
    "     }"
    "#endif"
    "     newInstance->real[vr[i]] = value[i];"
    "    }"
    "    status = (!nonStartPresent) ? fmiOK : fmiWarning;"
    "    return status;"
    "}"
    ""
    // fmiSetInteger
    "fmiStatus fmiSetInteger(fmiComponent c, const fmiValueReference vr[],"
    "    size_t nvr, const fmiInteger value[]) {"
    instance;
    "    int i;"
    "    fmiStatus status;"
    "    fmiBoolean nonStartPresent = fmiFalse;"
    "#ifdef NON_START_INT"
    "    int j;"
    "    fmiBoolean nonStartRef;"
    "    fmiValueReference nonStartInt["+string(length(nonStartInt))+"] = "+...
    "NON_START_INT;"
    "#endif"
    instantiatedInitialized("fmiSetInteger", 12);
    "    for (i=0; i<nvr; i++) {"
    "#ifdef NON_START_INT"
    "        nonStartRef = fmiFalse;"
    "#endif"
    "        if (vr[i] >= ALL_INTEGERS) {"
    "            newInstance->functions.logger(c, newInstance->instanceName,"
    "            fmiError, ""Warning"", ""%s: Wrong value reference #i%d"","
    "            ""fmiSetInteger"",vr[i]);"
    "            return fmiError;"
    "        }"
    "#ifdef NON_START_INT"
    "        if (newInstance->loggingOn) {"
    "            for (j=0; j<"+string(length(nonStartInt))+"; j++) {"
    "                if (nonStartInt[j] == vr[i]) {"
    "                    nonStartRef = fmiTrue;"
    "                    nonStartPresent = fmiTrue;"
    "                }"
    "             }"
    "                if (!nonStartRef) {"
    "                    newInstance->functions.logger(c, newInstance->instanceName,"
    "                    fmiOK, ""OK"",""%s: #i%d# = %.14f"", ""fmiSetInteger"","
    "                    vr[i], value[i]);"
    "                }"
    "                else {"
    "                    newInstance->functions.logger(c, newInstance->instanceName,"
    "                    fmiWarning, ""Warning"",""%s: It is not the reference to start value : #%d# = %.14f"","
    "                    ""fmiSetInteger"", vr[i], value[i]);"
    "                }"
    "         }"
    "#endif"
    "#ifndef NON_START_INT"
    "     if (newInstance->loggingOn) {"
    "         newInstance->functions.logger(c, newInstance->instanceName,"
    "         fmiOK, ""OK"",""%s: #%d# = %.14f"", ""fmiSetInteger"", vr[i], value[i]);"
    "     }"
    "#endif"
    "     newInstance->integer[vr[i]] = value[i];"
    "    }"
    "    status = (!nonStartPresent) ? fmiOK : fmiWarning;"
    "    return status;"
    "}"
    ""],fd);
    // fmiSetBoolean
    mputl(["fmiStatus fmiSetBoolean(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiBoolean value[]) {"
    instance;
    "    int i;"
    "    fmiStatus status;"
    "    fmiBoolean nonStartPresent = fmiFalse;"
    "#ifdef NON_START_BOOL"
    "    int j;"
    "    fmiBoolean nonStartRef;"
    "    fmiValueReference nonStartBool[1] = NON_START_BOOL;"
    "#endif"
    instantiatedInitialized("fmiSetBoolean", 12);
    "    for (i=0; i<nvr; i++) {"
    "#ifdef NON_START_BOOL"
    "    nonStartRef = fmiFalse;"
    "#endif"
    "    if (vr[i] >= ALL_BOOLEANS) {"
    "        newInstance->functions.logger(c, newInstance->instanceName,"
    "        fmiError, ""Warning"", ""%s: Wrong value reference #b%d"","
    "        ""fmiSetBoolean"",vr[i]);"
    "        return fmiError;"
    "    }"
    "#ifdef NON_START_BOOL"
    "    if (newInstance->loggingOn) {"
    "       for (j=0; j<1; j++) {"
    "           if (nonStartBool[j] == vr[i]) {"
    "               nonStartRef = fmiTrue;"
    "               nonStartPresent = fmiTrue;"
    "           }"
    "       }"
    "           if (!nonStartRef) {"
    "               newInstance->functions.logger(c, newInstance->instanceName,"
    "               fmiOK, ""OK"", ""%s: #b%d# = %d"", ""fmiSetBoolean"","
    "               vr[i], value[i]);"
    "           }"
    "           else {"
    "               newInstance->functions.logger(c, newInstance->instanceName,"
    "               fmiWarning, ""Warning"",""%s: It is not the reference to start value : #b%d# = %d"","
    "               ""fmiSetBoolean"", vr[i], value[i]);"
    "           }"
    "    }"
    "#endif"
    "#ifndef NON_START_BOOL"
    "    if (newInstance->loggingOn) {"
    "        newInstance->functions.logger(c, newInstance->instanceName,"
    "        fmiOK, ""OK"", ""%s: #b%d# = %d"", ""fmiSetBoolean"", vr[i], value[i]);"
    "    }"
    "#endif"

    "    newInstance->boolean[vr[i]] = value[i];"
    "    }"
    "    status = (!nonStartPresent) ? fmiOK : fmiWarning;"
    "    return status;"
    "}"
    ""
    // fmiSetString
    "fmiStatus fmiSetString(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiString value[]) {"
    instance;
    "    newInstance->functions.logger(c, newInstance->instanceName,"
    "    fmiWarning, ""Warning"",""%s: Not yet implemented."", ""fmiString"");"
    "    return fmiWarning;"
    "}"
    ""],fd);
    // fmiInitialize
    mputl([initialize;
    instance;
    "    int local_flag, i;"
    "    fmiStatus status = fmiOK;"],fd);
    if typ == "me" then
        mputl(["    if ((toleranceControlled) && (newInstance->loggingOn)) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiWarning, ""Warning"",""%s: Tolerance control not supported."", ""fmiInitialize"");"
        "    }"
        ""
        instantiatedInitialized("fmiInitialize", 1)
        "    for (i=0; i<"+string(length(tevts)+1)+"; i++) {"
        "         newInstance->internalEvent[i] = fmiFalse;"
        "    }"
        ""
        "    eventInfo->iterationConverged          = fmiTrue;"
        "    eventInfo->stateValueReferencesChanged = fmiTrue;"
        "    eventInfo->stateValuesChanged          = fmiTrue;"
        "    eventInfo->terminateSimulation         = fmiFalse;"
        "    eventInfo->upcomingTimeEvent           = fmiTrue;"
        "    eventInfo->nextEventTime = " + stepInit + " + newInstance->time;"
        ""
        "    /* set initial time */"
        "    t = newInstance->time;"
        ""],fd);
    else
        mputl([instantiatedInitialized("fmiInitialize", 1)
        "    for (i=0; i<"+string(length(tevts)+1)+"; i++) {"
        "         newInstance->internalEvent[i] = fmiFalse;"
        "    }"
        "    newInstance->eventInfo.iterationConverged          = fmiTrue;"
        "    newInstance->eventInfo.stateValueReferencesChanged = fmiFalse;"
        "    newInstance->eventInfo.stateValuesChanged          = fmiFalse;"
        "    newInstance->eventInfo.terminateSimulation         = fmiFalse;"
        "    newInstance->eventInfo.upcomingTimeEvent           = fmiTrue;"
        "    newInstance->eventInfo.nextEventTime = " + stepInit + " + newInstance->time;"
        ""],fd);
    end
    // affectation of work
    mputl(["    /* Get work ptr of blocks */"
    "    work = (void **)(z+"+string(size(z,"*")+size(outtb))+");"
    ""], fd);
    // affection of outtbptr
    if Code_outtb<>[] then
        mputl(["    /* Get outtbptr ptr of blocks */"
        "    "+rdnom+"_block_outtbptr = (void **)(z+"+string(nztotal)+");"
        ""], fd);
    end
    // inputs
    if newInput <> [] then
        mputl(["    /* Get inputs from instance */"
        "    "+newInput],fd);
    end
    if Code_outtbptr<>[] then
        mputl([Code_outtbptr
        ""], fd);
    end
    // xcos blocks
    mputl([""
    "    /* Set xcos blk structures and object parameters*/"],fd);
    mputl(finalTxtInstance,fd);
    for kf=1:nblk
        txt = call_block42(kf,0,4);
        if txt <> [] then
            mputl(["    "+txt], fd);
        end
    end
    // update outputs
    if txtOutAdress <> [] then
       mputl(txtOutAdress,fd);
    end
    mputl([""
    "    newInstance->isInitialized = fmiTrue;"
    "    if (newInstance->loggingOn && status == fmiOK) {"
    "        newInstance->functions.logger(c, newInstance->instanceName, fmiOK,"
    "        ""OK"",""%s: Initialization succeeded. ToleranceControlled = %s"","
    "        ""fmiInitialize"", ""fmiFalse"");"
    "    }"
    "    else if (status == fmiError) {"
    "        newInstance->functions.logger(c, newInstance->instanceName, fmiError,"
    "        ""ERROR"",""%s: Initialization failed."", ""fmiInitialize"");"
    "    }"
    "    return status;"
    "}"
    ""],fd);
    if typ == "me" then
        // fmiGetDerivatives
        mputl(["fmiStatus fmiGetDerivatives(fmiComponent c, fmiReal derivatives[], size_t nx) {"
        instance;
        "    fmiStatus status = fmiOK;"
        "#if ALL_STATES>0"
        "    fmiValueReference vr;"
        "    int i;"
        "    if (nx > ALL_STATES) {"
        "       newInstance->functions.logger(c, newInstance->instanceName, fmiError, ""Error"","
        "       ""%s: Incorrect number of continuous states. Expected : %d"","
        "       ""fmiGetDerivatives"", ALL_STATES);"
        "       return fmiError;"
        "    }"
        "    if (newInstance->isInitialized)"
        "    {"
        "       status = "+rdnom+"_sciOutputCompute(newInstance, 0);"
        "    }"
        "    for (i=0; i<nx; i++) {"
        "       vr = sciStates[i] + 1;"
        "       derivatives[i] = sciGetReal(newInstance,vr);"
        "       if (newInstance->loggingOn) newInstance->functions.logger(c, newInstance->instanceName,"
        "       fmiOK, ""OK"",""%s: #r%d# = %.14f"", ""fmiGetDerivatives"", vr, derivatives[i]);"
        "    }"
        "#endif"
        "    return status;"
        "}"
        ""],fd);
        // fmiGetEventIndicators
        mputl(["fmiStatus fmiGetEventIndicators(fmiComponent c, fmiReal eventIndicators[], size_t ni) {"
        instance;
        "    fmiStatus status = fmiOK;"
        "#if ALL_EVENTS>0"
        "    int i;"
        "    if (newInstance->isInitialized)"
        "    {"
        "       status = "+rdnom+"_sciOutputCompute(newInstance, 9);"
        "    }"
        "    eventIndicators[0] = 0;"
        "    for (i=0; i<ni; i++) {"
        "       eventIndicators[i] = sciGetEvent(newInstance, i); "
        "       if (newInstance->loggingOn) newInstance->functions.logger(c, newInstance->instanceName,"
        "       fmiOK, ""OK"",""%s: #r%d# = %.14f"", ""fmiGetEventIndicators"", i, eventIndicators[i]);"
        "    }"
        "#endif"
        "    return status;"
        "}"
        ""],fd);
    end
        // find blocks with tevts
        nblk = cpr.sim.nblk;
        clkptr=cpr.sim.clkptr;
        blkIntActiv = [];
        for i=1:nblk
            if ((clkptr(i+1)-clkptr(i)) <> 0) then
                blkIntActiv($+1) = i;
            end
        end
        txtEvent = []; isEvent = [];
        txtEventLoop = [];
        if blkIntActiv <> [] then
            nbblkAktiv = length(blkIntActiv);
            if typ == "me" then
                txtEvent = ["    int local_flag, i;";
                            "    fmiReal tEvent;";
                            "    tEvent = newInstance->time;"
                            ""
                            "    for (i=0; i<"+string(nbblkAktiv)+"; i++) {"]
                isEvent =  "           isEvent = fmiTrue;";
            else
                txtEvent = ["    int i;";
                            "    fmiReal tEvent;";];
            end
                        for i=1:nbblkAktiv
                            txtEventLoop = [txtEventLoop;
                            "       if ((tEvent >= tEvents["+string(i-1)+"]) && "+...
                            "(newInstance->internalEvent["+string(i-1)+"] != fmiTrue))"
                            "       {"
                            indentLines("           ", call_block42(blkIntActiv(i),1,1))
                            "           newInstance->internalEvent["+string(i-1)+"] = fmiTrue;"
                            isEvent;
                            "       }"];
                        end
                        txtEventLoop = [txtEventLoop; "    }"];
        end
        // fmiEventUpdate
    if typ == "me" then
        mputl(["fmiStatus fmiEventUpdate(fmiComponent c, fmiBoolean intermediateResults, fmiEventInfo* eventInfo) {"
        instance;
        "    fmiStatus status = fmiOK;"
        "    fmiBoolean isEvent = fmiFalse;"
        txtEvent;
        txtEventLoop;
        "       if (newInstance->isInitialized)"
        "       {"
        "           status = "+rdnom+"_sciOutputCompute(newInstance, 2);"
        "       }"
        "       eventInfo->upcomingTimeEvent = fmiTrue;"
        "       eventInfo->nextEventTime = " + stepVl + " + newInstance->time;"
        "       "
        "       eventInfo->iterationConverged          = fmiTrue;"
        "       eventInfo->stateValueReferencesChanged = fmiFalse;"
        "       eventInfo->stateValuesChanged          = fmiFalse;"
        "       eventInfo->terminateSimulation         = fmiFalse;"
        "    return status;"
        "}"
        ""
        // fmiGetContinuousStates
        "fmiStatus fmiGetContinuousStates(fmiComponent c, fmiReal states[], size_t nx) {"
        instance;
        "    fmiStatus status = fmiOK;"
        "#if ALL_STATES>0"
        "    fmiValueReference vr;"
        "    int i;"
        "    if (nx > ALL_STATES) {"
        "       newInstance->functions.logger(c, newInstance->instanceName, fmiError, ""Error"","
        "       ""%s: Incorrect number of continuous states. Expected : %d"","
        "       ""fmiGetContinuousStates"", ALL_STATES);"
        "       return fmiError;"
        "    }"
        "    if (newInstance->isInitialized)"
        "    {"
        "       status = "+rdnom+"_sciOutputCompute(newInstance, 1);"
        "       if (status == fmiError) return status;"
        "       status = "+rdnom+"_sciOutputCompute(newInstance, 0);"
        "    }"
        "    for (i=0; i<nx; i++) {"
        "       vr = sciStates[i];"
        "       states[i] = sciGetReal(newInstance,vr);"
        "       if (newInstance->loggingOn) newInstance->functions.logger(c, newInstance->instanceName,"
        "          fmiOK, ""OK"",""%s: #r%d# = %.14f"", ""fmiGetContinuousStates"", vr, states[i]);"
        "    }"
        "#endif"
        "    return status;"
        "}"
        // fmiGetNominalContinuousStates
        "fmiStatus fmiGetNominalContinuousStates(fmiComponent c, fmiReal x_nominal[], size_t nx) {"
        instance;
        "    int i;"
        "    if (nx > ALL_STATES) {"
        "       newInstance->functions.logger(c, newInstance->instanceName, fmiError, ""Error"","
        "       ""%s: Incorrect number of continuous states. Expected : %d"","
        "       ""fmiGetNominalContinuousStates"", ALL_STATES);"
        "       return fmiError;"
        "    }"
        "    for(i=0; i<ALL_STATES; i++) {"
        "       x_nominal[i] = 1;"
        "    }"
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiOK, ""OK"",""%s: Nominal continuous states = 1"","
        "        ""fmiGetNominalContinuousStates"");"
        "    }"
        "    return fmiOK;"
        "}"
        ""],fd);
        // fmiGetStateValueReferences
        mputl(["fmiStatus fmiGetStateValueReferences(fmiComponent c, fmiValueReference vrx[], size_t nx) {"
        instance;],fd);
        if strtod(description(3)) then
            mputl("    int i;",fd);
        end
        mputl([instantiatedInitialized("fmiGetStateValueReferences", 2);
        "    if (nx > ALL_STATES) {"
        "       newInstance->functions.logger(c, newInstance->instanceName, fmiError, ""Error"","
        "       ""%s: Incorrect number of continuous states. Expected : %d"","
        "       ""fmiGetStateValueReferences"", ALL_STATES);"
        "       return fmiError;"
        "    }"
        "#if ALL_STATES > 0"
        "    for(i=0; i<ALL_STATES; i++) {"
        "       vrx[i] = sciStates[i];"
        "       if (newInstance->loggingOn) {"
        "           newInstance->functions.logger(c, newInstance->instanceName,"
        "           fmiOK, ""OK"",""%s: Continuous state vr: #r%d#"","
        "           ""fmiGetStateValueReferences"", vrx[i]);"
        "       }"
        "    }"
        "#endif"
        "    return fmiOK;"
        "}"
        ""],fd);
    end
    // fmiGetReal
    mputl(["fmiStatus fmiGetReal(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiReal value[]) {"
    instance;
    "    fmiStatus status = fmiOK;"
    "#if ALL_REALS>0"
    "    int i;"
    "    if (newInstance->isInitialized)"
    "    {"
    "        status = "+rdnom+"_sciOutputCompute(newInstance, 1);"
    "    }"
    "    for (i=0; i<nvr; i++) {"
    "       if (vr[i] >= ALL_REALS) {"
    "           newInstance->functions.logger(c, newInstance->instanceName,"
    "           fmiError, ""Error"",""%s: Wrong value reference #r%d#"","
    "           ""fmiGetReal"",vr[i]);"
    "           return fmiError;"
    "       }"
    "    value[i] = sciGetReal(newInstance,vr[i]);"
    "    if (newInstance->loggingOn) newInstance->functions.logger(c, newInstance->instanceName,"
    "       fmiOK, ""OK"",""%s: #r%d# = %.14f"", ""fmiGetReal"", vr[i], value[i]);"
    "    }"
    "#endif"
    "    return status;"
    "}"
    ""],fd);
    // fmiGetInteger
    mputl(["fmiStatus fmiGetInteger(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiInteger value[]) {"
    instance;
    "    fmiStatus status = fmiOK;"
    "#if ALL_INTEGERS>0"
    "    int i;"
    "    if (newInstance->isInitialized)"
    "    {"
    "        status = "+rdnom+"_sciOutputCompute(newInstance, 1);"
    "    }"
    "    for (i=0; i<nvr; i++) {"
    "       if (vr[i] >= ALL_INTEGERS) {"
    "           newInstance->functions.logger(c, newInstance->instanceName,"
    "           fmiError, ""Error"",""%s: Wrong value reference #i%d"","
    "           ""fmiGetInteger"",vr[i]);"
    "           return fmiError;"
    "       }"
    "    value[i] = sciGetInteger(newInstance,vr[i]);"
    "    if (newInstance->loggingOn) newInstance->functions.logger(c, newInstance->instanceName,"
    "       fmiOK, ""OK"",""%s: #i%d# = %d"", ""fmiGetInteger"", vr[i], value[i]);"
    "    }"
    "#endif"
    "    return status;"
    "}"
    ""],fd);
     // fmiGetBoolean
    mputl(["fmiStatus fmiGetBoolean(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiBoolean value[]) {"
    instance;
    "    fmiStatus status = fmiOK;"
    "#if ALL_BOOLEANS>0"
    "    int i;"
    "    if (newInstance->isInitialized)"
    "    {"
    "        status = "+rdnom+"_sciOutputCompute(newInstance, 1);"
    "    }"
    "    for (i=0; i<nvr; i++) {"
    "       if (vr[i] >= ALL_BOOLEANS) {"
    "           newInstance->functions.logger(c, newInstance->instanceName,"
    "           fmiError, ""Error"", ""%s: Wrong value reference #b%d"","
    "           ""fmiGetBoolean"",vr[i]);"
    "           return fmiError;"
    "       }"
    "       value[i] = sciGetBoolean(newInstance,vr[i]);"
    "       if (newInstance->loggingOn) newInstance->functions.logger(c, newInstance->instanceName,"
    "       fmiOK, ""OK"", ""%s: #b%d# = %d"", ""fmiGetBoolean"", vr[i], value[i]);"
    "    }"
    "#endif"
    "    return status;"
    "}"
    ""
    // fmiGetString
    "fmiStatus fmiGetString(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiString value[]) {"
    instance;
    "    newInstance->functions.logger(c, newInstance->instanceName,"
    "    fmiWarning, ""Warning"",""%s: Not yet implemented."", ""fmiGetString"");"
    "    return fmiWarning;"
    "}"
    ""],fd);
    // fmiTerminate
    mputl([terminate;
    instance;
    "    int local_flag;"
    "    fmiStatus status = fmiOK;"
    "    double t = newInstance->time;"
    "    newInstance->isTerminated = fmiTrue;"
    instantiatedInitialized("fmiTerminate", 2)],fd);
    mputl(["    "+get_comment("flag",list(5))], fd);
    for kf=1:nblk
        if or(kf==act) | or(kf==cap) then
            // do nothing=)
        else
            txt = indentLines("  ", call_block42(kf,0,5));
            if txt <> [] then
                mputl(["";
                "  "+txt], fd);
            end
        end
    end
    mputl(["    if (newInstance->loggingOn && status == fmiOK) {"
    "        newInstance->functions.logger(c, newInstance->instanceName,"
    "        fmiOK, ""OK"",""%s: Stop model evaluation."", ""fmiTerminate"");"
    "    }"
    "    else if (status == fmiError) {"
    "        newInstance->functions.logger(c, newInstance->instanceName, fmiError,"
    "        ""ERROR"",""%s: Stop model evaluation failed."", ""fmiTerminate"");"
    "        newInstance->isTerminated = fmiFalse;"
    "    }"
    "    return status;"
    "}"
    ""],fd);
    // only Co-Simulation functions
    if typ == "cs" then
        // fmiResetSlave
        mputl(["fmiStatus fmiResetSlave(fmiComponent c) {"
        instance;
        instantiatedInitialized("fmiResetSlave", 2)],fd);
        setStartValues();
        mputl(["    return fmiOK;"
        "}"
        ""
        // fmiSetRealInputDerivatives
        "fmiStatus fmiSetRealInputDerivatives(fmiComponent c, const fmiValueReference vr[], size_t nvr,"
        "                                     const fmiInteger order[], const fmiReal value[]) {"
        instance;
        instantiatedInitialized("fmiSetRealInputDerivatives", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiWarning, ""Warning"",""%s: Input interpolation not supported: canInterpolateInputs = fmiFalse"", ""fmiSetRealInputDerivatives"");"
        "    }"
        "    return fmiWarning;"
        "}"
        ""
        // fmiGetRealOutputDerivatives
        "fmiStatus fmiGetRealOutputDerivatives(fmiComponent c, const fmiValueReference vr[], size_t nvr,"
        "                                      const fmiInteger order[], fmiReal value[]) {"
        instance;
        instantiatedInitialized("fmiGetRealOutputDerivatives", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiWarning, ""Warning"",""%s: Compute outputs derivatives not supported: "+...
        "maxOutputDerivativeOrder = fmiFalse"", ""fmiGetRealOutputDerivatives"");";
        "    }"
        "    return fmiWarning;"
        "}"
        ""
        // fmiCancelStep
        "fmiStatus fmiCancelStep(fmiComponent c) {"
        instance;
        instantiatedInitialized("fmiCancelStep", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiWarning, ""Warning"",""%s: Calling of fmiDoStep in an asynchronous way not supported: "+...
        "canRunAsynchronuously = fmiFalse"", ""fmiCancelStep"");"
        "    }"
        "    return fmiWarning;"
        "}"
        ""
        // fmiGetStatus
        "fmiStatus fmiGetStatus(fmiComponent c, const fmiStatusKind s, fmiStatus* value) {"
        instance;
        instantiatedInitialized("fmiGetStatus", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiWarning, ""Warning"",""%s: Slave status not supported."", ""fmiGetStatus"");"
        "    }"
        "    return fmiWarning;"
        "}"
        ""
        // fmiGetRealStatus
        "fmiStatus fmiGetRealStatus(fmiComponent c, const fmiStatusKind s, fmiReal* value) {"
        instance;
        instantiatedInitialized("fmiGetRealStatus", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiWarning, ""Warning"",""%s: Slave real status not supported."", ""fmiGetRealStatus"");"
        "    }"
        "    return fmiWarning;"
        "}"
        ""
        // fmiGetIntegerStatus
        "fmiStatus fmiGetIntegerStatus(fmiComponent c, const fmiStatusKind s, fmiInteger* value) {"
        instance;
        instantiatedInitialized("fmiGetIntegerStatus", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiWarning, ""Warning"",""%s: Slave integer status not supported."", ""fmiGetIntegerStatus"");"
        "    }"
        "    return fmiWarning;"
        "}"
        ""
        // fmiGetBooleanStatus
        "fmiStatus fmiGetBooleanStatus(fmiComponent c, const fmiStatusKind s, fmiBoolean* value) {"
        instance;

        instantiatedInitialized("fmiGetBooleanStatus", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiWarning, ""Warning"",""%s: Slave boolean status not supported."", ""fmiGetBooleanStatus"");"
        "    }"
        "    return fmiWarning;"
        "}"
        ""
        // fmiGetStringStatus
        "fmiStatus fmiGetStringStatus(fmiComponent c, const fmiStatusKind s, fmiString* value) {"
        instance;
        instantiatedInitialized("fmiGetStringStatus", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiWarning, ""Warning"",""%s: Slave string status not supported."", ""fmiGetStringStatus"");";
        "    }"
        "    return fmiWarning;"
        "}"
        ""
        ],fd);
        // fmiDoStep
        mputl(["fmiStatus fmiDoStep(fmiComponent c, fmiReal currentCommunicationPoint,"
        "                           fmiReal communicationStepSize, fmiBoolean newStep) {"
        instance;
        "    fmiStatus status = fmiOK;"
        "    fmiReal h = communicationStepSize;"
        "    fmiReal tEnd;"
        "    int local_flag, k;"
        "    size_t nbStep = 0;"
        txtEvent;
        instantiatedInitialized("fmiDoStep", 2);
        "    if (newInstance->loggingOn) {"
        "        newInstance->functions.logger(c, newInstance->instanceName,"
        "        fmiOK, ""OK"",""%s: currentCommunicationPoint: %f,"""
        "        ""communicationStepSize: %f, newStep: %s\n"", ""fmiDoStep"","
        "        currentCommunicationPoint, communicationStepSize,"
        "        newStep ? ""fmiTrue"" : ""fmiFalse"");"
        "    }"
        "    h = h/10;"
        "    tEnd = currentCommunicationPoint + communicationStepSize;"
        ""],fd);
        tEv = []; tEvloop = [];
        if txtEvent <> [] then
            tEv = "    tEvent = newInstance->time;"
            tEvloop = ["    /* time events */"
                    "        for (i=0; i<"+string(nbblkAktiv)+"; i++) {"
                    "    "+txtEventLoop;];
        end
        updT = []
        if txtUpdtStates <> [] then
            updT = ["    /* compute continuous states with Euler method */"
            "    if (ALL_STATES > 0) {"
            "    "+part(txtUpdtStates(2:2:$),1:$-1)+" + h*newInstance->real["+string(states+1)'+"];"
            ""
            indentLines("    ", write_code_odoit(0));
            "       "+part(txtUpdtStatesNew(2:2:$),1:$-1)+" = "+part(txtUpdtStatesNew(1:2:$),5:$-1)+";"
            "    }"];
        end
        mputl([
        "    while (currentCommunicationPoint < tEnd && nbStep < 10) {"
        "        set_scicos_time(currentCommunicationPoint);"
        "        newInstance->time = currentCommunicationPoint;"
        "        // update events"
        "        status = "+rdnom+"_sciOutputCompute(newInstance, 9);"
        "        if (status == fmiError) break;"
        "        // update outputs"
        "        status = "+rdnom+"_sciOutputCompute(newInstance, 1);"
        "        if (status == fmiError) break;"
        indentLines("    ", tEv);
        updT;
        tEvloop;
        "#if ALL_EVENTS > 0"
        "    for(k=0; k<ALL_EVENTS; k++) {"
        "       sciGetEvent(newInstance, k);"
        "    }"
        "#endif"
        "    if (newInstance->eventInfo.upcomingTimeEvent && newInstance->time >= "+ ...
        "newInstance->eventInfo.nextEventTime) {"
        "        status = "+rdnom+"_sciOutputCompute(newInstance, 2);"
        "        if (status == fmiError) break;"
        "        newInstance->eventInfo.nextEventTime = " + stepVl + " + newInstance->time;"
        "        newInstance->eventInfo.upcomingTimeEvent = fmiTrue;"
        "    }"
        "       currentCommunicationPoint += h;"
        "       nbStep++;"
        "    }"
        "    newInstance->time = currentCommunicationPoint;"
        "    if (status == fmiError) {"
        "        newInstance->functions.logger(c, newInstance->instanceName, fmiError,"
        "        ""ERROR"",""%s: Step failed."", ""fmiDoStep"");"
        "    }"
        "   return status;"

        "}"],fd);
    end
    mclose(fd);
endfunction

// helper functions

// Comma-separated values for a C initializer; "" instead of [] when v is
// empty, as strcat would return and "+" would propagate.
function s = joinValues(v)
    s = strcat(string(v), ",");
    if s == [] then
        s = "";
    end
endfunction

// Prefixes each line with pad; [] stays [] without the "+" warning.
function txt = indentLines(pad, txt)
    if txt <> [] then
        txt = pad + txt;
    end
endfunction

function newMsg = instantiatedInitialized(msg, flag)
    if flag == 1 then
        newMsg = ["    if (!newInstance->isInstantiated) {";
        "        newInstance->functions.logger(c, newInstance->instanceName, fmiError, ""Error"",";
        "        ""%s: Model is not instantiated."","""+msg+""");";
        "        return fmiError;";
        "    }"];
    elseif flag == 2 then
        newMsg = ["    if (!newInstance->isInitialized) {";
        "        newInstance->functions.logger(c, newInstance->instanceName, fmiError, ""Error"",";
        "        ""%s: Model is not initialized."","""+msg+""");";
        "        return fmiError;";
        "    }"];
    elseif flag == 12 then
        newMsg = ["    if (!newInstance->isInitialized & !newInstance->isInstantiated) {";
        "        newInstance->functions.logger(c, newInstance->instanceName, fmiError, ""Error"",";
        "        ""%s: Model must be initialized or instantiated."","""+msg+""");";
        "        return fmiError;";
        "    }"];
    end
endfunction

function setStartValues()
    // set start values of reals
    ind = find(~isnan(var.Real.start));
    if ind <> [] then
        txt = ("    newInstance->real["+string(var.Real.valueReference(ind))+"] = "+...
        string(var.Real.start(ind))+";")';
        mputl(["    // reals";txt],fd);
    end
    // set start values of integers
    ind = find(~isnan(var.Integer.start));
    if ind <> [] then
        txt = ("    newInstance->integer["+string(var.Integer.valueReference(ind))+"] = "+...
        string(var.Integer.start(ind))+";")';
        mputl(["    // integers";txt],fd);
    end
    // set start values of booleans
    if var.Boolean.name <> [] then
        txt = [];
        txt = ("    newInstance->boolean["+string(var.Boolean.valueReference)+"] = 1;")';
        if txt <> [] then
            mputl(["    // booleans";txt],fd);
        end
    end
    // set start values of outputs
    if Code_outtb_initial <> [] then
        mputl(["    // Xcos outputs";Code_outtb_initial], fd);
    end
    if z <> [] then
        mputl(["    z["+string(0:length(z)-1)'+"] = "+string(z)+";"], fd);
    end
endfunction

function [txt,rparamForBlock,iparamForBlock,oparamForBlock] = make_define_param(states, var)
    txt = [];
    rparamForBlock = [];
    iparamForBlock = [];
    oparamForBlock = [];
     //*** Continuous state ***//
    if x <> [] then
        txt=[txt;
        "/* ---- Define continuous state ---- */"
        ("#define "+var.Real.name(states+1)+" (newInstance->real["+string(states)+"])")'
        ("#define "+var.Real.name(states+2)+" (newInstance->real["+string(states+1)+"])")'];
    end
    //*** fmu real parameters ***//
    ind = [];
    ind = find(var.Real.variability == "parameter")
    if  ind  <> [] then
        txt=[txt;
            "/* ---- Define real parameters ---- */";
            ("#define "+var.Real.name(ind)+" (newInstance->real["+string(var.Real.valueReference(ind))+"])")']
        for i=1:(length(rpptr)-1)
            paramNames = string(cpr.sim.funs(i))+"_"+string(i)+"_";
            paramPosition = find(part(var.Real.name,1:length(paramNames)) == paramNames);
            for j=1:size(paramPosition,"*")
                rparamForBlock($+1) = " newInstance->block_"+rdnom+"["+string(i-1)+"].rpar["+string(j-1)+"]";
                rparamForBlock($+1) = " newInstance->real["+string(var.Real.valueReference(paramPosition(j)))+"];";
            end
        end
    end
    //*** fmu integer params ***//
    ind = [];
    ind = find(var.Integer.variability == "parameter")
    if  ind <> [] then
        txt=[txt;
            "/* ---- Define integer parameters ---- */";
            ("#define "+var.Integer.name(ind)+" (newInstance->integer["+string(var.Integer.valueReference(ind))+"])")']
        for i=1:(length(ipptr)-1)
            if part(cpr.sim.funs(i),1:5) <> "bidon" then
                paramNames = string(cpr.sim.funs(i))+"_"+string(i)+"_";
                paramPosition = find(part(var.Integer.name,1:length(paramNames)) == paramNames);
                for j=1:size(paramPosition,"*")
                    iparamForBlock($+1) = " newInstance->block_"+rdnom+"["+string(i-1)+"].ipar["+string(j-1)+"]";
                    iparamForBlock($+1) = " newInstance->integer["+string(var.Integer.valueReference(paramPosition(j)))+"];";
                end
            end
        end
    end
    //*** fmu object params ***//
    if size(opar)<>0 then
        txt=[txt;
        "/* ---- Define object parameters ---- */"]
        for i=1:(length(opptr)-1)
            if opptr(i+1)-opptr(i)>0  then
                //******************//
                for j=1:opptr(i+1)-opptr(i)
                    oparLength = length(cpr.sim.opar(opptr(i)));
                    for k=1:oparLength
                        paramNames = string(cpr.sim.funs(i))+"_"+string(i)+"_"+string(k)+"_";
                        t = []
                        // real opar
                        paramPosition = find(part(var.Real.name,1:length(paramNames)) == paramNames);
                        for m=1:size(paramPosition,"*")
                            ind = string(var.Real.valueReference(paramPosition(m)));
                            oparamForBlock = [oparamForBlock;
                                       "    OPAR_"+string(opptr(i)+j-1)+"["+string(k-1)+"] = newInstance->real["+...
                                       ind+"];"];
                            t = " // newInstance->real["+ind+"]";
                        end
                        // integer opar
                        paramPosition = find(part(var.Integer.name,1:length(paramNames)) == paramNames);
                        for m=1:size(paramPosition,"*")
                            ind = string(var.Integer.valueReference(paramPosition(m)));
                            oparamForBlock = [oparamForBlock;
                                       "    OPAR_"+string(opptr(i)+j-1)+"["+string(k-1)+"] = newInstance->integer["+...
                                       ind+"];"];
                            t = " // newInstance->integer["+ind+"]";
                        end
                    end
                txt =[txt;
                    cformatline("static "+mat2c_typ(opar(opptr(i)+j-1)) +...
                    " OPAR_"+string(opptr(i)+j-1) + "[] = {"+...
                    strcat(string(opar(opptr(i)+j-1)),",")+"};"+t,70)]
                end
            end
        end
    end
endfunction

// activate blocks for discrete state computation
function [txtBool, txtBoolNames] = make_doit_fmu()
    // find index in ord clock
    txtBoolNames = []; txtBool = []; stalone = %t;
    ind = [];
    ind = find(ordclk(:,2) > 0)
    // create new ordclk
    t = [ordclk(ind,1),ordclk(ind,2)];
    tNew = unique(t(:,1));
    tSize = size(tNew,1)
    for i=1:tSize
        ind = find(t(:,1) == tNew(i,1));
        tNew(i,2) = t(ind(1),2);
    end
    // find names of function that are differenet of act and capt and bidon
    ind = [];
    for i=1:tSize
        if part(funs(tNew(i,1)),1:10) <> "actionneur" &...
           part(funs(tNew(i,1)),1:7) <> "capteur" &...
           part(funs(tNew(i,1)),1:5) <> "bidon" then
           ind($+1) = i;
        end
    end
    tNew = [tNew(ind,1),tNew(ind,2)];
    // create fmu booleans names
    boolSize = length(tNew(:,1));
    scsSize = length(scs_m.objs);
    tFuncNames = [];
    for i=1:boolSize
        tFuncNames(i) = funs(tNew(i,1))
    end
    tFuncNames(:,2) = "0";
    boolNames = [];
    // number input activation ports
    for i=1:scsSize
        if typeof(scs_m.objs(i)) == "Block" then
            for j=1:boolSize
                if scs_m.objs(i).model.sim(1) == tFuncNames(j,1) then
                    tFuncNames(j,2) = string(length(scs_m.objs(i).model.evtin));
                end
            end
        end
    end
    tFuncNamesNew = []; tFuncIndNew = []; tFuncActNumnb = [];
    for i=1:boolSize
        for j=1:evstr(tFuncNames(i,2))
            tFuncNamesNew($+1) = tFuncNames(i,1)+"_"+string(i)+"_"+string(j);
            tFuncIndNew($+1) = tNew(i,1);
            tFuncActNumnb($+1) = tNew(i,2);
        end

    end
    txtBool = []; txtBooltmp = [];
    boolSize = sum(evstr(tFuncNames(:,2)));
    if boolSize <> 0 then
        txtBool = ["    "+get_comment("flag",list(2)); "    switch (nevprt) {"];
    end
    for i=1:boolSize
        txtBooltmp = "      case "+string(i)+" :"
        boolNames($+1) = "On_"+tFuncNamesNew(i);
        txtBooltmp = [txtBooltmp; indentLines("        ", call_block42(tFuncIndNew(i), tFuncActNumnb(i), 2))];
        txtBool = [txtBool; txtBooltmp; "      break;"];
    end
    if boolSize <> 0 then
        txtBool = [txtBool;
                   "    }"];
    end
    txtBoolNames = boolNames;
endfunction
//generates  static table definitions
//
//Author : Rachid Djenidi, Alan Layec
function txt=make_static_standalone42()
    oldFormat = format();
    format(25);
    txt=[];
    //*** Continuous state ***//
    if x <> [] then
        txt=[txt;
        "/* def continuous state */"
        cformatline("double x[]={"+strcat(string(x),",")+"};",70)
        cformatline("double xd[]={"+strcat(string(x),",")+"};",70)
        "static int c__1 = 1;"
        "static double c_b14 = 0.;"
        "static int neq="+string(nX)+";"
        ""]
    end
    //************************//
//    txt=[txt;
//    "char input[50], output[50],s[1],sf[1],se[1],ss[1],**p;"
//    "static double sci_time;"
//    "static int errflg = 0;"
//    "static char *optarg = NULL ;"
//    "static int optind = 1, offset = 0 ;"
//    "scicos_block block_"+rdnom+"["+string(nblk)+"];"
//    ""];
    //*** Real parameters ***//
    if size(rpar,1) <> 0 then
        txt=[txt;
        "/* def real parameters */"
        "static double RPAR1[ ] = {"];
        for i=1:(length(rpptr)-1)
            if rpptr(i+1)-rpptr(i)>0  then
                if size(corinv(i),"*")==1 then
                    OO=scs_m.objs(corinv(i));
                else
                    path=list("objs");
                    for l=cpr.corinv(i)(1:$-1)
                        path($+1)=l;
                        path($+1)="model";
                        path($+1)="rpar";
                        path($+1)="objs";
                    end
                    path($+1)=cpr.corinv(i)($);
                    OO=scs_m(path);
                end
                //** Add comments **//
                txt($+1)="/* Routine name of block: "+strcat(string(cpr.sim.funs(i)));
                txt($+1)=" * Gui name of block: "+strcat(string(OO.gui));
                txt($+1)=" * Compiled structure index: "+strcat(string(i));

                if stripblanks(OO.model.label)~=emptystr() then
                    txt=[txt;cformatline(" * Label: "+strcat(string(OO.model.label)),70)];
                end
                if stripblanks(OO.graphics.exprs(1))~=emptystr() then
                    txt=[txt;cformatline(" * Exprs: "+strcat(OO.graphics.exprs(1),","),70)];
                end
                if stripblanks(OO.graphics.id)~=emptystr() then
                    txt=[txt;
                    cformatline(" * Identification: "+strcat(string(OO.graphics.id)),70)];
                end
                txt=[txt;" * rpar="];
                txt($+1)=" */";
                //******************//
                txt=[txt;
                cformatline(strcat(msprintf("%.25E,\n",rpar(rpptr(i):rpptr(i+1)-1))),70);
                ""]
            end
        end
        txt=[txt;
        "};"
        ""]
    else
        txt($+1)="static double RPAR1[1];";
    end
    //***********************//

    //*** Integer parameters ***//
    if size(ipar,1) <> 0 then
        txt=[txt;
        "/* def integer parameters */"
        "static int IPAR1[ ] = {"];

        for i=1:(length(ipptr)-1)
            if ipptr(i+1)-ipptr(i)>0  then
                if size(corinv(i),"*")==1 then
                    OO=scs_m.objs(corinv(i));
                else
                    path=list("objs");
                    for l=cpr.corinv(i)(1:$-1)
                        path($+1)=l
                        path($+1)="model"
                        path($+1)="rpar"
                        path($+1)="objs"
                    end
                    path($+1)=cpr.corinv(i)($);
                    OO=scs_m(path);
                end
                //** Add comments **//
                txt($+1)="/* Routine name of block: "+strcat(string(cpr.sim.funs(i)));
                txt($+1)=" * Gui name of block: "+strcat(string(OO.gui));
                txt($+1)=" * Compiled structure index: "+strcat(string(i));
                if stripblanks(OO.model.label)~=emptystr() then
                    txt=[txt;cformatline(" * Label: "+strcat(string(OO.model.label)),70)];
                end
                if stripblanks(OO.graphics.exprs(1))~=emptystr() then
                    txt=[txt;
                    cformatline(" * Exprs: "+strcat(OO.graphics.exprs(1),","),70)];
                end
                if stripblanks(OO.graphics.id)~=emptystr() then
                    txt=[txt;
                    cformatline(" * Identification: "+strcat(string(OO.graphics.id)),70)];
                end
                txt=[txt;
                cformatline(" * ipar= {"+strcat(string(ipar(ipptr(i):ipptr(i+1)-1)),",")+"};",70)];
                txt($+1)=" */";
                //******************//
                txt=[txt;cformatline(strcat(string(ipar(ipptr(i):ipptr(i+1)-1))+","),70)];
                txt($+1)="";
            end
        end
        txt=[txt;
        "};"
        ""]
    else
        txt($+1)="static int IPAR1[1];";
    end
    //**************************//

    //Alan added opar (27/06/07)
    //*** Object parameters ***//
    if size(opar)<>0 then
        txt=[txt;
        "/* def object parameters */"]
        for i=1:(length(opptr)-1)
            if opptr(i+1)-opptr(i)>0  then
                if size(corinv(i),"*")==1 then
                    OO=scs_m.objs(corinv(i));
                else
                    path=list("objs");
                    for l=cpr.corinv(i)(1:$-1)
                        path($+1)=l;
                        path($+1)="model";
                        path($+1)="rpar";
                        path($+1)="objs";
                    end
                    path($+1)=cpr.corinv(i)($);
                    OO=scs_m(path);
                end
                //** Add comments **//
                txt($+1)="";
                txt($+1)="/* Routine name of block: "+strcat(string(cpr.sim.funs(i)));
                txt($+1)=" * Gui name of block: "+strcat(string(OO.gui));
                txt($+1)=" * Compiled structure index: "+strcat(string(i));
                if stripblanks(OO.model.label)~=emptystr() then
                    txt=[txt;cformatline(" * Label: "+strcat(string(OO.model.label)),70)];
                end
                if stripblanks(OO.graphics.id)~=emptystr() then
                    txt=[txt;
                    cformatline(" * Identification: "+strcat(string(OO.graphics.id)),70)];
                end
                txt($+1)=" */";
                //******************//
                for j=1:opptr(i+1)-opptr(i)
                    txt =[txt;
                    cformatline("static "+mat2c_typ(opar(opptr(i)+j-1)) +...
                    " OPAR_"+string(opptr(i)+j-1) + "[] = {"+...
                    strcat(string(opar(opptr(i)+j-1)),",")+"};",70)]
                end
            end
        end
    end
    //*************************//

    txt=[txt;
    ""]
    format(oldFormat(2), oldFormat(1));
endfunction

//utilitary fonction used to format long C instruction
//t : a string containing a C instruction
//l : max line length allowed

//Author : Rachid Djenidi
function t1=cformatline(t ,l)
    // Force format to avoid precision loss
    oldFormat = format();
    format(25);
    sep=[",","+"]
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
            t1=[t1;bl+t]
            break
        end
        k=strindex(t,sep);
        if k==[] then
            t1=[t1;bl+t]
            break
        end
        k($+1)=length(t)+1 // positions of the commas
        i=find(k(1:$-1)<=l&k(2:$)>l) //nearest left comma (reltively to l)
        if i==[] then
            i=1
        end
        t1=[t1;bl+part(t,1:k(i))]
        t=part(t,k(i)+1:length(t))
        if first then
            l1=l1-2;bl=bl+"  ";
            first=%f;
        end
    end

    format(oldFormat(2), oldFormat(1));
endfunction

//
// Generate Matrix of all binary code from
// 1 to 2^n-1
//
function vec = binaryTable(n)
    n = int(n);
    vec = [];
    for i = 1:n
        accu = [];
        for j = 1:2^(n-i-1)
            accu = [accu ; zeros(2^(i-1),1) ; ones(2^(i-1),1)]
        end
        vec = [accu, vec]
    end
    vec=vec(2:$, :); // Remove first line [ 0 --- 0 ]
endfunction

//used in do_compile_superblock
function vec=codebinaire(v,szclkIN)
    vec=zeros(1,szclkIN)
    for i=1:szclkIN
        w=v/2;
        vec(i)=v-2*int(w);
        v=int(w);
    end
endfunction


function t=filetype(m)
    m=int32(m)
    filetypes=["Directory","Character device","Block device",...
    "Regular file","FIFO","Symbolic link","Socket"]
    bits=[16384,8192,24576,32768,4096,40960,49152]
    m=int32(m)&int32(61440)
    t=filetypes(find(m==int32(bits)))
endfunction

//get_blank : return blanks with a length
//            of the given input string
//
//input : str : a string
//
//output : txt : blanks
//
//16/06/07 Author : A.Layec
function [txt] = get_blank(str)
    txt="";
    for i=1:length(str)
        txt=txt+" ";
    end
endfunction

// get_comment : return a C comment
//               for generated code
//
//input : typ : a string
//        param : a list
//
//output : a C comment
//
//16/06/07 Author : A.Layec
function [txt]=get_comment(typ,param)
    txt = [];
    select typ
        //** main flag
    case "flag" then
        select param(1)
        case 0 then
            txt = "/* Continuous state computation */"
        case 1 then
            txt = "/* Output computation */"
        case 2 then
            txt = "/* Discrete state computation */"
        case 3 then
            txt = "/* Output Event computation */"
        case 4 then
            txt = "/* Initialization */"
        case 5 then
            txt = "/* Ending */"
        case 9 then
            txt = "/* Update zero crossing surfaces */"
        end
        //** blocks activated on event number
    case "ev" then
        txt = "/* Blocks activated on the event number "+string(param(1))+" */"

        //** blk calling sequence
    case "call_blk" then
        txt = ["/* Call of ''"+param(1) + ...
        "'' (type "+string(param(2))+" - blk nb "+...
        string(param(3))+" - uid "+param(4)];
        if ztyp(param(3)) then
            txt=txt+" - with zcross) */";
        else
            txt=txt+") */";
        end
        //** proto calling sequence
    case "proto_blk" then
        txt = ["/* prototype of ''"+param(1) + ...
        "'' (type "+string(param(2))];
        if ztyp(param(3)) then
            txt=txt+" - with zcross) */";
        else
            txt=txt+") */";
        end
        //** ifthenelse calling sequence
    case "ifthenelse_blk" then
        txt = ["/* Call of ''if-then-else'' blk (blk nb "+...
        string(param(1))+") */"]
        //** eventselect calling sequence
    case "evtselect_blk" then
        txt = ["/* Call of ''event-select'' blk (blk nb "+...
        string(param(1))+") */"]
        //** set block structure
    case "set_blk" then
        txt = ["/* set blk struc. of ''"+param(1) + ...
        "'' (type "+string(param(2))+" - blk nb "+...
        string(param(3))+" - uid "+param(4)+") */"];
        //** Update xd vector ptr
    case "update_xd" then
        txt = ["/* Update xd vector ptr */"];
        //** Update g vector ptr
    case "update_g" then
        txt = ["/* Update g vector ptr */"];
    else
        break;
    end
endfunction

//mat2c_typ : matrix to C type
//sci2c_ttyp : get the C string of a scicos type
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//
//16/06/07 Author : A.Layec
function [txt]=mat2c_typ(outtb)
    select type(outtb)
        //real matrix
    case 1 then
        if isreal(outtb) then
            txt = "double"
        else
            txt = "double"
        end
        //integer matrix
    case 8 then
        select typeof(outtb)
        case "int32" then
            txt = "long"
        case "int16" then
            txt = "short"
        case "int8" then
            txt = "char"
        case "uint32" then
            txt = "unsigned long"
        case "uint16" then
            txt = "unsigned short"
        case "uint8" then
            txt = "unsigned char"
        end
    else
        break;
    end
endfunction

//mat2scs_c_nb  matrix to scicos C number (sci2sci_n)
//
//input : outtb : a matrix
//
//output : c_nb : the scicos C number
//
//16/06/07 Author : A.Layec
function [c_nb]=mat2scs_c_nb(outtb)
    select type(outtb)
        //real matrix
    case 1 then
        if isreal(outtb) then
            c_nb = 10
        else
            c_nb = 11
        end
        //integer matrix
    case 8 then
        select typeof(outtb)
        case "int32" then
            c_nb = 84
        case "int16" then
            c_nb = 82
        case "int8" then
            c_nb = 81
        case "uint32" then
            c_nb = 814
        case "uint16" then
            c_nb = 812
        case "uint8" then
            c_nb = 811
        end
    else
        break;
    end
endfunction

//mat2scs_c_ptr matrix to scicos C ptr (sci2c_typ)
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//
//16/06/07 Author : A.Layec
function [txt]=mat2scs_c_ptr(outtb)
    select type(outtb)
        //real matrix
    case 1 then
        if isreal(outtb) then
            txt = "SCSREAL_COP"
        else
            txt = "SCSCOMPLEX_COP"
        end
        //integer matrix
    case 8 then
        select typeof(outtb)
        case "int32" then
            txt = "SCSINT32_COP"
        case "int16" then
            txt = "SCSINT16_COP"
        case "int8" then
            txt = "SCSINT8_COP"
        case "uint32" then
            txt = "SCSUINT32_COP"
        case "uint16" then
            txt = "SCSUINT16_COP"
        case "uint8" then
            txt = "SCSUINT8_COP"
        end
    else
        break;
    end
endfunction

//mat2scs_c_typ matrix to scicos C type
//
//input : outtb : a matrix
//
//output : txt : the string of the C scicos type
//               of the data of outtb
//

//16/06/07 Author : A.Layec
function [txt]=mat2scs_c_typ(outtb)

    select type(outtb)
        //real matrix
    case 1 then
        if isreal(outtb) then
            txt = "SCSREAL_N"
        else
            txt = "SCSCOMPLEX_N"
        end
        //integer matrix
    case 8 then
        select typeof(outtb)
        case "int32" then
            txt = "SCSINT32_N"
        case "int16" then
            txt = "SCSINT16_N"
        case "int8" then
            txt = "SCSINT8_N"
        case "uint32" then
            txt = "SCSUINT32_N"
        case "uint16" then
            txt = "SCSUINT16_N"
        case "uint8" then
            txt = "SCSUINT8_N"
        end
    else
        break;
    end
endfunction

//scs_c_n2c_fmt : scicos C number to C format
//
//input : c_nb : a C scicos type
//
//output : txt : the string of the C format string
//               of the data of outtb
//
//16/06/07 Author : A.Layec
function [txt]=scs_c_n2c_fmt(c_nb)
    select c_nb
        //real matrix
    case 10 then
        txt = "%f";
        //complex matrix
    case 11 then
        txt = "%f,%f";
        //int8 matrix
    case 81 then
        txt = "%d";
        //int16 matrix
    case 82 then
        txt = "%d";
        //int32 matrix
    case 84 then
        txt = "%d";
        //uint8 matrix
    case 811 then
        txt = "%d";
        //uint16 matrix
    case 812 then
        txt = "%d";
        //uint32 matrix
    case 814 then
        txt = "%d";
    else
        txt="%f"
        break;
    end
endfunction

//scs_c_n2c_typ scicos C number to C type
//
//input : c_nb : a C scicos number
//
//output : txt : the string of the C format string
//               of the data of outtb
//
//16/06/07 Author : A.Layec
function [txt]=scs_c_n2c_typ(c_nb)
    select c_nb
        //real matrix
    case 10 then
        txt = "double";
        //complex matrix
    case 11 then
        txt = "double";
        //int8 matrix
    case 81 then
        txt = "char";

        //int16 matrix
    case 82 then
        txt = "short";
        //int32 matrix
    case 84 then
        txt = "long";
        //uint8 matrix
    case 811 then
        txt = "unsigned char";
        //uint16 matrix
    case 812 then

        txt = "unsigned short";
        //uint32 matrix
    case 814 then
        txt = "unsigned long";
    else
        txt="double"
        break;
    end
endfunction

//scs_c_nb2scs_nb : scicos C number to scicos number
//
//input : c_nb  : the scicos C number type
//
//output : scs_nb : the scilab number type
//
//16/06/07 Author : A.Layec
function [scs_nb]=scs_c_nb2scs_nb(c_nb)
    scs_nb=zeros(size(c_nb,1),size(c_nb,2));
    for i=1:size(c_nb,1)
        for j=1:size(c_nb,2)
            select (c_nb(i,j))
            case 10 then
                scs_nb(i,j) = 1
            case 11 then
                scs_nb(i,j) = 2
            case 81 then
                scs_nb(i,j) = 5
            case 82 then
                scs_nb(i,j) = 4
            case 84 then
                scs_nb(i,j) = 3
            case 811 then
                scs_nb(i,j) = 8
            case 812 then
                scs_nb(i,j) = 7
            case 814 then
                scs_nb(i,j) = 6
            else
                scs_nb(i,j) = 1
            end
        end
    end
endfunction

//used in do_compile_superblock
function XX=update_block(XX)
    execstr("o="+rdnom+"_c(''define'')")
    XX.model=o.model
    XX.gui=rdnom+"_c";
    XX.graphics.gr_i=o.graphics.gr_i
endfunction

//write_code_cdoit : generate body of the code for
//                   for all time dependant blocks
//
//input : flag : flag number for block's call
//
//output : txt for cord blocks
//
//12/07/07 Alan Layec
function [txt]=write_code_cdoit(flag)
    txt=[];

    for j=1:ncord
        bk=cord(j,1);
        pt=cord(j,2);
        //** blk
        if funtyp(bk)>-1 then
            if or(bk==act) | or(bk==cap) then
                if stalone then
                    txt2=call_block42(bk,pt,flag);
                    if txt2<>[] then
                        txt=[txt;
                        "    "+txt2
                        ""];
                    end
                end
            else
                txt2=call_block42(bk,pt,flag);
                if txt2<>[] then
                    txt=[txt;
                    "    "+txt2
                    ""];
                end
            end
            //** ifthenelse blk
        elseif funtyp(bk)==-1 then
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            thentxt=write_code_doit(clkptr(bk),flag);
            elsetxt=write_code_doit(clkptr(bk)+1,flag);
            if thentxt<>[] | elsetxt<>[] then
                txt=[txt;
                "    "+get_comment("ifthenelse_blk",list(bk));]
                //** C **//
                tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    if("+tmp_+">0) {"]
                //*******//
                txt=[txt;
                Indent+thentxt];
                if elsetxt<>[] then
                    //** C **//
                    txt=[txt;
                    "    }";
                    "    else {";]
                    //*******//
                    txt=[txt;
                    Indent+elsetxt];
                end
                //** C **//
                txt=[txt;
                "    }"]
                //*******//
            end
            //** eventselect blk
        elseif funtyp(bk)==-2 then
            Noutport=clkptr(bk+1)-clkptr(bk);
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            II=[];
            switchtxt=list()
            for i=1: Noutport
                switchtxt(i)=write_code_doit(clkptr(bk)+i-1,flag);
                if switchtxt(i)<>[] then II=[II i];end
            end
            if II<>[] then
                txt=[txt;
                "    "+get_comment("evtselect_blk",list(bk));]
                //** C **//
                tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    i=max(min((int) "+...
                tmp_+",block_"+rdnom+"["+string(bk-1)+"].evout),1);"
                "    switch(i)"
                "    {"]
                //*******//
                for i=II
                    //** C **//
                    txt=[txt;
                    "     case "+string(i)+" :";]
                    //*******//
                    txt=[txt;
                    BigIndent+write_code_doit(clkptr(bk)+i-1,flag);]
                    //** C **//
                    txt=[txt;
                    BigIndent+"break;"]
                    //*******//
                end
                //** C **//
                txt=[txt;
                "    }"];
                //*******//
            end
            //** Unknown block
        else
            error("Unknown block type "+string(bk));
        end
    end

endfunction

//write_code_doit : generate body of the code for
//                  ordering calls of blocks during
//                  flag 1,2 & flag 3
//


//input : ev  : evt number for block's call
//       flag : flag number for block's call

//
//output : txt for flag 1 or 2, or flag 3
//

//12/07/07 Alan Layec
function [txt]=write_code_doit(ev,flag)
    txt=[];

    for j=ordptr(ev):ordptr(ev+1)-1
        bk=ordclk(j,1);
        pt=ordclk(j,2);
        //** blk
        if funtyp(bk)>-1 then
            if or(bk==act) | or(bk==cap) then
                if stalone then
                    txt2=call_block42(bk,pt,flag);
                    if txt2<>[] then
                        txt=[txt;
                        "    "+txt2
                        ""];
                    end
                end
            else
                txt2=call_block42(bk,pt,flag);
                if txt2<>[] then
                    txt=[txt;
                    "    "+txt2
                    ""];
                end
            end
            //** ifthenelse blk
        elseif funtyp(bk)==-1 then
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            thentxt=write_code_doit(clkptr(bk),flag);
            elsetxt=write_code_doit(clkptr(bk)+1,flag);
            if thentxt<>[] | elsetxt<>[] then
                txt=[txt;
                "    "+get_comment("ifthenelse_blk",list(bk));]
                //** C **//
                tmp_ = "*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    if("+tmp_+">0) {"]
                //*******//
                txt=[txt;
                Indent+thentxt]
                if elsetxt<>[] then
                    //** C **//
                    txt=[txt;
                    "    }";
                    "    else {";]
                    //*******//
                    txt=[txt;
                    Indent+elsetxt];
                end
                //** C **//
                txt=[txt;
                "    }"]
                //*******//
            end
            //** eventselect blk
        elseif funtyp(bk)==-2 then
            Noutport=clkptr(bk+1)-clkptr(bk);
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            II=[];
            switchtxt=list()
            for i=1: Noutport
                switchtxt(i)=write_code_doit(clkptr(bk)+i-1,flag);
                if switchtxt(i)<>[] then II=[II i];end
            end
            if II<>[] then
                txt=[txt;
                "    "+get_comment("evtselect_blk",list(bk));]
                tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                //** C **//
                txt=[txt;
                "    i=max(min((int) "+...
                tmp_+",block_"+rdnom+"["+string(bk-1)+"].evout),1);"
                "    switch(i)"
                "    {"]
                //*******//
                for i=II
                    //** C **//
                    txt=[txt;
                    "     case "+string(i)+" :";]
                    //*******//
                    txt=[txt;
                    BigIndent+write_code_doit(clkptr(bk)+i-1,flag);]
                    //** C **//
                    txt=[txt;
                    BigIndent+"break;"]
                    //*******//
                end
                //** C **//
                txt=[txt;
                "    }"]
                //*******//
            end
            //** Unknown block
        else
            error("Unknown block type "+string(bk));
        end
    end

endfunction

//write_code_idoit : generate body of the code for
//                   ordering calls of initial
//                   called blocks
//
//input : nothing (blocks are called with flag 1)
//
//output : txt for iord
//
//15/07/07 Alan Layec
function [txt]=write_code_idoit()
    txt=[];

    for j=1:niord
        bk=iord(j,1);
        pt=iord(j,2);
        //** blk
        if funtyp(bk)>-1 then
            if or(bk==act) then
                if stalone then
                    txt2=call_block42(bk,pt,1);
                    if txt2<>[] then
                        txt=[txt;
                        "    "+txt2
                        ""];
                    end
                end
            else
                txt2=call_block42(bk,pt,1);
                if txt2<>[] then
                    txt=[txt;
                    "    "+txt2
                    ""];
                end
            end
            //** ifthenelse blk
        elseif funtyp(bk)==-1 then
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            thentxt=write_code_doit(clkptr(bk),1);
            elsetxt=write_code_doit(clkptr(bk)+1,1);
            if thentxt<>[] | elsetxt<>[] then
                txt=[txt;
                "    "+get_comment("ifthenelse_blk",list(bk));]
                //** C **//
                tmp_ = "*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    if("+tmp_+">0) {"]
                //*******//
                txt=[txt;
                Indent+thentxt];
                if elsetxt<>[] then
                    //** C **//
                    txt=[txt;
                    "    }";
                    "    else {";]
                    //*******//
                    txt=[txt;
                    Indent+elsetxt];
                end
                //** C **//
                txt=[txt;
                "    }"]
                //*******//
            end
            //** eventselect blk
        elseif funtyp(bk)==-2 then
            Noutport=clkptr(bk+1)-clkptr(bk);
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            II=[];
            switchtxt=list()
            for i=1: Noutport
                switchtxt(i)=write_code_doit(clkptr(bk)+i-1,1);
                if switchtxt(i)<>[] then II=[II i];end
            end
            if II<>[] then
                txt=[txt;
                "    "+get_comment("evtselect_blk",list(bk));]
                //** C **//
                tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    i=max(min((int) "+...
                tmp_+",block_"+rdnom+"["+string(bk-1)+"].evout),1);"]
                txt=[txt;
                "    switch(i)"
                "    {"]
                //*******//
                for i=II
                    //** C **//
                    txt=[txt;
                    "     case "+string(i)+" :";]
                    //*******//
                    txt=[txt;
                    BigIndent+write_code_doit(clkptr(bk)+i-1,1);]
                    //** C **//
                    txt=[txt;
                    BigIndent+"break;"]

                    //*******//
                end
                //** C **//
                txt=[txt;
                "    }"];
                //*******//
            end
            //** Unknown block
        else
            error("Unknown block type "+string(bk));
        end
    end

endfunction

//write_code_odoit : generate body of the code for
//                   ordering calls of blocks before
//                   continuous time integration
//
//input : flag : flag number for block's call
//
//output : txt for flag 0

//

//12/07/07 Alan Layec
function [txt]=write_code_odoit(flag)
    txt=[];

    for j=1:noord
        bk=oord(j,1);
        pt=oord(j,2);
        //** blk
        if funtyp(bk)>-1 then
            txt2=call_block42(bk,pt,flag);
            if txt2<>[] then
                txt=[txt;
                "    "+txt2

                ""];
            end
            //** ifthenelse blk
        elseif funtyp(bk)==-1 then
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            thentxt=write_code_ozdoit(clkptr(bk),flag);


            elsetxt=write_code_ozdoit(clkptr(bk)+1,flag);
            if thentxt<>[] | elsetxt<>[] then
                txt=[txt;
                "    "+get_comment("ifthenelse_blk",list(bk));]
                //** C **//
                tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    if ((block_"+rdnom+"["+string(bk-1)+"].nmode<0"+...
                " && "+tmp_+">0)"+...
                " || \"
                "        (block_"+rdnom+"["+string(bk-1)+"].nmode>0"+...
                " && block_"+rdnom+"["+string(bk-1)+"].mode[0]==1)) {"]
                //*******//
                txt=[txt;
                Indent+thentxt]
                //** C **//
                txt=[txt;
                "    }"];
                //*******//
                if elsetxt<>[] then
                    //** C **//
                    txt=[txt;
                    "    else if  ((block_"+rdnom+"["+string(bk-1)+"].nmode<0"+...
                    " && "+tmp_+"<=0)"+...
                    " || \"
                    "              (block_"+rdnom+"["+string(bk-1)+"].nmode>0"+...
                    " && block_"+rdnom+"["+string(bk-1)+"].mode[0]==2)) {";]
                    //*******//
                    txt=[txt;
                    Indent+elsetxt]
                    //** C **//
                    txt=[txt;
                    "    }"];
                    //*******//
                end
            end
            //** eventselect blk
        elseif funtyp(bk)==-2 then
            Noutport=clkptr(bk+1)-clkptr(bk);
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            II=[];
            switchtxt=list()
            for i=1: Noutport
                switchtxt(i)=write_code_ozdoit(clkptr(bk)+i-1,flag);
                if switchtxt(i)<>[] then II=[II i];end
            end
            if II<>[] then
                txt=[txt;
                "    "+get_comment("evtselect_blk",list(bk));]
                //** C **//
                tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    if (block_"+rdnom+"["+string(bk-1)+"].nmode<0) {";
                "      i=max(min((int) "+...
                tmp_+",block_"+rdnom+"["+string(bk-1)+"].evout),1);"
                "    }"
                "    else {"
                "      i=block_"+rdnom+"["+string(bk-1)+"].mode[0];"
                "    }"]
                txt=[txt;
                "    switch(i)"
                "    {"];
                //*******//
                for i=II
                    //** C **//
                    txt=[txt;
                    "     case "+string(i)+" :";]
                    //*******//
                    txt=[txt;
                    BigIndent+write_code_ozdoit(clkptr(bk)+i-1,flag);]
                    //** C **//
                    txt=[txt;
                    BigIndent+"break;"]
                    //*******//
                end
                //** C **//
                txt=[txt;
                "    }"];
                //*******//
            end
            //** Unknown block
        else
            error("Unknown block type "+string(bk));
        end
    end

endfunction

//write_code_ozdoit : generate body of the code for both
//                    flag 0 & flag 9
//
//input: ev  : evt number for block's call
//      flag : flag number for block's call
//
//output : txt for flag 0 or flag 9
//
//12/07/07 Alan Layec
function [txt]=write_code_ozdoit(ev,flag)
    txt=[];

    for j=ordptr(ev):ordptr(ev+1)-1
        bk=ordclk(j,1);
        pt=ordclk(j,2);
        //** blk
        if funtyp(bk)>-1 then
            if (or(bk==act) | or(bk==cap)) & (flag==1) then
                if stalone then
                    txt=[txt;
                    indentLines("    ", call_block42(bk,pt,flag))
                    ""];
                end
            else
                txt2=call_block42(bk,pt,flag);
                if txt2<>[] then
                    txt=[txt;
                    "    "+txt2
                    ""];
                end
            end
            //** ifthenelse blk
        elseif funtyp(bk)==-1 then
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            thentxt=write_code_ozdoit(clkptr(bk),flag);
            elsetxt=write_code_ozdoit(clkptr(bk)+1,flag);
            if thentxt<>[] | elsetxt<>[] then
                txt=[txt;
                "    "+get_comment("ifthenelse_blk",list(bk));]
                //** C **//
                tmp_ = "*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    if (((phase==1"+...
                " || block_"+rdnom+"["+string(bk-1)+"].nmode==0)"+...
                " && "+tmp_+">0)"+...
                " || \"
                "        ((phase!=1"+...
                " && block_"+rdnom+"["+string(bk-1)+"].nmode!=0)"+...
                " && block_"+rdnom+"["+string(bk-1)+"].mode[0]==1)) {"]
                //*******//
                txt=[txt;
                Indent+thentxt]
                //** C **//
                txt=[txt;
                "    }"];
                //*******//
                if elsetxt<>[] then
                    //** C **//
                    txt=[txt;
                    "      else if (((phase==1"+...
                    " || block_"+rdnom+"["+string(bk-1)+"].nmode==0)"+...
                    " && "+tmp_+"<=0)"+...
                    " || \"
                    "               ((phase!=1"+...
                    " && block_"+rdnom+"["+string(bk-1)+"].nmode!=0)"+...
                    " && block_"+rdnom+"["+string(bk-1)+"].mode[0]==2)) {";]
                    //*******//
                    txt=[txt;
                    Indent+elsetxt]
                    //** C **//
                    txt=[txt;
                    "    }"];
                    //*******//
                end
            end
            //** eventselect blk
        elseif funtyp(bk)==-2 then
            Noutport=clkptr(bk+1)-clkptr(bk);
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            II=[];
            switchtxt=list()
            for i=1: Noutport
                switchtxt(i)=write_code_ozdoit(clkptr(bk)+i-1,flag);
                if switchtxt(i)<>[] then II=[II i];end
            end
            if II<>[] then
                txt=[txt;
                "    "+get_comment("evtselect_blk",list(bk));]
                //** C **//
                tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    if (phase==1 || block_"+rdnom+"["+string(bk-1)+"].nmode==0) {";
                "      i=max(min((int) "+...
                tmp_+",block_"+rdnom+"["+string(bk-1)+"].evout),1);"
                "    }"
                "    else {"
                "      i=block_"+rdnom+"["+string(bk-1)+"].mode[0];"
                "    }"]
                txt=[txt;
                "    switch(i)"
                "    {"];
                //*******//
                for i=II
                    //** C **//
                    txt=[txt;
                    "     case "+string(i)+" :";]
                    //*******//
                    txt=[txt;
                    BigIndent+write_code_ozdoit(clkptr(bk)+i-1,flag);]
                    //** C **//
                    txt=[txt;
                    BigIndent+"break;"]
                    //*******//
                end
                //** C **//
                txt=[txt;
                "    }"];
                //*******//
            end
            //** Unknown block
        else
            error("Unknown block type "+string(bk));
        end
    end

endfunction

//write_code_zdoit : generate body of the code for
//                   ordering calls of blocks before
//                   continuous time zero crossing

//                   detection
//
//input : noting
//
//output : txt for flag 9
//
//12/07/07 Alan Layec

function [txt]=write_code_zdoit()
    txt=[];

    //** first pass (flag 1)
    for j=1:nzord
        bk=zord(j,1);
        pt=zord(j,2);
        //** blk
        if funtyp(bk)>-1 then
            if or(bk==act) | or(bk==cap) then
                if stalone then
                    txt=[txt;
                    indentLines("    ", call_block42(bk,pt,1))
                    ""];
                end
            else
                txt2=call_block42(bk,pt,1);
                if txt2<>[] then
                    txt=[txt;
                    "    "+txt2
                    ""];
                end
            end
            //** ifthenelse blk
        elseif funtyp(bk)==-1 then
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            thentxt=write_code_ozdoit(clkptr(bk),1);
            elsetxt=write_code_ozdoit(clkptr(bk)+1,1);
            if thentxt<>[] | elsetxt<>[] then
                txt=[txt;
                "    "+get_comment("ifthenelse_blk",list(bk));]
                //** C **//
                tmp_ = "*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    if (((phase==1"+...
                " || block_"+rdnom+"["+string(bk-1)+"].nmode==0)"+...
                " && "+tmp_+">0)"+...
                " || \"
                "        ((phase!=1"+...
                " && block_"+rdnom+"["+string(bk-1)+"].nmode!=0)"+...
                " && block_"+rdnom+"["+string(bk-1)+"].mode[0]==1)) {"]
                //*******//
                txt=[txt;
                Indent+thentxt]
                //** C **//
                txt=[txt;
                "    }"];
                //*******//
                if elsetxt<>[] then
                    //** C **//
                    txt=[txt;
                    "      else if (((phase==1"+...
                    " || block_"+rdnom+"["+string(bk-1)+"].nmode==0)"+...
                    " && "+tmp_+"<=0)"+...
                    " || \"
                    "               ((phase!=1"+...
                    " && block_"+rdnom+"["+string(bk-1)+"].nmode!=0)"+...
                    " && block_"+rdnom+"["+string(bk-1)+"].mode[0]==2)) {";]
                    //*******//
                    txt=[txt;
                    Indent+elsetxt]
                    //** C **//
                    txt=[txt;
                    "    }"];
                    //*******//
                end
            end
            //** eventselect blk
        elseif funtyp(bk)==-2 then
            Noutport=clkptr(bk+1)-clkptr(bk);
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            II=[];
            switchtxt=list()
            for i=1: Noutport
                switchtxt(i)=write_code_ozdoit(clkptr(bk)+i-1,1);
                if switchtxt(i)<>[] then II=[II i];end
            end
            if II<>[] then
                txt=[txt;
                "    "+get_comment("evtselect_blk",list(bk));]
                //** C **//
                tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
                txt=[txt;
                "    if (phase==1 || block_"+rdnom+"["+string(bk-1)+"].nmode==0){";
                "      i=max(min((int) "+...
                tmp_+",block_"+rdnom+"["+string(bk-1)+"].evout),1);"
                "    else {"
                "      i=block_"+rdnom+"["+string(bk-1)+"].mode[0];"
                "    }"]
                txt=[txt;
                "      switch(i)"
                "      {"];
                //*******//
                for i=II
                    //** C **//
                    txt=[txt;
                    "       case "+string(i)+" :";]
                    //*******//
                    txt=[txt;
                    BigIndent+write_code_ozdoit(clkptr(bk)+i-1,1);]
                    //** C **//
                    txt=[txt;
                    BigIndent+"break;"]
                    //*******//
                end
                //** C **//
                txt=[txt;
                "      }"];
                //*******//
            end
            //** Unknown block
        else
            error("Unknown block type "+string(bk));
        end
    end

    //** second pass (flag 9)
    for j=1:nzord
        bk=zord(j,1);
        pt=zord(j,2);
        //** blk
        if funtyp(bk)>-1 then
            if or(bk==act) | or(bk==cap) then
                if stalone then
                    txt=[txt;
                    indentLines("    ", call_block42(bk,pt,9))
                    ""];
                end
            else
                txt2=call_block42(bk,pt,9);
                if txt2<>[] then
                    txt=[txt;
                    "    "+txt2
                    ""];
                end
            end

            //** ifthenelse blk
        elseif funtyp(bk)==-1 then
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            //** C **//
            tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
            //*******//
            thentxt=write_code_zzdoit(clkptr(bk),9);
            elsetxt=write_code_zzdoit(clkptr(bk)+1,9);
            txt=[txt;
            "    "+get_comment("ifthenelse_blk",list(bk));]
            //** C **//
            txt=[txt;
            "    g["+string(zcptr(bk)-1)+"]=(double)"+tmp_+";"]
            //*******//
            if thentxt<>[] | elsetxt<>[] then
                //** C **//
                txt=[txt;
                "    if (g["+string(zcptr(bk)-1)+"] > 0.){"]
                //*******//
                txt=[txt;
                Indent+thentxt]
                //** C **//
                txt=[txt;
                "      }"]
                //*******//
                if elsetxt <> [] then
                    //** C **//
                    txt=[txt;
                    "      else {"]

                    //*******//
                    txt=[txt;
                    Indent+elsetxt]
                    //** C **//
                    txt=[txt;
                    "      }"]
                    //*******//
                end
            end
            //** C **//
            txt=[txt;
            "    if(phase==1 && block_"+rdnom+"["+string(bk-1)+"].nmode > 0){"
            "      if (g["+string(zcptr(bk)-1)+"] > 0.){"
            "        block_"+rdnom+"["+string(bk-1)+"].mode[0] = 1;"
            "      }"
            "      else {"
            "        block_"+rdnom+"["+string(bk-1)+"].mode[0] = 2;"
            "      }"
            "    }"]
            //*******//
            //** eventselect blk
        elseif funtyp(bk)==-2 then
            Noutport=clkptr(bk+1)-clkptr(bk);
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1));  //** scilab index start from 1
            //** C **//
            tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
            //*******//
            II=[];
            switchtxt=list()
            for i=1:Noutport
                switchtxt(i)=write_code_zzdoit(clkptr(bk)+i-1,9);
                if switchtxt(i)<>[] then II=[II i];end
            end
            txt=[txt;
            "    "+get_comment("evtselect_blk",list(bk));]
            if II<>[] then
                //** C **//
                txt=[txt;

                "    j=max(min((int) "+...
                tmp_+",block_"+rdnom+"["+string(bk-1)+"].nevout),1);"]
                txt=[txt;
                "    switch(j)"
                "    {"];
                //*******//
                for i=II
                    //** C **//
                    txt=[txt;
                    "     case "+string(j)+" :";]
                    //*******//
                    txt=[txt;
                    BigIndent+write_code_zzdoit(clkptr(bk)+i-1,9);]

                    //** C **//
                    txt=[txt;
                    BigIndent+"break;"]
                    //*******//

                end
                //** C **//
                txt=[txt;
                "    }"];
                //*******//
            end
            //** C **//
            txt=[txt;
            "  for (jj=0;jj<block_"+rdnom+"["+string(fun-1)+"].nevout-1;++jj) {"
            "    g["+string(zcptr(bk)-1)+"+jj]=(double)"+tmp_+"-(double)(jj+2);"
            "  }"
            "  if(phase==1 && block_"+rdnom+"["+string(bk-1)+"].nmode>0){"
            "    j=max(min((int) "+tmp_+","
            "              block_"+rdnom+"["+string(bk-1)+"].nevout),1);"
            "    block_"+rdnom+"["+string(bk-1)+"].mode[0]= j;"
            "  }"]
            //*******//
            //** Unknown block
        else
            error("Unknown block type "+string(bk));
        end
    end

endfunction

//write_code_zzdoit : generate body of the code for
//                    flag 9
//
//input: ev  : evt number for block's call
//      flag : flag number for block's call
//
//output : txt for flag 9
//
//12/07/07 Alan Layec
function [txt]=write_code_zzdoit(ev,flag)
    txt=[];

    for j=ordptr(ev):ordptr(ev+1)-1
        bk=ordclk(j,1);
        pt=ordclk(j,2);
        //** blk
        if funtyp(bk)>-1 then
            if or(bk==act) | or(bk==cap) then
                if stalone then
                    txt=[txt;
                    indentLines("    ", call_block42(bk,pt,flag))
                    ""];
                end
            else
                txt2=call_block42(bk,pt,flag);
                if txt2<>[] then
                    txt=[txt;
                    "    "+txt2
                    ""];
                end
            end
            //** ifthenelse blk
        elseif funtyp(bk)==-1 then

            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
            //** C **//
            tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
            //*******//
            thentxt=write_code_zzdoit(clkptr(bk),9);
            elsetxt=write_code_zzdoit(clkptr(bk)+1,9);
            txt=[txt;
            "    "+get_comment("ifthenelse_blk",list(bk));]
            //** C **//
            txt=[txt;
            "    g["+string(zcptr(bk)-1)+"]=(double)"+tmp_+";"]
            //*******//
            if thentxt<>[] | elsetxt<>[] then
                //** C **//
                txt=[txt;
                "    if (g["+string(zcptr(bk)-1)+"] > 0.){"]
                //*******//
                txt=[txt;
                Indent+thentxt]
                //** C **//
                txt=[txt;
                "      }"]
                //*******//
                if elsetxt <> [] then
                    //** C **//
                    txt=[txt;
                    "      else {"]
                    //*******//
                    txt=[txt;
                    Indent+elsetxt]
                    //** C **//
                    txt=[txt;
                    "      }"]
                    //*******//
                end
            end
            //** C **//
            txt=[txt;
            "    if(phase==1 && block_"+rdnom+"["+string(bk-1)+"].nmode > 0){"
            "      if (g["+string(zcptr(bk)-1)+"] > 0.){"
            "        block_"+rdnom+"["+string(bk-1)+"].mode[0] = 1;"
            "      }"
            "      else {"
            "        block_"+rdnom+"["+string(bk-1)+"].mode[0] = 2;"
            "      }"

            "    }"]
            //*******//
            //** eventselect blk
        elseif funtyp(bk)==-2 then
            Noutport=clkptr(bk+1)-clkptr(bk);
            ix=-1+inplnk(inpptr(bk));
            TYPE=mat2c_typ(outtb(ix+1));  //** scilab index start from 1
            //** C **//
            tmp_="*(("+TYPE+" *)"+rdnom+"_block_outtbptr["+string(ix)+"])"
            //*******//
            II=[];
            switchtxt=list()
            for i=1:Noutport
                switchtxt(i)=write_code_zzdoit(clkptr(bk)+i-1,9);
                if switchtxt(i)<>[] then II=[II i];end
            end
            txt=[txt;
            "    "+get_comment("evtselect_blk",list(bk));]
            if II<>[] then
                //** C **//
                txt=[txt;
                "    j=max(min((int) "+...
                tmp_+",block_"+rdnom+"["+string(bk-1)+"].nevout),1);"]
                txt=[txt;
                "    switch(j)"
                "    {"];
                //*******//
                for i=II
                    //** C **//
                    txt=[txt;
                    "     case "+string(j)+" :";]
                    //*******//
                    txt=[txt;
                    BigIndent+write_code_zzdoit(clkptr(bk)+i-1,9);]
                    //** C **//
                    txt=[txt;
                    BigIndent+"break;"]
                    //*******//
                end
                //** C **//
                txt=[txt;
                "    }"];
                //*******//
            end
            //** C **//
            txt=[txt;
            "  for (jj=0;jj<block_"+rdnom+"["+string(fun-1)+"].nevout-1;++jj) {"
            "    g["+string(zcptr(bk)-1)+"+jj]=(double)"+tmp_+"-(double)(jj+2);"
            "  }"
            "  if(phase==1 && block_"+rdnom+"["+string(bk-1)+"].nmode>0){"
            "    j=max(min((int) "+tmp_+","
            "              block_"+rdnom+"["+string(bk-1)+"].nevout),1);"
            "    block_"+rdnom+"["+string(bk-1)+"].mode[0]= j;"
            "  }"]
            //*******//
            //** Unknown block
        else
            error("Unknown block type "+string(bk));
        end
    end

endfunction

function ok = do_compile_fmu()
    ok = %t;
    // check selected block
    isexist = %t;
    if ~exists("blk") then
        isexist = %f;
    elseif typeof(blk) <> "Block" | blk.gui <> "SUPER_f" then
        isexist = %f;
    end

    if isexist then
        script = "[ok, XX] = do_compile_super_fmu(blk, [], [], %f); ";
        if exists("FMU_WRAPPER_BACKTRACE") && FMU_WRAPPER_BACKTRACE then
            ierr = 0;
            execstr(script);
        else
            ierr = execstr(script, "errcatch");
        end
        if ierr <> 0 then
            ok = %f;
            [msg, err] = lasterror();
            disp(msg);
            // push blk error
            blk = [];
            blk = resume(blk)
        end
    else
        message("Please select a superblock for the FMU generation.")
        return
    end

endfunction

// creating a binary
function ok = build_model(name, files, filepath, cflags, ldflags)
    ok = %t;
    if getos() <> "Windows" then
        if isdir(SCI+"/../../include/scilab/scicos_blocks/") then
            //binary version
            cflags = cflags + " -I" + SCI + "/../../include/scilab/scicos/";
            cflags = cflags + " -I" + SCI + "/../../include/scilab/scicos_blocks/";
            cflags = cflags + " -I" + SCI + "/../../include/scilab/dynamic_link/";
            ldflags = ldflags + SCI + "/lib/scilab/.libs/libsciscicos_blocks.so";
            ldflags = ldflags + SCI + "/lib/scilab/.libs/libsciscicos.so";
        else
            //source version
            cflags = cflags + " -I" + SCI + "/modules/scicos/includes/";
            cflags = cflags + " -I" + SCI + "/modules/scicos_blocks/includes/";
            cflags = cflags + " -I" + SCI + "/modules/dynamic_link/includes/";
            ldflags = ldflags + SCI + "/modules/scicos_blocks/.libs/libsciscicos_blocks.so";
            ldflags = ldflags + SCI + "/modules/scicos_blocks/.libs/libsciscicos.so";
        end
    else
        // Load dynamic_link Internal lib if it's not already loaded
        if ~exists("dynamic_linkwindowslib") then
            load("SCI/modules/dynamic_link/macros/windows/lib");
        end
        cflags = cflags + strcat(" -I""" + dlwGetXcosIncludes() + """");
        ldflags = ldflags + strcat(" """ + dlwGetLibrariesPath() + dlwGetXcosLibraries() + """");
    end
    libs = SCI + "/bin/scicos_blocks";
    libs($+1) = SCI +"/bin/scicos";
    libs($+1) = SCI +"/bin/scicos_blocks_f";

    cflags = cflags + " -I""" + filepath + """";
    cflags = cflags + " -I""" + TMPDIR + """";
    if getos() == "Windows" then
        cflags = cflags + " -Od"; // without optimisation
    end

    oldpwd = pwd()
    chdir(filepath);

    ierr = [];
    ierr = execstr("libn =ilib_for_link(name,files,libs,""c"","""","""",name,"""",cflags)", "errcatch");
    chdir(oldpwd);
    if ierr <> 0 then
        ok = %f;
        error(999, msprintf("Model has not been compiled: %s\n", lasterror()));
        return
    end
endfunction

// creating a FMU zip model
function ok = zip_model(fmuFileName)
    ok = %f; zip = 0;
    oldpwd = pwd();
    currentDir = rpat;
    chdir(currentDir);
    // create directory
    mkdir("sources");
    mkdir("binaries");
    movefile(currentDir+filesep()+fmuFileName,currentDir+filesep()+"sources"+filesep()+fmuFileName);
    movefile(currentDir+filesep()+"fmiModelFunctions.h",currentDir+filesep()+"sources"+filesep()+"fmiModelFunctions.h");
    movefile(currentDir+filesep()+"fmiModelTypes.h",currentDir+filesep()+"sources"+filesep()+"fmiModelTypes.h");
    if getos() == "Windows" then
        if win64() then
            mkdir("binaries\win64");
            movefile(currentDir+filesep()+"lib"+rdnom+"_fmiGetVersion.dll",...
            currentDir+"\binaries\win64\"+rdnom+".dll");
            // zip fmu
            zip = host(SCI+"\tools\zip\zip.exe "+rdnom+".fmu " +"modelDescription.xml "...
            +"sources\"+fmuFileName+" sources\fmiModelFunctions.h sources\fmiModelTypes.h binaries\win64\"+rdnom+".dll");
        else
            mkdir("binaries\win32");
            movefile(currentDir+filesep()+"lib"+rdnom+"_fmiGetVersion.dll",...
            currentDir+"\binaries\win32\"+rdnom+".dll");
            zip = host(SCI+"\tools\zip\zip.exe "+rdnom+".fmu " +"modelDescription.xml "...
            +"sources\"+fmuFileName+" sources\fmiModelFunctions.h sources\fmiModelTypes.h binaries\win32\"+rdnom+".dll");
        end
    else
        [sys,vers] = getversion();
        if vers(2) == "x64" then
            mkdir("binaries/linux64");
            movefile(currentDir+filesep()+"lib"+rdnom+"_fmiGetVersion.so",...
            currentDir+"/binaries/linux64/"+rdnom+".so");
            zip = host("zip "+rdnom+".fmu " +"modelDescription.xml "...
            +"sources/"+fmuFileName+" sources/fmiModelFunctions.h sources/fmiModelTypes.h binaries/linux64/"+rdnom+".so");
        else
            mkdir(currentDir+filesep()+"binaries/linux32");
            movefile(currentDir+filesep()+"lib"+rdnom+"_fmiGetVersion.so",...
            currentDir+"/binaries/linux32/"+rdnom+".so");
            zip = host("zip "+rdnom+".fmu " +"modelDescription.xml "...
            +"sources/"+fmuFileName+" sources/fmiModelFunctions.h sources/fmiModelTypes.h binaries/linux32/"+rdnom+".so");
        end
    end
    if zip <> 0 then
        error(999, msprintf("FMU archive file has not been created.\n"));
        chdir(oldpwd);
    else
        mdelete("modelDescription.xml");
        rmdir("sources","s");
        rmdir("binaries","s");
        exec("cleaner.sce", -1);
        mdelete("loader.sce");
        mdelete("cleaner.sce");
        chdir(oldpwd);
        ok = %t;
    end
endfunction

// Generating the XML description for the FMU
function [ok, description] = gen_fmuXML_description(funcNames)
    ok = %f;
    description = []; // modelIdentifier; guid; numberOfStates; numberOfEvents;
                      // description; author; modelVersion;
    // create xml doc
    fileXml = xmlDocument();
    // main attributes of fmu
    root = xmlElement(fileXml, "fmiModelDescription");

    root.attributes.fmiVersion = "1.0"; // only 1.0 is supported now
    root.attributes.modelName = rdnom;
    root.attributes.modelIdentifier = rdnom;

    // the same guid that for the first block
    root.attributes.guid = "{"+cpr.sim($)(1)+"}";
    root.attributes.description = descr;
    root.attributes.author = author;
    root.attributes.version = vers;

    // scilab version
    root.attributes.generationTool = getversion();
    // date and time selon the specification
    dateAndTime = string(clock());
    root.attributes.generationDateAndTime = dateAndTime(1)+"-"+dateAndTime(2)+"-"+...
    dateAndTime(3)+"T"+dateAndTime(4)+":"+dateAndTime(5)+":"+dateAndTime(6)+"Z";

    root.attributes.variableNamingConvention = "flat";
    root.attributes.numberOfContinuousStates = string(size(cpr.state.x,"*"));
    //find a number of events
    nblk = cpr.sim.nb;
    zcptr = cpr.sim.zcptr
    nevents = [];
    for i=1:nblk
        nevents(i) = zcptr(i+1)-zcptr(i);
    end
    nevents = length(find(nevents <> 0));
    root.attributes.numberOfEventIndicators = string(nevents);
    //root.attributes.numberOfEventIndicators = "0";
    // default experiment
    root.children(1) = "<DefaultExperiment> </DefaultExperiment>";
    root.children(1).attributes.startTime = "0.0";

    // all variables of fmu
    root.children(2) = "<ModelVariables> </ModelVariables>";
    // fmu Co Simulation
    if typ == "cs" then
        root.children(3) = "<Implementation> </Implementation>";
        root.children(3).children(1) = "<CoSimulation_StandAlone> </CoSimulation_StandAlone>";
        root.children(3).children(1).children(1) = "<Capabilities> </Capabilities>";
        root.children(3).children(1).children(1).attributes("canHandleVariableCommunicationStepSize") = "true";
        root.children(3).children(1).children(1).attributes.canHandleEvents = "true";
    end
    // boolean inputs
    [txtBool, txtBoolNames] = make_doit_fmu();
    // inputs/outputs
    inp = capt(:,3);
    inpTyp = scs_c_nb2scs_nb(capt(:,5));
    out = actt(:,3);
    outTyp = funcNames(11);
    // outTyp = scs_c_nb2scs_nb(actt(:,5));
    numInp = length(capt(:,3));
    numOut = length(actt(:,3));
    stateSize = length(funcNames(2));
    rparSize = length(funcNames(4));
    iparSize = length(funcNames(6));
    oparSize = size(funcNames(7),"*");
    boolSize = size(txtBoolNames,1);

    // find the same outputs
    finOut = [];
    for i=1:size(funcNames(10),"*")
        if find(isempty(funcNames(10)(1,i))) then
            finOut($+1) = i;
        end
    end
    // delete output separator ""
    funcNames(10) = funcNames(10)(1, setdiff(1:size(funcNames(10)(1,:),"*"), finOut));
    finOut = finOut - (1:length(finOut))';
    outTyp = [outTyp, finOut];
    // compute outputs
    j = 1;
    lengthOut = list();
    newOutTyp = [];
    for i=1:size(finOut,"r")
        lengthOut(i) = j:outTyp(i,2);
        j = lengthOut(i)($)+1;
        outTyp(i,3) = length(lengthOut(i)(1:3:$));
        if outTyp(i,1) == 1 then
            newOutTyp = [newOutTyp, ones(1:outTyp(i,3))];
        elseif outTyp(i,1) == 2 then
            newOutTyp = [newOutTyp, 2*ones(1:outTyp(i,3))];
        else
            newOutTyp = [newOutTyp, zeros(1:outTyp(i,3))];
        end
    end

    // size for valueReferences
    realOparRefs = 0;
    intOparRefs = 0;
    typeOpar = [];
    for i = 1:size(funcNames(8))
        if type(funcNames(8)(i)) == 1 then
           realOparRefs = realOparRefs + 1;
           typeOpar($+1) = 1; // double
        else
           intOparRefs = intOparRefs + 1;
           typeOpar($+1) = 8; // int
        end
    end

    description = [root.attributes.modelIdentifier;...
                   root.attributes.guid;...
                   root.attributes.numberOfContinuousStates;...
                   root.attributes.numberOfEventIndicators;...
                   root.attributes.description;...
                   root.attributes.author;...
                   root.attributes.version;...
                   "";"";string(boolSize)];

    realRefs = length(find(inpTyp == 1))+sum(outTyp(find(outTyp(:,1) == 1),3))+stateSize*2+rparSize+realOparRefs;
    description(8) = string(realRefs);
    realRefs = 0:realRefs;
    realRefs = realRefs($:-1:1);
    intRefs = length(find(inpTyp <> 1 & inpTyp <> []))+sum(outTyp(find(outTyp(:,1) <> 1 & outTyp(:,1) <> []),3))+iparSize+intOparRefs;
    description(9) = string(intRefs);
    intRefs = 0:intRefs;
    intRefs = intRefs($:-1:1);
    rootSize = root.children(2).children.size;
    // inputs
    for i = rootSize:numInp
        if inpTyp(i) == 1 then
            root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
            root.children(2).children(i).children(1) = "<Real/>";
            root.children(2).children(i).children(1).attributes.start = "0";
            if grep(funcNames(9)(1,i*3)," ")<>[] | grep(funcNames(9)(1,i*3),"-")<>[]...
                | grep(funcNames(9)(1,i*3),".")<>[] then
                    error(999, msprintf("Incorrect name of input: %s\n", funcNames(9)(1,i*3)));
            end
            root.children(2).children(i).attributes.name = funcNames(9)(1,i*3);
            root.children(2).children(i).attributes.valueReference = string(realRefs($));
            root.children(2).children(i).attributes.variability = "continuous";
            root.children(2).children(i).attributes.causality = "input";
            realRefs = realRefs(1):-1:realRefs($-1);
        elseif inpTyp(i) == 2 then
            error(999, msprintf("Complex values do not supported in FMU.\n"));
        else
            root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
            root.children(2).children(i).children(1) = "<Integer/>";
            root.children(2).children(i).children(1).attributes.start = "0";
            if grep(funcNames(9)(1,i*3)," ")<>[] | grep(funcNames(9)(1,i*3),"-")<>[]...
                | grep(funcNames(9)(1,i*3),".")<>[] then
                    error(999, msprintf("Incorrect name of input: %s\n", funcNames(9)(1,i*3)));
            end
            root.children(2).children(i).attributes.name = funcNames(9)(1,i*3);
            root.children(2).children(i).attributes.valueReference = string(intRefs($));
            root.children(2).children(i).attributes.variability = "discrete";
            root.children(2).children(i).attributes.causality = "input";
            intRefs = intRefs(1):-1:intRefs($-1);
        end

    end
    // outputs
    k = 1;
    for i = (numInp+1):(length(newOutTyp)+numInp)
        if newOutTyp(k) == 1 then
            root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
            root.children(2).children(i).children(1) = "<Real/>";
            if grep(funcNames(10)(1,k*3)," ")<>[] | grep(funcNames(10)(1,k*3),"-")<>[]...
                | grep(funcNames(10)(1,k*3),".")<>[] then
                    error(999, msprintf("Incorrect name of output: %s\n", funcNames(10)(1,k*3)));
            end
            root.children(2).children(i).attributes.name = funcNames(10)(1,k*3);
            root.children(2).children(i).attributes.valueReference = string(realRefs($));
            root.children(2).children(i).attributes.variability = "continuous";
            root.children(2).children(i).attributes.causality = "output";
            realRefs = realRefs(1):-1:realRefs($-1);
        elseif newOutTyp(k) == 2 then
            error(999, msprintf("Complex values do not supported in FMU.\n"));
        else
            root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
            root.children(2).children(i).children(1) = "<Integer/>";
            if grep(funcNames(10)(1,k*3)," ")<>[] | grep(funcNames(10)(1,k*3),"-")<>[]...
                | grep(funcNames(10)(1,k*3),".")<>[] then
                    error(999, msprintf("Incorrect name of output: %s\n", funcNames(10)(1,k*3)));
            end
            root.children(2).children(i).attributes.name = funcNames(10)(1,k*3);
            root.children(2).children(i).attributes.valueReference = string(intRefs($));
            root.children(2).children(i).attributes.variability = "discrete";
            root.children(2).children(i).attributes.causality = "output";
            intRefs = intRefs(1):-1:intRefs($-1);
        end
        k = k + 1;
    end
    // continious states
    // get current root size
    k = 1;
    rootSize = root.children(2).children.size;
    for i = (rootSize+1):2:(stateSize*2+rootSize)
        root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
        root.children(2).children(i+1) = "<ScalarVariable> </ScalarVariable>";
        root.children(2).children(i).children(1) = "<Real/>";
        root.children(2).children(i+1).children(1) = "<Real/>";
        root.children(2).children(i).children(1).attributes.start = string(funcNames(2)(k));
        root.children(2).children(i).children(1).attributes.fixed = "true";
        root.children(2).children(i).attributes.name = "st_"+funcNames(1)(k)+"_"+string(realRefs($));
        root.children(2).children(i+1).attributes.name = "der("+root.children(2).children(i).attributes.name+")";
        root.children(2).children(i).attributes.valueReference = string(realRefs($));
        root.children(2).children(i+1).attributes.valueReference = string(realRefs($-1));
        root.children(2).children(i).attributes.variability = "continuous";
        root.children(2).children(i+1).attributes.variability = "continuous";
        root.children(2).children(i).attributes.causality = "internal";
        root.children(2).children(i+1).attributes.causality = "internal";
        // continious states value references
        realRefs = realRefs(1):-1:realRefs($-2);
        k = k + 1;
    end

    // real parameters
    k = 1;
    rootSize = root.children(2).children.size;
    for i = (rootSize+1):(rparSize+rootSize)
        root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
        root.children(2).children(i).children(1) = "<Real/>";
        root.children(2).children(i).children(1).attributes.start = string(funcNames(4)(k));
        root.children(2).children(i).children(1).attributes.fixed = "true";
        root.children(2).children(i).attributes.name = funcNames(3)(k)+"_"+string(realRefs($));
        root.children(2).children(i).attributes.valueReference = string(realRefs($));
        root.children(2).children(i).attributes.variability = "parameter";
        root.children(2).children(i).attributes.causality = "internal";
        realRefs = realRefs(1):-1:realRefs($-1);
        k = k + 1;
    end
    // integer parameters

    k = 1;
    rootSize = root.children(2).children.size;
    for i = (rootSize+1):(iparSize+rootSize)
        // 'isempty' to avoid inputs and outputs
        if ~isempty(funcNames(5)(k)) then
            root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
            root.children(2).children(i).children(1) = "<Integer/>";
            root.children(2).children(i).children(1).attributes.start = string(funcNames(6)(k));
            root.children(2).children(i).children(1).attributes.fixed = "true";
            root.children(2).children(i).attributes.name = funcNames(5)(k)+"_"+string(intRefs($));
            root.children(2).children(i).attributes.valueReference = string(intRefs($));
            root.children(2).children(i).attributes.variability = "parameter";
            root.children(2).children(i).attributes.causality = "internal";
            intRefs = intRefs(1):-1:intRefs($-1);
        end
        k = k + 1;
    end
    // opar parameters
    k = 1;
    rootSize = root.children(2).children.size;
    for i = (rootSize+1):(oparSize+rootSize)
        // real
        if typeOpar(k) == 1 then
            root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
            root.children(2).children(i).children(1) = "<Real/>";
            root.children(2).children(i).children(1).attributes.start = string(funcNames(8)(k));
            root.children(2).children(i).children(1).attributes.fixed = "true";
            root.children(2).children(i).attributes.name = funcNames(7)(k)+"_"+string(realRefs($));
            root.children(2).children(i).attributes.valueReference = string(realRefs($));
            root.children(2).children(i).attributes.variability = "parameter";
            root.children(2).children(i).attributes.causality = "internal";
            realRefs = realRefs(1):-1:realRefs($-1);
        // integer
        elseif typeOpar(k) == 8 then
            root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
            root.children(2).children(i).children(1) = "<Integer/>";
            root.children(2).children(i).children(1).attributes.start = string(funcNames(8)(k));
            root.children(2).children(i).children(1).attributes.fixed = "true";
            root.children(2).children(i).attributes.name = funcNames(7)(k)+"_"+string(intRefs($));
            root.children(2).children(i).attributes.valueReference = string(intRefs($));
            root.children(2).children(i).attributes.variability = "parameter";
            root.children(2).children(i).attributes.causality = "internal";
            intRefs = intRefs(1):-1:intRefs($-1);
        end
        k = k + 1;
    end
    // boolean parameters
    k = 1;
    rootSize = root.children(2).children.size;
    for i = (rootSize+1):(boolSize+rootSize)
        root.children(2).children(i) = "<ScalarVariable> </ScalarVariable>";
        root.children(2).children(i).children(1) = "<Boolean/>";
        root.children(2).children(i).children(1).attributes.start = "true";
        root.children(2).children(i).attributes.name = txtBoolNames(k);
        root.children(2).children(i).attributes.valueReference = string(k-1);
        root.children(2).children(i).attributes.variability = "parameter";
        root.children(2).children(i).attributes.causality = "internal";
    k = k + 1;
    end

    fileXml.root = root;
    xmlDump(fileXml);

    // Create the xml file
    script = "xmlWrite(fileXml,rpat+filesep()+""modelDescription.xml"",%t)";
    if exists("FMU_WRAPPER_BACKTRACE") && FMU_WRAPPER_BACKTRACE then
        ierr = 0;
        execstr(script);
    else
        ierr = execstr(script, "errcatch");
    end
    xmlDelete(fileXml);
    if ierr == 0 then
        ok = %t;
    end

endfunction
