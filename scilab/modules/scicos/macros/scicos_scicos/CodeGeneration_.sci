//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//                      - Ramine Nikoukhah <ramine.nikoukhah@inria.fr>
//                      - Rachid Djenidi
//
//                      - Scilab 5 update by Simone Mannori 
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function CodeGeneration_()
//Input editor function of Scicos code generator
//
//@l@n, 14/10/07
//
//
//** 10 Set 2007 : cleaner startup code by Simone Mannori  

    k = [] ; //** index of the CodeGen source superbloc candidate

    xc = %pt(1); //** last valid click position 
    yc = %pt(2); 
    
    %pt = []   ;
    Cmenu = [] ;

    k  = getobj(scs_m,[xc;yc]) ; //** look for a block
    //** check if we have clicked near an object
    if k==[] then
      return
    //** check if we have clicked near a block
    elseif typeof(scs_m.objs(k))<>'Block' then
      return
    end

//** If the clicked/selected block is really a superblock 
//**             <k>
    if scs_m.objs(k).model.sim(1)=='super' then
      
        XX = scs_m.objs(k);
        [ok, XX, alreadyran, flgcdgen, szclkINTemp, freof] = do_compile_superblock42(XX, scs_m, k, alreadyran);
        
        //**quick fix for sblock that contains scope
        gh_curwin = scf(curwin); 

      if ok then
        scs_m = changeports(scs_m,list('objs',k), XX);  //scs_m.objs(k)=XX
        if flgcdgen <> szclkINTemp then
           // XX.graphics.pein($)=size(scs_m.objs)+2
            XX.graphics.pein = [XX.graphics.pein ; size(scs_m.objs)+2]
            scs_m.objs(k) = XX
            bk = SampleCLK('define');
            [posx,posy] = getinputports(XX)
             posx = posx($); posy = posy($);
             teta = XX.graphics.theta
             pos  = rotate([posx;posy],teta*%pi/180, ...
                           [XX.graphics.orig(1)+XX.graphics.sz(1)/2,...
                            XX.graphics.orig(2)+XX.graphics.sz(2)/2]) ; 
             posx = pos(1); posy = pos(2);
             bk.graphics.orig = [posx posy]+[-30 20]
             bk.graphics.sz = [60 40]
             bk.graphics.exprs = [sci2exp(freof(1));sci2exp(freof(2))]
             bk.model.rpar = freof;
             bk.graphics.peout = size(scs_m.objs)+2
             scs_m.objs($+1) = bk;
             [posx2,posy2] = getoutputports(bk);
             lnk    = scicos_link();
             lnk.xx = [posx2;posx];
             lnk.yy = [posy2;posy];
             lnk.ct = [5 -1]
             lnk.from = [size(scs_m.objs) 1 0]
             lnk.to = [k flgcdgen 1]
             scs_m.objs($+1) = lnk;
        end
        edited      = %t ;
        needcompile = 4  ;
        Cmenu = "Replot" ;
      else
        Cmenu = "Open/Set"
      end
    
    else
      //** the clicked/selected block is NOT a superblock 
      message("Generation Code only work for a Superblock ! ")
    end
endfunction
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

  Code($+1)='block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'

  Code=['/* Call of actuator (blk nb '+string(i)+') */'
        Code;
        'nport = '+string(nbact)+';';
        rdnom+'_actuator(&flag, &nport, &block_'+rdnom+'['+string(i-1)+'].nevprt, told, '+..
        '('+mat2scs_c_ptr(outtb(uk))+' *)outtbptr['+string(uk-1)+'], &nrd_'+string(nuk_1)+', &nrd_'+..
        string(nuk_2)+', &nrd_'+string(uk_t)+',bbb);'];

  proto='void '+rdnom+'_actuator('+..
        'int *, int *, int *, double *, void *, int *, int *,int *,int);'
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
  txt=[get_comment('call_blk',list(funs(bk),funtyp(bk),bk));]

  //** set nevprt and flag for called block
  txt=[txt;
       'block_'+rdnom+'['+string(bk-1)+'].nevprt = '+string(pt)+';'
       'local_flag = '+string(flag)+';']

  //**see if its bidon, actuator or sensor
  if funs(bk)=='bidon' then
    txt=[];
    return
  elseif funs(bk)=='bidon2' then
    txt=[];
    return
  elseif or(bk==actt(:,1)) then
    ind=find(bk==actt(:,1))
    uk=actt(ind,2)
    nuk_1=actt(ind,3)
    nuk_2=actt(ind,4)
    uk_t=actt(ind,5)
    txt = [txt;
           'nport = '+string(ind)+';']
    txt = [txt;
           rdnom+'_actuator(&local_flag, &nport, &block_'+rdnom+'['+string(bk-1)+'].nevprt, \'
           get_blank(rdnom+'_actuator')+' &t, ('+mat2scs_c_ptr(outtb(uk))+' *)'+rdnom+'_block_outtbptr['+string(uk-1)+'], \'
           get_blank(rdnom+'_actuator')+' &nrd_'+string(nuk_1)+', &nrd_'+string(nuk_2)+', &nrd_'+string(uk_t)+',bbb);']
    txt = [txt;
           'if(local_flag < 0) return(5 - local_flag);']
    return
  elseif or(bk==capt(:,1)) then
    ind=find(bk==capt(:,1))
    yk=capt(ind,2);
    nyk_1=capt(ind,3);
    nyk_2=capt(ind,4);
    yk_t=capt(ind,5);
    txt = [txt;
           'nport = '+string(ind)+';']
    txt = [txt;
           rdnom+'_sensor(&local_flag, &nport, &block_'+rdnom+'['+string(bk-1)+'].nevprt, \'
           get_blank(rdnom+'_sensor')+' &t, ('+mat2scs_c_ptr(outtb(yk))+' *)'+rdnom+'_block_outtbptr['+string(yk-1)+'], \'
           get_blank(rdnom+'_sensor')+' &nrd_'+string(nyk_1)+', &nrd_'+string(nyk_2)+', &nrd_'+string(yk_t)+',aaa);']
    txt = [txt;
           'if(local_flag < 0) return(5 - local_flag);']
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
          nuk=size(outtb(uk),'*');
          txt=[txt;
               'rdouttb['+string(k-1)+']=(double *)'+rdnom+'_block_outtbptr['+string(uk-1)+'];']
        end
        txt=[txt;
             'args[0]=&(rdouttb[0]);']
      elseif nin==0
        uk=0;
        nuk=0;
        txt=[txt;
             'args[0]=(double *)'+rdnom+'_block_outtbptr[0];']
      else
        uk=inplnk(inpptr(bk));
        nuk=size(outtb(uk),'*');
        txt=[txt;
             'args[0]=(double *)'+rdnom+'_block_outtbptr['+string(uk-1)+'];']
      end

      if nout>1 then
        for k=1:nout
          yk=outlnk(outptr(bk)-1+k);
          nyk=size(outtb(yk),'*');
          txt=[txt;
               'rdouttb['+string(k+nin-1)+']=(double *)'+rdnom+'_block_outtbptr['+string(yk-1)+'];'];
        end
        txt=[txt;
             'args[1]=&(rdouttb['+string(nin)+']);'];
      elseif nout==0
        yk=0;
        nyk=0;
        txt=[txt;
             'args[1]=(double *)'+rdnom+'_block_outtbptr[0];'];
      else
        yk=outlnk(outptr(bk));
        nyk=size(outtb(yk),'*'),;
        txt=[txt;
             'args[1]=(double *)'+rdnom+'_block_outtbptr['+string(yk-1)+'];'];
      end
      //*******************************************//

      //*********** call seq definition ***********//
      txtc=['(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&t,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double *)args[0],&nrd_'+string(nuk)+',(double *)args[1],&nrd_'+string(nyk)+');'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//


    //**
    case 1 then
      //*********** call seq definition ***********//
      txtc=['(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&t,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      if nin>=1 | nout>=1 then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'']
        if nin>=1 then
          for k=1:nin
            uk=inplnk(inpptr(bk)-1+k);
            nuk=size(outtb(uk),'*');
            txtc($)=txtc($)+'(double *)'+rdnom+'_block_outtbptr['+string(uk-1)+'],&nrd_'+string(nuk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
        if nout>=1 then
          if nin>=1 then
            txtc($)=txtc($)+', \'
            txtc=[txtc;'']
          end
          for k=1:nout
            yk=outlnk(outptr(bk)-1+k);
            nyk=size(outtb(yk),'*');
            txtc($)=txtc($)+'(double *)'+rdnom+'_block_outtbptr['+string(yk-1)+'],&nrd_'+string(nyk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtc($)=txtc($)+', \'
        txtc=[txtc;
              'block_'+rdnom+'['+string(bk-1)+'].g,&block_'+rdnom+'['+string(bk-1)+'].ng);']
      else
        txtc($)=txtc($)+');';
      end

      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    //**
    case 2 then

      //*********** call seq definition ***********//
      txtc=[funs(bk)+'(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&t,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].inptr,block_'+rdnom+'['+string(bk-1)+'].insz,&block_'+rdnom+'['+string(bk-1)+'].nin, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].outptr,block_'+rdnom+'['+string(bk-1)+'].outsz, &block_'+rdnom+'['+string(bk-1)+'].nout'];
      if ~ztyp(bk) then
        txtc($)=txtc($)+');';
      else
        txtc($)=txtc($)+', \';
        txtc=[txtc;
              'block_'+rdnom+'['+string(bk-1)+'].g,&block_'+rdnom+'['+string(bk-1)+'].ng);']
      end
      blank = get_blank(funs(bk)+'( ');
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    //**
    case 4 then
      txt=[txt;
           funs(bk)+'(&block_'+rdnom+'['+string(bk-1)+'],local_flag);'];

  end

  txt =[txt;'if(local_flag < 0) return(5 - local_flag);']

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
  txt=[get_comment('call_blk',list(funs(bk),funtyp(bk),bk));]

  //** write nevprt activation
//   nclock=abs(pt);
  txt=[txt;
       'block_'+rdnom+'['+string(bk-1)+'].nevprt=nevprt;']

  select ftyp
    //** zero funtyp
    case 0 then
      //**** input/output addresses definition ****//
      if nin>1 then
        for k=1:nin
          uk=inplnk(inpptr(bk)-1+k);
          nuk=size(outtb(uk),'*');
          TYPE=mat2scs_c_ptr(outtb(uk));//scilab index start from 1
          txt=[txt;
               'rdouttb['+string(k-1)+']=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(uk-1)+'];']
        end
        txt=[txt;
             'args[0]=&(rdouttb[0]);']
      elseif nin==0
        uk=0;
        nuk=0;
        txt=[txt;
             'args[0]=(SCSREAL_COP *)'+rdnom+'_block_outtbptr[0];']
      else
        uk=inplnk(inpptr(bk));
        nuk=size(outtb(uk),'*');
        TYPE=mat2scs_c_ptr(outtb(uk));//scilab index start from 1
        txt=[txt;
             'args[0]=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(uk-1)+'];']
      end

      if nout>1 then
        for k=1:nout
          yk=outlnk(outptr(bk)-1+k);
          nyk=size(outtb(yk),'*');
          TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
          txt=[txt;
               'rdouttb['+string(k+nin-1)+']=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(yk-1)+'];'];
        end
        txt=[txt;
             'args[1]=&(rdouttb['+string(nin)+']);'];
      elseif nout==0
        yk=0;
        nyk=0;
        txt=[txt;
             'args[1]=(SCSREAL_COP *)'+rdnom+'_block_outtbptr[0];'];
      else
        yk=outlnk(outptr(bk));
        nyk=size(outtb(yk),'*'),;
        TYPE=mat2scs_c_ptr(outtb(yk));//scilab index start from 1
        txt=[txt;
             'args[1]=('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(yk-1)+'];'];
      end
      //*******************************************//

      //*********** call seq definition ***********//
      txtc=['(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double *)args[0],&nrd_'+string(nuk)+',(double *)args[1],&nrd_'+string(nyk)+');'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
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
      txtc=['(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      if nin>=1 | nout>=1 then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'']
        if nin>=1 then
          for k=1:nin
            uk=inplnk(inpptr(bk)-1+k);
            nuk=size(outtb(uk),'*');
            txtc($)=txtc($)+'(SCSREAL_COP *)'+rdnom+'_block_outtbptr['+string(uk-1)+'],&nrd_'+string(nuk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
        if nout>=1 then
          if nin>=1 then
            txtc($)=txtc($)+', \'
            txtc=[txtc;'']
          end
          for k=1:nout
            yk=outlnk(outptr(bk)-1+k);
            nyk=size(outtb(yk),'*');
            txtc($)=txtc($)+'(SCSREAL_COP *)'+rdnom+'_block_outtbptr['+string(yk-1)+'],&nrd_'+string(nyk)+',';
          end
          txtc($)=part(txtc($),1:length(txtc($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtc($)=txtc($)+', \'
        txtc=[txtc;'w,&nrd_0);'];
      else
        txtc($)=txtc($)+');';
      end

      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    //**
    case 2 then

      //*********** call seq definition ***********//
      txtc=[funs(bk)+'(&flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,told,block_'+rdnom+'['+string(bk-1)+'].xd, \';
            'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
            'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
            '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
            'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].inptr,block_'+rdnom+'['+string(bk-1)+'].insz,&block_'+rdnom+'['+string(bk-1)+'].nin, \';
            '(double **)block_'+rdnom+'['+string(bk-1)+'].outptr,block_'+rdnom+'['+string(bk-1)+'].outsz, &block_'+rdnom+'['+string(bk-1)+'].nout'];
      if ~ztyp(bk) then
        txtc($)=txtc($)+');';
      else
        txtc($)=txtc($)+', \';
        txtc=[txtc;
              'block_'+rdnom+'['+string(bk-1)+'].g,&block_'+rdnom+'['+string(bk-1)+'].ng);']
      end
      blank = get_blank(funs(bk)+'( ');
      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

    //**
    case 4 then
      txt=[txt;
           funs(bk)+'(&block_'+rdnom+'['+string(bk-1)+'],flag);'];

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
  Code($+1)='block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'
  Code=['/* Call of sensor (blk nb '+string(i)+') */'
        Code;
        'nport = '+string(nbcap)+';';
        rdnom+'_sensor(&flag, &nport, &block_'+rdnom+'['+string(i-1)+'].nevprt, '+..
        'told, ('+mat2scs_c_ptr(outtb(yk))+' *)(outtbptr['+string(yk-1)+']), &nrd_'+string(nyk_1)+..
        ', &nrd_'+string(nyk_2)+', &nrd_'+string(yk_t)+',aaa);'];
  proto='void '+rdnom+'_sensor('+..
        'int *, int *, int *, double *,void *, int *, int *,int *, int);'
  proto=cformatline(proto,70);
endfunction

//Generates Code for dynamically linked Fortran and C Blocks
function [CCode,FCode]=gen_blocks()
  CCode=[]
  FCode=[]

  kdyn=find(funtyp>1000) //dynamically linked blocs
                         //100X : Fortran blocks
                         //200X : C blocks

  if (size(kdyn,'*') >1)
    kfuns=[]; 
    //get the block data structure in the initial scs_m structure
    if size(corinv(kdyn(1)),'*')==1 then
      O=scs_m.objs(corinv(kdyn(1)));
    else
      path=list('objs');
      for l=corinv(kdyn(1))(1:$-1)
        path($+1)=l;
        path($+1)='model';
        path($+1)='rpar';
        path($+1)='objs';
      end
      path($+1)=corinv(kdyn(1))($);
      O=scs_m(path);
    end
    if funtyp(kdyn(1))>2000 then
      //C block
      CCode=[CCode;O.graphics.exprs(2)]
    else
      FCode=[FCode;O.graphics.exprs(2)]
    end
    kfuns=funs(kdyn(1));
    for i=2:size(kdyn,'*')
      //get the block data structure in the initial scs_m structure
      if size(corinv(kdyn(i)),'*')==1 then
        O=scs_m.objs(corinv(kdyn(i)));
      else
        path=list('objs');
         for l=corinv(kdyn(i))(1:$-1)
           path($+1)=l;
           path($+1)='model';
           path($+1)='rpar';
           path($+1)='objs';
        end
        path($+1)=corinv(kdyn(i))($);
        O=scs_m(path);
      end
      if (find(kfuns==funs(kdyn(i))) == [])
        kfuns=[kfuns;funs(kdyn(i))];
        if funtyp(kdyn(i))>2000  then
          //C block
          CCode=[CCode;O.graphics.exprs(2)]
        else
          FCode=[FCode;O.graphics.exprs(2)]
        end
      end
    end
  elseif (size(kdyn,'*')==1)
    //get the block data structure in the initial scs_m structure
    if size(corinv(kdyn),'*')==1 then
      O=scs_m.objs(corinv(kdyn));
    else
      path=list('objs');
      for l=corinv(kdyn)(1:$-1)
        path($+1)=l;
        path($+1)='model';
        path($+1)='rpar';
        path($+1)='objs';
      end
      path($+1)=corinv(kdyn)($);
      O=scs_m(path);
    end
    if funtyp(kdyn)>2000 then
      //C block
      CCode=[CCode;O.graphics.exprs(2)]
    else
      FCode=[FCode;O.graphics.exprs(2)]
    end
  end
  if CCode==[]
    CCode=['void no_ccode()'
           '{'
           '  return;'
           '}']
  end
endfunction

//** Generates the C code for new block simulation
//
//12/07/07 Alan Layec
function ok=gen_ccode42();

  //** Generate code for scicos block
  Code=make_computational42()

  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

  //** Generate files for dynamically linked scicos blocks
  [CCode,FCode]=gen_blocks()
  if FCode<>[] then
    ierr=execstr('mputl(FCode,rpat+''/''+rdnom+''f.f'')','errcatch')
    if ierr<>0 then
      message(lasterror())
      ok=%f
      return
    end
  end
  if CCode<>[] then
    ierr=execstr('mputl(CCode,rpat+''/''+rdnom+''_Cblocks.c'')','errcatch')
    if ierr<>0 then
      message(lasterror())
      ok=%f
      return
    end
  end

  //** Generate _void_io.c
  Code=['/*---------------------------------------- Actuators */'
        'void '+rdnom+'_actuator(flag,nport,nevprt,t,u,nu1,nu2,ut,flag1)'
             '     int *flag,*nevprt,*nport;'
             '     int *nu1,*nu2,*ut,flag1;'
        '     double *t;'
        '     void *u;'
        '{'
        '  int k,l;'
        '}'
        '/*---------------------------------------- Sensor */'
        'void '+rdnom+'_sensor(flag,nport,nevprt,t,y,ny1,ny2,yt,flag1)'
        '     int *flag,*nevprt,*nport;'
        '     int *ny1,*ny2,*yt,flag1;'
        '     double *t;'
        '     void *y;'
        '{'
        '  int k,l;'
        '}']

  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_void_io.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

  //** Generate _standalone.c
  Code=make_standalone42()

  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_standalone.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

//** This copy is indispensable only for stand alone code generation
//** not supported in Scilab 5 

  //** copy source code of machine.h/scicos_block4.h
  //   in target path
//   txt=mgetl(SCI+'/modules/core/includes/machine.h');
//   ierr=execstr('mputl(txt,rpat+''/machine.h'')','errcatch')
//   if ierr<>0 then
//     message(lasterror())
//     ok=%f
//     return
//   end
// 
//   txt=mgetl(SCI+'/modules/scicos_blocks/includes/scicos_block4.h');
//   ierr=execstr('mputl(txt,rpat+''/scicos_block4.h'')','errcatch')
//   if ierr<>0 then
//     message(lasterror())
//     ok=%f
//     return
//   end

  //** Generate _act_sens_events.c
  Code=['#include <stdio.h>'
        '#include <stdlib.h>'
        '#include <math.h>'
        '#include <string.h>'
        'extern void **'+rdnom+'_block_outtbptr;'
        'extern char input[50],output[50];'
        'FILE *fprr,*fprw;'
        make_outevents()
        make_actuator(%t)
        make_sensor(%t)]

  created=[];
  reponse=[];

  created=fileinfo(rpat+'/'+rdnom+'_act_sens_events.c')
  if created~=[] then
    reponse=messagebox(['File: ""'+rdnom+'_act_sens_events.c"" already exists,';
                       'do you want to replace it ?'],"modal","question",['Yes','No']);
  end

  if reponse==1 |  reponse==[] then
    ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_act_sens_events.c'')', ...
                 'errcatch')
    if ierr<>0 then
      message(lasterror())
      ok=%f
      return
    end
  end
endfunction

//creates the Scicos GUI function associated with the new block
//
//Author : Rachid Djenidi, A.Layec
//
//20/06/07, A.Layec : update with in2,out2,intyp,outtyp
//27/06/07, A.Layec : update opar,oz
//
function ok=gen_gui42();
  clkinput=ones(clkIN)';
  clkoutput=ones(clkOUT)';
  //outtb=outtb;
  oz=cpr.state.oz;

  new_oz=list();
  for i=1:lstsize(oz)
    new_oz($+1) = oz(i)
  end
  for i=1:lstsize(outtb)
    new_oz($+1) = outtb(i)
  end

  //outtb($+1) = zeros(nblk,1);
  Code=['function [x,y,typ]='+rdnom+'_c(job,arg1,arg2)';
        '// Copyright INRIA';
        ' x=[];y=[];typ=[];';
        ' select job';
        ' case ''plot'' then';
        '   standard_draw(arg1)';
        ' case ''getinputs'' then';
        '   [x,y,typ]=standard_inputs(arg1)';
        ' case ''getoutputs'' then';
        '   [x,y,typ]=standard_outputs(arg1)';
        ' case ''getorigin'' then';
        '   [x,y]=standard_origin(arg1)';
        ' case ''set'' then';
        '   x=arg1;';
        ' case ''define'' then'
        '   '+sci2exp(capt(:,3),'in',70); //input ports sizes 1
        '   '+sci2exp(capt(:,4),'in2',70); //input ports sizes 2
        '   '+sci2exp(scs_c_nb2scs_nb(capt(:,5)),'intyp',70); //input ports type
        '   '+sci2exp(actt(:,3),'out',70); //output ports sizes 1
        '   '+sci2exp(actt(:,4),'out2',70); //output ports sizes 2
        '   '+sci2exp(scs_c_nb2scs_nb(actt(:,5)),'outtyp',70); //output ports type
        '   '+sci2exp(x,'x',70); //initial continuous state
        '   '+sci2exp(z,'z',70); //initial discrete state
        '   work=zeros('+string(nblk)+',1)';
        '   Z=[z;work]';
        '   '+sci2exp(new_oz,'odstate',70);
        '   '+sci2exp(cpr.sim.rpar,'rpar',70); //real parameters
        '   '+sci2exp(cpr.sim.ipar,'ipar',70); //integer parameters
        '   '+sci2exp(cpr.sim.opar,'opar',70); //object parameters
        '   '+sci2exp(clkinput,'clkinput',70);
        '   '+sci2exp(clkoutput,'clkoutput',70);
        '   '+sci2exp(FIRING,'firing',70);
        '   nzcross='+string(sum(cpr.sim.zcptr(2:$)-cpr.sim.zcptr(1:$-1)))';
        '   nmode='+string(sum(cpr.sim.modptr(2:$)-cpr.sim.modptr(1:$-1)))';]

  for i=1:length(bllst)
    deput=[%t,%f]
    if (bllst(i).dep_ut(2) == %t) then
      deput(1,2)=%t;
      break;
    end
  end
  Code($+1)='   '+sci2exp(deput,'dep_ut',70);
  Code=[Code
        '   model=scicos_model(sim=list('''+rdnom+''',4),..'
        '                      in=in,..'
        '                      in2=in2,..'
        '                      intyp=intyp,..'
        '                      out=out,..'
        '                      out2=out2,..'
        '                      outtyp=outtyp,..'
        '                      evtin=clkinput,..'
        '                      evtout=clkoutput,..'
        '                      firing=firing,..'
        '                      state=x,..'
        '                      dstate=Z,..'
        '                      odstate=odstate,..'
        '                      rpar=rpar,..'
        '                      ipar=ipar,..'
        '                      opar=opar,..'
        '                      blocktype=''c'',..'
        '                      dep_ut=dep_ut,..'
        '                      nzcross=nzcross,..'
        '                      nmode=nmode)'
        '   gr_i=''xstringb(orig(1),orig(2),'''''+rdnom+''''',sz(1),..'
        '          sz(2),''''fill'''')''';
        '   x=standard_define([2 2],model,[],gr_i)';
        ' end'
        'endfunction'];
  //Create file
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_c.sci'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
  end
endfunction

function  [ok,XX,alreadyran,flgcdgen,szclkINTemp,freof] = do_compile_superblock42(XX,all_scs_m,numk,alreadyran)
// Transforms a given Scicos discrete and continuous SuperBlock into a C defined Block

  scs_m=XX.model.rpar
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

  IN=[];OUT=[];clkIN=[];clkOUT=[];numa=[];numc=[];
  for i=1:size(scs_m.objs)
    if typeof(scs_m.objs(i))=='Block' then
      if scs_m.objs(i).gui=='CLKOUT_f' then
        ok=%f;%cpr=list()
        message('Superblock should not have any activation output port.')
        return
      elseif scs_m.objs(i).gui=='IN_f' then
        //replace input ports by sensor blocks
        numc=numc+1
        scs_m.objs(i).gui='INPUTPORTEVTS';
        scs_m.objs(i).model.evtin=1
        scs_m.objs(i).model.sim(1)='capteur'+string(numc)
        IN=[IN scs_m.objs(i).model.ipar]
      elseif scs_m.objs(i).gui=='OUT_f' then
        //replace output ports by actuator blocks
        numa=numa+1
        scs_m.objs(i).gui='OUTPUTPORTEVTS';
        scs_m.objs(i).model.sim(1)='actionneur'+string(numa)
        OUT=[OUT  scs_m.objs(i).model.ipar]
      elseif scs_m.objs(i).gui=='CLKINV_f' then
        //replace event input ports by  fictious block
        scs_m.objs(i).gui='EVTGEN_f';
        scs_m.objs(i).model.sim(1)='bidon'
        clkIN=[clkIN scs_m.objs(i).model.ipar];
        //elseif scs_m.objs(i).model.dep_ut(2)==%t then
        //check for time dependency PAS IICI
        //ok=%f;%cpr=list()
        //message('a block have time dependence.')
        //return
      elseif scs_m.objs(i).gui=='CLKOUTV_f' then
        scs_m.objs(i).gui='EVTOUT_f';
        scs_m.objs(i).model.sim(1)='bidon2'
        clkOUT=[clkOUT scs_m.objs(i).model.ipar];
      end
    end
  end

  //Check if input/output ports are numered properly
  IN=-gsort(-IN);
  if or(IN<>[1:size(IN,'*')]) then
    ok=%f;%cpr=list()
    message('Input ports are not numbered properly.')
    return
  end
  OUT=-gsort(-OUT);
  if or(OUT<>[1:size(OUT,'*')]) then
    ok=%f;%cpr=list()
    message('Output ports are not numbered properly.')
    return
  end
  clkIN=-gsort(-clkIN);
  if or(clkIN<>[1:size(clkIN,'*')]) then
    ok=%f;%cpr=list()
    message('Event input ports are not numbered properly.')
    return
  end
  clkOUT=-gsort(-clkOUT);
  if or(clkOUT<>[1:size(clkOUT,'*')]) then
    ok=%f;%cpr=list()
    message('Event output ports are not numbered properly.')
    return
  end

  //Check if there is more than one clock in the diagram
  szclkIN=size(clkIN,2);
  if szclkIN==0 then
    szclkIN=[]
  end
  flgcdgen=szclkIN;
  [bllst,connectmat,clkconnect,cor,corinv,ok,scs_m,flgcdgen,freof]=c_pass1(scs_m,flgcdgen);
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
      if part(bllst(Ii).sim(1),1:7)=='capteur' then
        bllst(Ii).dep_ut($)=%t
//       elseif part(bllst(Ii).sim(1),1:10)=='actionneur' then
//         bllst(Ii).dep_ut($)=%t
      end
    end
  end

  if ~ok then
    message('Sorry: problem in the pre-compilation step.')
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
    for j=1:size(bllst)
      if (bllst(i).sim(1)=='actionneur'+string(j)) then
        if tt<>i then
          act=[act;i];
          tt=i;
        end
      elseif (bllst(i).sim(1)=='capteur'+string(j)) then
        if tt<>i then
          cap=[cap;i];
          tt=i;
        end
      elseif (bllst(i).sim(1)=='bidon') then
        if tt<>i then
          allhowclk=[allhowclk;i];
          tt=i;
        end
      elseif (bllst(i).sim(1)=='bidon2') then
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
    bllst($+1)=scicos_model(sim=list('bidon',1),evtout=output,..
                            blocktype='d',..
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
    output=ones((2^(size(cap,'*')))-1,1)
    if (output == []) then
      output=0;
    end
    bllst($+1)=scicos_model(sim=list('bidon',1),evtout=output,..
                            firing=-output,blocktype='d',dep_ut=[%f %f])
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
  //Test for  ALWAYS_ACTIVE sblock (RN -25/06/07)
  if ~(ALWAYS_ACTIVE&szclkIN==[]) then
    n=size(cap,1)
    for i=1:n
      if szclkIN>1 then
        for j=1:(2^szclkIN)-1
          clkconnect=[clkconnect;[howclk j cap(i) 1]];
        end
      elseif szclkIN==1 then
        clkconnect=[clkconnect;[howclk 1 cap(i) 1]];
      end
    end

    //codage de l'activation des capteurs dans le cas de l'heritage
    for i=1:2^n-1
      vec=codebinaire(i,n);
      for j=1:n
        if (vec(j)==1) then
          clkconnect=[clkconnect;[howclk i cap(j) 1]];
        end
      end
    end
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

  if cpr==list() then ok=%f,return, end

  //** Alan's patch 5/07/07: try to solve
  //   which blocks use work
  funs_save=cpr.sim.funs;
  funtyp_save=cpr.sim.funtyp;
  with_work = zeros(cpr.sim.nblk,1)
  for i=1:lstsize(cpr.sim.funs)
    if part(cpr.sim.funs(i),1:10)=='actionneur' then
       cpr.sim.funs(i) ='bidon'
       cpr.sim.funtyp(i) = 1
    elseif part(cpr.sim.funs(i),1:7)=='capteur' then
       cpr.sim.funs(i) ='bidon'
       cpr.sim.funtyp(i) = 1
    end
  end

  //**
  //** retrieve all open Scilab windows with winsid()
  //**

  BeforeCG_WinList = winsid(); 

  ierr=execstr('[state,t]=scicosim(cpr.state,0,0,cpr.sim,'+..
               '''start'',scs_m.props.tol)','errcatch')
  if ierr==0 then
    for i=1:cpr.sim.nblk
       if state.iz(i)<>0 then
          with_work(i)=%t
       end
    end
    ierr=execstr('[state,t]=scicosim(state,0,0,cpr.sim,'+..
                 '''finish'',scs_m.props.tol)','errcatch')
  end

  //**
  //** retrieve all open Scilab windows with winsid
  //** and close the additional windows open since first
  //** 
 
  //** This code does not cover 100% of the possible situations because the user can 
  //** destroy one or more Scicos wins manually during this intermediate phase
  //** This code is 100% functional if the the user does not close manually any win.
  //** TO BE updated in Scilab 5.0

  AfterCG_WinList = winsid();
  
  AfterCG_size = size(AfterCG_WinList); //** matrix
  AfterCG_size = AfterCG_size(2) ; //** vector lenght 

  BeforeCG_size = size(BeforeCG_WinList); //** matrix
  BeforeCG_size = BeforeCG_size(2) ; //** vector lenght

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
  //taille totale de z : nztotal
  nztotal=size(z,1);

  //*******************************
  //Checking if superblock is valid
  //*******************************
  msg=[]
  for i=1:length(funs)-1
    if funtyp(i)==3 then
      msg=[msg;'Type 3 block''s not allowed']
//RN   elseif ztyp(i)<>0 then
    //elseif (zcptr(i+1)-zcptr(i))<>0 then
//
      //msg=[msg;'Active zero crossing block''s not allowed']
    elseif (clkptr(i+1)-clkptr(i))<>0 &funtyp(i)>-1 &funs(i)~='bidon' then
      msg=[msg;'Regular block generating activation not allowed yet']
    end
    if msg<>[] then message(msg),ok=%f,return,end
  end

  //********************************************************
  // Change logical units for readf and writef blocks if any ???
  //********************************************************
  lunit=0
  for d=1:length(funs)
    if funs(d)=='readf'  then
      z(zptr(d)+2)=lunit
      lunit=lunit+1;
    elseif funs(d)=='writef'
      z(zptr(d)+1)=lunit
      lunit=lunit+1;
    end
  end

  //***********************************
  // Get the name of the file
  //***********************************
  foo=3;
  okk=%f;
  rdnom='foo';
  rpat=pwd();
  libs='';
  label1=[hname;pwd()+'/'+hname;''];

  while %t do
    ok=%t  // to avoid infinite loop
    [okk,..
     rdnom,..
     rpat,..
     libs,..
     label1]=getvalue('Set code generator parameters :',..
                      ['New block''s name :';
                       'Created files Path :';
                       'Other object files to link with (if any)'],..
                       list('str',1,'str',1,'str',1),label1);
    if okk==%f then
      ok=%f
      return
    end
    rpat=stripblanks(rpat);

    //** 1/07/06 Alan trying to solve multiple libraries during week-end
    if strindex(libs,'''')<>[] | strindex(libs,'""')<>[] then
      ierr=execstr('libs=evstr(libs)','errcatch')
      if ierr<>0  then
        message(['Can''t solve other files to link'])
        ok=%f;
        return
      end
    end

    //** Alan, I put a warning here in order to inform the user
    //** that the name of the superblock will change
    //** because the space char in name isn't allowed.
    if grep(rdnom," ")<>[] then
      message(['Superblock name cannot contains space characters.';
               'space chars will be automatically substituted by ""_"" '])
    end
    rdnom = strsubst(rdnom,' ','_');

    //** Put a warning here in order to inform the user
    //** that the name of the superblock will change
    //** because the "-" char could generate GCC problems
    //** (the C functions contains the name of the superblock).
    if grep(rdnom,"-")<>[] then
      message(['For full C compiler compatibility ';
               'Superblock name cannot contains ""-"" characters';
               '""-"" chars will be automatically substituted by ""_"" '])
    end
    rdnom = strsubst(rdnom,'-','_');

    dirinfo=fileinfo(rpat)

    if dirinfo==[] then
      [pathrp,fnamerp,extensionrp]=fileparts(rpat)
      ok=mkdir(pathrp,fnamerp+extensionrp)
      if ~ok then 
        messagebox('Directory '+rpat+' cannot be created',"modal","info");
      end
    elseif filetype(dirinfo(2))<>'Directory' then
      ok=%f;
      messagebox(rpat+' is not a directory',"modal","error");
    end

    if stripblanks(rdnom)==emptystr() then 
      ok=%f;
      messagebox('sorry C file name not defined',"modal","error");
    end
    if ok then break,end
  end

  //////////////////////////////////////////////////
  maxnrpar=max(rpptr(2:$)-rpptr(1:$-1))
  maxnipar=max(ipptr(2:$)-ipptr(1:$-1))
  maxnx=max(xptr(2:$)-xptr(1:$-1))
  maxnz=max(zptr(2:$)-zptr(1:$-1))
  maxnin=max(inpptr(2:$)-inpptr(1:$-1))
  maxnout=max(outptr(2:$)-outptr(1:$-1))
  maxdim=[];
  for i=1:lstsize(cpr.state.outtb)
    maxdim=max(size(cpr.state.outtb(i)))
  end
  maxtotal=max([maxnrpar;maxnipar;maxnx;maxnz;maxnin;maxnout;maxdim]);

  //************************************************************************
  //generate the call to the blocks and blocs simulation function prototypes
  //************************************************************************
  wfunclist=list();
  nbcap=0;nbact=0;capt=[];actt=[];Protostalone=[];Protos=[];
  dfuns=[]
  for i=1:length(funs)
    if or(i==act) then //block is an actuator
      nbact=nbact+1;
      [Code,actti,protoi]=call_actuator(i)
      wfunclist($+1)=[Code;'if(flag < 0 ) return(5 - flag);']
      if nbact==1 then Protostalone=[Protostalone;'';protoi],end
      actt=[actt;actti]
    elseif or(i==cap) then //block is a sensor
      nbcap=nbcap+1;
      [Code,capti,protoi]=call_sensor(i)
      wfunclist($+1)=[Code;'if(flag < 0 ) return(5 - flag);']
      if nbcap==1 then Protostalone=[Protostalone;'';protoi] ,end
      capt=[capt;capti]
    elseif funs(i)=='bidon'
      wfunclist($+1)=' '
    elseif funs(i)=='bidon2'
      wfunclist($+1)=' '
    else
      ki=find(funs(i)==dfuns)
      dfuns=[dfuns;funs(i)]
      //[Code,protoi]=call_block(i)
      [Code]=call_block4(i)
      if Code<>[] then
        wfunclist($+1)=[Code;'if(flag < 0 ) return(5 - flag);']
      else
        wfunclist($+1)=' ';
      end
      if ki==[] then
        Protos=[Protos;'';BlockProto(i)];
        Protostalone=[Protostalone;'';BlockProto(i)];
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
  for r=1:length(cap),rdcpr(cap(r))='bidon';end
  for r=1:length(act),rdcpr(act(r))='bidon';end
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
  // Scilab and C files generation
  //***********************************

  //** generate scilab interfacing funtion 
  //   of the generated scicos block
  ok=gen_gui42();


  //** generate C files
  //   of the generated scicos block
  if ok then
    ok=gen_ccode42()
  end

  //** Generates Makefile, loader
  //   and compile and link C files

  //** def files to build
  files=[rdnom rdnom+'_void_io' rdnom+'_Cblocks']

  //** def files to build for standalone
  filestan=[rdnom+'_standalone' rdnom+'_act_sens_events' rdnom+'_Cblocks']

  if ok then
    ok = buildnewblock(rdnom,files,filestan,libs,rpat,'','')
  end

  //**
  //if ok then ok=gen_loader(),end

  if ok then
    //load the gui function
    exec(rpat+'/'+rdnom+'_c.sci');
    //Change diagram superblock to new generated block
    XX=update_block(XX);
    execstr(rdnom+'_c=resume('+rdnom+'_c)')
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
  txt=[get_comment('proto_blk',list(funs(bk),funtyp(bk),bk));]

  select ftyp
    //** zero funtyp
    case 0 then

      //*********** prototype definition ***********//
      txtp=['(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *,int *, int *, \';
            ' double *, int *, double *, int *);'];
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank('void '+funs(bk)+'(');
        txtp(1) = 'void '+funs(bk)+txtp(1);
      elseif (funtyp(bk)<2000)
        txtp(1) = 'void C2F('+funs(bk)+')'+txtp(1);
        blank = get_blank('void C2F('+funs(bk)+')');
      end
      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //*******************************************//


    //**
    case 1 then

      //*********** prototype definition ***********//
      txtp=['(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *,int *, int *';]
      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank('void '+funs(bk)+'(');
        txtp(1) = 'void '+funs(bk)+txtp(1);
      elseif (funtyp(bk)<2000)
        txtp(1) = 'void C2F('+funs(bk)+')'+txtp(1);
        blank = get_blank('void C2F('+funs(bk)+')');
      end
      if nin>=1 | nout>=1 then
        txtp($)=txtp($)+', \'
        txtp=[txtp;'']
        if nin>=1 then
          for k=1:nin
            txtp($)=txtp($)+' double *, int * ,'
          end
          txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
        end
        if nout>=1 then
          if nin>=1 then
            txtp($)=txtp($)+', \'
            txtp=[txtp;'']
          end
          for k=1:nout
            txtp($)=txtp($)+' double *, int * ,'
          end
          txtp($)=part(txtp($),1:length(txtp($))-1); //remove last ,
        end
      end

      if ztyp(bk) then
        txtp($)=txtp($)+', \'
        txtp=[txtp;' double *,int *);'];
      else
        txtp($)=txtp($)+');';
      end

      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //*******************************************//

    //**
    case 2 then

      //*********** prototype definition ***********//

      txtp=['void '+funs(bk)+...
            '(int *, int *, double *, double *, double *, int *, double *, \';
            ' int *, double *, int *, double *, int *, int *, int *, \'
            ' double **, int *, int *, double **,int *, int *'];
      if ~ztyp(bk) then
        txtp($)=txtp($)+');';
      else
        txtp($)=txtp($)+', \';
        txtp=[txtp;
              ' double *,int *);']
      end
      blank = get_blank('void '+funs(bk));
      txtp(2:$) = blank + txtp(2:$);
      txt = [txt;txtp];
      //********************************************//

    //**
    case 4 then
      txt=[txt;
           'void '+funs(bk)+'(scicos_block *, int );'];

  end
endfunction

//Generating the routine for actuators interfacing
//
//
//Authors : R. Djenid, R. Nikoukhah, A. Layec
//
//actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]
function Code=make_actuator(standalone)
  Call=['/*'+part('-',ones(1,40))+' Actuators */';
        'void '+rdnom+'_actuator(flag,nport,nevprt,t,u,nu1,nu2,ut,flag1)']

  comments=['     /*'
            '      * To be customized for standalone execution';
            '      * flag  : specifies the action to be done'
            '      * nport : specifies the  index of the Super Bloc'
            '      *         regular input (The input ports are numbered'
            '      *         from the top to the bottom )'
            '      * nevprt: indicates if an activation had been received'
            '      *         0 = no activation'
            '      *         1 = activation'
            '      * t     : the current time value'
            '      * u     : the vector inputs value'
            '      * nu1   : the input size 1'
            '      * nu2   : the input size 2'
            '      * ut    : the input type'
            '      * flag1 : learn mode (0 from terminal,1 from input file'
            '      */']

  dcl=['     int *flag,*nevprt,*nport;'
       '     int *nu1,*nu2,*ut,flag1;'
       ''
       '     double *t;'
       '     void *u;'
       '{'
       '  int j,k,l;'];

  if standalone then
    a_actuator=['  /* skeleton to be customized */'
                '    switch (*flag) {'
                '    case 2 :'
                '      /*if(*nevprt>0) { get the input value */'
                '        switch (*ut) {'
                '        case 10 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %f '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((double *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 11 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %f,%f '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((double *) u+(k+l*(*nu1))),'+...
                                      '*((double *) u+((*nu1)*(*nu2)+k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 81 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %i '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((char *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 82 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %hd '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((short *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 84 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %ld '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((long *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 811 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %d '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((unsigned char *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 812 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %hu '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((unsigned short *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                ''
                '        case 814 :'
                '          for (l=0;l<*nu2;l++) {'
                '            for (k=0;k<*nu1;k++) {'
                '              printf(""Actuator: time=%f, '+...
                                  'u(%d,%d) of actuator %d is %lu '+...
                                  '\n"", \'
                '                     *t, k, l, *nport,'+...
                                      '*((unsigned long *) u+(k+l*(*nu1))));'
                '            }'
                '          }'
                '          break;'
                '        }'
                '      /*} */'
                '      break;'
                '    case 4 : /* actuator initialisation */'
                '      /* do whatever you want to initialize the actuator */'
                '      break;'
                '    case 5 : /* actuator ending */'
                '      /* do whatever you want to end the actuator */'
                '      break;'
                '    }']
  else
    a_actuator=[]
  end

  // pour fprintf
  nc=size(act,'*') // Alan : d'o� viens act ?????
                   // reponse : de do_compile_superblock!
  typ=['""%f ']; //time
  for i=1:nc
    typ($)=typ($)+'"" \'
    typ=[typ;'""'];
    for j=1:sum(actt(i,3)*actt(i,4))
      //typ=typ+'%f ';
      typ($)=typ($)+scs_c_n2c_fmt(actt(i,5))+' ';
    end
  end
  typ($)=typ($)+'\n"", \'
  typ(1)='    fprintf(fprw,'+typ(1);
  bl    ='                 ';
  if size(typ,1) <> 1 then
    typ(2:$) = bl+typ(2:$);
  end
  //Code1='    fprintf(fprw, '"'+typ+' \n'",*t'
  Code1=[typ;bl+'*t'];
  //actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]
  for i=1:size(actt,1)
    ni=actt(i,3)*actt(i,4) // dimension of ith output
    Code1($)=Code1($)+','
    Code1=[Code1;bl];
    for j=1:ni
      if actt(i,5)<>11 then
        Code1($)=Code1($)+...
                  '*((('+scs_c_n2c_typ(actt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'))'+...
                   '+'+string(j-1)+')';
      else //CAS CMPLX
        Code1($)=Code1($)+...
                  '*((('+scs_c_n2c_typ(actt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'))'+...
                   '+'+string((j-1))+'), '+...
                  '*((('+scs_c_n2c_typ(actt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'))'+...
                   '+'+string(ni+(j-1))+')';
      end
      if j<>ni then
       Code1($)=Code1($)+', ';
      end
    end
  end
  Code1($)=Code1($)+');'

  Code=[]

  if nc==1|~standalone then
    Code=[Call
          comments
          dcl
          '  if (flag1 == 0) {'
          a_actuator
          '  }'
          '  else if (flag1 == 1) {'
          '    if (*flag == 4 && *nport == 1) {'
          '      fprw=fopen(output,'"wt'");'
          '      if( fprw == NULL )'
          '        {'
          '          printf('"Error opening file: %s\n'", output);'
          '          return;'
          '        }'
          '    }else if (*flag == 2 /* && *nevprt>0 */) {'
          Code1
          '    }else if (*flag == 5 && *nport == 1) {'
          '      fclose(fprw);'
          '    }'
          '  }'
          '}']
  elseif nc>1 then
    S='  switch (*nport) {'
    for k=1:nc
      S=[S;
         '  case '+string(k)+' :/* Port number '+string(k)+' ----------*/'
         '  '+a_actuator
         '  break;']
    end
    S=[S;'  }']

    Code=[Code
          Call
          comments
          dcl
          '  if (flag1 == 0){'
          S
          '  }'
          '  else if (flag1 == 1) {'
          '    if (*flag == 4 && *nport == 1) {'
          '      fprw=fopen(output,'"wt'");'
          '      if( fprw == NULL ) {'
          '        printf('"Error opening file: %s\n'", output);'
          '        return;'
          '        }'
          '    }'
          '    else if (*flag == 2 /*&& *nevprt>0*/ ) {'
          Code1
          '    }'
          '    else if (*flag == 5 && *nport == 1) {'
          '      fclose(fprw);'
          '    }'
          '  }'
          '}']
  end
endfunction

//** Generates the scicos computational function
//   associated with the block
//12/07/07 Alan Layec
function Code=make_computational42()
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

  Indent='  ';
  Indent2=Indent+Indent;
  BigIndent='          ';

  nZ=size(z,'*'); //** index of work in z
  nO=lstsize(oz); //** index of outtb in oz

  stalone=%f

  Code=['/* SCILAB Computational function  */'
        '/*     Copyright INRIA */'
        '/*     Generated by Code_Generation toolbox of Scicos with '+ ..
        getversion()+' */';
        '/*     date : '+date()+' */'
        ''
        '/* ---- Headers ---- */'
        '#include <stdio.h>'
        '#include <memory.h>'
        '#include <string.h>'
        '#include ""machine.h"" '
        '#include ""dynamic_link.h"" '
        '#include ""scicos.h"" '
        '']

  if MSDOS then
   Code=[Code;
         ' '
         '#define max(a,b) ((a) >= (b) ? (a) : (b))'
         '#define min(a,b) ((a) <= (b) ? (a) : (b))'
         ' '
        ]
  end

  Code=[Code;
        Protos
        '']

  Code=[Code;
        '/* Table of constant values */'
        'static int nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';']

  if maxtotal<10 then
    Code=[Code;
          'static int nrd_10 = 10;']
  end
  if maxtotal<11 then
    Code=[Code;
          'static int nrd_11 = 11;']
  end

  if maxtotal<81 then
    Code=[Code;
          'static int nrd_81 = 81;']
  end
  if maxtotal<82 then
    Code=[Code;
          'static int nrd_82 = 82;']
  end
  if maxtotal<84 then
    Code=[Code;
          'static int nrd_84 = 84;']
  end
  if maxtotal<811 then
    Code=[Code;
          'static int nrd_811 = 811;']
  end
  if maxtotal<812 then
    Code=[Code;
          'static int nrd_812 = 812;']
  end
  if maxtotal<814 then
    Code=[Code;
          'static int nrd_814 = 814;']
  end

  Code=[Code;
        ''
        '/* Some general static variables */'
        'static double zero=0;'
        'static double w[1];'
        'static int aaa=0, bbb=0;']

  Code=[Code;
        ''
        '/*'+part('-',ones(1,40))+' Block Computational function */ ';
        'int  '+rdnom+'(scicos_block *block, int flag)'
        '{'
        '  /* declaration of local variables for that block struct */'
        '  double* z      = block->z;'
        '  void **ozptr   = block->ozptr;'
        '  void **oparptr = block->oparptr;'
        '  int nopar      = block->nopar;'
        '  double* x      = block->x;'
        '  double* xd     = block->xd;'
        '  double* res    = block->res;'
        '  void** u       = block->inptr;'
        '  void** y       = block->outptr;'
        '  int nevprt     = block->nevprt;'
        '  double* rpar   = block->rpar;'
        '  int nrpar      = block->nrpar;'
        '  int* ipar      = block->ipar;'
        '  int nipar      = block->nipar;'
        '  double* g      = block->g;'
        '  int* mode      = block->mode;'
        '  void **work    = block->work;'
        ''
        '  double t     = get_scicos_time();'
        '  int    phase = get_phase_simulation();'
        ''
        '  int kf;'
        '  int i;'
        '  int* reentryflag;'
        ''
        '  double *args[100];'
        '  int local_flag;'
        '  int nport;'
        '  void **'+rdnom+'_block_outtbptr;'
        '  scicos_block *block_'+rdnom+';'
        ''
        '  /*  work of blocks are catenated at the end of z */'
        '  work = (void **)(z+'+string(nZ)+');'
        ''
        '  /*  '+rdnom+'_block_outtbptr is catenated at the end of oz */'
        '  '+rdnom+'_block_outtbptr = (void **)(ozptr+'+string(nO)+');'
        ''
        '  /* struct of all blocks are stored in work of that block struct */'
        '  block_'+rdnom+'=(scicos_block*) *block->work;'
        ''];

  Code=[Code;
        '  /* Copy inputs in the block outtb */'];

  for i=1:size(capt,1)
    ni=capt(i,3)*capt(i,4); //** dimension of ith input
    if capt(i,5)<>11 then
      Code=[Code;
            '  memcpy(*('+rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+'),'+...
            '*(u+'+string(capt(i,6)-1)+'),'+...
             string(ni)+'*sizeof('+mat2c_typ(capt(i,5))+'));']
    else //** Cas cmplx
      Code=[Code;
            '  memcpy(*('+rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+'),'+...
            '*(u+'+string(capt(i,6)-1)+'),'+...
             string(2*ni)+'*sizeof('+mat2c_typ(capt(i,5))+'));']
    end
  end

  Code=[Code;
        ''
        '  if (flag != 4 && flag != 6 && flag != 5){'
        '    reentryflag=(int*) ((scicos_block *)(*block->work)+'+string(nblk)+');'
        '    if (*reentryflag==0){'
        '      *reentryflag=1;']

  for kf=1:nblk
    nin=inpptr(kf+1)-inpptr(kf);  //** number of input ports
    nout=outptr(kf+1)-outptr(kf); //** number of output ports
    nx=xptr(kf+1)-xptr(kf);
    ng=zcptr(kf+1)-zcptr(kf);
    nmode=modptr(kf+1)-modptr(kf);

    //** add comment
    txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

    Code=[Code;
          ''
          '      '+txt];

    for k=1:nin
      lprt=inplnk(inpptr(kf)-1+k);
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+']  = '+...
            rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '      block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+'] = '+...
             rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end
    Code=[Code
          '      block_'+rdnom+'['+string(kf-1)+'].z         = &(z['+...
          string(zptr(kf)-1)+']);']
    if nx <> 0 then
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].x         = &(x['+...
            string(xptr(kf)-1)+']);']
    else
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].x         = &(zero);'
            '      block_'+rdnom+'['+string(kf-1)+'].xd        = w;']
    end
    if ng <> 0 then
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].g         = &(g['+...
            string(zcptr(kf)-1)+']);']
    else
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].g         = &(zero);';]
    end
    if nmode <> 0 then
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].mode      = &(mode['+...
            string(modptr(kf)-1)+']);']
    end
    if (part(funs(kf),1:7) ~= 'capteur' &...
        part(funs(kf),1:10) ~= 'actionneur' &...
        funs(kf) ~= 'bidon' &...
        funs(kf) ~= 'bidon2') then
      //** rpar **//
      if (rpptr(kf+1)-rpptr(kf)>0) then
        Code=[Code;
              '      block_'+rdnom+'['+string(kf-1)+'].rpar      = &(rpar['+...
              string(rpptr(kf)-1)+']);']
      end
      //** ipar **//
      if (ipptr(kf+1)-ipptr(kf)>0) then
         Code=[Code;
               '      block_'+rdnom+'['+string(kf-1)+'].ipar      = &(ipar['+...
               string(ipptr(kf)-1)+']);']
      end
      //** opar **//
      if (opptr(kf+1)-opptr(kf)>0) then
        nopar = opptr(kf+1)-opptr(kf);
        for k=1:nopar
          Code=[Code;
                '      block_'+rdnom+'['+string(kf-1)+'].oparptr['+string(k-1)+...
                '] = oparptr['+string(opptr(kf)-1+k-1)+'];'];
        end
      end
      //** oz **//
      if (ozptr(kf+1)-ozptr(kf)>0) then
        noz = ozptr(kf+1)-ozptr(kf);
        for k=1:noz
          Code=[Code;
                '      block_'+rdnom+'['+string(kf-1)+'].ozptr['+string(k-1)+...
                ']  = ozptr['+string(ozptr(kf)-1+k-1)+'];'];
        end
      end
    end
    Code=[Code;
          '      block_'+rdnom+'['+string(kf-1)+'].work      ='+...
          ' (void **)(((double *)work)+'+string(kf-1)+');']
  end
  Code=[Code
        '    }'
        '  }'
        ''
       ]

  /////////////////////////////////////////////


  //** find activation number
  blks=find(funtyp>-1);
  evs=[];

  for blk=blks
    for ev=clkptr(blk):clkptr(blk+1)-1
      if funs(blk)=='bidon' then
        if ev > clkptr(howclk) -1
         evs=[evs,ev];
        end
      end
    end
  end

  //** flag 0
  flag = 0;

  block_has_output=%f
  txt   = [];
  txt22 = [];

  for kf=1:nblk
     nx=xptr(kf+1)-xptr(kf);
     if nx <> 0 then
          txt=[txt;
               '    block_'+rdnom+'['+string(kf-1)+'].xd=&(xd['+...
                string(xptr(kf)-1)+']);']
     end
     if part(funs(kf),1:10) == 'actionneur' then
       block_has_output=%t
     end
  end
  if txt<>[] then
    txt22=[txt22;
           '    '+get_comment('update_xd',list())
           txt
           ''
          ]
  end
  txt22=[txt22;
         write_code_odoit(1) //** first pass
         write_code_odoit(0) //** second pass
        ]

  if txt22<>[] then
    Code=[Code;
          '  if (flag == 0) { '+get_comment('flag',list(flag))
          txt22
          '  }'];
  end

  //** flag 1,2,3
  for flag=[1,2,3]

    txt3=[]

    //** continuous time blocks must be activated
    //** for flag 1
    if flag==1 then
      txt = write_code_cdoit(flag);

      if txt <> [] then
        txt3=[txt3;
             Indent+'  switch (nevprt) {'
            ];
        txt3=[txt3;
              Indent2+'  case '+string(0)+' : '+...
              get_comment('ev',list(0))
              '    '+txt;
             ];
        txt3=[txt3;'      break;';'']
      end
    else
      txt=[];
    end

    //** blocks with input discrete event must be activated
    //** for flag 1, 2 and 3
    if size(evs,2)>=1 then
      txt4=[]
      //**
      for ev=evs
        txt2=write_code_doit(ev,flag);
        if txt2<>[] then
          //** adjust event number because of bidon block
          new_ev=ev-(clkptr(howclk)-1)
          //**
          txt4=[txt4;
                Indent2+['  case '+string(new_ev)+' : '+...
                get_comment('ev',list(new_ev))
                   txt2];
                '      break;';'']
        end
      end

      //**
      if txt == [] then
        if txt4 <> [] then
          txt3=[txt3;
                Indent+'  switch (nevprt) {'
                txt4
                '    }'];
        end
      else
        txt3=[txt3;
              txt4]
      end
    end

    //**
    if txt <> [] then
      txt3=[txt3;
            '    }'];
    end

    //**
    if txt3<>[] then
      if flag==1 & txt22==[] then
        Code=[Code;
              '  if (flag == '+string(flag)+') { '+...
              get_comment('flag',list(flag))
              txt3
              '  }'];
      else
        Code=[Code;
              '  else if (flag == '+string(flag)+') { '+...
              get_comment('flag',list(flag))
              txt3
              '  }'];
      end
    end
  end

  //** flag 9
  ng=zcptr($)-1;
  if (ng ~= 0) then
    flag = 9;
    Code=[Code;
          '  else if (flag == '+string(flag)+') { '+...
           get_comment('flag',list(flag))
         ];

    txt=[]
    for kf=1:nblk
      if zcptr(kf+1)-zcptr(kf) <> 0 then
         txt=[txt;
              '    block_'+rdnom+'['+string(kf-1)+'].g='+...
              '&(g['+string(zcptr(kf)-1)+']);']
      end
    end

    Code=[Code;
          '    '+get_comment('update_g',list())
          txt
          ''
          write_code_zdoit()
         ]

    Code=[Code;
          '  }'];
  end

  //** flag 4
  Code=[Code;
        '  else if (flag == 4) { '+get_comment('flag',list(4))
        '    if ((*block->work=scicos_malloc(sizeof(scicos_block)*'+...
         string(nblk)+'+sizeof(int)))== NULL) return 0;';
        '    reentryflag=(int*) ((scicos_block *)(*block->work)+'+string(nblk)+');'
        '    *reentryflag=0;'
        '    block_'+rdnom+'=(scicos_block*) *block->work;'];

  for kf=1:nblk
    nin=inpptr(kf+1)-inpptr(kf);  //* number of input ports */
    nout=outptr(kf+1)-outptr(kf); //* number of output ports */
    nx=xptr(kf+1)-xptr(kf);
    ng=zcptr(kf+1)-zcptr(kf);
    nmode=modptr(kf+1)-modptr(kf);

    //** add comment
    txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

    Code=[Code;
          ''
          '    '+txt];

    Code=[Code;
          '    block_'+rdnom+'['+string(kf-1)+'].type   = '+...
            string(funtyp(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].ztyp   = '+...
            string(ztyp(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].ng     = '+...
            string(zcptr(kf+1)-zcptr(kf))+';']

    if nx <> 0 then
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].nx     = '+...
             string(nx)+';';
            '    block_'+rdnom+'['+string(kf-1)+'].x      = &(x['+...
             string(xptr(kf)-1)+']);']
    else
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].nx     = 1;';
            '    block_'+rdnom+'['+string(kf-1)+'].x      = &(zero);'
            '    block_'+rdnom+'['+string(kf-1)+'].xd     = w;']
    end

    if ng <> 0 then
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].g      = &(g['+...
            string(zcptr(kf)-1)+']);']
    else
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].g      = &(zero);';]
    end
    if nmode <> 0 then
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].mode   = &(mode['+...
            string(modptr(kf)-1)+']);']
    end
    Code=[Code;
          '    block_'+rdnom+'['+string(kf-1)+'].nz     = '+...
            string(zptr(kf+1)-zptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].noz    = '+...
            string(ozptr(kf+1)-ozptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nrpar  = '+...
            string(rpptr(kf+1)-rpptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nopar  = '+...
            string(opptr(kf+1)-opptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nipar  = '+...
            string(ipptr(kf+1)-ipptr(kf))+';'
          '    block_'+rdnom+'['+string(kf-1)+'].nin    = '+...
            string(inpptr(kf+1)-inpptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nout   = '+...
            string(outptr(kf+1)-outptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nevout = '+...
            string(clkptr(kf+1)-clkptr(kf))+';';
          '    block_'+rdnom+'['+string(kf-1)+'].nmode  = '+...
            string(modptr(kf+1)-modptr(kf))+';'];

    Code=[Code;
          '    if ((block_'+rdnom+'['+string(kf-1)+'].evout  = '+...
          'calloc(block_'+rdnom+'['+string(kf-1)+'].nevout,sizeof(double)))== NULL) return 0;'];

    //***************************** input port *****************************//
    //** alloc insz/inptr **//
    Code=[Code;
          '    if ((block_'+rdnom+'['+string(kf-1)+'].insz   = '+...
          'malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;';
          '    if ((block_'+rdnom+'['+string(kf-1)+'].inptr  = '+...
          'malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;'];

    //** inptr **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+']  = '+...
             rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1))+']   = '+...
              string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+nin)+']   = '+...
              string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+2*nin)+']   = '+...
              mat2scs_c_typ(outtb(lprt))+';'];
    end
    //**********************************************************************//

    //***************************** output port *****************************//
    //** alloc outsz/outptr **//
    Code=[Code
          '    if ((block_'+rdnom+'['+string(kf-1)+'].outsz  = '+...
           'malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;';
          '    if ((block_'+rdnom+'['+string(kf-1)+'].outptr = '+...
           'malloc(sizeof(void*)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;'];

    //** outptr **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+'] = '+...
              rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1))+...
             ']  = '+string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+nout)+...
             ']  = '+string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+2*nout)+...
             ']  = '+mat2scs_c_typ(outtb(lprt))+';'];
    end
    //**********************************************************************//

    Code=[Code
          '    block_'+rdnom+'['+string(kf-1)+'].z         = &(z['+...
          string(zptr(kf)-1)+']);']

    //***************************** object state *****************************//
    if (ozptr(kf+1)-ozptr(kf)>0) then
      noz = ozptr(kf+1)-ozptr(kf);
      Code=[Code
            '    if ((block_'+rdnom+'['+string(kf-1)+'].ozptr = '+...
            'malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
            '    if ((block_'+rdnom+'['+string(kf-1)+'].ozsz  = '+...
            'malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
            '    if ((block_'+rdnom+'['+string(kf-1)+'].oztyp = '+...
            'malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';]

      //** ozptr **//
      for k=1:noz
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].ozptr['+string(k-1)+...
              ']  = ozptr['+string(ozptr(kf)-1+k-1)+'];'];
      end

      //** 1st dim **//
      for k=1:noz
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(k-1)+...
              ']   = '+string(size(oz(ozptr(kf)-1+k),1))+';'];
      end

      //** 2nd dim **//
      for k=1:noz
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(k-1)+...
              ']   = '+string(size(oz(ozptr(kf)-1+k),2))+';'];
      end

      //** typ **//
      for k=1:noz
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].oztyp['+string(k-1)+...
              ']  = '+mat2scs_c_typ(oz(ozptr(kf)-1+k))+';'];
      end
    end
    //************************************************************************//

    if (part(funs(kf),1:7) ~= 'capteur' &...
        part(funs(kf),1:10) ~= 'actionneur' &...
        funs(kf) ~= 'bidon' &...
        funs(kf) ~= 'bidon2') then
      if (rpptr(kf+1)-rpptr(kf)>0) then
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+...
              '].rpar      = &(rpar['+string(rpptr(kf)-1)+']);']
      end
      if (ipptr(kf+1)-ipptr(kf)>0) then
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+...
              '].ipar      = &(ipar['+string(ipptr(kf)-1)+']);'] 
      end
      //** opar
      if (opptr(kf+1)-opptr(kf)>0) then
        Code=[Code;
              '    if ((block_'+rdnom+'['+string(kf-1)+'].oparptr = '+...
               'malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL) return 0;';
              '    if ((block_'+rdnom+'['+string(kf-1)+'].oparsz  = '+...
               'malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL) return 0;';
              '    if ((block_'+rdnom+'['+string(kf-1)+'].opartyp = '+...
               'malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL) return 0;';
              ]
        nopar = opptr(kf+1)-opptr(kf);
        //** oparptr **//
        for k=1:nopar
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].oparptr['+string(k-1)+...
                 ']  = oparptr['+string(opptr(kf)-1+k-1)+'];'];
        end
        //** 1st dim **//
        for k=1:nopar
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(k-1)+...
                 ']   = '+string(size(opar(opptr(kf)-1+k),1))+';'];
        end
        //** 2dn dim **//
        for k=1:nopar
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(nopar+(k-1))+...
                 ']   = '+string(size(opar(opptr(kf)-1+k),2))+';'];
        end
        //** typ **//
        for k=1:nopar
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].opartyp['+string(k-1)+...
                 ']  = '+mat2scs_c_typ(opar(opptr(kf)-1+k))+';'];
        end
      end
    end

    Code=[Code;
          '    block_'+rdnom+'['+string(kf-1)+...
           '].work      = (void **)(((double *)work)+'+string(kf-1)+');'
          '    block_'+rdnom+'['+string(kf-1)+...
           '].nevprt    = nevprt;']

  end //for kf=1:nblk

  //** init
  for kf=1:nblk
//     if or(kf==act) | or(kf==cap) then
//       if stalone then
//         txt = call_block42(kf,0,4);
//         if txt <> [] then
//           Code=[Code;
//                 '';
//                 '    '+txt];
//         end
//       end
//     else
      txt = call_block42(kf,0,4);
      if txt <> [] then
        Code=[Code;
              '';
              '    '+txt];
      end
//     end
  end

  //** cst blocks and it's dep
  txt=write_code_idoit()

  if txt<>[] then
    Code=[Code;
          ''
          '    /* initial blocks must be called with flag 1 */'
          txt];
  end
//   for kf=iord(:,1)'
// //     if or(kf==act) then
// //       if stalone then
// //         txt = call_block42(kf,0,1);
// //         if txt <> [] then
// //           Code=[Code;
// //                 '';
// //                 '    '+txt];
// //         end
// //       end
// //     else
//       txt = call_block42(kf,0,1);
//       if txt <> [] then
//         Code=[Code;
//               '';
//               '    '+txt];
//       end
// //     end
//   end

  Code=[Code;
        '  }'];

  //** flag 5
  Code=[Code;
        '  else if (flag == 5) { '+get_comment('flag',list(5))
        '    block_'+rdnom+'=*block->work;']

  for kf=1:nblk
     nin=inpptr(kf+1)-inpptr(kf);  //* number of input ports */
     nout=outptr(kf+1)-outptr(kf); //* number of output ports */

     //** add comment
     txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

     Code=[Code;
           ''
           '    '+txt];

     for k=1:nin
        lprt=inplnk(inpptr(kf)-1+k);
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+...
               ']  = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
     end
     for k=1:nout
        lprt=outlnk(outptr(kf)-1+k);
        Code=[Code
              '    block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+...
              '] = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
     end
     Code=[Code
           '    block_'+rdnom+'['+string(kf-1)+'].z=&(z['+...
            string(zptr(kf)-1)+']);']
     if (part(funs(kf),1:7) ~= 'capteur' &...
          part(funs(kf),1:10) ~= 'actionneur' &...
           funs(kf) ~= 'bidon' &...
            funs(kf) ~= 'bidon2') then
       //** rpar **//
       if (rpptr(kf+1)-rpptr(kf)>0) then
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+...
                '].rpar=&(rpar['+string(rpptr(kf)-1)+']);']
       end
       //** ipar **//
       if (ipptr(kf+1)-ipptr(kf)>0) then
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+...
                '].ipar=&(ipar['+string(ipptr(kf)-1)+']);'] 
       end
       //** opar **//
       if (opptr(kf+1)-opptr(kf)>0) then
         nopar = opptr(kf+1)-opptr(kf);
         for k=1:nopar
           Code=[Code;
                 '    block_'+rdnom+'['+string(kf-1)+'].oparptr['+string(k-1)+...
                 ']  = oparptr['+string(opptr(kf)-1+k-1)+'];'];
         end
       end
       //** oz **//
       if (ozptr(kf+1)-ozptr(kf)>0) then
         noz = ozptr(kf+1)-ozptr(kf);
         for k=1:noz
           Code=[Code;
                 '    block_'+rdnom+'['+string(kf-1)+'].ozptr['+string(k-1)+...
                ']  = ozptr['+string(ozptr(kf)-1+k-1)+'];'];
         end
       end
     end

     Code=[Code;
           '    block_'+rdnom+'['+string(kf-1)+...
            '].work=(void **)(((double *)work)+'+string(kf-1)+');']
  end

  for kf=1:nblk
//     if or(kf==act) | or(kf==cap) then
//       if stalone then
//         txt = call_block42(kf,0,5);
//         if txt <> [] then
//           Code=[Code;
//                 '';
//                 '    '+txt];
//         end
//       end
//     else
      txt = call_block42(kf,0,5);
      if txt <> [] then
        Code=[Code;
              '';
              '    '+txt];
      end
//     end
  end

  Code=[Code;
        ''
        '    for (kf = 0; kf < '+string(nblk)+'; ++kf) {'
        '      if (block_'+rdnom+'[kf].insz!=NULL) {'
        '        free(block_'+rdnom+'[kf].insz);'
        '      }else {'
        '        break;'
        '      }'
        '      if (block_'+rdnom+'[kf].outsz!=NULL){'
        '        free(block_'+rdnom+'[kf].outsz);'
        '      }else {'
        '        break;'
        '      }'
        '      if (block_'+rdnom+'[kf].nopar!=0){'
        '        if (block_'+rdnom+'[kf].oparptr!=NULL){'
        '          free(block_'+rdnom+'[kf].oparptr);'
        '        }else {'
        '          break;'
        '        }'
        '        if (block_'+rdnom+'[kf].oparsz!=NULL){'
        '          free(block_'+rdnom+'[kf].oparsz);'
        '        }else {'
        '          break;'
        '        }'
        '        if (block_'+rdnom+'[kf].opartyp!=NULL){'
        '          free(block_'+rdnom+'[kf].opartyp);'
        '        }else {'
        '          break;'
        '        }'
        '      }'
        '      if (block_'+rdnom+'[kf].noz!=0){'
        '        if (block_'+rdnom+'[kf].ozptr!=NULL){'
        '          free(block_'+rdnom+'[kf].ozptr);'
        '        }else {'
        '          break;'
        '        }'
        '        if (block_'+rdnom+'[kf].ozsz!=NULL){'
        '          free(block_'+rdnom+'[kf].ozsz);'
        '        }else {'
        '          break;'
        '        }'
        '        if (block_'+rdnom+'[kf].oztyp!=NULL){'
        '          free(block_'+rdnom+'[kf].oztyp);'
        '        }else {'
        '          break;'
        '        }'
        '      }'
        '      if (block_'+rdnom+'[kf].evout!=NULL){'
        '        free(block_'+rdnom+'[kf].evout);'
        '      }else {'
        '        break;'
        '      }'
        '    }'
        '    scicos_free(block_'+rdnom+');'
        '  }'
        ''];

  for i=1:size(actt,1)
    ni=actt(i,3)*actt(i,4); // dimension of ith input
    if actt(i,5)<>11 then
      Code=[Code;
            '  memcpy(*(y+'+string(actt(i,6)-1)+'),'+...
            '*('+rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'),'+...
             string(ni)+'*sizeof('+mat2c_typ(actt(i,5))+'));']
    else //** Cas cmplx
      Code=[Code;
            '  memcpy(*(y+'+string(actt(i,6)-1)+'),'+...
            '*('+rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'),'+...
             string(2*ni)+'*sizeof('+mat2c_typ(actt(i,5))+'));']
    end
  end

  //**
  Code=[Code;
        '  return 0;'
        ''
        '} /* '+rdnom+' */']

endfunction

//generates skeleton of external world events handling function
function Code=make_outevents()
  z='0'
  if szclkIN==[] then
    newszclkIN=0;
  else
    newszclkIN=szclkIN;
  end

  Code=['/*'+part('-',ones(1,40))+'  External events handling function */';
         'void '+rdnom+'_events(int *nevprt,double *t)';
         '{'
         '/*  set next event time and associated events ports'
         ' *  nevprt has binary expression b1..b'+string(newszclkIN)+' where bi is a bit'
         ' *  bi is set to 1 if an activation is received by port i. Note that'
         ' *  more than one activation can be received simultaneously'
         ' *  Caution: at least one bi should be equal to one */'
         '']

  if (newszclkIN <> 0) then
    Code=[Code;
          '    int i,p,b[]={'+strcat(z(ones(1,newszclkIN)),',')+'};'
          ''
          '/* this is an example for the activation of events ports */'
          '    b[0]=1;']

    if newszclkIN>1 then
      for bb=2:newszclkIN
        Code($+1)='    b['+string(bb-1)+']=1;'
      end
    end

    Code=[Code;
          ''
          '/* definition of the step time  */'
          '    *t = *t + 0.1;'
          ''
          '/* External events handling process */'
          '    *nevprt=0;p=1;'
          '    for (i=0;i<'+string(newszclkIN)+';i++) {'
          '      *nevprt=*nevprt+b[i]*p;'
          '      p=p*2;'
          '    }'
          '}']
  else
    Code=[Code;
          '';
          '/* definition of the step time  */'
          '    *t = *t + 0.1;'
          '}']
  end
endfunction

//Generating the routine for sensors interfacing
//
//
//Author : R. Djenidi, R. Nikoukhah, A. Layec
//
function Code=make_sensor(standalone)
  Call=['/*'+part('-',ones(1,40))+' Sensor */';
        'void '+rdnom+'_sensor(flag,nport,nevprt,t,y,ny1,ny2,yt,flag1)']

  comments=['     /*'
            '      * To be customized for standalone execution';
            '      * flag  : specifies the action to be done'
            '      * nport : specifies the  index of the Super Bloc'
            '      *         regular input (The input ports are numbered'
            '      *         from the top to the bottom )'
            '      * nevprt: indicates if an activation had been received'
            '      *         0 = no activation'
            '      *         1 = activation'
            '      * t     : the current time value'
            '      * y     : the vector outputs value'
            '      * ny1   : the output size 1'
            '      * ny2   : the output size 2'
            '      * yt    : the output type'
            '      * flag1 : learn mode (0 from terminal,1 from input file'
            '      */']

  dcl=['     int *flag,*nevprt,*nport;'
       '     int *ny1,*ny2,*yt,flag1;'
       ''
       '     double *t;'
       '     void *y;'
       '{'
       '  int j,k,l;'
       '  double temps;']

  if standalone then

    a_sensor=['    switch (*flag) {'
              '    case 1 : /* set the output value */'
              '      printf(""Require outputs of sensor number %d\n"", *nport);'
              '      printf(""time is: %f\n"", *t);'
              '      printf(""sizes of the sensor output is: %d,%d\n"", *ny1,*ny2);'
              '      switch (*yt) {'
              '      case 10 :'
              '        printf(""type of the sensor output is: %d (double) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%lf"", (double *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 11 :'
              '        printf(""type of the sensor output is: %d (complex) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) real part : "",k,l);'
              '            scanf(""%lf"", (double *) y+(k+l*(*ny1)));'
              '            printf(""y(%d,%d) imag part : "",k,l);'
              '            scanf(""%lf"", (double *) y+((*ny1)*(*ny2)+k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 81 :'
              '        printf(""type of the sensor output is: %d (char) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%i"", (char *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 82 :'
              '        printf(""type of the sensor output is: %d (char) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%hd"", (short *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 84 :'
              '        printf(""type of the sensor output is: %d (long) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%ld"", (long *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 811 :'
              '        printf(""type of the sensor output is: %d (unsigned char) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%d"", (unsigned char *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 812 :'
              '        printf(""type of the sensor output is: %d (unsigned short) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%hu"", (unsigned short *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      case 814 :'
              '        printf(""type of the sensor output is: %d (unsigned long) \n"", *yt);'
              '        puts(""Please set the sensor output values"");'
              '        for (l=0;l<*ny2;l++) {'
              '          for (k=0;k<*ny1;k++) {'
              '            printf(""y(%d,%d) : "",k,l);'
              '            scanf(""%lu"", (unsigned long *) y+(k+l*(*ny1)));'
              '          }'
              '        }'
              '        break;'
              ''
              '      }'
              '      break;'
              '    case 4 : /* sensor initialisation */'
              '      /* do whatever you want to initialize the sensor */'
              '      break;'
              '    case 5 : /* sensor ending */'
              '      /* do whatever you want to end the sensor */'
              '      break;'
              '    }']
  else
    a_sensor=[]
  end

  nc=size(cap,'*')

  // pour fscanf
  typ=['""%lf ']; //temps
  for i=1:nc
    typ($)=typ($)+'"" \'
    typ=[typ;'""'];
    for j=1:sum(capt(i,3)*capt(i,4))
      //typ=typ+'%f ';
      typ($)=typ($)+scs_c_n2c_fmt(capt(i,5))+' ';
    end
  end
  typ($)=typ($)+'\n"", \'
  typ=strsubst(typ,'%f','%lf');
  typ(1)='    fscanf(fprr,'+typ(1);
  bl    ='                ';
  if size(typ,1) <> 1 then
    typ(2:$) = bl+typ(2:$);
  end
  //Code1=['      fscanf( fprr, '"'+typ+' \n'",&temps'] 
  Code1=[typ;bl+'&temps'];
  for i=1:size(capt,1)
    ni=capt(i,3)*capt(i,4); // dimension of ith input
    Code1($)=Code1($)+',';
    Code1=[Code1;bl];
    for j=1:ni
      if capt(i,5)<>11 then
        Code1($)=Code1($)+...
                  '('+scs_c_n2c_typ(capt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+')'+...
                   '+'+string(j-1)+'';
      else //CAS CMPLX
        Code1($)=Code1($)+...
                  '('+scs_c_n2c_typ(capt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+')'+...
                   '+'+string((j-1))+', '+...
                  '('+scs_c_n2c_typ(capt(i,5))+' *)('+...
                   rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+')'+...
                   '+'+string(ni+(j-1))+'';
      end
      if j<>ni then
       Code1($)=Code1($)+', ';
      end
    end
  end
  Code1($)=Code1($)+');'

  Code=[]
  if nc==1|~standalone then
    Code=[Code;
          Call
          comments
          dcl
          '  if (flag1 == 0) {'
          a_sensor;
          '  } '
          '  else if (flag1 == 1) {'
          '    if (*flag == 4 && *nport == 1) {'
          '      fprr=fopen(input,'"r'");'
          '      if( fprr == NULL ) {'
          '        printf('"Error opening file: %s\n'", input);'
          '        return;'
          '      }'
          '    }'
          '    else if (*flag == 1) {'
          Code1
          '    }'
          '    else if (*flag == 5 && *nport == 1) {'
          '      fclose(fprr);'
          '    }'
          '  }'
          '}'];

  elseif nc>1 then
    S='  switch (*nport) {'
    for k=1:nc
      S=[S;
         '  case '+string(k)+' : /* Port number '+string(k)+' ----------*/'
         '  '+a_sensor
         '  break;']
    end
    S=[S;'  }']

    Code=[Code
          Call
          comments
          dcl
          '  if (flag1 == 0) {'
          S
          '  }'
          '  else if (flag1 == 1){'
          '    if (*flag == 4 && *nport == 1) {'
          '      fprr=fopen(input,'"r'");'
          '      if( fprr == NULL ) {'
          '        printf('"Error opening file: %s\n'", input);'
          '        return ;'
          '      }'
          '    }'
          '    else if (*flag == 1) {'
          Code1
          '    }'
          '    else if (*flag == 5 && *nport == 1) {'
          '      fclose(fprr);'
          '    }'
          '  }'
          '}']
  end
endfunction

//generates code of the standalone simulation procedure
//
//
// rmq : La fonction zdoit n'est pas utilis�e pour le moment
function Code=make_standalone42()
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

  Indent='  ';
  Indent2=Indent+Indent;
  BigIndent='          ';

  work=zeros(nblk,1)
  Z=[z;zeros(lstsize(outtb),1);work]';
  nX=size(x,'*');
  nztotal=size(z,1);

  stalone = %t;

  Code=['/* Code prototype for standalone use  */'
        '/*     Generated by Code_Generation toolbox of Scicos with '+ ..
        getversion()+' */'
        '/*     date : '+date()+' */'
        ''
        '/* To learn how to use the standalone code, type '"./standalone -h'" */'
        ''
        '/* ---- Headers ---- */'
        '#include <stdio.h>'
        '#include <stdlib.h>'
        '#include <math.h>'
        '#include <string.h>'
        '#include <memory.h>'
        '#include '"scicos_block4.h'"'
        '#include '"machine.h'"'
        ''
        '/* ---- Internals functions declaration ---- */'
        'int '+rdnom+'_sim(double, double, double, int);'
        Protostalone
        '']
  
  if MSDOS then
   Code=[Code;
         ' '
         '#define max(a,b) ((a) >= (b) ? (a) : (b))'
         '#define min(a,b) ((a) <= (b) ? (a) : (b))'
         ' '
        ]
  end
  

  if x<>[] then
    Code=[Code
          '/* Code prototype for standalone use  */'
          'int C2F('+rdnom+'simblk)(double , double *, double *);'
          'extern  int C2F(dset)();'
          'int ode1();'
          'int ode2();'
          'int ode4();'
          '']
  end

  Code=[Code;
        '/* Table of constant values */'
        'static int nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';']

  if maxtotal<10 then
    Code=[Code;
          'static int nrd_10 = 10;']
  end
  if maxtotal<11 then
    Code=[Code;
          'static int nrd_11 = 11;']
  end

  if maxtotal<81 then
    Code=[Code;
          'static int nrd_81 = 81;']
  end
  if maxtotal<82 then
    Code=[Code;
          'static int nrd_82 = 82;']
  end
  if maxtotal<84 then
    Code=[Code;
          'static int nrd_84 = 84;']
  end
  if maxtotal<811 then
    Code=[Code;
          'static int nrd_811 = 811;']
  end
  if maxtotal<812 then
    Code=[Code;
          'static int nrd_812 = 812;']
  end
  if maxtotal<814 then
    Code=[Code;
          'static int nrd_814 = 814;']
  end

  Code=[Code;
        ''
        '/* Some general static variables */'
        'static double zero=0;'
        'static double w[1];'
        'static int aaa=0, bbb=0;'
        'void **'+rdnom+'_block_outtbptr;'] //** !!

  Code=[Code;
        make_static_standalone42()]

  Code=[Code;
        'static void usage();'
        ''
        '/* Main program */'
        'int main(int argc, char *argv[])'
        '{'
        '  double tf=30,dt=0.1,h=0.001;'
        '  int solver=3;'
        '  char * progname = argv[0];'
        '  int c;'
        '  strcpy(input,'"'");'
        '  strcpy(output,'"'");'
        '  while ((c = getopt(argc , argv, '"i:o:d:t:e:s:hv'")) != -1)'
        '    switch (c) {'
        '    case ''i'':'
        '      strcpy(input,argv[optind-1]);'
        '      break;'
        '    case ''o'':'
        '      strcpy(output,argv[optind-1]);'
        '      break;'
        '    case ''d'':'
        '      strcpy(s,argv[optind-1]);'
        '      dt=strtod(s,p);'
        '      break;'
        '    case ''t'':'
        '      strcpy(sf,argv[optind-1]);'
        '      tf=strtod(sf,p);'
        '      break;'
        '    case ''e'':'
        '      strcpy(se,argv[optind-1]);'
        '      h=strtod(se,p);'
        '      break;'
        '    case ''s'':'
        '      strcpy(ss,argv[optind-1]);'
        '      solver=(int) strtod(ss,p);'
        '      break;'
        '    case ''h'':'
        '      usage(progname);'
        '      printf(""Options : \n'");'
        '      printf(""     -h for the help  \n"");'
        '      printf(""     -v for printing the Scilab Version \n"");'
        '      printf(""     -i for input file name, by default is Terminal \n"");'
        '      printf(""     -o for output file name, by default is Terminal \n"");'
        '      printf(""     -d for the clock period, by default is 0.1 \n"");'
        '      printf(""     -t for the final time, by default is 30 \n"");'
        '      printf(""     -e for the solvers step size, by default is 0.001 \n"");'
        '      printf(""     -s integer parameter for select the numerical solver : \n"");'
        '      printf(""        1 for Euler''s method \n"");'
        '      printf(""        2 for Heun''s method \n"");'
        '      printf(""        3 (default value) for the Fourth-Order Runge-Kutta'+...
         ' (RK4) Formula \n"");'
        '      return 0;'
        '      break;'
        '    case ''v'':'
        '      printf(""Generated by Code_Generation toolbox of Scicos ""'
        '             ""with '+getversion()+' version \n"");'
        '      return 0;'
        '      break;'
        '    case ''?'':'
        '      errflg++;'
        '      break;'
        '    }'
        '    if (errflg){'
        '      usage(progname);'
        '      return 0;'
        '    }'
        ' '
        '  '+rdnom+'_sim(tf,dt,h,solver);'
        '  return 0;'
        '}'
        ''
        'static void usage(prog)'
        '       char *prog;'
        '{'
        '  fprintf(stderr, ""Usage: %s [-h] [-v] [-i arg] [-o arg] ""'
        '                  ""[-d arg] [-t arg] [-e arg] [-s arg]\n"", prog);'
        '}'
        '']

  Code=[Code
        '/*'+part('-',ones(1,40))+'  External simulation function */'
        'int '+rdnom+'_sim(tf,dt,h,solver)'
        ''
        '   double tf,dt,h;'
        '   int solver;'
        '{'
        '  double t;'
        '  int nevprt=1;'
        '  int local_flag;'
        '  int nport;'
        '  int i;'
        '  double *args[100];'
        '']

  if (x <> []) then
    Code=[Code
          '  double tout;'
          '  double he=0.1;'
          '']
  end

  Code=[Code
        '  /* Initial values */'
        ''
        '  /* Note that z[]=[z_initial_condition;outtbptr;work]'
        cformatline('     z_initial_condition={'+...
          strcat(string(z),",")+'};',70)
        cformatline('     outtbptr={'+...
          strcat(string(zeros(lstsize(outtb),1)),"," )+'};',70)
        cformatline('     work= {'+...
          strcat(string(work),"," )+'};',70)
        '  */'
        ''
        cformatline('  double z[]={'+strcat(string(Z),',')+'};',70)]

  if size(z,1) <> 0 then
    for i=1:(length(zptr)-1)
      if zptr(i+1)-zptr(i)>0 then
        if size(corinv(i),'*')==1 then
          OO=scs_m.objs(corinv(i))
        else
          path=list('objs')
          for l=cpr.corinv(i)(1:$-1)
            path($+1)=l;path($+1)='model'
            path($+1)='rpar'
            path($+1)='objs'
          end
          path($+1)=cpr.corinv(i)($)
          OO=scs_m(path)
        end
        aaa=OO.gui
        bbb=emptystr(3,1);
        if and(aaa+bbb~=['INPUTPORTEVTS';'OUTPUTPORTEVTS';'EVTGEN_f']) then
          Code($+1)='';
          Code($+1)='  /* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
          Code($+1)='     Gui name of block: '+strcat(string(OO.gui));
          //Code($+1)='/* Name block: '+strcat(string(cpr.sim.funs(i)));
          //Code($+1)='Object number in diagram: '+strcat(string(cpr.corinv(i)));
          Code($+1)='     Compiled structure index: '+strcat(string(i));
          if stripblanks(OO.model.label)~=emptystr() then
            Code=[Code;
                  cformatline('     Label: '+strcat(string(OO.model.label)),70)]
          end
          if stripblanks(OO.graphics.exprs(1))~=emptystr() then
            Code=[Code;
                  cformatline('     Exprs: '+strcat(OO.graphics.exprs(1),","),70)]
          end
          if stripblanks(OO.graphics.id)~=emptystr() then
            Code=[Code;
                  cformatline('     Identification: '+..
                     strcat(string(OO.graphics.id)),70)]
          end
          Code=[Code;
                cformatline('     z={'+...
                strcat(string(z(zptr(i):zptr(i+1)-1)),",")+'};',70)]
          Code($+1)=' */';
        end
      end
    end
  end

  //** declaration of oz
  Code_oz = [];
  for i=1:lstsize(oz)
    if mat2scs_c_nb(oz(i)) <> 11 then
      Code_oz=[Code_oz;
               cformatline('  '+mat2c_typ(oz(i))+...
                           ' oz_'+string(i)+'[]={'+...
                           strcat(string(oz(i)(:)),',')+'};',70)]
    else //** cmplx test
      Code_oz=[Code_oz;
               cformatline('  '+mat2c_typ(oz(i))+...
                           ' oz_'+string(i)+'[]={'+...
                           strcat(string([real(oz(i)(:));
                                          imag(oz(i)(:))]),',')+'};',70)]
    end
  end

  if Code_oz <> [] then
    Code=[Code;
          '  /* oz declaration */'
          Code_oz]
  end

  //** declaration of outtb
  Code_outtb = [];
  for i=1:lstsize(outtb)
    if mat2scs_c_nb(outtb(i)) <> 11 then
      Code_outtb=[Code_outtb;
                  cformatline('  '+mat2c_typ(outtb(i))+...
                              ' outtb_'+string(i)+'[]={'+...
                              strcat(string(outtb(i)(:)),',')+'};',70)]
    else //** cmplx test
      Code_outtb=[Code_outtb;
                  cformatline('  '+mat2c_typ(outtb(i))+...
                              ' outtb_'+string(i)+'[]={'+...
                              strcat(string([real(outtb(i)(:));
                                             imag(outtb(i)(:))]),',')+'};',70)]
    end
  end

  if Code_outtb<>[] then
    Code=[Code
          ''
          '  /* outtb declaration */'
          Code_outtb
          '']
  end

  Code_outtbptr=[];
  for i=1:lstsize(outtb)
    Code_outtbptr=[Code_outtbptr;
                   '  '+rdnom+'_block_outtbptr['+...
                    string(i-1)+'] = (void *) outtb_'+string(i)+';'];
  end

  //** declaration of work
  Code=[Code
        '  /* work ptr declaration */'
        '  void **work;'
        '']

  //## affectation of work
  Code=[Code
        '  /* Get work ptr of blocks */'
        '  work = (void **)(z+'+string(size(z,'*')+lstsize(outtb))+');'
        '']

  //## affection of outtbptr
  if Code_outtb<>[] then
    Code=[Code
          '  /* Get outtbptr ptr of blocks */'
          '  '+rdnom+'_block_outtbptr = (void **)(z+'+string(nztotal)+');'
          '']
  end
  if Code_outtbptr<>[] then
    Code=[Code;
          Code_outtbptr
          '']
  end

  for kf=1:nblk
    nx=xptr(kf+1)-xptr(kf);       //** number of continuous state
    nin=inpptr(kf+1)-inpptr(kf);  //** number of input ports
    nout=outptr(kf+1)-outptr(kf); //** number of output ports

    //** add comment
    txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

    Code=[Code;
          ''
          '  '+txt];

    Code=[Code;
          '  block_'+rdnom+'['+string(kf-1)+'].type   = '+string(funtyp(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].ztyp   = '+string(ztyp(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].ng     = '+string(zcptr(kf+1)-zcptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nz     = '+string(zptr(kf+1)-zptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].noz    = '+string(ozptr(kf+1)-ozptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nrpar  = '+string(rpptr(kf+1)-rpptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nopar  = '+string(opptr(kf+1)-opptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nipar  = '+string(ipptr(kf+1)-ipptr(kf))+';'
          '  block_'+rdnom+'['+string(kf-1)+'].nin    = '+string(inpptr(kf+1)-inpptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nout   = '+string(outptr(kf+1)-outptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nevout = '+string(clkptr(kf+1)-clkptr(kf))+';';
          '  block_'+rdnom+'['+string(kf-1)+'].nmode  = '+string(modptr(kf+1)-modptr(kf))+';';]

    if nx <> 0 then
      Code=[Code;
            '  block_'+rdnom+'['+string(kf-1)+'].nx = '+string(nx)+';';
            '  block_'+rdnom+'['+string(kf-1)+'].x  = &(x['+string(xptr(kf)-1)+']);'
            '  block_'+rdnom+'['+string(kf-1)+'].xd = &(xd['+string(xptr(kf)-1)+']);']
    end

    Code=[Code;
          '  if ((block_'+rdnom+'['+string(kf-1)+'].evout  = '+...
          'calloc(block_'+rdnom+'['+string(kf-1)+'].nevout,sizeof(double)))== NULL) return 0;'];

    //***************************** input port *****************************//
    //** alloc insz/inptr **//
    Code=[Code;
          '  if ((block_'+rdnom+'['+string(kf-1)+'].inptr  = '+...
          'malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;';
          '  if ((block_'+rdnom+'['+string(kf-1)+'].insz   = '+...
          'malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL) return 0;'];

    //** inptr **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+...
             ']  = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1))+...
             ']   = '+string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+nin)+...
             ']   = '+string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].insz['+string((k-1)+2*nin)+...
             ']   = '+mat2scs_c_typ(outtb(lprt))+';'];
    end
    //**********************************************************************//

    //***************************** output port *****************************//
    //** alloc outsz/outptr **//
    Code=[Code
          '  if ((block_'+rdnom+'['+string(kf-1)+'].outsz  = '+...
          'malloc(3*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;';
          '  if ((block_'+rdnom+'['+string(kf-1)+'].outptr = '+...
          'malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL) return 0;'];

    //** outptr **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+...
             '] = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //** 1st dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1))+...
             ']  = '+string(size(outtb(lprt),1))+';']
    end

    //** 2dn dim **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+nout)+...
             ']  = '+string(size(outtb(lprt),2))+';']
    end

    //** typ **//
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '  block_'+rdnom+'['+string(kf-1)+'].outsz['+string((k-1)+2*nout)+...
             ']  = '+mat2scs_c_typ(outtb(lprt))+';']
    end
    //**********************************************************************//

    Code=[Code
          '  block_'+rdnom+'['+string(kf-1)+...
          '].z = &(z['+string(zptr(kf)-1)+']);']

    if (part(funs(kf),1:7) ~= 'capteur' &...
        part(funs(kf),1:10) ~= 'actionneur' &...
        funs(kf) ~= 'bidon') then
      //** rpar **//
      if (rpptr(kf+1)-rpptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+...
              '].rpar=&(RPAR1['+string(rpptr(kf)-1)+']);']
      end
      //** ipar **//
      if (ipptr(kf+1)-ipptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+...
              '].ipar=&(IPAR1['+string(ipptr(kf)-1)+']);']
      end
      //** opar **//
      if (opptr(kf+1)-opptr(kf)>0) then
        Code=[Code;
              '  if ((block_'+rdnom+'['+string(kf-1)+'].oparptr = '+...
               'malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL ) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].oparsz  = '+...
               'malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL ) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].opartyp = '+...
               'malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nopar))== NULL ) return 0;';
              ]
        nopar = opptr(kf+1)-opptr(kf);
        //** oparptr **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oparptr['+string(k-1)+...
                 ']   = (void *) OPAR_'+string(opptr(kf)-1+k)+';'];
        end
        //** 1st dim **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(k-1)+...
                 ']    = '+string(size(opar(opptr(kf)-1+k),1))+';'];
        end
        //** 2dn dim **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oparsz['+string(nopar+(k-1))+...
                 ']    = '+string(size(opar(opptr(kf)-1+k),2))+';'];
        end
        //** typ **//
        for k=1:nopar
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].opartyp['+string(k-1)+...
                ']   = '+mat2scs_c_typ(opar(opptr(kf)-1+k))+';'];
        end
      end
      //** oz **//
      if (ozptr(kf+1)-ozptr(kf)>0) then
        noz = ozptr(kf+1)-ozptr(kf);
        Code=[Code
              '  if ((block_'+rdnom+'['+string(kf-1)+'].ozptr = '+...
               'malloc(sizeof(void *)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].ozsz  = '+...
               'malloc(2*sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';
              '  if ((block_'+rdnom+'['+string(kf-1)+'].oztyp = '+...
               'malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].noz))== NULL) return 0;';]
        //** ozptr **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].ozptr['+string(k-1)+...
                 ']  = (void *) oz_'+string(ozptr(kf)-1+k)+';'];
        end
        //** 1st dim **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(k-1)+...
                 ']  = '+string(size(oz(ozptr(kf)-1+k),1))+';'];
        end
        //** 2nd dim **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(noz+(k-1))+...
                 ']  = '+string(size(oz(ozptr(kf)-1+k),2))+';'];
        end
        //** typ **//
        for k=1:noz
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].oztyp['+string(k-1)+...
                 ']  = '+mat2scs_c_typ(oz(ozptr(kf)-1+k))+';'];
        end
      end
    end
    Code=[Code;
          '  block_'+rdnom+'['+string(kf-1)+'].work = '+...
           '(void **)(((double *)work)+'+string(kf-1)+');']
  end

  Code=[Code;
        '  if (strlen(input) > 0)'
        '    aaa=1;'
        '  if (strlen(output)> 0)'
        '    bbb=1;'
        '']

  Code=[Code;
        '  /* set initial time */'
        '  t=0.0;'
        '']

  //** init
  Code=[Code;
        '  '+get_comment('flag',list(4))]

  for kf=1:nblk
    if or(kf==act) | or(kf==cap) then
        txt = call_block42(kf,0,4);
        if txt <> [] then
          Code=[Code;
                '';
                '  '+txt];
        end
    else
      txt = call_block42(kf,0,4);
      if txt <> [] then
        Code=[Code;
              '';
              '  '+txt];
      end
    end
  end

  //** cst blocks and it's dep
  txt=write_code_idoit()

  if txt<>[] then
    Code=[Code;
          ''
          '    /* Initial blocks must be called with flag 1 */'
          txt];
  end

  //** begin input main loop on time
  Code=[Code;
        ''
        '  while (t<=tf) {';
        '    /* */'
        '    sci_time=t;'
        '']

  //** find source activation number
  blks=find(funtyp>-1);
  evs=[];

  for blk=blks
    for ev=clkptr(blk):clkptr(blk+1)-1
      if funs(blk)=='bidon' then
        if ev > clkptr(howclk) -1
         evs=[evs,ev];
        end
      end
    end
  end

  //** flag 1,2,3
  for flag=[1,2,3]

    txt3=[]

    //** continuous time blocks must be activated
    //** for flag 1
    if flag==1 then
      txt = write_code_cdoit(flag);

      if txt <> [] then
        txt3=[''
              '    '+get_comment('ev',list(0))
              txt;
             ];
      end
    end

    //** blocks with input discrete event must be activated
    //** for flag 1, 2 and 3
    if size(evs,2)>=1 then
      txt4=[]
      //**
      for ev=evs
        txt2=write_code_doit(ev,flag);
        if txt2<>[] then
          //** adjust event number because of bidon block
          new_ev=ev-(clkptr(howclk)-1)
          //**
          txt4=[txt4;
                Indent2+['  case '+string(new_ev)+' : '+...
                get_comment('ev',list(new_ev))
                   txt2];
                '      break;';'']
        end
      end

      //**
      if txt4 <> [] then
        txt3=[txt3;
              Indent+'  /* Discrete activations */'
              Indent+'  switch (nevprt) {'
              txt4
              '    }'];
      end
    end

    //**
    if txt3<>[] then
      Code=[Code;
            '    '+get_comment('flag',list(flag))
            txt3];
    end
  end

  if x<>[] then
    Code=[Code
          ''
          '    tout=t;'
          ''
          '   /* integrate until the cumulative add of the integration'
          '    * time step doesn''t cross the sample time step'
          '    */'
          '    while (tout+h<t+dt){'
          '      switch (solver) {'
          '      case 1:'
          '        ode1(C2F('+rdnom+'simblk),tout,h);'
          '        break;'
          '      case 2:'
          '        ode2(C2F('+rdnom+'simblk),tout,h);'
          '        break;'
          '      case 3:'
          '        ode4(C2F('+rdnom+'simblk),tout,h);'
          '        break;'
          '      default :'
          '        ode4(C2F('+rdnom+'simblk),tout,h);'
          '        break;'
          '        }'
          '       tout=tout+h;'
          '    }'
          ''
          '    /* integration for the remainder piece of time */'
          '    he=t+dt-tout;'
          '    switch (solver) {'
          '    case 1:'
          '      ode1(C2F('+rdnom+'simblk),tout,he);'
          '      break;'
          '    case 2:'
          '      ode2(C2F('+rdnom+'simblk),tout,he);'
          '      break;'
          '    case 3:'
          '      ode4(C2F('+rdnom+'simblk),tout,he);'
          '      break;'
          '    default :'
          '      ode4(C2F('+rdnom+'simblk),tout,he);'
          '      break;'
          '    }']
  end

  //** fix bug provided by Roberto Bucher
  //** Alan, 13/10/07
  if nX <> 0 then
    Code=[Code;
          ''
          '    /* update ptrs of continuous array */']
    for kf=1:nblk
      nx=xptr(kf+1)-xptr(kf);  //** number of continuous state
      if nx<>0 then
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].nx = '+...
                string(nx)+';';
              '    block_'+rdnom+'['+string(kf-1)+'].x  = '+...
               '&(x['+string(xptr(kf)-1)+']);'
              '    block_'+rdnom+'['+string(kf-1)+'].xd = '+...
               '&(xd['+string(xptr(kf)-1)+']);']
      end
    end
  end

  Code=[Code
        ''
        '    /* update current time */'
        '    t=t+dt;'
        '  }']

  //** flag 5
  Code=[Code;
        ''
        '  '+get_comment('flag',list(5))]

  for kf=1:nblk
    if or(kf==act) | or(kf==cap) then
        txt = call_block42(kf,0,5);
        if txt <> [] then
          Code=[Code;
                '';
                '  '+txt];
        end
    else
      txt = call_block42(kf,0,5);
      if txt <> [] then
        Code=[Code;
              '';
              '  '+txt];
      end
    end
  end

  Code=[Code
        ''
        '  return 0;'
        '}'
        ''
        '/*'+part('-',ones(1,40))+'  Lapack messag function */';
        'void C2F(xerbla)(SRNAME,INFO,L)'
        '     char *SRNAME;'
        '     int *INFO;'
        '     long int L;'
        '{'
        '  printf(""** On entry to %s, parameter number %d""'
        '         ""  had an illegal value\n"",SRNAME,*INFO);'
        '}'
        '']

  Code=[Code;
        'void set_block_error(int err)'
        '{'
        '  return;'
        '}'
        ''
        'int get_phase_simulation()'
        '{'
        '  return 1;'
        '}'
        ''
        'void * scicos_malloc(size_t size)'
        '{'
        '  return malloc(size);'
        '}'
        ''
        'void scicos_free(void *p)'
        '{'
        '  free(p);'
        '}'
        ''
        'double get_scicos_time()'
        '{'
          '  return sci_time;'
        '}'
        ''
        'void do_cold_restart()'
        '{'
        '  return;'
        '}'
        ''
        'void sciprint (char *fmt)'
        '{'
        '  return;'
        '}'
        ''
        'int getopt (int argc, char *argv[], char *optstring)'
        '{'
        '  char *group, option, *sopt;'
        '  int len;'
        '  option = -1;'
        '  optarg = NULL;'
        '  while ( optind < argc )'
        '    { '
        '      group = argv[optind];'
        '      if ( *group != ''-'' )'
        '        {'
        '         option = -1;'
        '         optarg = group;'
        '         optind++;'
        '         break;'
        '        }'
        '      len = strlen (group);'
        '      group = group + offset;'
        '      if ( *group == ''-'' )'
        '        {'
        '         group++;'
        '         offset += 2;'
        '        }'
        '      else'
        '        offset++ ;'
        '      option = *group ;'
        '      sopt = strchr ( optstring, option ) ;'
        '      if ( sopt != NULL )'
        '        {'
        '         sopt++ ;'
        '         if ( *sopt == '':'' )'
        '           {'
        '             optarg = group + 1;'
        '             if ( *optarg == ''\0'' )'
        '                optarg = argv[++optind];'
        '             if ( *optarg == ''-'' )'
        '                {'
        '                 fprintf ( stderr, '"\n%s: illegal option -- %c'",'
        '                           argv[0], option );'
        '                 option = ''?'';'
        '                 break;'
        '                }'
        '             else'
        '                {'
        '                 optind++;'
        '                 offset = 0;'
        '                 break;'
        '                }'
        '           }'
        '         if ( offset >= len )'
        '           {'
        '             optind++;'
        '             offset = 0;'
        '           }'
        '         break;'
        '        }'
        '      else'
        '        {'
        '         fprintf ( stderr, '"\n%s: illegal option -- %c'", argv[0], option );'
        '         option = ''?'';'
        '         break;'
        '        }'
        '    }'
        '  return ( option );'
        '}'
        '']

  if (x <> []) then
    Code=[Code;
          'int C2F('+rdnom+'simblk)(t, x, xd)'
          ''
          '   double t, *x, *xd;'
          ''
          '     /*'
          '      *  !purpose'
          '      *  compute state derivative of the continuous part'
          '      *  !calling sequence'
          '      *  neq   : integer the size of the  continuous state'
          '      *  t     : current time'
          '      *  x     : double precision vector whose contains the continuous state'
          '      *  xd    : double precision vector, contain the computed derivative'
          '      *  of the state'
          '      */'
          '{'
          '  int phase=2;'
          '  int local_flag;'
          '  int nport;'
          '  int nevprt=1;'
          '  double *args[100];'
          '  C2F(dset)(&neq, &c_b14,xd , &c__1);'
          '']

    Code=[Code;
          '    '+get_comment('update_xd',list())]

    for kf=1:nblk
      if (xptr(kf+1)-xptr(kf)) > 0 then
        Code=[Code;
              '    block_'+rdnom+'['+string(kf-1)+'].x='+...
                '&(x['+string(xptr(kf)-1)+']);'
              '    block_'+rdnom+'['+string(kf-1)+'].xd='+...
                '&(xd['+string(xptr(kf)-1)+']);']
      end
    end

    Code=[Code;
          ''
          write_code_odoit(1)
          write_code_odoit(0)
         ]

    Code=[Code
          ''
          '  return 0;'
          '}'
          ''
          '/* Euler''s Method */'
          'int ode1(f,t,h)'
          '  int (*f) ();'
          '  double t, h;'
          '{'
          '  int i;'
          ''
          '  /**/'
          '  (*f)(t,x, xd);'
          ''
          '  for (i=0;i<neq;i++) {'
          '   x[i]=x[i]+h*xd[i];'
          '  }'
          ''
          '  return 0;'
          '}'
          ''
          '/* Heun''s Method */'
          'int ode2(f,t,h)'
          '  int (*f) ();'
          '  double t, h;'
          '{'
          '  int i;'
          '  double y['+string(nX)+'],yh['+string(nX)+'],temp,f0['+string(nX)+'],th;'
          ''
          '  /**/'
          '  memcpy(y,x,neq*sizeof(double));'
          '  memcpy(f0,xd,neq*sizeof(double));'
          ''
          '  /**/'
          '  (*f)(t,y, f0);'
          ''
          '  /**/'
          '  for (i=0;i<neq;i++) {'
          '    x[i]=y[i]+h*f0[i];'
          '  }'
          '  th=t+h;'
          '  for (i=0;i<neq;i++) {'
          '    yh[i]=y[i]+h*f0[i];'
          '  }'
          '  (*f)(th,yh, xd);'
          ''
          '  /**/'
          '  temp=0.5*h;'
          '  for (i=0;i<neq;i++) {'
          '    x[i]=y[i]+temp*(f0[i]+xd[i]);'
          '  }'
          ''
          '  return 0;'
          '}'
          ''
          '/* Fourth-Order Runge-Kutta (RK4) Formula */'
          'int ode4(f,t,h)'
          '  int (*f) ();'
          '  double t, h;'
          '{'
          '  int i;'
          '  double y['+string(nX)+'],yh['+string(nX)+'],'+...
            'temp,f0['+string(nX)+'],th,th2,'+...
            'f1['+string(nX)+'],f2['+string(nX)+'];'
          ''
          '  /**/'
          '  memcpy(y,x,neq*sizeof(double));'
          '  memcpy(f0,xd,neq*sizeof(double));'
          ''
          '  /**/'
          '  (*f)(t,y, f0);'
          ''
          '  /**/'
          '  for (i=0;i<neq;i++) {'
          '    x[i]=y[i]+h*f0[i];'
          '  }'
          '  th2=t+h/2;'
          '  for (i=0;i<neq;i++) {'
          '    yh[i]=y[i]+(h/2)*f0[i];'
          '  }'
          '  (*f)(th2,yh, f1);'
          ''
          '  /**/'
          '  temp=0.5*h;'
          '  for (i=0;i<neq;i++) {'
          '    x[i]=y[i]+temp*f1[i];'
          '  }'
          '  for (i=0;i<neq;i++) {'
          '    yh[i]=y[i]+(h/2)*f1[i];'
          '  }'
          '  (*f)(th2,yh, f2);'
          ''
          '  /**/'
          '  for (i=0;i<neq;i++) {'
          '    x[i]=y[i]+h*f2[i];'
          '  }'
          '  th=t+h;'
          '  for (i=0;i<neq;i++) {'
          '    yh[i]=y[i]+h*f2[i];'
          '  }'
          '  (*f)(th2,yh, xd);'
          ''
          '  /**/'
          '  temp=h/6;'
          '  for (i=0;i<neq;i++) {'
          '    x[i]=y[i]+temp*(f0[i]+2.0*f1[i]+2.0*f2[i]+xd[i]);'
          '  }'
          ''
          'return 0;'
          '}']
  end
endfunction

//generates  static table definitions
//
//Author : Rachid Djenidi, Alan Layec
function txt=make_static_standalone42()
  txt=[];

  //*** Continuous state ***//
  if x <> [] then
   txt=[txt;
        '/* def continuous state */'
        cformatline('double x[]={'+strcat(string(x),',')+'};',70)
        cformatline('double xd[]={'+strcat(string(x),',')+'};',70)
        'static int c__1 = 1;'
        'static double c_b14 = 0.;'
        'static int neq='+string(nX)+';'
        '']
  end
  //************************//

  txt=[txt;
       'char input[50], output[50],s[1],sf[1],se[1],ss[1],**p;'
       'static double sci_time;'
       'static int errflg = 0;'
       'static char *optarg = NULL ;'
       'static int optind = 1, offset = 0 ;'
       'scicos_block block_'+rdnom+'['+string(nblk)+'];'
       ''];

  //*** Real parameters ***//
  if size(rpar,1) <> 0 then
    txt=[txt;
         '/* def real parameters */'
         'static double RPAR1[ ] = {'];

    for i=1:(length(rpptr)-1)
      if rpptr(i+1)-rpptr(i)>0  then

        if size(corinv(i),'*')==1 then
          OO=scs_m.objs(corinv(i));
        else
          path=list('objs');
          for l=cpr.corinv(i)(1:$-1)
            path($+1)=l;
            path($+1)='model';
            path($+1)='rpar';
            path($+1)='objs';
          end
          path($+1)=cpr.corinv(i)($);
          OO=scs_m(path);
        end

        //** Add comments **//
        txt($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
        txt($+1)=' * Gui name of block: '+strcat(string(OO.gui));
        txt($+1)=' * Compiled structure index: '+strcat(string(i));

        if stripblanks(OO.model.label)~=emptystr() then
          txt=[txt;cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
        end
        if stripblanks(OO.graphics.exprs(1))~=emptystr() then
          txt=[txt;cformatline(' * Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
        end
        if stripblanks(OO.graphics.id)~=emptystr() then
          txt=[txt;
               cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
        end
        txt=[txt;' * rpar='];
        txt($+1)=' */';
        //******************//

        txt=[txt;
             cformatline(strcat(msprintf('%.16g,\n',rpar(rpptr(i):rpptr(i+1)-1))),70);
             '']

      end
    end
    txt=[txt;
         '};'
         '']

  else
    txt($+1)='static double RPAR1[1];';
  end
  //***********************//

  //*** Integer parameters ***//
  if size(ipar,1) <> 0 then
    txt=[txt;
         '/* def integer parameters */'
         'static int IPAR1[ ] = {'];

    for i=1:(length(ipptr)-1)
      if ipptr(i+1)-ipptr(i)>0  then
        if size(corinv(i),'*')==1 then
          OO=scs_m.objs(corinv(i));
        else
          path=list('objs');
          for l=cpr.corinv(i)(1:$-1)
            path($+1)=l
            path($+1)='model'
            path($+1)='rpar'
            path($+1)='objs'
          end
          path($+1)=cpr.corinv(i)($);
          OO=scs_m(path);
        end

        //** Add comments **//
        txt($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
        txt($+1)=' * Gui name of block: '+strcat(string(OO.gui));
        txt($+1)=' * Compiled structure index: '+strcat(string(i));
        if stripblanks(OO.model.label)~=emptystr() then
          txt=[txt;cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
        end

        if stripblanks(OO.graphics.exprs(1))~=emptystr() then
          txt=[txt;
               cformatline(' * Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
        end
        if stripblanks(OO.graphics.id)~=emptystr() then
          txt=[txt;
               cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
        end
        txt=[txt;
             cformatline(' * ipar= {'+strcat(string(ipar(ipptr(i):ipptr(i+1)-1)),",")+'};',70)];
        txt($+1)=' */';
        //******************//

        txt=[txt;cformatline(strcat(string(ipar(ipptr(i):ipptr(i+1)-1))+','),70)];
        txt($+1)='';
      end
    end
    txt=[txt;
         '};'
         '']
  else
    txt($+1)='static int IPAR1[1];';
  end
  //**************************//

  //Alan added opar (27/06/07)
  //*** Object parameters ***//
  if lstsize(opar)<>0 then
    txt=[txt;
          '/* def object parameters */']
    for i=1:(length(opptr)-1)
      if opptr(i+1)-opptr(i)>0  then

        if size(corinv(i),'*')==1 then
          OO=scs_m.objs(corinv(i));
        else
          path=list('objs');
          for l=cpr.corinv(i)(1:$-1)
            path($+1)=l;
            path($+1)='model';
            path($+1)='rpar';
            path($+1)='objs';
          end
          path($+1)=cpr.corinv(i)($);
          OO=scs_m(path);
        end

        //** Add comments **//
        txt($+1)='';
        txt($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
        txt($+1)=' * Gui name of block: '+strcat(string(OO.gui));
        txt($+1)=' * Compiled structure index: '+strcat(string(i));
        if stripblanks(OO.model.label)~=emptystr() then
          txt=[txt;cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
        end
        if stripblanks(OO.graphics.id)~=emptystr() then
          txt=[txt;
               cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
        end
        txt($+1)=' */';
        //******************//

        for j=1:opptr(i+1)-opptr(i)
          txt =[txt;
                cformatline('static '+mat2c_typ(opar(opptr(i)+j-1)) +...
                            ' OPAR_'+string(opptr(i)+j-1) + '[] = {'+...
                            strcat(string(opar(opptr(i)+j-1)),',')+'};',70)]
        end
      end
    end
  end
  //*************************//

  txt=[txt;
       '']
endfunction

//utilitary fonction used to format long C instruction
//t : a string containing a C instruction
//l : max line length allowed

//Author : Rachid Djenidi
function t1=cformatline(t ,l)

  sep=[',','+']
  l1=l-2
  t1=[]
  kw=strindex(t,' ')
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
  bl=part(' ',ones(1,nw))
  l1=l-nw;first=%t
  while %t 
    if length(t)<=l then t1=[t1;bl+t],return,end
    k=strindex(t,sep);
    if k==[] then t1=[t1;bl+t],return,end
    k($+1)=length(t)+1 // positions of the commas
    i=find(k(1:$-1)<=l&k(2:$)>l) //nearest left comma (reltively to l)
    if i==[] then i=1,end
    t1=[t1;bl+part(t,1:k(i))]
    t=part(t,k(i)+1:length(t))
    if first then l1=l1-2;bl=bl+'  ';first=%f;end
  end
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
  filetypes=['Directory','Character device','Block device',...
             'Regular file','FIFO','Symbolic link','Socket']
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
 txt='';
 for i=1:length(str)
     txt=txt+' ';
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
    case 'flag' then
        select param(1)
          case 0 then
             txt = '/* Continuous state computation */'
          case 1 then
             txt = '/* Output computation */'
          case 2 then
             txt = '/* Discrete state computation */'
          case 3 then
             txt = '/* Output Event computation */'
          case 4 then
             txt = '/* Initialization */'
          case 5 then
             txt = '/* Ending */'
          case 9 then
             txt = '/* Update zero crossing surfaces */'
        end
    //** blocks activated on event number
    case 'ev' then
       txt = '/* Blocks activated on the event number '+string(param(1))+' */'

    //** blk calling sequence
    case 'call_blk' then
        txt = ['/* Call of '''+param(1) + ...
               ''' (type '+string(param(2))+' - blk nb '+...
                    string(param(3))];
        if ztyp(param(3)) then
          txt=txt+' - with zcross) */';
        else
          txt=txt+') */';
        end
    //** proto calling sequence
    case 'proto_blk' then
        txt = ['/* prototype of '''+param(1) + ...
               ''' (type '+string(param(2))];
        if ztyp(param(3)) then
          txt=txt+' - with zcross) */';
        else
          txt=txt+') */';
        end
    //** ifthenelse calling sequence
    case 'ifthenelse_blk' then
        txt = ['/* Call of ''if-then-else'' blk (blk nb '+...
                    string(param(1))+') */']
    //** eventselect calling sequence
    case 'evtselect_blk' then
        txt = ['/* Call of ''event-select'' blk (blk nb '+...
                    string(param(1))+') */']
    //** set block structure
    case 'set_blk' then
        txt = ['/* set blk struc. of '''+param(1) + ...
               ''' (type '+string(param(2))+' - blk nb '+...
                    string(param(3))+') */'];
    //** Update xd vector ptr
    case 'update_xd' then
        txt = ['/* Update xd vector ptr */'];
    //** Update g vector ptr
    case 'update_g' then
        txt = ['/* Update g vector ptr */'];
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
         case 'int32' then
           txt = "long"
         case 'int16' then
           txt = "short"
         case 'int8' then
           txt = "char"
         case 'uint32' then
           txt = "unsigned long"
         case 'uint16' then
           txt = "unsigned short"
         case 'uint8' then
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
         case 'int32' then
           c_nb = 84
         case 'int16' then
           c_nb = 82
         case 'int8' then
           c_nb = 81
         case 'uint32' then
           c_nb = 814
         case 'uint16' then
           c_nb = 812
         case 'uint8' then
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
         case 'int32' then
           txt = "SCSINT32_COP"
         case 'int16' then
           txt = "SCSINT16_COP"
         case 'int8' then
           txt = "SCSINT8_COP"
         case 'uint32' then
           txt = "SCSUINT32_COP"
         case 'uint16' then
           txt = "SCSUINT16_COP"
         case 'uint8' then
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
         case 'int32' then
           txt = "SCSINT32_N"
         case 'int16' then
           txt = "SCSINT16_N"
         case 'int8' then
           txt = "SCSINT8_N"
         case 'uint32' then
           txt = "SCSUINT32_N"
         case 'uint16' then
           txt = "SCSUINT16_N"
         case 'uint8' then
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
     txt = '%f';
   //complex matrix
   case 11 then
     txt = '%f,%f';
   //int8 matrix
   case 81 then
     txt = '%d';
   //int16 matrix
   case 82 then
     txt = '%d';
   //int32 matrix
   case 84 then
     txt = '%d';
   //uint8 matrix
   case 811 then
     txt = '%d';
   //uint16 matrix
   case 812 then
     txt = '%d';
   //uint32 matrix
   case 814 then
     txt = '%d';
   else
     txt='%f'
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
     txt = 'double';
   //complex matrix
   case 11 then
     txt = 'double';
   //int8 matrix
   case 81 then
     txt = 'char';
   //int16 matrix
   case 82 then
     txt = 'short';
   //int32 matrix
   case 84 then
     txt = 'long';
   //uint8 matrix
   case 811 then
     txt = 'unsigned char';
   //uint16 matrix
   case 812 then
     txt = 'unsigned short';
   //uint32 matrix
   case 814 then
     txt = 'unsigned long';
   else
     txt='double'
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
  execstr('o='+rdnom+'_c(''define'')')
  XX.model=o.model
  XX.gui=rdnom+'_c';
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
                 '    '+txt2
                 ''];
          end
        end
      else
        txt2=call_block42(bk,pt,flag);
        if txt2<>[] then
          txt=[txt;
               '    '+txt2
               ''];
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
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if('+tmp_+'>0) {']
        //*******//
        txt=[txt;
             Indent+thentxt];
        if elsetxt<>[] then
          //** C **//
          txt=[txt;
               '    }';
               '    else {';]
          //*******//
          txt=[txt;
               Indent+elsetxt];
        end
        //** C **//
        txt=[txt;
             '    }']
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
             '    '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);'
             '    switch(i)'
             '    {']
        //*******//
        for i=II
         //** C **//
         txt=[txt;
              '     case '+string(i)+' :';]
         //*******//
         txt=[txt;
              BigIndent+write_code_doit(clkptr(bk)+i-1,flag);]
         //** C **//
         txt=[txt;
              BigIndent+'break;']
         //*******//
        end
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
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
                 '    '+txt2
                 ''];
          end
        end
      else
        txt2=call_block42(bk,pt,flag);
        if txt2<>[] then
          txt=[txt;
               '    '+txt2
               ''];
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
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_ = '*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if('+tmp_+'>0) {']
        //*******//
        txt=[txt;
             Indent+thentxt]
        if elsetxt<>[] then
           //** C **//
           txt=[txt;
                '    }';
                '    else {';]
           //*******//
           txt=[txt;
                Indent+elsetxt];
        end
        //** C **//
        txt=[txt;
             '    }']
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
             '    '+get_comment('evtselect_blk',list(bk));]
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        //** C **//
        txt=[txt;
             '    i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);'
             '    switch(i)'
             '    {']
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '     case '+string(i)+' :';]
          //*******//
          txt=[txt;
               BigIndent+write_code_doit(clkptr(bk)+i-1,flag);]
          //** C **//
          txt=[txt;
               BigIndent+'break;']
          //*******//
        end
        //** C **//
        txt=[txt;
             '    }']
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
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
                 '    '+txt2
                 ''];
          end
        end
      else
        txt2=call_block42(bk,pt,1);
        if txt2<>[] then
          txt=[txt;
               '    '+txt2
               ''];
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
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_ = '*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if('+tmp_+'>0) {']
        //*******//
        txt=[txt;
             Indent+thentxt];
        if elsetxt<>[] then
           //** C **//
           txt=[txt;
                '    }';
                '    else {';]
           //*******//
           txt=[txt;
                Indent+elsetxt];
        end
        //** C **//
        txt=[txt;
             '    }']
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
             '    '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);']
        txt=[txt;
             '    switch(i)'
             '    {']
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '     case '+string(i)+' :';]
          //*******//
          txt=[txt;
               BigIndent+write_code_doit(clkptr(bk)+i-1,1);]
          //** C **//
          txt=[txt;
               BigIndent+'break;']
          //*******//
        end
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
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
             '    '+txt2
             ''];
      end
    //** ifthenelse blk
    elseif funtyp(bk)==-1 then
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      thentxt=write_code_ozdoit(clkptr(bk),flag);
      elsetxt=write_code_ozdoit(clkptr(bk)+1,flag);
      if thentxt<>[] | elsetxt<>[] then
        txt=[txt;
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if ((block_'+rdnom+'['+string(bk-1)+'].nmode<0'+...
              ' && '+tmp_+'>0)'+...
              ' || \'
             '        (block_'+rdnom+'['+string(bk-1)+'].nmode>0'+...
              ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==1)) {']
        //*******//
        txt=[txt;
             Indent+thentxt]
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
        if elsetxt<>[] then
          //** C **//
          txt=[txt;
               '    else if  ((block_'+rdnom+'['+string(bk-1)+'].nmode<0'+...
                ' && '+tmp_+'<=0)'+...
                ' || \'
               '              (block_'+rdnom+'['+string(bk-1)+'].nmode>0'+...
                ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==2)) {';]
          //*******//
          txt=[txt;
               Indent+elsetxt]
          //** C **//
          txt=[txt;
               '    }'];
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
             '    '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if (block_'+rdnom+'['+string(bk-1)+'].nmode<0) {';
             '      i=max(min((int) '+...
                tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);'
             '    }'
             '    else {'
             '      i=block_'+rdnom+'['+string(bk-1)+'].mode[0];'
             '    }']
        txt=[txt;
             '    switch(i)'
             '    {'];
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '     case '+string(i)+' :';]
          //*******//
          txt=[txt;
               BigIndent+write_code_ozdoit(clkptr(bk)+i-1,flag);]
          //** C **//
          txt=[txt;
               BigIndent+'break;']
          //*******//
        end
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
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
               '    '+call_block42(bk,pt,flag)
               ''];
        end
      else
        txt2=call_block42(bk,pt,flag);
        if txt2<>[] then
          txt=[txt;
               '    '+txt2
               ''];
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
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_ = '*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if (((phase==1'+...
              ' || block_'+rdnom+'['+string(bk-1)+'].nmode==0)'+...
              ' && '+tmp_+'>0)'+...
              ' || \'
             '        ((phase!=1'+...
              ' && block_'+rdnom+'['+string(bk-1)+'].nmode!=0)'+...
              ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==1)) {']
        //*******//
        txt=[txt;
             Indent+thentxt]
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
        if elsetxt<>[] then
           //** C **//
           txt=[txt;
                '      else if (((phase==1'+...
                 ' || block_'+rdnom+'['+string(bk-1)+'].nmode==0)'+...
                 ' && '+tmp_+'<=0)'+...
                 ' || \'
                '               ((phase!=1'+...
                 ' && block_'+rdnom+'['+string(bk-1)+'].nmode!=0)'+...
                 ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==2)) {';]
          //*******//
          txt=[txt;
               Indent+elsetxt]
          //** C **//
          txt=[txt;
                '    }'];
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
             '    '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if (phase==1 || block_'+rdnom+'['+string(bk-1)+'].nmode==0) {';
             '      i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);'
             '    }'
             '    else {'
             '      i=block_'+rdnom+'['+string(bk-1)+'].mode[0];'
             '    }']
        txt=[txt;
             '    switch(i)'
             '    {'];
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '     case '+string(i)+' :';]
          //*******//
          txt=[txt;
               BigIndent+write_code_ozdoit(clkptr(bk)+i-1,flag);]
          //** C **//
          txt=[txt;
               BigIndent+'break;']
          //*******//
        end
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
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
               '    '+call_block42(bk,pt,1)
               ''];
        end
      else
        txt2=call_block42(bk,pt,1);
        if txt2<>[] then
          txt=[txt;
               '    '+txt2
               ''];
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
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_ = '*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if (((phase==1'+...
              ' || block_'+rdnom+'['+string(bk-1)+'].nmode==0)'+...
              ' && '+tmp_+'>0)'+...
              ' || \'
             '        ((phase!=1'+...
              ' && block_'+rdnom+'['+string(bk-1)+'].nmode!=0)'+...
              ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==1)) {']
        //*******//
        txt=[txt;
             Indent+thentxt]
        //** C **//
        txt=[txt;
             '    }'];
        //*******//
        if elsetxt<>[] then
           //** C **//
           txt=[txt;
                '      else if (((phase==1'+...
                 ' || block_'+rdnom+'['+string(bk-1)+'].nmode==0)'+...
                 ' && '+tmp_+'<=0)'+...
                 ' || \'
                '               ((phase!=1'+...
                 ' && block_'+rdnom+'['+string(bk-1)+'].nmode!=0)'+...
                 ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==2)) {';]
          //*******//
          txt=[txt;
               Indent+elsetxt]
          //** C **//
          txt=[txt;
               '    }'];
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
             '    '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '    if (phase==1 || block_'+rdnom+'['+string(bk-1)+'].nmode==0){';
             '      i=max(min((int) '+...
               tmp_+',block_'+rdnom+'['+string(bk-1)+'].evout),1);'
             '    else {'
             '      i=block_'+rdnom+'['+string(bk-1)+'].mode[0];'
             '    }']
        txt=[txt;
             '      switch(i)'
             '      {'];
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '       case '+string(i)+' :';]
          //*******//
          txt=[txt;
               BigIndent+write_code_ozdoit(clkptr(bk)+i-1,1);]
          //** C **//
          txt=[txt;
               BigIndent+'break;']
          //*******//
        end
        //** C **//
        txt=[txt;
             '      }'];
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
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
                 '    '+call_block42(bk,pt,9)
                 ''];
          end
        else
          txt2=call_block42(bk,pt,9);
          if txt2<>[] then
            txt=[txt;
                 '    '+txt2
                 ''];
          end
        end

    //** ifthenelse blk
    elseif funtyp(bk)==-1 then
        ix=-1+inplnk(inpptr(bk));
        TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        //*******//
        thentxt=write_code_zzdoit(clkptr(bk),9);
        elsetxt=write_code_zzdoit(clkptr(bk)+1,9);
        txt=[txt;
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        txt=[txt;
              '    g['+string(zcptr(bk)-1)+']=(double)'+tmp_+';']
        //*******//
        if thentxt<>[] | elsetxt<>[] then
           //** C **//
           txt=[txt;
                '    if (g['+string(zcptr(bk)-1)+'] > 0.){']
           //*******//
           txt=[txt;
                Indent+thentxt]
           //** C **//
           txt=[txt;
                '      }']
           //*******//
           if elsetxt <> [] then
             //** C **//
             txt=[txt;
                  '      else {']
             //*******//
             txt=[txt;
                  Indent+elsetxt]
             //** C **//
             txt=[txt;
                  '      }']
             //*******//
           end
        end
        //** C **//
        txt=[txt;
              '    if(phase==1 && block_'+rdnom+'['+string(bk-1)+'].nmode > 0){'
              '      if (g['+string(zcptr(bk)-1)+'] > 0.){'
              '        block_'+rdnom+'['+string(bk-1)+'].mode[0] = 1;'
              '      }'
              '      else {'
              '        block_'+rdnom+'['+string(bk-1)+'].mode[0] = 2;'
              '      }'
              '    }']
        //*******//
    //** eventselect blk
    elseif funtyp(bk)==-2 then
        Noutport=clkptr(bk+1)-clkptr(bk);
        ix=-1+inplnk(inpptr(bk));
        TYPE=mat2c_typ(outtb(ix+1));  //** scilab index start from 1
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        //*******//
        II=[];
        switchtxt=list()
        for i=1:Noutport
          switchtxt(i)=write_code_zzdoit(clkptr(bk)+i-1,9);
          if switchtxt(i)<>[] then II=[II i];end
        end
        txt=[txt;
             '    '+get_comment('evtselect_blk',list(bk));]
        if II<>[] then
          //** C **//
          txt=[txt;
               '    j=max(min((int) '+...
                tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);']
          txt=[txt;
               '    switch(j)'
               '    {'];
          //*******//
          for i=II
            //** C **//
            txt=[txt;
                 '     case '+string(j)+' :';]
            //*******//
            txt=[txt;
                 BigIndent+write_code_zzdoit(clkptr(bk)+i-1,9);]
            //** C **//
            txt=[txt;
                 BigIndent+'break;']
            //*******//
          end
          //** C **//
          txt=[txt;
               '    }'];
          //*******//
        end
        //** C **//
        txt=[txt;
             '  for (jj=0;jj<block_'+rdnom+'['+string(fun-1)+'].nevout-1;++jj) {'
             '    g['+string(zcptr(bk)-1)+'+jj]=(double)'+tmp_+'-(double)(jj+2);'
             '  }'
             '  if(phase==1 && block_'+rdnom+'['+string(bk-1)+'].nmode>0){'
             '    j=max(min((int) '+tmp_+','
             '              block_'+rdnom+'['+string(bk-1)+'].nevout),1);'
             '    block_'+rdnom+'['+string(bk-1)+'].mode[0]= j;'
             '  }']
        //*******//
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
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
               '    '+call_block42(bk,pt,flag)
               ''];
        end
      else
        txt2=call_block42(bk,pt,flag);
        if txt2<>[] then
          txt=[txt;
               '    '+txt2
               ''];
        end
      end
    //** ifthenelse blk
    elseif funtyp(bk)==-1 then
        ix=-1+inplnk(inpptr(bk));
        TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        //*******//
        thentxt=write_code_zzdoit(clkptr(bk),9);
        elsetxt=write_code_zzdoit(clkptr(bk)+1,9);
        txt=[txt;
             '    '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        txt=[txt;
              '    g['+string(zcptr(bk)-1)+']=(double)'+tmp_+';']
        //*******//
        if thentxt<>[] | elsetxt<>[] then
           //** C **//
           txt=[txt;
                '    if (g['+string(zcptr(bk)-1)+'] > 0.){']
           //*******//
           txt=[txt;
                Indent+thentxt]
           //** C **//
           txt=[txt;
                '      }']
           //*******//
           if elsetxt <> [] then
             //** C **//
             txt=[txt;
                  '      else {']
             //*******//
             txt=[txt;
                  Indent+elsetxt]
             //** C **//
             txt=[txt;
                  '      }']
             //*******//
           end
        end
        //** C **//
        txt=[txt;
              '    if(phase==1 && block_'+rdnom+'['+string(bk-1)+'].nmode > 0){'
              '      if (g['+string(zcptr(bk)-1)+'] > 0.){'
              '        block_'+rdnom+'['+string(bk-1)+'].mode[0] = 1;'
              '      }'
              '      else {'
              '        block_'+rdnom+'['+string(bk-1)+'].mode[0] = 2;'
              '      }'
              '    }']
        //*******//

    //** eventselect blk
    elseif funtyp(bk)==-2 then
        Noutport=clkptr(bk+1)-clkptr(bk);
        ix=-1+inplnk(inpptr(bk));
        TYPE=mat2c_typ(outtb(ix+1));  //** scilab index start from 1
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        //*******//
        II=[];
        switchtxt=list()
        for i=1:Noutport
          switchtxt(i)=write_code_zzdoit(clkptr(bk)+i-1,9);
          if switchtxt(i)<>[] then II=[II i];end
        end
        txt=[txt;
             '    '+get_comment('evtselect_blk',list(bk));]
        if II<>[] then
          //** C **//
          txt=[txt;
               '    j=max(min((int) '+...
                tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);']
          txt=[txt;
               '    switch(j)'
               '    {'];
          //*******//
          for i=II
            //** C **//
            txt=[txt;
                 '     case '+string(j)+' :';]
            //*******//
            txt=[txt;
                 BigIndent+write_code_zzdoit(clkptr(bk)+i-1,9);]
            //** C **//
            txt=[txt;
                 BigIndent+'break;']
            //*******//
          end
          //** C **//
          txt=[txt;
               '    }'];
          //*******//
        end
        //** C **//
        txt=[txt;
             '  for (jj=0;jj<block_'+rdnom+'['+string(fun-1)+'].nevout-1;++jj) {'
             '    g['+string(zcptr(bk)-1)+'+jj]=(double)'+tmp_+'-(double)(jj+2);'
             '  }'
             '  if(phase==1 && block_'+rdnom+'['+string(bk-1)+'].nmode>0){'
             '    j=max(min((int) '+tmp_+','
             '              block_'+rdnom+'['+string(bk-1)+'].nevout),1);'
             '    block_'+rdnom+'['+string(bk-1)+'].mode[0]= j;'
             '  }']
        //*******//

    //** Unknown block
    else
      error('Unknown block type '+string(bk));
    end
  end

endfunction


