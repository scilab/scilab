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
function [ok,XX,gui_path,flgcdgen,szclkINTemp,freof,c_atomic_code]=do_compile_superblock42(all_scs_m,numk,atomicflag)
// Transforms a given Scicos discrete and continuous SuperBlock into a C defined Block
// Copyright INRIA
  //******************* Fady *****************
  [lhs,rhs] = argn(0)
  if rhs<3 then atomicflag=%f; end
  c_atomic_code=[];
  freof=[];
  flgcdgen=[];
  szclkINTemp=[];
  //*****************************************

  //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  //@@ check and set global variables
  //@@ ALL
  if ~exists('ALL') then ALL=%f, end
  if ALL<>%f & ALL<>%t then ALL=%f, end

  //@@ list_of_scopes
  if ~exists('list_of_scopes') then
    list_of_scopes=[]
  end
  list_of_scopes=[list_of_scopes
                  'bouncexy'  'ipar(1)';
                  'canimxy'   'ipar(1)';
                  'canimxy3d' 'ipar(1)';
                  'cevscpe'   'ipar(1)';
                  'cfscope'   'ipar(1)';
                  'cmat3d'    ''
                  'cmatview'  ''
                  'cmscope'   'ipar(1)';
                  'cscope'    'ipar(1)';
                  'cscopxy'   'ipar(1)';
                  'cscopxy3d' 'ipar(1)']

  //@@ sim_to_be_removed
  if ~exists('sim_to_be_removed') then
    sim_to_be_removed=[]
  end
  sim_to_be_removed = [sim_to_be_removed
                       'cfscope' 'Floating Scope';
                       'affich'  'Affich';
                       'affich2' 'Affich']

  //@@ Todo : sim_to_be_replaced

  //@@ debug_agenda
  if ~exists('debug_agenda') then debug_agenda=%f, end
  if debug_agenda<>%f & debug_agenda<>%t then debug_agenda=%f, end

  //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


  //## set void value for gui_path
  gui_path=[];

  if numk<>-1 then
    //## get the model of the sblock
    XX=all_scs_m.objs(numk);

    //## get the diagram inside the sblock
    scs_m=XX.model.rpar
  else
    XX=[]
    scs_m=all_scs_m;
  end

  par=scs_m.props;
  hname=scs_m.props.title(1) //superblock name
  //***********************************************************
  //Check blocks properties and adapt them if necessary
  //***********************************************************

  IN=[];OUT=[];clkIN=[];clkOUT=[];numa=[];numc=[];
  for i=1:size(scs_m.objs)
    if typeof(scs_m.objs(i))=='Block' then
      if scs_m.objs(i).gui=='CLKOUT_f' | scs_m.objs(i).gui=='CLKOUTV_f' then
        ok=%f
        %cpr=list()
        messagebox(_('Superblock should not have any activation output port.'),'modal','error')
        return
      elseif scs_m.objs(i).gui=='INIMPL_f' then
        ok=%f
        %cpr=list()
        messagebox(_('Superblock should not have any input implicit port.'),'modal','error')
        return
      elseif scs_m.objs(i).gui=='OUTIMPL_f' then
        ok=%f
        %cpr=list()
        messagebox(_('Superblock should not have any output implicit port.'),'modal','error')
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
        //messagebox('a block have time dependence.','modal')
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
    messagebox(_('Input ports are not numbered properly.'),'modal','error')
    return
  end
  OUT=-gsort(-OUT);
  if or(OUT<>[1:size(OUT,'*')]) then
    ok=%f;%cpr=list()
    messagebox(_('Output ports are not numbered properly.'),'modal','error')
    return
  end
  clkIN=-gsort(-clkIN);
  if or(clkIN<>[1:size(clkIN,'*')]) then
    ok=%f;%cpr=list()
    messagebox(_('Event input ports are not numbered properly.'),'modal','error')
    return
  end
  clkOUT=-gsort(-clkOUT);
  if or(clkOUT<>[1:size(clkOUT,'*')]) then
    ok=%f;%cpr=list()
    messagebox(_('Event output ports are not numbered properly.'),'modal','error')
    return
  end

  //Check if there is more than one clock in the diagram
  szclkIN=size(clkIN,2);
  if szclkIN==0 then
    szclkIN=[]
  end
  flgcdgen=szclkIN

 
  //## first pass of compilation
  if ~ALL then
    [bllst,connectmat,clkconnect,cor,corinv,ok,scs_m,flgcdgen,freof]=c_pass1(scs_m,flgcdgen);
  else
    [bllst,connectmat,clkconnect,cor,corinv,ok,scs_m,flgcdgen,freof]=c_pass1(scs_m);
    szclkINTemp=flgcdgen;
  end

  //## restore buildnewblock
  clear buildnewblock

  if ~ok then
    %cpr=list()
    messagebox('Sorry: problem in the pre-compilation step.','modal')
    return
  end

  //@@ Alan, 04/12/08, adjust scope win number if needed
  [bllst,ok]=adjust_id_scopes(list_of_scopes,bllst)
  if ~ok then
    %cpr=list()
    messagebox(_('Problem adjusting scope id number.'),'modal','error')
    return
  end

  //###########################//
  //## Detect implicit block ##//

  //## Force here generation of implicit block
  //## impl_blk = %t;
  //## %scicos_solver=100;

  impl_blk = %f;
  for blki=bllst
    if type(blki.sim)==15 then
      if blki.sim(2)>10000 then
        impl_blk = %t;
        %scicos_solver=100;
        break;
      end
    end
  end
  //###########################//

  if ~ALL then
    if flgcdgen<> szclkIN
      clkIN=[clkIN flgcdgen]
    end
    szclkINTemp=szclkIN;
    szclkIN=flgcdgen;
  end

  //Test for  ALWAYS_ACTIVE sblock (RN -25/06/07)
  ALWAYS_ACTIVE=%f;
  for blki=bllst
    if blki.dep_ut($) then
      ALWAYS_ACTIVE=%t;
      break;
    end
  end

  if ALWAYS_ACTIVE then
    for Ii=1:length(bllst)
      if type(bllst(Ii).sim(1))==10 then
        if part(bllst(Ii).sim(1),1:7)=='capteur' then
          bllst(Ii).dep_ut($)=%t
        end
      end
    end
  end

  // *********************************************************
  // build various index tables :
  // cap  : indices of sensors blk in bllst
  // act  : indices of actuators blk in bllst
  // allhowclk  : indices of evt sensors blk in bllst
  // allhowclk2 : indices of evt actuators blk in bllst
  // *********************************************************

  a=[];
  b=[];
  tt=[];
  howclk=[];
  allhowclk=[];
  allhowclk2=[];
  cap=[];
  act=[];

  for i=1:size(bllst)
    for j=1:size(bllst)
      if (bllst(i).sim(1)=='capteur'+string(j)) then
        if tt<>i then
          cap=[cap;i];
          tt=i;
        end
      elseif (bllst(i).sim(1)=='actionneur'+string(j)) then
        if tt<>i then
          act=[act;i];
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
  ///**********************************************************

  if szclkIN>1 then
    //replace the N Event inputs by a fictious block with
    // 2^N as many event outputs
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
  elseif szclkIN==[] then
    if ~ALL & ~ALWAYS_ACTIVE then
      //superblock has no Event input, add a fictious clock
      output=ones((2^(size(cap,'*')))-1,1)
      if (output == []) then
        output=0;
      end
      bllst($+1)=scicos_model(sim=list('bidon',1),evtout=output,..
                              firing=-output,blocktype='d',dep_ut=[%f %f])
      corinv(size(bllst))=size(bllst)+1;
      howclk=size(bllst);
    else
      //@@ find block with output events
      ind_evtout_blk=[]
      nb_evtout=0
      firing_evtout=[]
      for i=1:size(bllst)
        if bllst(i).evtout<>[] then
           if type(bllst(i).sim)==15 then
             if bllst(i).sim(2)<>-1 & bllst(i).sim(2)<>-2 then
               ind_evtout_blk=[ind_evtout_blk;i]
               for j=1:size(bllst(i).evtout,'*')
                 firing_evtout=[firing_evtout;bllst(i).firing(j)]
               end
               nb_evtout=nb_evtout+size(bllst(i).evtout,'*')
             end
           else
             ind_evtout_blk=[ind_evtout_blk;i]
             for j=1:size(bllst(i).evtout,'*')
               firing_evtout=[firing_evtout;bllst(i).firing(j)]
             end
             nb_evtout=nb_evtout+size(bllst(i).evtout,'*')
           end
        end
      end
      //@@ Add an agenda_blk
      if ind_evtout_blk<>[] then
        bllst($+1)=scicos_model(sim=list('agenda_blk',1),evtout=ones(size(firing_evtout,1),1),..
                                firing=-ones(size(firing_evtout,1),1),blocktype='d',dep_ut=[%f %f])
        agenda_blk = size(bllst);
        corinv(size(bllst))=size(bllst)+1;
        //@@ evt output connection from agenda_blk
        kk=1;
        for i=1:size(ind_evtout_blk,'*')
          for j=1:size(bllst(ind_evtout_blk(i)).evtout,'*')
            for jj=1:size(clkconnect,1)
               if clkconnect(jj,1)==ind_evtout_blk(i) & clkconnect(jj,2)==j then
                 clkconnect(jj,1) = agenda_blk;
                 clkconnect(jj,2) = kk;
               end
            end
            kk=kk+1
          end
        end
      end

    end
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
  if ~ALL then
    //Test for  ALWAYS_ACTIVE sblock (RN -25/06/07)
    // Fady 15/12/2008
    n=size(cap,1)
    if ~(szclkIN==[]) then  //triggered case
      for i=1:n
	if szclkIN>1 then
	  for j=1:(2^szclkIN)-1
	    clkconnect=[clkconnect;[howclk j cap(i) 1]];
	  end
	elseif szclkIN==1 then
	  clkconnect=[clkconnect;[howclk 1 cap(i) 1]];
	end
      end
    elseif ~ALWAYS_ACTIVE then
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
  end
  //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  //@@ Alan, 04/12/08, Remove problematic scopes
  for i=1:lstsize(bllst)
    ind = find(bllst(i).sim(1)==sim_to_be_removed(:,1))
    if ind<>[] then
      mess=mprintf("%s  block is not allowed.\nIt will be not called.',sim_to_be_removed(ind,2))
      okk = messagebox(mess,[_('Ok');_('Go Back')],'modal')
      if okk==1 then
        bllst(i).sim(1)='bidon'
        if type(bllst(i).sim(1))==13 then
          bllst(i).sim(2)=0
        end
      else
        ok=%f
        %cpr=list()
        return
      end
    end
  end
  //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

  FIRING=[]
  for i=1:size(allhowclk2,1)
    j = find(clkconnect(:,3)==allhowclk2(i))
    if j<>[] then
      FIRING=[FIRING;bllst(clkconnect(j,1)).firing(clkconnect(j,2))]
    end
  end

  Code_gene_run=[];
  //******************** To avoid asking for size or type more than one time in incidence_mat*******
  //************************** when creating atomicity *********************************************
  //************************** in other cases it can be done in adjust_all_scs_m *******************
  //************************** Fady 15/12/2008 *****************************************************
  c_pass2=c_pass2;
  if ok then
    [ok,bllst]=adjust_inout(bllst,connectmat);
  end
  
  if ok then
    [ok,bllst]=adjust_typ(bllst,connectmat);
  end
  //*************************************************************************************************
  //## second pass of compilation
  cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv,"silent")

  if cpr==list() then
    ok=%f,
    messagebox(_("Compilation problem\n perhaps an algebraic loop."),'modal','error')
    return,
  end

  // computing the incidence matrix to derive actual block's depu
  [depu_mat,ok]=incidence_mat(bllst,connectmat,clkconnect,cor,corinv)
  if ~ok then
    return
  else
    depu_vec=depu_mat*ones(size(depu_mat,2),1)>0
  end

  //## Detect synchro block and type1
  funs_save   = cpr.sim.funs;
  funtyp_save = cpr.sim.funtyp;
  with_work   = zeros(cpr.sim.nb,1)
  with_synchro = %f
  with_nrd     = %f
  with_type1   = %f

  //## loop on blocks
  for i=1:lstsize(cpr.sim.funs)
    //## look at for funs of type string
    if type(cpr.sim.funs(i))==10 then
      if part(cpr.sim.funs(i),1:10)=='actionneur' then
        cpr.sim.funs(i) ='bidon'
        cpr.sim.funtyp(i) = 0
      elseif part(cpr.sim.funs(i),1:7)=='capteur' then
        cpr.sim.funs(i) ='bidon'
        cpr.sim.funtyp(i) = 0
      elseif cpr.sim.funs(i)=='bidon2' then
        cpr.sim.funs(i) ='bidon'
        cpr.sim.funtyp(i) = 0
      elseif cpr.sim.funs(i)=='agenda_blk' then
        cpr.sim.funs(i) ='bidon'
        cpr.sim.funtyp(i) = 0
      elseif cpr.sim.funs(i)=='affich' then
        cpr.sim.funs(i) ='bidon'
        cpr.sim.funtyp(i) = 0
      elseif cpr.sim.funs(i)=='affich2' then
        cpr.sim.funs(i) ='bidon'
        cpr.sim.funtyp(i) = 0
      end
    end
    //## look at for type of block
    if cpr.sim.funtyp(i) < 0 then
       with_synchro = %t //## with_synchro flag comes global
    elseif cpr.sim.funtyp(i) == 0 then
       with_nrd = %t //## with_nrd flag comes global
    elseif cpr.sim.funtyp(i) == 1 then
       if cpr.sim.funs(i) ~= 'bidon' then
         with_type1 = %t //## with_type1 flag comes global
       end
    end
  end //## end of for

  //**** solve which blocks use work ****//
  BeforeCG_WinList = winsid();

  ierr=execstr('[state,t]=scicosim(cpr.state,0,0,cpr.sim,'+..
               '''start'',scs_m.props.tol)','errcatch')

  //** retrieve all open Scilab windows with winsid()
  AfterCG_WinList = winsid();

  if ierr==0 then
    for i=1:cpr.sim.nb
       if state.iz(i)<>0 then
          with_work(i)=%t
       end
    end
    ierr=execstr('[state,t]=scicosim(state,0,0,cpr.sim,'+..
                 '''finish'',scs_m.props.tol)','errcatch')
  end

  //@@ Remove windows opened by simulation
  xdel(setdiff(AfterCG_WinList,BeforeCG_WinList))
  //*************************************//

  //fix for slow computer
  xpause(10000)

  //@@ retrieve original funs name
  cpr.sim.funs=funs_save;
  cpr.sim.funtyp=funtyp_save;

  //@@ add a work ptr for agenda blk
  for i=cpr.sim.nb:-1:1
    if cpr.sim.funs(i)=='agenda_blk' then
      with_work(i)=%t
      break
    end
  end

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
    if funtyp(i)==3 | funtyp(i)==5 then
      msg=[msg;_('Scilab block''s not allowed')]
//RN   elseif ztyp(i)<>0 then
    //elseif (zcptr(i+1)-zcptr(i))<>0 then
//
      //msg=[msg;'Active zero crossing block''s not allowed']
     elseif (clkptr(i+1)-clkptr(i))<>0 &funtyp(i)>-1 &funs(i)~='bidon' then
//Alan      //msg=[msg;'Regular block generating activation not allowed yet']
    end
    if msg<>[] then messagebox(msg,'modal'),ok=%f,return,end
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
  okk=%f;
  rdnom='foo';
  rpat=pwd();
  if %scicos_libs<>[] then
    libs=sci2exp(%scicos_libs(:)',0);
  else
    libs=''
  end
  label1=[hname;pwd()+filesep()+hname;libs];

  while %t do //loop while the user answer are not ok
    ok=%t  // to avoid infinite loop

    if atomicflag then
      rdnom=all_scs_m.props.title(1)+'_'+strcat(string(super_path),'_')+'_'+string(numk)
      rpat=TMPDIR;
    else

      [okk,..
       rdnom,..
       rpat,..
       libs,..
       label1]=scicos_getvalue(_('Set code generator parameters :'),..
                        [_('New block''s name :');
                         _('Created files Path :');
                         _('Other object files to link with (if any)')],..
                         list('str',1,'str',1,'str',1),label1);
      if okk==%f then ok=-1; return; end
      
      rpat=stripblanks(rpat);

      //** 1/07/06 Alan trying to solve multiple libraries
      if strindex(libs,'''')<>[] | strindex(libs,'""')<>[] then
        ierr=execstr('libs=evstr(libs)','errcatch')
        if ierr<>0  then
          messagebox(_('Can''t solve other files to link'),'modal','error')
          ok=-1;
          return
        end
      end
    end  //if atomicflag then

    if stripblanks(rdnom)==emptystr() then
      ok=%f;
      messagebox(_('sorry C file name is not defined'),'modal','error');
      if atomicflag then return, end
    end

    if ok then
      //@@replace special character which may cause problem to the compiler
      if strindex(rdnom,[' ','-','.'])<>[] then
	new = strsubst(strsubst(strsubst(rdnom,' ','_'),'-','_'),'.','_');
        mess=msprintf(_(' Superblock name cannot contains space, ""."" and \n'+..
			'""-"" characters. \n'+..
			'The superblock will be renamed : %s'),new);
        okk=messagebox(mess,_('Scicos message'),'question',[_('Ok');_('Go Back')],'modal')
        if okk==1 then
          rdnom = new
          label1=[rdnom;label1(2);label1(3)];
        else
           ok=%f;
           if atomicflag then return, end
        end
      end 
    end
    if ok then
      //@@ 13/12/08,
      //@@ Check if rdnom already exists in the linked routine table
      if c_link(rdnom) & ~atomicflag then
        mess=msprintf(_('Warning. An entry point with name ""%s"" is already linked.\n'+..
			'The new generated block may have another name or the \n'+..
			'old entry point will be unlinked\n'),rdnom);
        okk=messagebox(mess,_('Scicos message'),'question',[_('Change block name');_('Force unlink')],'modal')
        if okk==1 then //"Change block name" selected
          ok=%f
        end
      end
    end
    
    if ok then
      //@@ create destination directory
      [status,mess]=mkdir(rpat)
      
      if and(status<>[1 2]) then
	messagebox(mess,'error','modal');
	ok=%f
	if atomicflag then return, end
      end
    end
    
    if ok then    
      //@@ 13/12/08,
      //@@ add a test for %scicos_libs
      if MSDOS then
	target_lib = rpat+'\lib'+rdnom+'.dll'
      else
	target_lib =  rpat+'/lib'+rdnom+'.so'
      end
      ind = find(libs==target_lib)
      if ind<>[] then
	mess=msprintf(_(' Warning. You want to link an external library\n'+..
			'which is the same than the target library.\n'+..
			'That library can be here removed from the\n'+..
			'list of external libraries (only for expert user).'))
	okk=message(mess,_('Scicos Message'),'question',[_('Change block name');_('Ok')],'modal')
	if okk==2 then
	  new_libs=[]
	  for i=1:size(libs,'*')
	    if find(i==ind)==[] then
	      new_libs=[new_libs,libs(i)]
	    end
	  end
	  libs=new_libs
	else
	  ok=%f;
	  if atomicflag then return, end
	end
      end
    end
    if ok then break;end
  end //end of while loop

  //###################################################
  //generate blocs simulation function prototypes
  //and extract infos from ports for sensors/actuators
  //###################################################

  nbcap=0;
  nbact=0;
  capt=[];
  actt=[];
  Protostalone=[];
  Protos=[];
  dfuns=[]

  //## loop on number of blk
  for i=1:length(funs)
    //## block is a sensor
    if or(i==cap) then
      nbcap = nbcap+1;
      //## number of output ports
      nout=outptr(i+1)-outptr(i);
      if nout==0 then
         //yk    = 0;
         //nyk_1 = 0;
         //nyk_2 = 0;
         //yk_t  = 1;
         disp('nout=0 pour un capteur');pause
      else
         yk    = outlnk(outptr(i));
         nyk_1 = size(outtb(yk),1);
         nyk_2 = size(outtb(yk),2);
         yk_t  = mat2scs_c_nb(outtb(yk));
      end
      capt=[capt;
            i yk nyk_1 nyk_2 yk_t bllst(i).ipar]

      //## only one proto for sensor
      if nbcap==1 then
        Protostalone=[Protostalone;
                      '';
                      'void '+rdnom+'_sensor(int *, int *, int *, double *, void *, \';
                      get_blank(rdnom)+'             int *, int *, int *, int, void *);']
      end

    //## block is an actuator
    elseif or(i==act) then
      nbact = nbact+1;
      //## number of input ports
      nin=inpptr(i+1)-inpptr(i);
      if nin==0 then
        //uk    = 0;
        //nuk_1 = 0;
        //nuk_2 = 0;
        //uk_t  = 1;
        disp('nin=0 pour un actionneur');pause
      else
        uk    = inplnk(inpptr(i));
        nuk_1 = size(outtb(uk),1);
        nuk_2 = size(outtb(uk),2);
        uk_t  = mat2scs_c_nb(outtb(uk));
      end
      actt=[actt;
            i uk nuk_1 nuk_2 uk_t bllst(i).ipar]

      //## only one proto for actuator
      if nbact==1 then
        Protostalone=[Protostalone;
                      ''
                      'void '+rdnom+'_actuator(int *, int *, int *, double *, void *, \';
                      get_blank(rdnom)+'               int *, int *, int *, int, void *);']
      end

    //## all other types of blocks excepts evt sensors and evt actuators
    else
      if funs(i)<>'bidon' & funs(i)<>'bidon2' then
        ki=find(funs(i)==dfuns)
        dfuns=[dfuns;funs(i)]
        if ki==[] then
          Protos=[Protos;'';BlockProto(i)];
          Protostalone=[Protostalone;'';BlockProto(i)];
        end
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
  // Compute the initial state and outtb (links) values
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

  //** generate scilab interfacing function
  //   of the generated scicos block
  ok=gen_gui42();

  //** generate code for atomic blocks
  // Fady*******************
  if ok & atomicflag then
    [ok,c_atomic_code]=gen_atomic_ccode42()
  else
    //***********************
    //** generate C files
    //   of the generated scicos block
    if ok then
      ok=gen_ccode42()
    end

    //** Generates Makefile, loader
    //   and compile and link C files

    //** def files to build
    files=[rdnom rdnom+'_Cblocks']
 
    //** def files to build for standalone
    filestan=[rdnom+'_standalone' rdnom+'_act_sens_events' rdnom+'_Cblocks']

    //## def files to build for interfacing of the standalone
    filesint=[rdnom+'_void_io' rdnom+'_Cblocks' 'int'+rdnom rdnom+'_standalone']

    //  In case of fortran blocks  (see =gen_ccode42)
    [fif,ierr]=fileinfo(rpat+'/'+rdnom+'f.f')
    if ierr==0&fif<>[] then 
      files=[files,rdnom+'f'],
      filestan=[filestan,rdnom+'f'],
      filesint=[filesint,rdnom+'f'],
    end
    if ok then
      ok=buildnewblock(rdnom,files,filestan,filesint,libs,rpat,'','')
    end
  end

  if ok then

    if ~ALL then
      //global gui_path
      gui_path=rpat+'/'+rdnom+'_c.sci'

      //exec the gui function
      exec(gui_path)

      //Change diagram superblock to new generated block
      XX=update_block(XX)

      //## update %scicos_libs if needed
      if MSDOS then
        libnam=rpat+'\lib'+rdnom+'.dll'
      else
        libnam=rpat+'/lib'+rdnom+'.so'
      end
      if exists('%scicos_libs') then
        if find(libnam==%scicos_libs)==[] then
          %scicos_libs=[%scicos_libs,libnam];
        end
      else
        %scicos_libs=libnam
      end

      //## resume the interfacing function to the upper-level
      execstr('['+rdnom+'_c,%scicos_libs]=resume('+rdnom+'_c,%scicos_libs)')

    else
      //@@ get new generated block
      XX = gen_allblk()

    end
  end
endfunction
//**----------------------------------------------------------------------------------------
//** Subsidiary functions
//**----------------------------------------------------------------------------------------

//CallBlock : generate C calling sequence
//            of a scicos block
//
//inputs : bk   : bloc index
//         pt   : evt activation number
//         flag : flag
//
//output : txt  :
//
//Copyright INRIA
function txt=call_block42(bk,pt,flag)
  txt=[]
  //**
  if flag==2 & ((zcptr(bk+1)-zcptr(bk))<>0) & pt<0 then

  else
    if flag==2 & ((zptr(bk+1)-zptr(bk))+..
                  (ozptr(bk+1)-ozptr(bk))+..
                  (xptr(bk+1)-xptr(bk)+..
                  with_work(bk))==0 |..
                  pt<=0) & ~(stalone & or(bk==actt(:,1))) then
      return // block without state or continuously activated
    end
  end
  if flag==0 & ((xptr(bk+1)-xptr(bk))==0) then
    return // block without continuous state
  end
  if flag==7 & ((xptr(bk+1)-xptr(bk))==0) then
    return // block without continuous state
  end
  if flag==9 & ((zcptr(bk+1)-zcptr(bk))==0) then
    return // block without continuous state
  end
  if flag==3 & ((clkptr(bk+1)-clkptr(bk))==0) then
    return
  end

  //** adjust pt
  if ~(flag==3 & ((zcptr(bk+1)-zcptr(bk))<>0) |..
       flag==2 & ((zcptr(bk+1)-zcptr(bk))<>0)) then
    pt=abs(pt)
  end

  //## check and adjust function type
  ftyp=funtyp(bk)
  ftyp_tmp=modulo(funtyp(bk),10000)
  if ftyp_tmp>2000 then
    ftyp=ftyp-2000
  elseif ftyp_tmp>1000 then
    ftyp=ftyp-1000
  end

  //** change flag 7 to flag 0 for ftyp<10000
  flagi=flag
  if flag==7 & ftyp < 10000 then
    flag=0;
  end

  //** set nevprt and flag for called block
  txt_nf=['block_'+rdnom+'['+string(bk-1)+'].nevprt = '+string(pt)+';'
          'local_flag = '+string(flag)+';']

  //@@ init evout
  if flag==3 then
    txt_init_evout=['/* initialize evout */'
                    'for(kf=0;kf<block_'+rdnom+'['+string(bk-1)+'].nevout;kf++) {'
                    '  block_'+rdnom+'['+string(bk-1)+'].evout[kf]=-1.;'
                    '}']
  else
    txt_init_evout=[]
  end

  //** add comment
  txt=[get_comment('call_blk',list(funs(bk),funtyp(bk),bk));]

  //** see if its bidon, actuator or sensor
  if funs(bk)=='bidon' then
    txt=[];
    return
  elseif funs(bk)=='bidon2' then
    txt=[];
    return
  //@@ agenda_blk
  elseif funs(bk)=='agenda_blk' then
    txt=[];
    return
  //## sensor
  elseif or(bk==capt(:,1)) then
    ind=find(bk==capt(:,1))
    yk=capt(ind,2);

    txt = [txt_init_evout;
           txt;
           txt_nf
           'nport = '+string(ind)+';']

    txt = [txt;
           rdnom+'_sensor(&local_flag, &nport, &block_'+rdnom+'['+string(bk-1)+'].nevprt, \'
           get_blank(rdnom+'_sensor')+' &t, ('+mat2scs_c_ptr(outtb(yk))+' *)block_'+rdnom+'['+string(bk-1)+'].outptr[0], \'
           get_blank(rdnom+'_sensor')+' &block_'+rdnom+'['+string(bk-1)+'].outsz[0], \'
           get_blank(rdnom+'_sensor')+' &block_'+rdnom+'['+string(bk-1)+'].outsz[1], \'
           get_blank(rdnom+'_sensor')+' &block_'+rdnom+'['+string(bk-1)+'].outsz[2], \'
           get_blank(rdnom+'_sensor')+' block_'+rdnom+'['+string(bk-1)+'].insz[0], \'
           get_blank(rdnom+'_sensor')+' block_'+rdnom+'['+string(bk-1)+'].inptr[0]);']
    //## errors management
    txt = [txt;
           '/* error handling */'
           'if(local_flag < 0) {']
    if stalone then
      txt =[txt;
            '  set_block_error(5 - local_flag);']
      if flag<>5 then
        txt =[txt;
              '  Cosend();']
      end
    else
      txt =[txt;
            '  set_block_error(local_flag);']
      if flag<>5 then
        txt = [txt;
               '  return get_block_error();']
      end
    end
    txt = [txt;
           '}']
    return
  //## actuator
  elseif or(bk==actt(:,1)) then
    ind=find(bk==actt(:,1))
    uk=actt(ind,2)

    txt = [txt_init_evout;
           txt;
           txt_nf
           'nport = '+string(ind)+';']

    txt = [txt;
           rdnom+'_actuator(&local_flag, &nport, &block_'+rdnom+'['+string(bk-1)+'].nevprt, \'
           get_blank(rdnom+'_actuator')+' &t, ('+mat2scs_c_ptr(outtb(uk))+' *)block_'+rdnom+'['+string(bk-1)+'].inptr[0], \'
           get_blank(rdnom+'_actuator')+' &block_'+rdnom+'['+string(bk-1)+'].insz[0], \'
           get_blank(rdnom+'_actuator')+' &block_'+rdnom+'['+string(bk-1)+'].insz[1], \'
           get_blank(rdnom+'_actuator')+' &block_'+rdnom+'['+string(bk-1)+'].insz[2], \'
           get_blank(rdnom+'_actuator')+' block_'+rdnom+'['+string(bk-1)+'].outsz[0], \'
           get_blank(rdnom+'_actuator')+' block_'+rdnom+'['+string(bk-1)+'].outptr[0]);']
    //## errors management
    txt = [txt;
           '/* error handling */'
           'if(local_flag < 0) {']
    if stalone then
      txt =[txt;
            '  set_block_error(5 - local_flag);']
      if flag<>5 then
        txt =[txt;
              '  Cosend();']
      end
    else
      txt =[txt;
            '  set_block_error(local_flag);']
      if flag<>5 then
        txt = [txt;
               '  return get_block_error();']
      end
    end
    txt = [txt;
           '}']
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

  //** check function type
  if ftyp < 0 then //** ifthenelse eselect blocks
      txt = [];
      return;
  else
    if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4  & ftyp<>10004) then
      disp("Types other than 0,1,2,3 or 4/10004 are not supported.")
      txt = [];
      return;
    end
  end

  select ftyp

    case 0 then

      txt=[txt_init_evout;
           txt;
           txt_nf]

      //**** input/output addresses definition ****//
      //## concatenate input
      if nin>1 then
        for k=1:nin
          uk=inplnk(inpptr(bk)-1+k);
          nuk=size(outtb(uk),1);
          //## Alan, 24/04/08 : YAUNEERREURICIFAUTRECOPIERTOUTDANSRDOUTTB
//           txt=[txt;
//                'rdouttb['+string(k-1)+']=(double *)'+rdnom+'_block_outtbptr['+string(uk-1)+'];']

          txt=[txt;
               'rdouttb['+string(k-1)+']=(double *)block_'+rdnom+'['+string(bk-1)+'].inptr['+string(k-1)+'];']

        end
        txt=[txt;
             'args[0]=&(rdouttb[0]);']
      elseif nin==0
        uk=0;
        nuk=0;
        txt=[txt;
             'args[0]=NULL;']
      else
        uk=inplnk(inpptr(bk));
        nuk=size(outtb(uk),1);
        txt=[txt;
             'args[0]=(double *)block_'+rdnom+'['+string(bk-1)+'].inptr[0];']
      end

      //## concatenate outputs
      if nout>1 then
        for k=1:nout
          yk=outlnk(outptr(bk)-1+k);
          nyk=size(outtb(yk),1);
          //## Alan, 24/04/08 : YAUNEERREURICIFAUTRECOPIERTOUTDANSRDOUTTB
//           txt=[txt;
//                'rdouttb['+string(k+nin-1)+']=(double *)'+rdnom+'_block_outtbptr['+string(yk-1)+'];'];
          txt=[txt;
               'rdouttb['+string(k+nin-1)+']=(double *)block_'+rdnom+'['+string(bk-1)+'].outptr['+string(k-1)+'];']
        end
        txt=[txt;
             'args[1]=&(rdouttb['+string(nin)+']);'];
      elseif nout==0
        yk=0;
        nyk=0;
        txt=[txt;
             'args[1]=NULL;'];
      else
        yk=outlnk(outptr(bk));
        nyk=size(outtb(yk),1);
        txt=[txt;
             'args[1]=(double *)block_'+rdnom+'['+string(bk-1)+'].outptr[0];'];
      end
      //*******************************************//

      //@@ This is for compatibility, jroot is returned in g for old type
      if (zcptr(bk+1)-zcptr(bk))<>0 & pt<0 then
        txt=[txt;
             '/* Update g array */'
             'for(i=0;i<block_'+rdnom+'['+string(bk-1)+'].ng;i++) {'
             '  block_'+rdnom+'['+string(bk-1)+'].g[i]=(double)block_'+rdnom+'['+string(bk-1)+'].jroot[i];'
             '}']
      end

      //## adjust continuous state array before call
      if impl_blk & flag==0 then
        txt=[txt;
             '/* adjust continuous state array before call */'
             'block_'+rdnom+'['+string(bk-1)+'].res = &(res['+string(xptr(bk)-1)+']);'];

        //*********** call seq definition ***********//
        txtc=['(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&t,block_'+rdnom+'['+string(bk-1)+'].res, \';
              'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
              'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
              '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
              'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
              '(double *)args[0],(nrd_1='+string(nuk)+',&nrd_1),(double *)args[1],(nrd_2='+string(nyk)+',&nrd_2));'];
      else
        //*********** call seq definition ***********//
        txtc=['(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&t,block_'+rdnom+'['+string(bk-1)+'].xd, \';
              'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
              'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
              '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
             'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
             '(double *)args[0],(nrd_1='+string(nuk)+',&nrd_1),(double *)args[1],(nrd_2='+string(nyk)+',&nrd_2));'];
      end

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

      //## adjust continuous state array after call
      if impl_blk & flag==0 then
        if flagi==7 then
          txt=[txt;
               '/* adjust continuous state array after call */'
               'for (i=0;i<block_'+rdnom+'['+string(bk-1)+'].nx;i++) {'
               '  block_'+rdnom+'['+string(bk-1)+'].xd[i] = block_'+rdnom+'['+string(bk-1)+'].res[i];'
               '}']
        else
          txt=[txt;
               '/* adjust continuous state array after call */'
               'for (i=0;i<block_'+rdnom+'['+string(bk-1)+'].nx;i++) {'
               '  block_'+rdnom+'['+string(bk-1)+'].res[i] = block_'+rdnom+'['+string(bk-1)+'].res[i] - '+...
                 'block_'+rdnom+'['+string(bk-1)+'].xd[i];'
               '}']
        end
      end
      //## errors management
      txt = [txt;
             '/* error handling */'
             'if(local_flag < 0) {']
      if stalone then
        txt =[txt;
              '  set_block_error(5 - local_flag);']
        if flag<>5 then
          txt =[txt;
                '  Cosend();']
        end
      else
        txt =[txt;
              '  set_block_error(local_flag);']
        if flag<>5 then
          txt = [txt;
                 '  return get_block_error();']
        end
      end
      txt = [txt;
             '}']

      if flag==3 then
        //@@ addevs function call
        if ~stalone & ALL & size(evs,'*')<>0 then
          ind=get_ind_clkptr(bk,clkptr,funtyp)
          txt = [txt;
                 '/* addevs function call */'
                 'for(kf=0;kf<block_'+rdnom+'['+string(bk-1)+'].nevout;kf++) {'
                 '  if (block_'+rdnom+'['+string(bk-1)+'].evout[kf]>=t) {'
                 '    '+rdnom+'_addevs(ptr, block_'+rdnom+'['+string(bk-1)+'].evout[kf], '+string(ind)+'+kf);'
                 '  }'
                 '}']
        else
          //@@ adjust values of output register
          //@@ TODO : multiple output event block
          txt = [txt;
                 '/* adjust values of output register */'
                 '/* TODO :  multiple output event block */'
                 'block_'+rdnom+'['+string(bk-1)+'].evout[0] = block_'+rdnom+'['+string(bk-1)+'].evout[0] - t;']
        end
      end
      return

    //**
    case 1 then

      txt=[txt_init_evout;
           txt;
           txt_nf]

      //@@ This is for compatibility, jroot is returned in g for old type
      if (zcptr(bk+1)-zcptr(bk))<>0 & pt<0 then
        txt=[txt;
             '/* Update g array */'
             'for(i=0;i<block_'+rdnom+'['+string(bk-1)+'].ng;i++) {'
             '  block_'+rdnom+'['+string(bk-1)+'].g[i]=(double)block_'+rdnom+'['+string(bk-1)+'].jroot[i];'
             '}']
      end

      //## adjust continuous state array before call
      if impl_blk & flag==0 then
        txt=[txt;
             '/* adjust continuous state array before call */'
             'block_'+rdnom+'['+string(bk-1)+'].res = &(res['+string(xptr(bk)-1)+']);'];

        //*********** call seq definition ***********//
        txtc=['(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&t,block_'+rdnom+'['+string(bk-1)+'].res, \';
              'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
              'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
              '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
              'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar , \'];
      else
        //*********** call seq definition ***********//
        txtc=['(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&t,block_'+rdnom+'['+string(bk-1)+'].xd, \';
              'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
              'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
              '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
              'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar , \'];
      end

      if (funtyp(bk)>2000 & funtyp(bk)<3000)
        blank = get_blank(funs(bk)+'( ');
        txtc(1) = funs(bk)+txtc(1);
      elseif (funtyp(bk)<2000)
        txtc(1) = 'C2F('+funs(bk)+')'+txtc(1);
        blank = get_blank('C2F('+funs(bk)+') ');
      end
      if nin>=1 | nout>=1 then
        if nin>=1 then
          for k=1:nin
            uk=inplnk(inpptr(bk)-1+k);
            txtc=[txtc;
                  '(double *)block_'+rdnom+'['+string(bk-1)+'].inptr['+string(k-1)+'],&block_'+rdnom+'['+string(bk-1)+'].insz['+string(k-1)+'], \';]
          end
        end
        if nout>=1 then
          for k=1:nout
            yk=outlnk(outptr(bk)-1+k);
            txtc=[txtc;
                  '(double *)block_'+rdnom+'['+string(bk-1)+'].outptr['+string(k-1)+'],&block_'+rdnom+'['+string(bk-1)+'].outsz['+string(k-1)+'], \';]
          end
        end
      end

      if ztyp(bk) then
        txtc=[txtc;
              'block_'+rdnom+'['+string(bk-1)+'].g,&block_'+rdnom+'['+string(bk-1)+'].ng);']
      else
        txtc($)=part(txtc($),1:length(txtc($))-3)+');';
      end

      txtc(2:$) = blank + txtc(2:$);
      txt = [txt;txtc];
      //*******************************************//

      //## adjust continuous state array after call
      if impl_blk & flag==0 then
        if flagi==7 then
          txt=[txt;
               '/* adjust continuous state array after call */'
               'for (i=0;i<block_'+rdnom+'['+string(bk-1)+'].nx;i++) {'
               '  block_'+rdnom+'['+string(bk-1)+'].xd[i] = block_'+rdnom+'['+string(bk-1)+'].res[i];'
               '}']
        else
          txt=[txt;
               '/* adjust continuous state array after call */'
               'for (i=0;i<block_'+rdnom+'['+string(bk-1)+'].nx;i++) {'
               '  block_'+rdnom+'['+string(bk-1)+'].res[i] = block_'+rdnom+'['+string(bk-1)+'].res[i] - '+...
                 'block_'+rdnom+'['+string(bk-1)+'].xd[i];'
               '}']
        end
      end
      //## errors management
      txt = [txt;
             '/* error handling */'
             'if(local_flag < 0) {']
      if stalone then
        txt =[txt;
              '  set_block_error(5 - local_flag);']
        if flag<>5 then
          txt =[txt;
                '  Cosend();']
        end
      else
        txt =[txt;
              '  set_block_error(local_flag);']
        if flag<>5 then
          txt = [txt;
                 '  return get_block_error();']
        end
      end
      txt = [txt;
             '}']

      if flag==3 then
        //@@ addevs function call
        if ~stalone & ALL & size(evs,'*')<>0 then
          ind=get_ind_clkptr(bk,clkptr,funtyp)
          txt = [txt;
                 '/* addevs function call */'
                 'for(kf=0;kf<block_'+rdnom+'['+string(bk-1)+'].nevout;kf++) {'
                 '  if (block_'+rdnom+'['+string(bk-1)+'].evout[kf]>=t) {'
                 '    '+rdnom+'_addevs(ptr, block_'+rdnom+'['+string(bk-1)+'].evout[kf], '+string(ind)+'+kf);'
                 '  }'
                 '}']
        else
          //@@ adjust values of output register
          //@@ TODO : multiple output event block
          txt = [txt;
                 '/* adjust values of output register */'
                 '/* TODO :  multiple output event block */'
                 'block_'+rdnom+'['+string(bk-1)+'].evout[0] = block_'+rdnom+'['+string(bk-1)+'].evout[0] - t;']
        end
      end
      return

    //**
    case 2 then

      txt=[txt_init_evout;
           txt;
           txt_nf]

      //@@ This is for compatibility, jroot is returned in g for old type
      if (zcptr(bk+1)-zcptr(bk))<>0 & pt<0 then
        txt=[txt;
             '/* Update g array */'
             'for(i=0;i<block_'+rdnom+'['+string(bk-1)+'].ng;i++) {'
             '  block_'+rdnom+'['+string(bk-1)+'].g[i]=(double)block_'+rdnom+'['+string(bk-1)+'].jroot[i];'
             '}']
      end

      //## adjust continuous state array before call
      if impl_blk & flag==0 then
        txt=[txt;
             '/* adjust continuous state array before call */'
             'block_'+rdnom+'['+string(bk-1)+'].res = &(res['+string(xptr(bk)-1)+']);'];

        //*********** call seq definition ***********//
        txtc=[funs(bk)+'(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&t,block_'+rdnom+'['+string(bk-1)+'].res, \';
              'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
              'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
              '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
              'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
              '(double **)block_'+rdnom+'['+string(bk-1)+'].inptr,block_'+rdnom+'['+string(bk-1)+'].insz,&block_'+rdnom+'['+string(bk-1)+'].nin, \';
              '(double **)block_'+rdnom+'['+string(bk-1)+'].outptr,block_'+rdnom+'['+string(bk-1)+'].outsz, &block_'+rdnom+'['+string(bk-1)+'].nout'];
      else
        //*********** call seq definition ***********//
        txtc=[funs(bk)+'(&local_flag,&block_'+rdnom+'['+string(bk-1)+'].nevprt,&t,block_'+rdnom+'['+string(bk-1)+'].xd, \';
              'block_'+rdnom+'['+string(bk-1)+'].x,&block_'+rdnom+'['+string(bk-1)+'].nx, \';
              'block_'+rdnom+'['+string(bk-1)+'].z,&block_'+rdnom+'['+string(bk-1)+'].nz,block_'+rdnom+'['+string(bk-1)+'].evout, \';
              '&block_'+rdnom+'['+string(bk-1)+'].nevout,block_'+rdnom+'['+string(bk-1)+'].rpar,&block_'+rdnom+'['+string(bk-1)+'].nrpar, \';
              'block_'+rdnom+'['+string(bk-1)+'].ipar,&block_'+rdnom+'['+string(bk-1)+'].nipar, \';
              '(double **)block_'+rdnom+'['+string(bk-1)+'].inptr,block_'+rdnom+'['+string(bk-1)+'].insz,&block_'+rdnom+'['+string(bk-1)+'].nin, \';
              '(double **)block_'+rdnom+'['+string(bk-1)+'].outptr,block_'+rdnom+'['+string(bk-1)+'].outsz, &block_'+rdnom+'['+string(bk-1)+'].nout'];
      end

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

      //## adjust continuous state array after call
      if impl_blk & flag==0 then
        if flagi==7 then
          txt=[txt;
               '/* adjust continuous state array after call */'
               'for (i=0;i<block_'+rdnom+'['+string(bk-1)+'].nx;i++) {'
               '  block_'+rdnom+'['+string(bk-1)+'].xd[i] = block_'+rdnom+'['+string(bk-1)+'].res[i];'
               '}']
        else
          txt=[txt;
               '/* adjust continuous state array after call */'
               'for (i=0;i<block_'+rdnom+'['+string(bk-1)+'].nx;i++) {'
               '  block_'+rdnom+'['+string(bk-1)+'].res[i] = block_'+rdnom+'['+string(bk-1)+'].res[i] - '+...
                 'block_'+rdnom+'['+string(bk-1)+'].xd[i];'
               '}']
        end
      end
      //## errors management
      txt = [txt;
             '/* error handling */'
             'if(local_flag < 0) {']
      if stalone then
        txt =[txt;
              '  set_block_error(5 - local_flag);']
        if flag<>5 then
          txt =[txt;
                '  Cosend();']
        end
      else
        txt =[txt;
              '  set_block_error(local_flag);']
        if flag<>5 then
          txt = [txt;
                 '  return get_block_error();']
        end
      end
      txt = [txt;
             '}']

      if flag==3 then
        //@@ addevs function call
        if ~stalone & ALL & size(evs,'*')<>0 then
          ind=get_ind_clkptr(bk,clkptr,funtyp)
          txt = [txt;
                 '/* addevs function call */'
                 'for(kf=0;kf<block_'+rdnom+'['+string(bk-1)+'].nevout;kf++) {'
                 '  if (block_'+rdnom+'['+string(bk-1)+'].evout[kf]>=t) {'
                 '    '+rdnom+'_addevs(ptr, block_'+rdnom+'['+string(bk-1)+'].evout[kf], '+string(ind)+'+kf);'
                 '  }'
                 '}']
        else
          //@@ adjust values of output register
          //@@ TODO : multiple output event block
          txt = [txt;
                 '/* adjust values of output register */'
                 '/* TODO :  multiple output event block */'
                 'block_'+rdnom+'['+string(bk-1)+'].evout[0] = block_'+rdnom+'['+string(bk-1)+'].evout[0] - t;']
        end
      end
      return

    //**
    case 4 then

      txt=[txt_init_evout;
           txt;
           txt_nf]

      //## adjust continuous state array before call
      if impl_blk & flag==0 then
        txt=[txt;
             '/* adjust continuous state array before call */'
             'block_'+rdnom+'['+string(bk-1)+'].xd  = &(res['+string(xptr(bk)-1)+']);'
             'block_'+rdnom+'['+string(bk-1)+'].res = &(res['+string(xptr(bk)-1)+']);'];
      end

      txt=[txt;
           funs(bk)+'(&block_'+rdnom+'['+string(bk-1)+'],local_flag);'];

      //## adjust continuous state array after call
      if impl_blk & flag==0  then
        if flagi==7 then
          txt=[txt;
               '/* adjust continuous state array after call */'
               'block_'+rdnom+'['+string(bk-1)+'].xd = &(xd['+string(xptr(bk)-1)+']);'
               'for (i=0;i<block_'+rdnom+'['+string(bk-1)+'].nx;i++) {'
               '  block_'+rdnom+'['+string(bk-1)+'].xd[i] = block_'+rdnom+'['+string(bk-1)+'].res[i];'
               '}']
        else
          txt=[txt;
               '/* adjust continuous state array after call */'
               'block_'+rdnom+'['+string(bk-1)+'].xd = &(xd['+string(xptr(bk)-1)+']);'
               'for (i=0;i<block_'+rdnom+'['+string(bk-1)+'].nx;i++) {'
               '  block_'+rdnom+'['+string(bk-1)+'].res[i] = block_'+rdnom+'['+string(bk-1)+'].res[i] - '+...
                 'block_'+rdnom+'['+string(bk-1)+'].xd[i];'
               '}']
        end
      end

    //**
    case 10004 then

      txt=[txt_init_evout;
           txt;
           txt_nf]

      txt=[txt;
           funs(bk)+'(&block_'+rdnom+'['+string(bk-1)+'],local_flag);'];

  end

  //## errors management
  if stalone then
    txt =[txt;
          '/* error handling */'
          'if (get_block_error() < 0) {'
          '  set_block_error(5 - get_block_error());']
    if flag<>5 then
        txt = [txt;
               '  Cosend();']
    end
    txt =[txt;
          '}']
  else
    if flag<>5 then
      txt =[txt;
            '/* error handling */'
            'if (get_block_error() < 0) {'
            '  return get_block_error();'
            '}']
    end
  end

  //@@ addevs function call
  if flag==3 then
    if ~stalone & ALL & size(evs,'*')<>0 then
      ind=get_ind_clkptr(bk,clkptr,funtyp)
      txt = [txt;
             '/* addevs function call */'
             'for(kf=0;kf<block_'+rdnom+'['+string(bk-1)+'].nevout;kf++) {'
             '  if (block_'+rdnom+'['+string(bk-1)+'].evout[kf]>=0.) {'
             '    '+rdnom+'_addevs(ptr, block_'+rdnom+'['+string(bk-1)+'].evout[kf] + t, '+string(ind)+'+kf);'
             '  }'
             '}']
    else
      //@@ adjust values of output register
      //@@ TODO : multiple output event block
      txt=txt
    end
  end

endfunction

function XX=gen_allblk()
//@@ creates the Scicos C generick block
//@@ associated to an entire diagram
//@@ generated code
//
//@@ 28/09/08, Alan : initial rev
//@@
//@@ Copyright INRIA

  //@@ adjust oz
  oz=cpr.state.oz;
  //new_oz=list();
  //for i=1:lstsize(oz)
  //  new_oz($+1) = oz(i)
  //end
  //for i=1:lstsize(outtb)
  //  new_oz($+1) = outtb(i)
  //end

  new_oz_str=[];
  for i=1:lstsize(oz)
    new_oz_str = [new_oz_str , sci2exp(oz(i),0)];
  end
  for i=1:lstsize(outtb)
    new_oz_str = [new_oz_str , 'zeros('+string(size(outtb(i),1))+','+string(size(outtb(i),2))+')'];
  end

  new_oz_str='list('+strcat(new_oz_str,',')+')';

  //@@ adjust z
  work=zeros(nblk,1);
  Z=[z;work]

  //@@ get nmode
  nmode = cpr.sim.modptr($)-1

  //@@ get nzcross
  nzcross = cpr.sim.zcptr($)-1

  //@@ get firing
  //firing = min(firing_evtout)
  if find(firing_evtout>=0)<>[] then
    firing = min(firing_evtout(find(firing_evtout>=0)))
  else
    firing = min(firing_evtout)
  end

  //@@ get rpar/ipar/opar
  rpar = cpr.sim.rpar
  ipar = cpr.sim.ipar
  opar = cpr.sim.opar

  //@@ get a new CBLOCK4
  XX             = CBLOCK4('define')

  //@@ set the size
  XX.graphics.sz = 20 *XX.graphics.sz

  //@@ load computational function
  toto=mgetl(rpat+'/'+rdnom+'.c')

  //@@ set the graphics exprs
  XX.graphics.exprs(1)(1)  = rdnom             //simulation function
  if impl_blk then
    XX.graphics.exprs(1)(2)  = 'y'            //implicit blck
  end
  XX.graphics.exprs(1)(3)  = '[]'              //regular input port size
  XX.graphics.exprs(1)(5)  = '[]'              //regular output port size
  XX.graphics.exprs(1)(7)  = '1'               //event input port size
  XX.graphics.exprs(1)(8)  = '1'               //event output port size
  XX.graphics.exprs(1)(9)  = sci2exp(x,0)      //continuous state
  XX.graphics.exprs(1)(10) = sci2exp(Z,0)      //discrete state
  //XX.graphics.exprs(1)(11) = sci2exp(new_oz,0) //object state
  XX.graphics.exprs(1)(11) = new_oz_str        //object state
  XX.graphics.exprs(1)(12) = sci2exp(rpar,0)   //real parameters
  XX.graphics.exprs(1)(13) = sci2exp(ipar,0)   //ipar parameters
  XX.graphics.exprs(1)(14) = sci2exp(opar,0)   //opar parameters
  XX.graphics.exprs(1)(15) = sci2exp(nmode)    //number of modes
  XX.graphics.exprs(1)(16) = sci2exp(nzcross)  //number of zero crossings
  XX.graphics.exprs(1)(17) = sci2exp(firing)   //initial event date
  XX.graphics.exprs(1)(18) = 'n'               //direct feedthrough
  if ALWAYS_ACTIVE then
    XX.graphics.exprs(1)(19) = 'y'             //time dependence
  end

  //@@ put text of computational function
  //@@ in CBLOCK
  XX.graphics.exprs(2)=toto

  //@@ run 'set' job of the CBLOCK4
  prot=funcprot();funcprot(0)
  getvalue=setvalue;
  deff('message(txt)',['messagebox(''In block ''+XX.gui+'': ''+txt,''Warning'',''info'',''modal'');'
		    'global %scicos_prob;'
		    '%scicos_prob=%t'])

  %scicos_prob = %f
  XX = CBLOCK4('set',XX)
  funcprot(prot)

endfunction

function [ok,c_atomic_code]=gen_atomic_ccode42();
  //** Generate code for atomic scicos block
  //** Fady Nassif, inital rev 10/12/07
  Code=make_computational42()
  [CCode,FCode]=gen_blocks()
  flag_no_ccode = %f;
  for i=1:size(CCode,1)
    if CCode(i)=='void no_ccode()' then flag_no_ccode = %t, end
    if flag_no_ccode then
      if CCode(i)=='}' then flag_no_ccode = %f, end
      CCode(i)='//'+CCode(i)
    end
  end
  c_atomic_code=[Code;CCode]
  //c_atomic_code=Code
endfunction

function [CCode,FCode]=gen_blocks()
//Generates Code for dynamically linked Fortran and C Blocks
  CCode=[]
  FCode=[]

  //## look at for the modelica block
  kdyn=find(funtyp==10004);
  ind=[]
  if (size(kdyn,'*')>=1)
    for i=1:size(kdyn,'*')
      if type(corinv(kdyn(i)))==15 then
        ind=kdyn(i);
        break;
      end
    end
  end

  if ind<>[] then
    CCode=mgetl(TMPDIR+'/'+funs(ind)+'.c');
  end

  //## remove implicit number
  funtyp=modulo(funtyp,10000)

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
//Copyright INRIA
function ok=gen_ccode42()
//** 30/06/2009 S. Steer : 
//  -  WriteCodeToFile definition and use for a clearer code + localization
//  -  use of copyfile instead of mputl(..,mgetl())  


  //** Generate code for scicos block
  ok=WriteCodeToFile(make_computational42(),rpat+'/'+rdnom+'.c')
  if ~ok then return,end

  //** Generate files for dynamically linked scicos blocks
  [CCode,FCode]=gen_blocks()
  if FCode<>[] then
    ok=WriteCodeToFile(FCode,rpat+'/'+rdnom+'f.f')
    if ~ok then return,end
  end
  if CCode<>[] then
    ok=WriteCodeToFile(CCode,rpat+'/'+rdnom+'_Cblocks.c')
    if ~ok then return,end
  end

  //** Generate _void_io.c
  ok=WriteCodeToFile(make_void_io(),rpat+'/'+rdnom+'_void_io.c')
  if ~ok then return,end

  //** Generate _standalone.c
  ok=WriteCodeToFile(make_standalone42(),rpat+'/'+rdnom+'_standalone.c')
  if ~ok then return,end

  //## Generate intrdnom.c
  ok=WriteCodeToFile(make_sci_interf(),rpat+'/int'+rdnom+'.c')
  if ~ok then return,end


  //copy scicos_block4.h and machine.h include files in destination directory
  // scicos_block4.h  
  from = scicos_include_paths('scicos_blocks')+'/scicos_block4.h'
  to   = rpat+'/scicos_block4.h';
  [ierr,mess] = copyfile(from,to)
  if ierr==0 then  messagebox(mess,'modal','error'); ok=%f;return;end

  // machine.h
  from = scicos_include_paths('core')+'/machine.h'
  to   = rpat+'/machine.h';
  [ierr,mess] = copyfile(from,to)
  if ierr==0 then  messagebox(mess,'modal','error'); ok=%f;return;end


  //## Generate _act_sens_events.c
  [Code,reponse]=make_act_sens_events()
  if reponse==1 |  reponse==[] then
    ok=WriteCodeToFile(Code,rpat+'/'+rdnom+'_act_sens_events.c')
    if ~ok then return,end
  end
  
endfunction

//creates the Scicos GUI function associated with the new block
//
//Author : Rachid Djenidi, A.Layec
//
//20/06/07, A.Layec : update with in2,out2,intyp,outtyp
//27/06/07, A.Layec : update opar,oz
//
//Copyright INRIA
function ok=gen_gui42();
  clkinput=ones(clkIN)';
  clkoutput=ones(clkOUT)';
  //outtb=outtb;
  oz=cpr.state.oz;

//   new_oz=list();
//   for i=1:lstsize(oz)
//     new_oz($+1) = oz(i)
//   end
//   for i=1:lstsize(outtb)
//     new_oz($+1) = outtb(i)
//   end

  new_oz_str=[];
  for i=1:lstsize(oz)
    new_oz_str = [new_oz_str , sci2exp(oz(i),0)];
  end
  for i=1:lstsize(outtb)
    new_oz_str = [new_oz_str , 'zeros('+string(size(outtb(i),1))+','+string(size(outtb(i),2))+')'];
  end

  new_oz_str='list('+strcat(new_oz_str,',...'+ascii(10)+'                ')+')';

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
        '   odstate='+new_oz_str
        //'   '+sci2exp(new_oz,'odstate',70);
        '   '+sci2exp(cpr.sim.rpar,'rpar',70); //real parameters
        '   '+sci2exp(cpr.sim.ipar,'ipar',70); //integer parameters
        '   '+sci2exp(cpr.sim.opar,'opar',70); //object parameters
        '   '+sci2exp(clkinput,'clkinput',70);
        '   '+sci2exp(clkoutput,'clkoutput',70);
        '   '+sci2exp(FIRING,'firing',70);
        '   nzcross='+string(sum(cpr.sim.zcptr(2:$)-cpr.sim.zcptr(1:$-1)))';
        '   nmode='+string(sum(cpr.sim.modptr(2:$)-cpr.sim.modptr(1:$-1)))';]

  for i=1:length(bllst)
    deput=[depu_vec',%f]
    if (bllst(i).dep_ut($) == %t) then
      deput(1,$)=%t;
      break;
    end
  end
  Code($+1)='   '+sci2exp(deput,'dep_ut',70);
  if impl_blk then
    Code=[Code
          '   model=scicos_model(sim=list('''+rdnom+''',10004),..']
  else
    Code=[Code
          '   model=scicos_model(sim=list('''+rdnom+''',4),..']
  end
  Code=[Code
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
  ok=WriteCodeToFile(Code,rpat+'/'+rdnom+'_c.sci')
endfunction



//BlockProto : generate prototype
//            of a scicos block
//
//inputs : bk   : bloc index
//
//output : txt  :
//
//Copyright INRIA
function [txt]=BlockProto(bk)

  nin=inpptr(bk+1)-inpptr(bk);  //* number of input ports */
  nout=outptr(bk+1)-outptr(bk); //* number of output ports */

  //## check and adjust function type
  ftyp=funtyp(bk)
  ftyp_tmp=modulo(funtyp(bk),10000)
  if ftyp_tmp>2000 then
    ftyp=ftyp-2000
  elseif ftyp_tmp>1000 then
    ftyp=ftyp-1000
  end

  //** check function type
  if ftyp < 0 then //** ifthenelse eselect blocks
      txt = [];
      return;
  else
    if (ftyp<>0 & ftyp<>1 & ftyp<>2 & ftyp<>3 & ftyp<>4  & ftyp<>10004) then
      disp("types other than 0,1,2,3 or 4/10004 are not yet supported.")
      txt = [];
      return;
    end
  end

  //@@ agenda_blk
  if funs(bk)=='agenda_blk' then
    txt=[]
    return;
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

    //**
    case 10004 then
      txt=[txt;
           'void '+funs(bk)+'(scicos_block *, int );'];

  end
endfunction

//Generating the routine for actuators
//sensors & events
//
//Copyright INRIA
function [Code,reponse]=make_act_sens_events()
  Code=['#include <stdio.h>'
        '#include <stdlib.h>'
        '#include <math.h>'
        '#include <string.h>'
        '/* define file ptr to read/write signals data*/'
        'FILE *fprr,*fprw;'
        make_outevents()
        make_actuator()
        make_sensor()]

  created=[];
  reponse=[];

  created=fileinfo(rpat+'/'+rdnom+'_act_sens_events.c')
  if created~=[] then
    mess=msprintf(_('File: ""%s"" already exists,\n'+..
		    'do you want to replace it ?'),rdnom+'_act_sens_events.c')
    reponse=messagebox(mess,_('Scicos message'),'question',['Yes','No'],'modal');
  end
endfunction


//Generating the routine for actuators interfacing
//
//Copyright INRIA
//
//Authors : R. Djenid, R. Nikoukhah, A. Layec
//
//actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]
function Code=make_actuator()

  //## 
  Call=['/*'+part('-',ones(1,40))+' Actuators */';
        'void '+rdnom+'_actuator(flag,nport,nevprt,t,u,nu1,nu2,ut,typout,outptr)']

  //## 
  comments=['     /*'
            '      * To be customized for standalone execution';
            '      * flag   : specifies the action to be done'
            '      * nport  : specifies the  index of the Super Bloc'
            '      *          regular input (The input ports are numbered'
            '      *          from the top to the bottom )'
            '      * nevprt : indicates if an activation had been received'
            '      *          0 = no activation'
            '      *          1 = activation'
            '      * t      : the current time value'
            '      * u      : the vector inputs value'
            '      * nu1    : the input size 1'
            '      * nu2    : the input size 2'
            '      * ut     : the input type'
            '      * typout : learn mode (0 from terminal,1 from input file)'
            '      * outptr : pointer to out data'
            '      *          typout=0, outptr not used'
            '      *          typout=1, outptr contains the output file name'
            '      */']

  //## 
  dcl=['     int *flag,*nevprt,*nport;'
       '     int *nu1,*nu2,*ut;'
       ''
       '     int typout;'
       '     void *outptr;'
       ''
       '     double *t;'
       '     void *u;'
       '{'
       '  int k,l;'];

  //## code for terminal
  a_actuator=['    /* skeleton to be customized */'
              '    switch (*flag) {'
              '    case 4 : /* actuator initialisation */'
              '      /* do whatever you want to initialize the actuator */'
              '      break;']

  if szclkIN==[]&ALWAYS_ACTIVE then
    a_actuator=[a_actuator;
                '    case 1 :']
  else
    a_actuator=[a_actuator;
                '    case 2 :']
  end

  a_actuator=[a_actuator;
              '      /*if(*nevprt>0) { print the input value */'
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
              '    case 5 : /* actuator ending */'
              '      /* do whatever you want to end the actuator */'
              '      break;'
              '    }']

  //## code for output file
  b_actuator=['    /* skeleton to be customized */'
              '    switch (*flag) {'
              '    case 4 : /* actuator initialisation */'
              '      if (*nport == 1) {'
              '        fprw=fopen((char *) outptr,'"wt'");'
              '        if( fprw == NULL ) {'
              '          printf('"Error opening file: %s\n'", (char *) outptr);'
              '          return;'
              '        }'
              '      }'
              '      break;']

  if szclkIN==[]&ALWAYS_ACTIVE then
    b_actuator=[b_actuator;
                '    case 1 : /* fprintf the input value */']
  else
    b_actuator=[b_actuator;
                '    case 2 : /* fprintf the input value */']
  end

  b_actuator=[b_actuator
              '      /*if(*nevprt>0) {*/'
              '        /* write time */'
              '        fprintf(fprw,""%f "",*t);'
              ''
              '        switch (*ut) {'
              '        case 10 :'
              '          for (l=0;l<*nu2;l++) {'
              '            for (k=0;k<*nu1;k++) {'
              '              fprintf(fprw,""%f "", \'
              '                           *((double *) u+(k+l*(*nu1))));'
              '            }'
              '          }'
              '          fprintf(fprw,""\n"");'
              '          break;'
              ''
              '        case 11 :'
              '          for (l=0;l<*nu2;l++) {'
              '            for (k=0;k<*nu1;k++) {'
              '              fprintf(fprw,""%f "" \'
              '                           ""%f "", \'
              '                           *((double *) u+(k+l*(*nu1))), \'
              '                           *((double *) u+((*nu1)*(*nu2)+k+l*(*nu1))));'
              '            }'
              '          }'
              '          fprintf(fprw,""\n"");'
              '          break;'
              ''
              '        case 81 :'
              '          for (l=0;l<*nu2;l++) {'
              '            for (k=0;k<*nu1;k++) {'
              '              fprintf(fprw,""%i "", \'
              '                           *((char *) u+(k+l*(*nu1))));'
              '            }'
              '          }'
              '          fprintf(fprw,""\n"");'
              '          break;'
              ''
              '        case 82 :'
              '          for (l=0;l<*nu2;l++) {'
              '            for (k=0;k<*nu1;k++) {'
              '              fprintf(fprw,""%hd "", \'
              '                           *((short *) u+(k+l*(*nu1))));'
              '            }'
              '          }'
              '          fprintf(fprw,""\n"");'
              '          break;'
              ''
              '        case 84 :'
              '          for (l=0;l<*nu2;l++) {'
              '            for (k=0;k<*nu1;k++) {'
              '              fprintf(fprw,""%ld "", \'
              '                           *((long *) u+(k+l*(*nu1))));'
              '            }'
              '          }'
              '          fprintf(fprw,""\n"");'
              '          break;'
              ''
              '        case 811 :'
              '          for (l=0;l<*nu2;l++) {'
              '            for (k=0;k<*nu1;k++) {'
              '              fprintf(fprw,""%d \n"", \'
              '                           *((unsigned char *) u+(k+l*(*nu1))));'
              '            }'
              '          }'
              '          fprintf(fprw,""\n"");'
              '          break;'
              ''
              '        case 812 :'
              '          for (l=0;l<*nu2;l++) {'
              '            for (k=0;k<*nu1;k++) {'
              '              fprintf(fprw,""%hu "", \'
              '                           *((unsigned short *) u+(k+l*(*nu1))));'
              '            }'
              '          }'
              '          fprintf(fprw,""\n"");'
              '          break;'
              ''
              '        case 814 :'
              '          for (l=0;l<*nu2;l++) {'
              '            for (k=0;k<*nu1;k++) {'
              '              fprintf(fprw,""%lu "", \'
              '                           *((unsigned long *) u+(k+l*(*nu1))));'
              '            }'
              '          }'
              '          fprintf(fprw,""\n"");'
              '          break;'
              '        }'
              '      /*} */'
              '      break;'
              '    case 5 : /* actuator ending */'
              '      if (*nport == 1) {'
              '        fclose(fprw);'
              '      }'
              '      break;'
              '    }']


  // pour fprintf
  nc=size(act,'*')

//   typ=['""%f ']; //time
//   for i=1:nc
//     typ($)=typ($)+'"" \'
//     typ=[typ;'""'];
//     for j=1:sum(actt(i,3)*actt(i,4))
//       //typ=typ+'%f ';
//       typ($)=typ($)+scs_c_n2c_fmt(actt(i,5))+' ';
//     end
//   end
//   typ($)=typ($)+'\n"", \'
//   typ(1)='    fprintf(fprw,'+typ(1);
//   bl    ='                 ';
//   if size(typ,1) <> 1 then
//     typ(2:$) = bl+typ(2:$);
//   end
//   //Code1='    fprintf(fprw, '"'+typ+' \n'",*t'
//   Code1=[typ;bl+'*t'];
//   //actt=[i uk nuk_1 nuk_2 uk_t bllst(i).ipar]
//   for i=1:size(actt,1)
//     ni=actt(i,3)*actt(i,4) // dimension of ith output
//     Code1($)=Code1($)+','
//     Code1=[Code1;bl];
//     for j=1:ni
//       if actt(i,5)<>11 then
//         Code1($)=Code1($)+...
//                   '*((('+scs_c_n2c_typ(actt(i,5))+' *)('+...
//                    rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'))'+...
//                    '+'+string(j-1)+')';
//       else //CAS CMPLX
//         Code1($)=Code1($)+...
//                   '*((('+scs_c_n2c_typ(actt(i,5))+' *)('+...
//                    rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'))'+...
//                    '+'+string((j-1))+'), '+...
//                   '*((('+scs_c_n2c_typ(actt(i,5))+' *)('+...
//                    rdnom+'_block_outtbptr+'+string(actt(i,2)-1)+'))'+...
//                    '+'+string(ni+(j-1))+')';
//       end
//       if j<>ni then
//        Code1($)=Code1($)+', ';
//       end
//     end
//   end
//   Code1($)=Code1($)+');'

  Code=[]

  if nc==1 then
    Code=[Call
          comments
          dcl
          '  if (typout == 0) { /* terminal */'
          a_actuator
          '  }'
          '  else if (typout == 1) { /* file */'
          b_actuator
          '  }'
          '}']
  elseif nc>1 then
    S='    switch (*nport) {'
    for k=1:nc
      S=[S;
         '    case '+string(k)+' :/* Port number '+string(k)+' ----------*/'
         '    '+a_actuator
         '    break;']
    end
    S=[S;'    }']

    T='    switch (*nport) {'
    for k=1:nc
      T=[T;
         '    case '+string(k)+' :/* Port number '+string(k)+' ----------*/'
         '    '+b_actuator
         '    break;']
    end
    T=[T;'    }']

    Code=[Code
          Call
          comments
          dcl
          '  if (typout == 0) { /* terminal */'
          S
          '  }'
          '  else if (typout == 1) { /* file */'
          T
          '  }'
          '}']
  end
endfunction

//** Generates the scicos computational function
//   associated with the block
//12/07/07 Alan Layec
//Copyright INRIA
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
        get_scicos_version()+' */';
        '/*     date : '+date()+' */'
        ''
        '/* ---- Headers ---- */'
        '#include <stdio.h>'
        '#include <stdlib.h>'
        '#include <memory.h>'
        '#include <string.h>'
        '#include <machine.h>'
        '#include <link.h>'
        '#include <scicos.h>'
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
        Protos]

  //** find activation number
  blks=find(funtyp>-1);
  evs=[];

  if ~ALL then
    for blk=blks
      for ev=clkptr(blk):clkptr(blk+1)-1
        if funs(blk)=='bidon' then
          if ev > clkptr(howclk) -1
          evs=[evs,ev];
         end
        end
      end
    end
  else
    for blk=blks
      for ev=clkptr(blk):clkptr(blk+1)-1
        if funs(blk)=='agenda_blk' then
          nb_agenda_blk=blk
          //if ev > clkptr(howclk) -1
          evs=[evs,ev];
         //end
        end
      end
    end
  end

  if ALL & size(evs,'*')<>0 then
    Code=[Code;
          ''
          '/* define agenda struct */'
          'typedef struct {'
          '  int pointi;'
          '  int fromflag3;'
          '  int old_pointi;'
          '  int evtspt['+string(size(evs,'*'))+'];'
          '  double tevts['+string(size(evs,'*'))+'];'
          '} agenda_struct ;'
          ''
          '/* prototype of addevs function */'
          'void '+rdnom+'_addevs(agenda_struct *, double, int);'
         ]
  end

  Code=[Code;
        ''
        '/*'+part('-',ones(1,40))+' Block Computational function */ ';
        'void '+rdnom+'(scicos_block *block, int flag)'
        '{']

  //@@ TOBE IMPROVED
  Code=[Code;
        ''
        '  /* Some general static variables */'
        '  static double zero=0;'
        '  static double w[1];'
        ''
        '  /* declaration of local variables for that block struct */'
        '  double* z      = block->z;'
        '  void **ozptr   = block->ozptr;']

  if max(opptr)>1 then
    Code=[Code;
          '  void **oparptr = block->oparptr;'
          '  /*int nopar      = block->nopar;*/']
  end

  //## Add res if blk contains continuous state register
  if max(xptr)>1 then
    Code=[Code;
          '  double* x      = block->x;'
          '  double* xd     = block->xd;']
  end

  //## Add res if blk is implicit
  if impl_blk then
    Code=[Code;
          '  double* res    = block->res;'
          '  int *xprop     = block->xprop;']
  end

  if size(capt,'*')>0 then
    Code=[Code;
          '  void** u       = block->inptr;']
  end

  Code=[Code;
        '  void** y       = block->outptr;']

  //##12/04/08, Alan's patch : look at for use of
  // ipar,rpar (to disable warning)
  with_rpar=%f;
  with_ipar=%f;
  with_nrd2=%f;

  for kf=1:nblk
    //## all blocks without sensor/actuator
    if (part(funs(kf),1:7) ~= 'capteur' &...
        part(funs(kf),1:10) ~= 'actionneur' &...
        funs(kf) ~= 'bidon' &...
        funs(kf) ~= 'bidon2') then
      //** rpar **//
      if (rpptr(kf+1)-rpptr(kf)>0) then
        with_rpar=%t;
      end
      //** ipar **//
      if (ipptr(kf+1)-ipptr(kf))>0 then
        with_ipar=%t;
      end
      //## with_nrd2 ##//
      if funtyp(kf)==0 then
        with_nrd2=%t;
      end
    end
  end

  Code=[Code;
        '  int nevprt     = block->nevprt;']

  if with_rpar then
    Code=[Code;
          '  double* rpar   = block->rpar;'
          '  /*int nrpar      = block->nrpar;*/']
  end

  if with_ipar then
    Code=[Code;
          '  int* ipar      = block->ipar;'
          '  /*int nipar      = block->nipar;*/']
  end

  if ALL & size(evs,'*')<>0 then
    Code=[Code;
          '  double *evout  = block->evout;']
  end

  if max(zcptr)>1 then
    Code=[Code;
          '  double* g      = block->g;'
          '  int* jroot     = block->jroot;']
  end
  if max(modptr)>1 then
    Code=[Code;
          '  int* mode      = block->mode;']
  end
  Code=[Code;
        '  void **work    = block->work;'
        '']

  if ALL & size(evs,'*')<>0 then
    Code=[Code;
          '  /* agenda struct ptr */'
          '  agenda_struct *ptr;'
          '']
  end

//  if (with_nrd&with_nrd2) | with_type1 then
    Code=[Code;
          '  /* time is given in argument of function block */'
          '  double t     = get_scicos_time();']
//  end

  if (max(zcptr)>1 | max(modptr)>1) & with_synchro then
    Code=[Code;
          '  int    phase = get_phase_simulation();']
  end

  Code=[Code;
        '']

  if with_nrd then
    if with_nrd2 then
      Code=[Code;
            '  /* Variables for constant values */'
            '  int nrd_1, nrd_2;'
            ''
            '  double *args[100];'
            '']
    end
  end

  Code=[Code;
        '  int kf;']

  if with_synchro | impl_blk | (zcptr($)-1)~=0 then
    Code=[Code;
          '  int i;']
  end

  if ALL & size(evs,'*')<>0 then
    Code=[Code;
          '  int kever;']
  end

  Code=[Code;
        '  int* reentryflag;'
        ''
        '  int local_flag;'
        //'  int nport;'
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
        '  if (flag != 4 && flag != 6 && flag != 5) {']

  //## adjust ptr array of continuous state before call
  txt = []
  block_has_output=%f
  for kf=1:nblk
     nx=xptr(kf+1)-xptr(kf);
     if nx <> 0 then
       txt=[txt;
            '    block_'+rdnom+'['+string(kf-1)+'].xd    = &(xd['+...
             string(xptr(kf)-1)+']);'
            '    block_'+rdnom+'['+string(kf-1)+'].x     = &(x['+...
             string(xptr(kf)-1)+']);']
       if funtyp(kf)>10000 then
        txt=[txt;
             '    block_'+rdnom+'['+string(kf-1)+'].res   = &(res['+...
              string(xptr(kf)-1)+']);'
             '    block_'+rdnom+'['+string(kf-1)+'].xprop = &(xprop['+...
              string(xptr(kf)-1)+']);']
       end
       if part(funs(kf),1:10) == 'actionneur' then
         block_has_output=%t
       end
     end
  end

  if txt <> [] then
    Code=[Code;
          '    /* Adjust ptr array of continuous state */'
          txt
          ''];
  end

  //## adjust ptr array of zero crossing before call
  txt = []
  for kf=1:nblk
     ng=zcptr(kf+1)-zcptr(kf);
     if ng <> 0 then
       txt=[txt;
            '    block_'+rdnom+'['+string(kf-1)+'].g    = &(g['+...
             string(zcptr(kf)-1)+']);'
            '    block_'+rdnom+'['+string(kf-1)+'].jroot = &(jroot['+...
             string(zcptr(kf)-1)+']);']
     end
  end

  if txt <> [] then
    Code=[Code;
          '    /* Adjust ptr array of zero crossing */'
          txt
          ''];
  end

  //@@ reentry flag
  Code=[Code;
        '    /*  Adjust ptr array that must be done only one times */'
        '    reentryflag=(int*) ((scicos_block *)(*block->work)+'+string(nblk)+');'
        '    if (*reentryflag==0) {'
        '      *reentryflag=1;']

  for kf=1:nblk
    nin   = inpptr(kf+1)-inpptr(kf); //** number of input ports
    nout  = outptr(kf+1)-outptr(kf); //** number of output ports
    nx    = xptr(kf+1)-xptr(kf);     //@@ number of continuous state
    nz    = zptr(kf+1)-zptr(kf);     //@@ number of continuous state
    nmode = modptr(kf+1)-modptr(kf); //@@ number of mode

    //** add comment
    txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

    Code=[Code;
          ''
          '      '+txt];

    //@@ regular input
    for k=1:nin
      lprt=inplnk(inpptr(kf)-1+k);
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+']  = '+...
            rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //@@ regular output
    for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
             '      block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+'] = '+...
             rdnom+'_block_outtbptr['+string(lprt-1)+'];']
    end

    //@@ discrete state
    if nz>0 then
      Code=[Code
            '      block_'+rdnom+'['+string(kf-1)+'].z         = &(z['+...
            string(zptr(kf)-1)+']);']
    end

    //@@ mode
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
      if (ipptr(kf+1)-ipptr(kf))>0 then
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

    //@@ work
    if with_work(kf)==1 then
      Code=[Code;
            '      block_'+rdnom+'['+string(kf-1)+'].work      ='+...
            ' (void **)(((double *)work)+'+string(kf-1)+');']
    end
  end

  //** cst blocks and it's dep
  txt=write_code_idoit()

  if txt<>[] then
    Code=[Code;
          ''
          '      /* initial blocks must be called with flag 1 */'
          '    '+txt];
  end

  Code=[Code
        '    }'
        '  }'
        ''
       ]
  /////////////////////////////////////////////

  //## get number of zero crossing
  ng=zcptr($)-1;

  //** flag 0
  flag = 0;

  txt22 = [];

  txt22=[txt22;
         '  '+write_code_odoit(1) //** first pass
         '  '+write_code_odoit(0) //** second pass
        ]

  if txt22<>[] then
    Code=[Code;
          '  if (flag == 0) { '+get_comment('flag',list(flag))
          txt22
          '  }'];
  end

  //@@
  with_flag2=%f;
  with_flag3=%f;

  //** flag 1,2,3
  for flag=[1,2,3]

    txt3=[]

    //** continuous time blocks must be activated
    //** for flag 1
    if flag==1 then
      txt = write_code_cdoit(flag);

      if txt <> [] then
        if ~ALL then
          txt3=[txt3;
                Indent+'  switch (nevprt) {'
               ];
          txt3=[txt3;
                Indent2+'  case '+string(0)+' : '+...
                  get_comment('ev',list(0))
                '    '+txt;
               ];
          txt3=[txt3;'      break;';'']
        else
          txt3=[txt3;
                Indent+'  if (nevprt==0) {'+...
                  get_comment('ev',list(0))
                '    '+txt;
                Indent+'  }'
               ];
        end
      end

    else
      txt=[];
    end

    //** blocks with input discrete event must be activated
    //** for flag 1, 2 and 3
    if size(evs,'*')>=1 then
      txt4=[]
      //**
      for ev=evs

        if ~ALL then
          new_ev=ev-(clkptr(howclk)-1)
        else
          new_ev=ev-min(evs)+1
        end

        txt2=write_code_doit(ev,flag);

        if ALL then
          if flag==2 then
            if txt2<>[] | with_flag2 then
              with_flag2=%t
              if ~with_flag3 then
                tt = write_code_doit(ev,3)
                if tt<>[] then
                  with_flag3=%t
                end
              end
            end
          end
        end

        if txt2<>[] then
          //** adjust event number because of bidon block
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
          if ~ALL then
            txt3=[txt3;
                  Indent+'  switch (nevprt) {'
                  txt4
                  '    }'];
          else
            txt3=[txt3;
                  Indent+'  ptr = *(block_'+rdnom+'['+string(nb_agenda_blk-1)+'].work);']
            if flag==2 & with_flag2 & with_flag3 then
              txt3=[txt3;
                    Indent+'  if (ptr->fromflag3) {'
                    Indent+'    kever = ptr->old_pointi;'
                    Indent+'    ptr->fromflag3 = 0;'
                    Indent+'  }'
                    Indent+'  else {'
                    Indent+'    kever = ptr->pointi;'
                    Indent+'  }']
            else
              txt3=[txt3;
                    Indent+'  kever = ptr->pointi;']
            end
            if flag==3 then
              txt3=[txt3;
                    Indent+'  ptr->pointi = ptr->evtspt[kever-1];'
                    Indent+'  ptr->evtspt[kever-1] = -1;']
              if with_flag2 & with_flag3 then
                txt3=[txt3;
                      Indent+'  ptr->old_pointi = kever;'
                      Indent+'  ptr->fromflag3  = 1;']
              end
            end
            txt3=[txt3;
                  Indent+'  switch (kever) {'
                  txt4
                  '    }'];
            if flag==3 then
              txt3=[txt3;
                    Indent+'  block->evout[0] = ptr->tevts[ptr->pointi-1] - t;']
            end
          end
        end
      else
        if ~ALL then
          txt3=[txt3;
                txt4]
        else
          txt3=[txt3;
                '    else {'
                '      ptr = *(block_'+rdnom+'['+string(nb_agenda_blk-1)+'].work);'
                '      kever = ptr->pointi;']
            if flag==3 then
              txt3=[txt3;
                  '      ptr->pointi = ptr->evtspt[kever-1];'
                  '      ptr->evtspt[kever-1] = -1;']
            end
            txt3=[txt3;
                Indent+'    switch (kever) {'
                txt4
                '      }'
                '    }'];
          if flag==3 then
            txt3=[txt3;
                  Indent+'  block->evout[0] = ptr->tevts[ptr->pointi-1] - t;']
          end
        end
      end
    end

    //**
    if ~ALL then
      if txt <> [] then
        txt3=[txt3;
              '    }'];
      end
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
        //## test for zero crossing
        if (ng ~= 0) & (flag~=1) then
          Code=[Code;
                '  else if ((flag == '+string(flag)+')&&(nevprt>=0)) { '+...
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

    //## zero crossing internal events
    if (ng ~= 0) then
      //##
      if (flag<>1) then
        txt22 = [];

        for k=1:nzord
          bk=zord(k,1);
          //@@ Ooups
          pt=abs(zord(k,2));
          pt=1;
          txt_tmp=call_block42(bk,-pt,flag)
          if txt_tmp<> [] then
            txt_tmp='    '+txt_tmp;

            txt22=[txt22;
                   txt_tmp];

          end
        end

        //@@ get ptr of agenda blk
        txt222=[];
        if ALL & flag==3 then
          txt222=['    ptr = *(block_'+rdnom+'['+string(nb_agenda_blk-1)+'].work);';
                  '']
        end

        if txt22<>[] then
          Code=[Code;
                '  else if ((flag == '+string(flag)+')&&(nevprt<0)) { '+...
                 '/* zero crossing internal events */'
                txt222
                txt22]
          if ALL & flag==3 then
            Code=[Code;
                  ''
                  Indent+'  block->evout[0] = ptr->tevts[ptr->pointi-1] - t;'
                  '  }']
          else
            Code=[Code;
                  '  }']
          end
        end
      end
    end

  end //## end of for flag=[1,2,3]

  //** flag 7
  if impl_blk then
    txt22 = [];

    txt22=[txt22;
           '  '+write_code_reinitdoit(1) //** first pass
           '  '+write_code_reinitdoit(7) //** second pass
          ]

    if txt22<>[] then
      Code=[Code;
            '  else if (flag == 7) { /* x_pointer properties */'
            txt22
            '  }'];
    end
  end

  //** flag 9
  if (ng ~= 0) then
    flag = 9;
//     Code=[Code;
//           '  else if (flag == '+string(flag)+') { '+...
//            get_comment('flag',list(flag))
//          ];
    Code=[Code;
          '  else if (flag == '+string(flag)+') { '+...
           get_comment('flag',list(flag))
         ];

    txt=[]
//     for kf=1:nblk
//       if zcptr(kf+1)-zcptr(kf) <> 0 then
//          txt=[txt;
//               '    block_'+rdnom+'['+string(kf-1)+'].g='+...
//               '&(g['+string(zcptr(kf)-1)+']);']
//       end
//     end

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
        '    /* work array allocation */'
        '    if ((*block->work=scicos_malloc(sizeof(scicos_block)*'+...
         string(nblk)+'+sizeof(int)))== NULL) return 0;';
        '    reentryflag=(int*) ((scicos_block *)(*block->work)+'+string(nblk)+');'
        '    *reentryflag=0;'
        '    block_'+rdnom+'=(scicos_block*) *block->work;'];

  for kf=1:nblk
    nin   = inpptr(kf+1)-inpptr(kf); //* number of input ports */
    nout  = outptr(kf+1)-outptr(kf); //* number of output ports */
    nx    = xptr(kf+1)-xptr(kf);
    nz    = zptr(kf+1)-zptr(kf);
    ng    = zcptr(kf+1)-zcptr(kf);
    nmode = modptr(kf+1)-modptr(kf);

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

    //@@ continuous state
    if nx <> 0 then
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].nx     = '+...
             string(nx)+';';
            '    block_'+rdnom+'['+string(kf-1)+'].x      = &(x['+...
             string(xptr(kf)-1)+']);'
            '    block_'+rdnom+'['+string(kf-1)+'].xd     = &(xd['+...
               string(xptr(kf)-1)+']);']
      if impl_blk then
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].res    = &(res['+...
                      string(xptr(kf)-1)+']);'
                '    block_'+rdnom+'['+string(kf-1)+'].xprop  = &(xprop['+...
                      string(xptr(kf)-1)+']);']
      end
    else
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].nx     = 0;';
            '    block_'+rdnom+'['+string(kf-1)+'].x      = &(zero);'
            '    block_'+rdnom+'['+string(kf-1)+'].xd     = w;']
    end

    //@@ zero-crossing
    if ng <> 0 then
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].g      = &(g['+...
            string(zcptr(kf)-1)+']);']
    else
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+'].g      = &(zero);';]
    end

    //@@ mode
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
    if nin>0 then
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
    end
    //**********************************************************************//

    //***************************** output port *****************************//
    if nout>0 then
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
    end
    //**********************************************************************//

    //@@ discrete state
    if nz>0 then
      Code=[Code
            '    block_'+rdnom+'['+string(kf-1)+'].z         = &(z['+...
            string(zptr(kf)-1)+']);']
    end

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
              '    block_'+rdnom+'['+string(kf-1)+'].ozsz['+string(noz+(k-1))+...
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

    //@@ work
    if with_work(kf)==1 then
      Code=[Code;
            '    block_'+rdnom+'['+string(kf-1)+...
             '].work      = (void **)(((double *)work)+'+string(kf-1)+');']
    end

    //@@ nevptr
    Code=[Code;
          '    block_'+rdnom+'['+string(kf-1)+...
           '].nevprt    = nevprt;']

    //@@ label
    Code=[Code;
          '    block_'+rdnom+'['+string(kf-1)+...
          '].label     = NULL;']

//     if length(cpr.sim.labels(kf))== 0 then
//       Code=[Code;
//             '    block_'+rdnom+'['+string(kf-1)+...
//             '].label     = NULL;']
//     else
//       Code=[Code;
//             '    if ((block_'+rdnom+'['+string(kf-1)+'].label  = '+...
//              'malloc(sizeof(char)*'+string(length(cpr.sim.labels(kf))+1)+'))==NULL) return 0;';]
//       Code=[Code;
//             '    block_'+rdnom+'['+string(kf-1)+'].label     = ""'+cpr.sim.labels(kf)+'"";']
//     end

  end //for kf=1:nblk

  //** init
  for kf=1:nblk
    if funs(kf)=='agenda_blk' then
      if ALL & size(evs,'*')<>0 then
        new_pointi=adjust_pointi(cpr.state.pointi,clkptr,funtyp)
        Code=[Code;
              '';
              '    /* Init of agenda_blk (blk nb '+string(kf)+') */'
              '    *(block_'+rdnom+'['+string(kf-1)+'].work) = '+...
                '(agenda_struct*) scicos_malloc(sizeof(agenda_struct));'
              '    ptr = *(block_'+rdnom+'['+string(kf-1)+'].work);'
              '    ptr->pointi     = '+string(new_pointi)+';'
              '    ptr->fromflag3  = 0;'
              '    ptr->old_pointi = 0;'
             ]
        new_evtspt=adjust_agenda(cpr.state.evtspt,clkptr,funtyp)
        for i=1:size(new_evtspt,1)
          if new_evtspt(i)>0 then
            new_evtspt(i)=adjust_pointi(new_evtspt(i),clkptr,funtyp)
          end
        end
        for i=1:size(evs,'*')
          Code=[Code;
                '    ptr->evtspt['+string(i-1)+'] = '+string(new_evtspt(i))+';'
               ]
        end
        new_tevts=adjust_agenda(cpr.state.tevts,clkptr,funtyp)
        for i=1:size(evs,'*')
          Code=[Code;
                '    ptr->tevts['+string(i-1)+']  = '+string_to_c_string(new_tevts(i))+';'
               ]
        end

      end
    else
      if ~(or(kf==act) | or(kf==cap)) then
        txt = call_block42(kf,0,4);
        if txt <> [] then
          Code=[Code;
                '';
                '    '+txt];
        end
      end
    end
  end

  //## disable Jacobian computation
  if impl_blk then
    Code=[Code;
          '';
          '     /* Disable Jacobian computation */'
          '     Set_Jacobian_flag(0);'];
  end

  Code=[Code;
        '  }'];

  //** flag 5
  Code=[Code;
        '  else if (flag == 5) { '+get_comment('flag',list(5))
        '    /* get work ptr of thaht block */'
        '    block_'+rdnom+'=*block->work;']

  for kf=1:nblk
     nin  = inpptr(kf+1)-inpptr(kf); //* number of input ports */
     nout = outptr(kf+1)-outptr(kf); //* number of output ports */
     nx   = xptr(kf+1)-xptr(kf);     //* number of continuous state */
     nz   = zptr(kf+1)-zptr(kf);     //@@ number of discrete state

     //** add comment
     txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

     Code=[Code;
           ''
           '    '+txt];

     //@@ regular input
     for k=1:nin
        lprt=inplnk(inpptr(kf)-1+k);
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+...
               ']  = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
     end

     //@@ regular output
     for k=1:nout
        lprt=outlnk(outptr(kf)-1+k);
        Code=[Code
              '    block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+...
              '] = '+rdnom+'_block_outtbptr['+string(lprt-1)+'];']
     end

     //@@ discrete state
     if nz>0 then
       Code=[Code
             '    block_'+rdnom+'['+string(kf-1)+'].z         = &(z['+...
              string(zptr(kf)-1)+']);']
     end

     //@@ continuous state
     if nx <> 0 then
       Code=[Code;
             '    block_'+rdnom+'['+string(kf-1)+'].nx     = '+...
              string(nx)+';';
             '    block_'+rdnom+'['+string(kf-1)+'].x      = &(x['+...
              string(xptr(kf)-1)+']);'
             '    block_'+rdnom+'['+string(kf-1)+'].xd     = &(xd['+...
              string(xptr(kf)-1)+']);']
       if impl_blk then
           Code=[Code;
                 '    block_'+rdnom+'['+string(kf-1)+'].res    = &(res['+...
                       string(xptr(kf)-1)+']);']
       end
     end

     if (part(funs(kf),1:7) ~= 'capteur' &...
          part(funs(kf),1:10) ~= 'actionneur' &...
           funs(kf) ~= 'bidon' &...
            funs(kf) ~= 'bidon2') then
       //** rpar **//
       if (rpptr(kf+1)-rpptr(kf)>0) then
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+...
                '].rpar   = &(rpar['+string(rpptr(kf)-1)+']);']
       end
       //** ipar **//
       if (ipptr(kf+1)-ipptr(kf)>0) then
         Code=[Code;
               '    block_'+rdnom+'['+string(kf-1)+...
                '].ipar   = &(ipar['+string(ipptr(kf)-1)+']);'] 
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

     //@@ work
     if with_work(kf)==1 then
       Code=[Code;
             '    block_'+rdnom+'['+string(kf-1)+...
              '].work   = (void **)(((double *)work)+'+string(kf-1)+');']
     end
  end

  for kf=1:nblk
    if funs(kf)=='agenda_blk' then
      if ALL & size(evs,'*')<>0 then
        Code=[Code;
              '';
              '    /* Free agenda_blk (blk nb '+string(kf)+') */'
              '    if(*(block_'+rdnom+'['+string(kf-1)+'].work) != NULL) {'
              '      scicos_free(*(block_'+rdnom+'['+string(kf-1)+'].work));'
              '    }'
             ]

      end
    else
      if ~(or(kf==act) | or(kf==cap)) then
        txt = call_block42(kf,0,5);
        if txt <> [] then
          Code=[Code;
                '';
                '    '+txt];
        end
      end
    end
  end

  Code=[Code;
        ''
        '    for (kf = 0; kf < '+string(nblk)+'; ++kf){'
        '      if (block_'+rdnom+'[kf].nin!=0){'
        '        if (block_'+rdnom+'[kf].insz!=NULL){'
        '          free(block_'+rdnom+'[kf].insz);'
        '        }'
        '      }'
        '      if (block_'+rdnom+'[kf].nout!=0){'
        '        if (block_'+rdnom+'[kf].outsz!=NULL){'
        '          free(block_'+rdnom+'[kf].outsz);'
        '        }'
        '      }'
        '      if (block_'+rdnom+'[kf].nopar!=0){'
        '        if (block_'+rdnom+'[kf].oparptr!=NULL){'
        '          free(block_'+rdnom+'[kf].oparptr);'
        '        }'
        '        if (block_'+rdnom+'[kf].oparsz!=NULL){'
        '          free(block_'+rdnom+'[kf].oparsz);'
        '        }'
        '        if (block_'+rdnom+'[kf].opartyp!=NULL){'
        '          free(block_'+rdnom+'[kf].opartyp);'
        '        }'
        '      }'
        '      if (block_'+rdnom+'[kf].noz!=0){'
        '        if (block_'+rdnom+'[kf].ozptr!=NULL){'
        '          free(block_'+rdnom+'[kf].ozptr);'
        '        }'
        '        if (block_'+rdnom+'[kf].ozsz!=NULL){'
        '          free(block_'+rdnom+'[kf].ozsz);'
        '        }'
        '        if (block_'+rdnom+'[kf].oztyp!=NULL){'
        '          free(block_'+rdnom+'[kf].oztyp);'
        '        }'
        '      }'
        '      if (block_'+rdnom+'[kf].evout!=NULL){'
        '        free(block_'+rdnom+'[kf].evout);'
        '      }'
        '      if (block_'+rdnom+'[kf].label!=NULL){'
        '        free(block_'+rdnom+'[kf].label);'
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

  //@@ addevs function
  if ALL & size(evs,'*')<>0 then
    Code=[Code;
          ''
          '/* addevs function */'
          'void '+rdnom+'_addevs(agenda_struct *ptr, double t, int evtnb)'
          '{'
          '  int i,j;'
          '']

    if debug_agenda then
      Code=[Code;
            '  fprintf(stderr,""addevs (begin)\n \tpointi=%d\n \tevtnb=%d\n \tptr->evtspt[evtnb-1]=%d\n \tt=%f\n"", \'
            '                 ptr->pointi,evtnb,ptr->evtspt[evtnb-1],t);'
            '']
    end

    Code=[Code;
          '  /*  */'
          '  if (ptr->evtspt[evtnb-1] != -1) {'
          '    if ((ptr->evtspt[evtnb-1] == 0) && (ptr->pointi==evtnb)) {'
          '      ptr->tevts[evtnb-1] = t;'
          '      return;'
          '    }'
          '    /* */'
          '    else {'
          '      /* (ptr->pointi == evtnb) && ((ptr->evtspt[evtnb] == 0) || (ptr->evtspt[evtnb] != 0)) */'
          '      if (ptr->pointi == evtnb) {'
          '        ptr->pointi = ptr->evtspt[evtnb-1]; /* remove from chain, pointi is now the event provided by ptr->evtspt[evtnb] */'
          '      }'
          '      /* (ptr->pointi != evtnb) && ((ptr->evtspt[evtnb] == 0) || (ptr->evtspt[evtnb] != 0)) */'
          '      else {'
          '        /* find where is the event to be updated in the agenda */'
          '        i = ptr->pointi;'
          '        while (evtnb != ptr->evtspt[i-1]) {'
          '          i = ptr->evtspt[i-1];'
          '        }'
          '        ptr->evtspt[i-1] = ptr->evtspt[evtnb-1]; /* remove old evtnb from chain */'
          ''
          '        /* if (TCritWarning == 0) {'
          '         *  sciprint(""\n\r Warning:an event is reprogrammed at t=%g by removing another"",t );'
          '         *  sciprint(""\n\r         (already programmed) event. There may be an error in"");'
          '         *  sciprint(""\n\r         your model. Please check your model\n\r"");'
          '         *  TCritWarning=1;'
          '         * }'
          '         */'
          ''
          '        /*do_cold_restart();*/ /* the erased event could be a critical event, '
          '                            * so do_cold_restart is added to'
          '                            * refresh the critical event table'
          '                            */'
          '      }'
          ''
          '      /* */'
          '      ptr->evtspt[evtnb-1] = 0;'
          '      ptr->tevts[evtnb-1]  = t;'
          '    }'
          '  }'
          '  else {'
          '    ptr->evtspt[evtnb-1] = 0;'
          '    ptr->tevts[evtnb-1]  = t;'
          '  }'
          ''
          '  /* */'
          '  if (ptr->pointi == 0) {'
          '    ptr->pointi = evtnb;'
          '    return;'
          '  }'
          '  if (t < ptr->tevts[ptr->pointi-1]) {'
          '    ptr->evtspt[evtnb-1] = ptr->pointi;'
          '    ptr->pointi = evtnb;'
          '    return;'
          '  }'
          ''
          '  /* */'
          '  i = ptr->pointi;'
          ''
          ' L100:'
          '  if (ptr->evtspt[i-1] == 0) {'
          '    ptr->evtspt[i-1] = evtnb;'
          '    return;'
          '  }'
          '  if (t >= ptr->tevts[ptr->evtspt[i-1]-1]) {'
          '    j = ptr->evtspt[i-1];'
          '    if (ptr->evtspt[j-1] == 0) {'
          '      ptr->evtspt[j-1] = evtnb;'
          '      return;'
          '    }'
          '    i = j;'
          '    goto L100;'
          '  }'
          '  else {'
          '    ptr->evtspt[evtnb-1] = ptr->evtspt[i-1];'
          '    ptr->evtspt[i-1] = evtnb;'
          '  }'
          '']

    if debug_agenda then
      Code=[Code;
            '  fprintf(stderr,""addevs (end), pointi=%d\n"",ptr->pointi);'
            '']
    end

    Code=[Code;
          '  return;'
          '}'
         ]
  end

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

//## [out]=rdnom([in][,te][,tf][,h][,solver])
//##
//## Rhs :
//## in     : input signal(s) for sensors
//## te     : sampling time (default : 0.1)
//## tf     : final time (default : 30)
//## h      : solver step (default : 0.001)
//## solver : type of solver (1:Euler, 2:Heun, 3:R.Kutta 4th order)
//##          (default : 3)
//## Lhs :
//## out    : output signal(s) coming from actuator
//##
//## Alan, initial rev xx/12/07
//##
//Copyright INRIA
function Code=make_sci_interf()

 //## get the number of sensors/actuators
 nbcapt=size(capt,1)
 nbact=size(actt,1)

 //@@ get the length of the name of the interfacing scilab function
 l_rdnom=length(rdnom)
 l_rdnom=(l_rdnom>17)*17 + (l_rdnom<=17)*l_rdnom

 //## header
 Code=['#include <string.h>'
       '#include <stdlib.h>'
       '#include ""stack-c.h""'
       '#include ""mex.h""'
       '']

 //## external definition of standalone simulation function
 Code=[Code;
       '/* external defintion of standalone simulation function */'
       'extern int '+rdnom+'_sim(double tf, double dt, double h, int solver, \'
       get_blank(rdnom)+'                int *typin, void **inptr, int *typout, void **outptr);'
       ''
       '/* external definition of error table function */'
       'extern void get_err_msg(int ierr,char *err_msg);'
       ''
       '#if WIN32'
       '  #ifndef coserr'
       '    #define coserr _imp__coserr'
       '  #endif'
       '#endif'
       ''
       '/* standalone still use scicos.c here ! */'
       'extern struct {char buf[4096];} coserr;'
       '']

 //## type of in/out structure definition
 Code=[Code;
       '/* structure definition of in/out sensors/actuators */'
       'typedef struct {'
       '  int typ;      /* data type */'
       '  int ndims;    /* number of dims */'
       '  int ndata;    /* number of data */'
       '  int *dims;    /* size of data (length ndims) */'
       '  double *time; /* date of data (length ndata) */'
       '  void *data;   /* data (length ndata*prod(dims)) */'
       '} scicos_inout;'
       '']

 //## comment
 txt_in  = []
 txt_out = []
 for i=1:nbcapt
   txt_in = txt_in+'in'+string(i)+',';
   if i==nbcapt then
     txt_in=part(txt_in,1:length(txt_in)-1);
   end
 end
 for i=1:nbact
   txt_out = txt_out+'out'+string(i)+',';
   if i==nbact then
     txt_out=part(txt_out,1:length(txt_out)-1);
   end
 end

 if txt_in<>[] then
   txt_rhs=txt_in+',[,te][,tf][,h][,solver]';
 else
   txt_rhs='[,te][,tf][,h][,solver]';
 end

 Code=[Code;
       '/* ['+txt_out+']='+rdnom+'('+txt_rhs+')'
       ' *'
       ' * Rhs :']
 if txt_in<>[] then
   Code=[Code;
         ' * in     : input signal(s) for sensors']
 end
 Code=[Code;
       ' * te     : sampling time (default : 0.1)'
       ' * tf     : final time (default : 30)'
       ' * h      : solver step (default : 0.001)'
       ' * solver : type of solver (1:Euler, 2:Heun, 3:R.Kutta 4th order)'
       ' *          (default : 3)'
       ' *']
  if txt_out<>0 then
    Code=[Code;
          ' * Lhs :'
          ' * out    : output signal(s) coming from actuator']
  end
  Code=[Code;
        ' */']

 //## interfacing function
 Code=[Code;
       'int int'+part(rdnom,1:l_rdnom)+'(fname,fname_len)'
       '   char *fname;'
       '   unsigned long fname_len;'
       '{']

 //## declaration of variables for scilab stack
 Code = [Code;
         '  /* variables to handle ptr and dims coming from scilab stack */']
 //## for sensors
 for i=1:nbcapt
    Code = [Code;
            '  static int l'+string(i)+','+...
              'n'+string(i)+',m'+string(i)+';'+...
              '  /* sensor '+string(i)+' */']
 end
 //## for [,te][,tf][,h][,solver])
 Code = [Code;
         '  static int l'+string(nbcapt+1)+','+...
           'n'+string(nbcapt+1)+',m'+string(nbcapt+1)+';'+...
           '  /* te */'
         '  static int l'+string(nbcapt+2)+','+...
           'n'+string(nbcapt+2)+',m'+string(nbcapt+2)+';'+...
           '  /* tf */'
         '  static int l'+string(nbcapt+3)+','+...
           'n'+string(nbcapt+3)+',m'+string(nbcapt+3)+';'+...
           '  /* h */'
         '  static int l'+string(nbcapt+4)+','+...
           'n'+string(nbcapt+4)+',m'+string(nbcapt+4)+';'+...
           '  /* solver */']
 //## for actuators
 for i=1:nbact
    Code = [Code;
            '  static int l'+string(nbcapt+4+i)+','+...
              'n'+string(nbcapt+4+i)+',m'+string(nbcapt+4+i)+';'+...
              '  /* actuator '+string(i)+' */']
 end
 //## for scicos signal (mlist)
 if nbact>0 then
    Code = [Code;
            '  static int mL=4,nL=1,lL; /* def scicos signal */'
            '  static char *Str[]={""st"",""dims"",""values"",""time""};'
            '  static int V[]={1,1};'
            '  static SciIntMat M={1,2,4,-1,V};  /* dims field */']
 end

 //## declaration of variables for standalone simulation function
 Code = [Code;
         ''
         '  /* variables for standalone simulation function */']

 //## default values for te, tf, h and solver
 Code=[Code
       '  /* default values te,tf,h and solver */'
       '  double te  = 0.1;'
       '  double tf  = 30;'
       '  double h   = 0.001;'
       '  int solver = 3;']

 //## sensors
 if nbcapt<>0 then
   Code=[Code
         ''
         '  /* Inputs of sensors */'
         '  /* int nin='+string(nbcapt)+'; */'
         cformatline('  int typin[]={'+...
              strcat(string(2*ones(nbcapt,1)),"," )+'};',70)
         cformatline('  void *inptr[]={'+...
              strcat(string(zeros(nbcapt,1)),"," )+'};',70)]

   for i=1:nbcapt
     Code=[Code
           '  scicos_inout in_'+string(i)+';']
   end
 else
   Code=[Code;
         ''
         '  /* Inputs of sensors */'
         '  /* int nin=0; */'
         '  int *typin=NULL;'
         '  void **inptr=NULL;']
 end

 //## actuators
 if nbact<>0 then
   Code=[Code
         ''
         '  /* Outputs of actuators */'
         '  /* int nout='+string(nbact)+'; */'
         cformatline('  int typout[]={'+...
              strcat(string(2*ones(nbact,1)),"," )+'};',70)
         cformatline('  void *outptr[]={'+...
              strcat(string(zeros(nbact,1)),"," )+'};',70)]

   for i=1:nbact
     Code=[Code
           '  scicos_inout out_'+string(i)+';']
   end
 else
   Code=[Code;
         ''
         '  /* Outputs of actuators */'
         '  /* int nout=0; */'
         '  int *typout=NULL;'
         '  void **outptr=NULL;']
 end

 //## template for actuator transposition
 if nbact<>0 then
   Code=[Code
         ''
         '  /* ptr for output transposition */']
   for i=1:nbact
     Code=[Code
           '  void *out_ptr'+string(i)+';']
   end
 end

 //## error handling
 Code=[Code
       ''
       '  /* Ouput standalone error handling */'
       '  int ierr;'
       '  char err_msg[4096];']

 //## counter variable
 Code=[Code
       ''
       '  /* counter local variable */'
       '  int i,j;']

 //## CheckRhs min=nb sensors, max= nb sensors+4
 Code=[Code;
       ''
       '  /* check numbers of rhs/lhs */'
       '  CheckRhs('+string(nbcapt)+','+string(nbcapt+4)+');']

 //## CheckLhs min/max=nb actuators
 Code=[Code;
       '  CheckLhs('+string(nbact)+','+string(nbact)+');'
       '']

 Code=[Code;
       '  switch(Rhs) {']

 //## Check/get rhs var
 for i=nbcapt+4:-1:1

   //## str for stack handling
   i_str = string(i);
   m_str = 'm'+string(i);
   n_str = 'n'+string(i);
   l_str = 'l'+string(i);

  //## solver (solver type)
   if i==nbcapt+4 then
     Code=[Code;
           '    case '+i_str+' :    /* check/get solver */'
           '       GetRhsVar('+i_str+',""d"",&'+m_str+',&'+n_str+',&'+l_str+');'
           '       CheckDims('+i_str+','+m_str+','+n_str+',1,1);'
           '       solver = (int) *stk(l'+i_str+');'
           '']

  //## h (solver time step)
  elseif i==nbcapt+3 then
     Code=[Code;
           '    case '+i_str+' :    /* check/get h */'
           '       GetRhsVar('+i_str+',""d"",&'+m_str+',&'+n_str+',&'+l_str+');'
           '       CheckDims('+i_str+','+m_str+','+n_str+',1,1);'
           '       h = *stk(l'+i_str+');'
           '']

  //## tf (final time simulation)
  elseif i==nbcapt+2 then
     Code=[Code;
           '    case '+i_str+' :    /* check/get tf */'
           '       GetRhsVar('+i_str+',""d"",&'+m_str+',&'+n_str+',&'+l_str+');'
           '       CheckDims('+i_str+','+m_str+','+n_str+',1,1);'
           '       tf = *stk(l'+i_str+');'
           '']

  //## te (clock sampling time)
  elseif i==nbcapt+1 then
     Code=[Code;
           '    case '+i_str+' :    /* check/get te */'
           '       GetRhsVar('+i_str+',""d"",&'+m_str+',&'+n_str+',&'+l_str+');'
           '       CheckDims('+i_str+','+m_str+','+n_str+',1,1);'
           '       te = *stk(l'+i_str+');'
           '']

  //## sensors
  else
     Code=[Code;
           '    case '+i_str+' :    /* check/get sensor '+i_str+' */'
           '       GetRhsVar('+i_str+',""d"",&'+m_str+',&'+n_str+',&'+l_str+');'
           '       /* Many thing to do */'
           '']

  end
 end

 Code=[Code;
       '  }'
       '']

 //## inform in/out structure
 //## sensors
 for i=1:nbcapt
   //## in_x.dims
   Code=[Code
         '  /* inform in struct of sensor '+string(i)+' */'
         '  in_'+string(i)+'.typ       = '+string(capt(i,5))+';'
         '  in_'+string(i)+'.ndims     = 2;'
         '  in_'+string(i)+'.ndata     = m'+string(i)+';'
         '  if ((in_'+string(i)+'.dims = (int*) malloc(in_'+string(i)+'.ndims*sizeof(int)))==NULL){'];
   if i<>1 then
     for j=1:i-1
       Code=[Code
             '    free(in_'+string(j)+'.dims);'];
     end
   end
   Code=[Code
         '    return 0;'
         '  }']

   //## in_x.data
   Code=[Code
         '  in_'+string(i)+'.dims[0] = 1;'
         '  in_'+string(i)+'.dims[1] = n'+string(i)+';']
   //## Alan : 05/09/08 : add complex number
   //##                   arbitrary data type should be done.
   if capt(i,5) == 11 then // cmplx case
     Code=[Code
           '  if ((in_'+string(i)+'.data = (double *) malloc(in_'+string(i)+'.ndata*'+...
                 'in_'+string(i)+'.dims[0]*in_'+string(i)+'.dims[1]*2*sizeof(double)))==NULL){']
   else
     Code=[Code
           '  if ((in_'+string(i)+'.data = (double *) malloc(in_'+string(i)+'.ndata*'+...
                 'in_'+string(i)+'.dims[0]*in_'+string(i)+'.dims[1]*sizeof(double)))==NULL){']
   end

   for j=1:i
     Code=[Code
           '    free(in_'+string(j)+'.dims);']
   end

   Code=[Code
         '    return 0;'
         '  }'
         '  if ((in_'+string(i)+'.dims[0] == 1) &&  (in_'+string(i)+'.dims[1] != 1)) {'
         '    for(i=0;i<in_'+string(i)+'.ndata;i++) {'
         '      for(j=0;j<in_'+string(i)+'.dims[1];j++) {'
         '         *((double *)in_'+string(i)+'.data + i*in_'+string(i)+'.dims[1] + j) = \'
         '             *((double *) stk(l'+string(i)+') + j*in_'+string(i)+'.ndata + i);']

   if capt(i,5) == 11 then // cmplx case
     Code=[Code
           '         *((double *)in_'+string(i)+'.data + i*in_'+string(i)+'.dims[1] + j + '+...
                       'in_'+string(i)+'.dims[0]*in_'+string(i)+'.dims[1]) = \'
           '             *((double *) stk(l'+string(i)+') + j*in_'+string(i)+'.ndata + i + '+...
                       'in_'+string(i)+'.dims[0]*in_'+string(i)+'.dims[1]);']
   end

   Code=[Code
         '      }'
         '    }'
         '  }'
         '  else {']

   if capt(i,5) == 11 then // cmplx case
     Code=[Code
           '    for(i=0;i<in_'+string(i)+'.ndata*in_'+string(i)+'.dims[0]*in_'+string(i)+'.dims[1]*2;i++) {'
           '      *((double *)in_'+string(i)+'.data + i) = *((double *) stk(l'+string(i)+') + i);'
           '    }']
   else
     Code=[Code
           '    for(i=0;i<in_'+string(i)+'.ndata*in_'+string(i)+'.dims[0]*in_'+string(i)+'.dims[1];i++) {'
           '      *((double *)in_'+string(i)+'.data + i) = *((double *) stk(l'+string(i)+') + i);'
           '    }']
   end

   Code=[Code
         '  }'
         ''];
 end

 //## actuators
 for i=1:nbact
   //## out_x.dims
   Code=[Code
         '  /* inform out struct of actuator '+string(i)+' */'
         '  out_'+string(i)+'.typ       = '+string(actt(i,5))+';'
         '  out_'+string(i)+'.ndims     = 2;'
         '  out_'+string(i)+'.ndata     = 1000; /* default buffer size */'
         '  if ((out_'+string(i)+'.dims = (int *) malloc(out_'+string(i)+'.ndims*sizeof(int)))==NULL){'];
   for j=1:nbcapt
     Code=[Code
           '    free(in_'+string(j)+'.data);'
           '    free(in_'+string(j)+'.dims);'];
   end

   for j=1:i-1
     Code=[Code
           '    free(out_'+string(j)+'.dims);'
           '    free(out_'+string(j)+'.data);'
           '    free(out_'+string(j)+'.time);'];
   end

   Code=[Code
         '    return 0;'
         '  }']

   //## out_x.data
   Code=[Code
         '  out_'+string(i)+'.dims[0]   = '+string(actt(i,3))+';'
         '  out_'+string(i)+'.dims[1]   = '+string(actt(i,4))+';'
         '  out_'+string(i)+'.data      = NULL;']

   if actt(i,5) == 11 then // cmplx case
     Code=[Code
           '  if ((out_'+string(i)+'.data = (double *) realloc(out_'+string(i)+'.data, \'
           '       out_'+string(i)+'.ndata*'+...
            'out_'+string(i)+'.dims[0]*out_'+string(i)+'.dims[1]'+'*sizeof(double)*2))==NULL){'];
   else
     Code=[Code
           '  if ((out_'+string(i)+'.data = (double *) realloc(out_'+string(i)+'.data, \'
           '       out_'+string(i)+'.ndata*'+...
            'out_'+string(i)+'.dims[0]*out_'+string(i)+'.dims[1]'+'*sizeof(double)))==NULL){'];
   end

   for j=1:nbcapt
     Code=[Code
           '    free(in_'+string(j)+'.data);'
           '    free(in_'+string(j)+'.dims);'];
   end

   for j=1:i-1
     Code=[Code
           '    free(out_'+string(j)+'.dims);'
           '    free(out_'+string(j)+'.data);'
           '    free(out_'+string(j)+'.time);'];
   end

   Code=[Code
         '    free(out_'+string(i)+'.dims);']


   Code=[Code
         '    return 0;'
         '  }']

   //## out_x.time
   Code=[Code
         '  out_'+string(i)+'.time      = NULL;'
         '  if ((out_'+string(i)+'.time = (double *) realloc(out_'+string(i)+'.time,'+...
                 'out_'+string(i)+'.ndata*sizeof(double)))==NULL){'];
   for j=1:nbcapt
     Code=[Code
           '    free(in_'+string(j)+'.data);'
           '    free(in_'+string(j)+'.dims);'];
   end

   for j=1:i-1
     Code=[Code
           '    free(out_'+string(j)+'.dims);'
           '    free(out_'+string(j)+'.data);'
           '    free(out_'+string(j)+'.time);'];
   end

   Code=[Code
         '    free(out_'+string(i)+'.dims);'
         '    free(out_'+string(i)+'.data);'];

   Code=[Code
         '    return 0;'
         '  }'
         ''
        ];
 end

 //## store ptr of sensors/actuators
 if nbact<>0 & nbcapt<>0 then
   Code=[Code
         '  /* store ptr of sensors/actuators in inptr/outptr */']
 elseif nbcapt<>0 then
   Code=[Code
         '  /* store ptr of sensors in inptr */']
 elseif nbact<>0 then
   Code=[Code
         '  /* store ptr of actuators in outptr */']
 end

 if nbcapt<>0 then
   for i=1:nbcapt
     Code=[Code;
           '  inptr['+string(i-1)+']  = &in_'+string(i)+';']
   end
 end

 if nbact<>0 then
   for i=1:nbact
     Code=[Code;
           '  outptr['+string(i-1)+'] = &out_'+string(i)+';']
   end
 end

 //## call standalone simulation function
 Code=[Code
       ''
       '  /* call standalone simulation function */'
       '  ierr='+rdnom+'_sim(tf, te, h, solver, typin, inptr, typout, outptr);'
       ''
       '  /* display error message */'
       '  if (ierr!=0) {'
       '    /* standalone still use scicos.c here ! */'
       '    if(ierr!=10) {'
       '      get_err_msg(ierr,err_msg);'
       '    }'
       '    else {'
       '      strcpy(err_msg,coserr.buf);'
       '    }'
       '    sciprint(""Simulation fails with error number %d :'+...
             '\n%s\n"",ierr,err_msg);'
       '  }'
      ]

 //## create Lhsvar (actuators)
 if nbact<>0 then
   Code=[Code
         ''
         '  /* Create LhsVar */']

   for i=1:nbact
     //## str for stack handling
     i_str = string(i+nbcapt+4);
     m_str = 'm'+string(i+nbcapt+4);
     n_str = 'n'+string(i+nbcapt+4);
     l_str = 'l'+string(i+nbcapt+4);

     Code=[Code
           '  /* actuator '+string(i)+' */'
           '  CreateVar('+i_str+', ""m"", &mL, &nL, &lL);              /* mlist */'
           '  CreateListVarFromPtr('+i_str+',1,""S"", &nL, &mL, Str);  /* str of scicos signal */'
           '  CreateListVarFromPtr('+i_str+',2,""I"", &M.m, &M.n, &M); /* def dims of mlist */'
           '  '+m_str+'=out_'+string(i)+'.ndata;']

     Code=[Code
           '  /* vector case */'
           '  if ((out_'+string(i)+'.dims[0]==1 && out_'+string(i)+'.dims[1]!=1) || \'
           '      (out_'+string(i)+'.dims[0]!=1 && out_'+string(i)+'.dims[1]==1)) {'
           '    if (out_'+string(i)+'.dims[0]!=1) {'
           '      '+n_str+'=out_'+string(i)+'.dims[0];'
           '    }'
           '    else {'
           '      '+n_str+'=out_'+string(i)+'.dims[1];'
           '    }'
           '    /*transpose output */']

     if actt(i,5) == 11 then // cmplx case
       Code=[Code
             '    if ((out_ptr'+string(i)+' = (double *) malloc('+m_str+'*'+n_str+'*2*sizeof(double)))==NULL){']
     else
       Code=[Code
             '    if ((out_ptr'+string(i)+' = (double *) malloc('+m_str+'*'+n_str+'*sizeof(double)))==NULL){']
     end

     if nbcapt<>0 then
       for j=1:nbcapt
         Code=[Code
               '      free(in_'+string(j)+'.data);'
               '      free(in_'+string(j)+'.dims);'];
       end
     end
     for j=1:nbact
       Code=[Code
             '      free(out_'+string(j)+'.dims);'
             '      free(out_'+string(j)+'.data);'
             '      free(out_'+string(j)+'.time);'];
     end

     Code=[Code
           '      return 0;'
           '    }'
           '    for(i=0;i<'+m_str+';i++) {'
           '      for(j=0;j<'+n_str+';j++) {'
           '        *((double *)out_ptr'+string(i)+' + j*'+m_str+' + i) = *((double *) out_'+string(i)+'.data + i*'+n_str+' + j);']

     if actt(i,5) == 11 then // cmplx case
       Code=[Code
             '        *((double *)out_ptr'+string(i)+' + j*'+m_str+' + i + m7*n7) = *((double *) out_'+string(i)+'.data + i*'+n_str+' + j + m7*n7);']
     end

     Code=[Code
           '      }'
           '    }']

     if actt(i,5) == 11 then // cmplx case
       Code=[Code
             '   {'
             '     int it=1;'
             '     double *ptr_d = ((double*) out_ptr'+string(i)+' + m7*n7);'
             '     CreateListCVarFromPtr('+i_str+',3,""d"", &it, &'+m_str+', &'+n_str+', &out_ptr'+string(i)+', &ptr_d);   /* data */'
             '   }']
     else
       Code=[Code
             '    CreateListVarFromPtr('+i_str+',3,""d"", &'+m_str+', &'+n_str+', &out_ptr'+string(i)+');   /* data */']
     end

     Code=[Code
           '  }'
           '  else {'
           '    /* scalar case */'
           '    if (out_'+string(i)+'.dims[0]==1 && out_'+string(i)+'.dims[1]==1) {']

     if actt(i,5) == 11 then // cmplx case
       Code=[Code
             '      '+n_str+'=1;'
             '      int it=1;'
             '      double *ptr_d = ((double*) out_'+string(i)+'.data + m7*n7);'
             '      CreateListCVarFromPtr('+i_str+',3,""d"", &it, &'+m_str+', &'+n_str+', &out_'+string(i)+'.data, &ptr_d);   /* data */']
     else
       Code=[Code
             '      '+n_str+'=1;'
             '      CreateListVarFromPtr('+i_str+',3,""d"", &'+m_str+', &'+n_str+', &out_'+string(i)+'.data);   /* data */']
     end

     Code=[Code
           '    }'
           '    /* matrix case : TODO */'
           '    else {']

     if nbcapt<>0 then
       for j=1:nbcapt
         Code=[Code
               '      free(in_'+string(j)+'.data);'
               '      free(in_'+string(j)+'.dims);'];
       end
     end
     for j=1:nbact
       Code=[Code
             '      free(out_'+string(j)+'.dims);'
             '      free(out_'+string(j)+'.data);'
             '      free(out_'+string(j)+'.time);'];
     end

     Code=[Code
           '      return 0;'
           '    }'
           '  }']

     //## event date
     Code=[Code
           '  '+n_str+'=1;'
           '  CreateListVarFromPtr('+i_str+',4,""d"", &'+m_str+', &'+n_str+...
              ', &out_'+string(i)+'.time);   /* event date */'
           '  /* Many thing to do */'
           ''];
   end
 end

 //## put LhsVar
 if nbact<>0 then
   Code=[Code
         '  /* Put LhsVar */']

   for i=1:nbact
     Code=[Code
           '  LhsVar('+string(i)+')='+string(i+nbcapt+4)+';']
   end
 end

 //## free allocated array
 //## array of sensors
 if nbcapt<>0 | nbact<>0 then
   Code=[Code
         ''
         '  /* free allocated array */'];
 end

 if nbcapt<>0 then
   for i=1:nbcapt
     Code=[Code
           '  free(in_'+string(i)+'.data);'
           '  free(in_'+string(i)+'.dims);'];
   end
 end

 //## array of actuators
 if nbact<>0 then
   Code=[Code
         ''];
   for i=1:nbact
     Code=[Code
           '  free(out_'+string(i)+'.dims);'];
   end

   for i=1:nbact
     Code=[Code
           '  free(out_'+string(i)+'.data);'];
   end

   for i=1:nbact
     Code=[Code
           '  free(out_'+string(i)+'.time);'];
   end

   for i=1:nbact
     n_str = 'n'+string(i+nbcapt+4);
     Code=[Code
           '  if ('+n_str+' != 1) {'
           '    free(out_ptr'+string(i)+');'
           '  }']

   end

 end

 //## end
 Code=[Code;
       '  return 0;'
       '}'
       '']

 //## Gateway
 // Code=[Code;
//        'static GenericTable Tab[]='
//        '{'
//        ' {(Myinterfun)sci_gateway,int'+part(rdnom,1:l_rdnom)+'_sci,'
//        ' """+rdnom+"""}'
//        '};'
//        ''
//        'int C2F(int'+part(rdnom,1:l_rdnom)+'_sci)()'
//        '{'
//        ' Rhs = Max(0, Rhs);'
//        ' (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);'
//        ' return 0;'
//        '}'
//        '']

endfunction

//Generating the routine for sensors interfacing
//
//Copyright INRIA
//
//Author : R. Djenidi, R. Nikoukhah, A. Layec
//
function Code=make_sensor()
  Call=['/*'+part('-',ones(1,40))+' Sensors */';
        'void '+rdnom+'_sensor(flag,nport,nevprt,t,y,ny1,ny2,yt,typin,inptr)']

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
            '      * typin : learn mode (0 from terminal,1 from input file)'
            '      * inptr : pointer to out data'
            '      *          typin=0, inptr not used'
            '      *          typin=1, inptr contains the input file name'
            '      */']

  dcl=['     int *flag,*nevprt,*nport;'
       '     int *ny1,*ny2,*yt;'
       ''
       '     int typin;'
       '     void *inptr;'
       ''
       '     double *t;'
       '     void *y;'
       '{'
       '  int j,k,l;'
       '  double temps;']

  //## code for terminal
  a_sensor=['    /* skeleton to be customized */'
            '    switch (*flag) {'
            '    case 4 : /* sensor initialisation */'
            '      /* do whatever you want to initialize the sensor */'
            '      break;'
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
            '            scanf(""%i"", &j);'
            '            *((char *) y+(k+l*(*ny1))) = (char) j;'
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
            '            scanf(""%d"", &j);'
            '            *((unsigned char *) y+(k+l*(*ny1))) = (unsigned char) j;'
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
            '    case 5 : /* sensor ending */'
            '      /* do whatever you want to end the sensor */'
            '      break;'
            '    }']

  //## code for input file
  b_sensor=['    /* skeleton to be customized */'
            '    switch (*flag) {'
            '    case 4 : /* sensor initialisation */'
            '      if (*nport == 1) {'
            '        fprr=fopen((char *) inptr,'"r'");'
            '        if( fprr == NULL ) {'
            '          printf('"Error opening file: %s\n'", (char *) inptr);'
            '          return;'
            '        }'
            '      }'
            '      break;'
            '    case 1 : /* fscanf the output value */'
            '    /*if(*nevprt>0) {*/'
            '      /* read time */'
            '      fscanf(fprr,""%lf "",&temps);'
            ''
            '      switch (*yt) {'
            '      case 10 :'
            '        for (l=0;l<*ny2;l++) {'
            '          for (k=0;k<*ny1;k++) {'
            '            fscanf(fprr,""%lf "", \'
            '                        (double *) y+(k+l*(*ny1)));'
            '          }'
            '        }'
            '        fscanf(fprr,""\n"");'
            '        break;'
            ''
            '      case 11 :'
            '        for (l=0;l<*ny2;l++) {'
            '          for (k=0;k<*ny1;k++) {'
            '            fscanf(fprr,""%lf "" \'
            '                        ""%lf "", \'
            '                        (double *) y+(k+l*(*ny1)), \'
            '                        (double *) y+((*ny1)*(*ny2)+k+l*(*ny1)));'
            '          }'
            '        }'
            '        fscanf(fprr,""\n"");'
            '        break;'
            ''
            '      case 81 :'
            '        for (l=0;l<*ny2;l++) {'
            '          for (k=0;k<*ny1;k++) {'
            '            fscanf(fprr,""%i "", \'
            '                        &j);'
            '            *((char *) y+(k+l*(*ny1))) = (char) j;'
            '          }'
            '        }'
            '        fscanf(fprr,""\n"");'
            '        break;'
            ''
            '      case 82 :'
            '        for (l=0;l<*ny2;l++) {'
            '          for (k=0;k<*ny1;k++) {'
            '            fscanf(fprr,""%hd "", \'
            '                        (short *) y+(k+l*(*ny1)));'
            '          }'
            '        }'
            '        fscanf(fprr,""\n"");'
            '        break;'
            ''
            '      case 84 :'
            '        for (l=0;l<*ny2;l++) {'
            '          for (k=0;k<*ny1;k++) {'
            '            fscanf(fprr,""%ld "", \'
            '                        (long *) y+(k+l*(*ny1)));'
            '          }'
            '        }'
            '        fscanf(fprr,""\n"");'
            '        break;'
            ''
            '      case 811 :'
            '        for (l=0;l<*ny2;l++) {'
            '          for (k=0;k<*ny1;k++) {'
            '            fscanf(fprr,""%d "", &j);'
            '            *((unsigned char *) y+(k+l*(*ny1))) = (unsigned char) j;'
            '          }'
            '        }'
            '        fscanf(fprr,""\n"");'
            '        break;'
            ''
            '      case 812 :'
            '        for (l=0;l<*ny2;l++) {'
            '          for (k=0;k<*ny1;k++) {'
            '            fscanf(fprr,""%hu "", \'
            '                        (unsigned short *) y+(k+l*(*ny1)));'
            '          }'
            '        }'
            '        fscanf(fprr,""\n"");'
            '        break;'
            ''
            '      case 814 :'
            '        printf(""type of the sensor output is: %d (unsigned long) \n"", *yt);'
            '        puts(""Please set the sensor output values"");'
            '        for (l=0;l<*ny2;l++) {'
            '          for (k=0;k<*ny1;k++) {'
            '            fscanf(fprr,""%lu "", \'
            '                        (unsigned long *) y+(k+l*(*ny1)));'
            '          }'
            '        }'
            '        fscanf(fprr,""\n"");'
            '        break;'
            '      }'
            '    /*} */'
            '      break;'
            '    case 5 : /* sensor ending */'
            '      /* do whatever you want to end the sensor */'
            '      break;'
            '    }']

  // pour fscanf
  nc=size(cap,'*')

//   typ=['""%lf ']; //temps
//   for i=1:nc
//     typ($)=typ($)+'"" \'
//     typ=[typ;'""'];
//     for j=1:sum(capt(i,3)*capt(i,4))
//       //typ=typ+'%f ';
//       typ($)=typ($)+scs_c_n2c_fmt(capt(i,5))+' ';
//     end
//   end
//   typ($)=typ($)+'\n"", \'
//   typ=strsubst(typ,'%f','%lf');
//   typ(1)='    fscanf(fprr,'+typ(1);
//   bl    ='                ';
//   if size(typ,1) <> 1 then
//     typ(2:$) = bl+typ(2:$);
//   end
//   //Code1=['      fscanf( fprr, '"'+typ+' \n'",&temps'] 
//   Code1=[typ;bl+'&temps'];
//   for i=1:size(capt,1)
//     ni=capt(i,3)*capt(i,4); // dimension of ith input
//     Code1($)=Code1($)+',';
//     Code1=[Code1;bl];
//     for j=1:ni
//       if capt(i,5)<>11 then
//         Code1($)=Code1($)+...
//                   '('+scs_c_n2c_typ(capt(i,5))+' *)('+...
//                    rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+')'+...
//                    '+'+string(j-1)+'';
//       else //CAS CMPLX
//         Code1($)=Code1($)+...
//                   '('+scs_c_n2c_typ(capt(i,5))+' *)('+...
//                    rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+')'+...
//                    '+'+string((j-1))+', '+...
//                   '('+scs_c_n2c_typ(capt(i,5))+' *)('+...
//                    rdnom+'_block_outtbptr+'+string(capt(i,2)-1)+')'+...
//                    '+'+string(ni+(j-1))+'';
//       end
//       if j<>ni then
//        Code1($)=Code1($)+', ';
//       end
//     end
//   end
//   Code1($)=Code1($)+');'

  Code=[]

  if nc==1 then
    Code=[Code;
          Call
          comments
          dcl
          '  if (typin == 0) {'
          a_sensor;
          '  } '
          '  else if (typin == 1) {'
          b_sensor;
          '  }'
          '}']
  elseif nc>1 then
    S='    switch (*nport) {'
    for k=1:nc
      S=[S;
         '    case '+string(k)+' : /* Port number '+string(k)+' ----------*/'
         '    '+a_sensor
         '    break;']
    end
    S=[S;'    }']

    T='    switch (*nport) {'
    for k=1:nc
      T=[T;
         '    case '+string(k)+' :/* Port number '+string(k)+' ----------*/'
         '    '+b_sensor
         '    break;']
    end
    T=[T;'    }']

    Code=[Code
          Call
          comments
          dcl
          '  if (typin == 0) {'
          S
          '  }'
          '  else if (typin == 1) {'
          T
          '  }'
          '}']
  end
endfunction

//generates code of the standalone simulation procedure
//
//Copyright INRIA
//
// rmq : La fonction zdoit n'est pas utilisee pour le moment
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
  ozptr=cpr.sim.ozptr;
  clkptr=cpr.sim.clkptr;
  ordptr=cpr.sim.ordptr;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  funtyp=cpr.sim.funtyp;
  noord=size(cpr.sim.oord,1);
  nzord=size(cpr.sim.zord,1);
  niord=size(cpr.sim.iord,1);

  Indent='  ';
  Indent2=Indent+Indent;
  BigIndent='          ';

  nX=size(x,'*');

  stalone = %t;

  //** evs : find source activation number
  //## with_nrd2 : find blk type 0 (wihtout)
  blks=find(funtyp>-1);
  evs=[];

  with_nrd2=%f;
  for blk=blks
    for ev=clkptr(blk):clkptr(blk+1)-1
      if funs(blk)=='bidon' then
        if ev > clkptr(howclk) -1
         evs=[evs,ev];
        end
      end
    end

    //## all blocks without sensor/actuator
    if (part(funs(blk),1:7) ~= 'capteur' &...
        part(funs(blk),1:10) ~= 'actionneur' &...
        funs(blk) ~= 'bidon') then
      //## with_nrd2 ##//
      if funtyp(blk)==0 then
        with_nrd2=%t;
      end
    end

  end

  Code=['/* Code prototype for standalone use  */'
        '/*     Generated by Code_Generation toolbox of Scicos with '+ ..
        get_scicos_version()+' */'
        '/*     date : '+date()+' */'
        ''
        '/* To learn how to use the standalone code, type '"./standalone -h'" */'
        ''
        '/* ---- Headers ---- */'
        '#include <stdio.h>'
        '#include <stdlib.h>'
        '#include <math.h>'
        '#include <string.h>'
        '#ifdef __STDC__'
        '#include <stdarg.h>'
        '#else'
        '#include <varargs.h>'
        '#endif'
        '#include <memory.h>'
        '#include '"scicos_block4.h'"'
        '#include '"machine.h'"'
        ''
        '/* ---- Internals functions and global variables declaration ---- */'
        Protostalone
        '']

  Code=[Code
        '/* prototype for input simulation function */'
        'int '+rdnom+'_sim(double, double, double, int, \'
        '                  int *, void **, int *, void **);'
        '']

  if x<>[] then
    if impl_blk then
      Code=[Code
            '/* ---- Solver functions prototype for standalone use ---- */'
            'int '+rdnom+'simblk_imp(double , double *, double *, double *);'
            'int dae1();'
            '']
    else
      Code=[Code
            '/* ---- Solver functions prototype for standalone use ---- */'
            'int '+rdnom+'simblk(double , double *, double *);'
            'int ode1();'
            'int ode2();'
            'int ode4();'
            '']
    end
  end

  Code=[Code;
        '/* ---- Specific declaration for the main() function ---- */'
        'int getopt (int, char **, char *);'
        make_static_standalone42()]


  //## Alan, 06/09/08 : add a C macro for cosend
  //##                  to properly handle error in standalone
  Code_end=[''
            '  '+get_comment('flag',list(5))]

  for kf=1:nblk
    if or(kf==act) | or(kf==cap) then
        txt = call_block42(kf,0,5);
        if txt <> [] then
          Code_end=[Code_end;
                    '';
                    '  '+txt];
        end
    else
      txt = call_block42(kf,0,5);
      if txt <> [] then
        Code_end=[Code_end;
                  '';
                  '  '+txt];
      end
    end
  end

  Code_end_mac = ['#define Cosend() '+Code_end(1)+'\']
  for i=2:size(Code_end,1)
    len=length(Code_end(i))
    if len <> 0 then
      if part(Code_end(i),len)<>'\' then
         Code_end_mac($+1) = Code_end(i) + '\'
      else
         Code_end_mac($+1) = Code_end(i)
      end
    else
      Code_end_mac($+1) = '  \'
    end
  end
  //Code_end_mac = [Code_end_mac;Code_end($)]

  Code=[Code;
        '/* Define Cosend macro */'
        Code_end_mac
        '  \'
        '  return get_block_error()'
        '']

  //*** Continuous state ***//
  if x <> [] then
    //## implicit block
    if impl_blk then
      Code=[Code;
            '/* def number of continuous state */'
            '#define NEQ '+string(nX/2)
            '']
    //## explicit block
    else
      Code=[Code;
            '/* def number of continuous state */'
            '#define NEQ '+string(nX)
            '']
    end
  end

  Code=[Code;
        '/* def phase sim variable */'
        'int phase;'
        ''
        '/* a variable for the current time */'
        'double scicos_time;'
        '']

  if impl_blk then
    Code=[Code;
          '/* Jacobian parameters */'
          'double Jacobian_Flag;'
          'double CJJ;'
          'double SQuround;'
          '']
  end

  Code=[Code
        '/* block_error must be pass in argument of _sim function */'
        'int *block_error;'
        'char err_msg[2048];'
        ''
        '/* prototype of error table function */'
        'void get_err_msg(int ierr,char *err_msg);'
        '']

  //## rmk: Alan, 06/09/08 : on devrait pouvoir enlever une structure
  //##      ici : celle du bloc bidon tjs a la fin
  if funs(nblk)=='bidon' then nblk=nblk-1, end;

  Code=[Code;
        '/* declaration of scicos block structures */'
        'scicos_block block_'+rdnom+'['+string(nblk)+'];'
        '']

  Code=[Code;
        '/* Main program */'
        'int main(int argc, char *argv[])'
        '{'
        '  /* local variables */'
        '  char input[50],output[50];'
        '  char **p=NULL;'
        ''
        '  /* default values for parameters of _sim function */'
        '  double tf=30;         /* final time */'
        '  double dt=0.1;        /* clock time */'
        '  double h=0.001;       /* solver step */']

  if impl_blk then
    Code=[Code;
       '  int solver=3;         /* type of solver */']
  else
    Code=[Code;
       '  int solver=1;         /* type of solver */']
  end

  Code_in=[]
  if size(capt,1)>0 then
    Code_in='  int nin='+string(size(capt,1))+';'
    Code_in=[Code_in;
             cformatline('  int typin[]={'+...
                  strcat(string(zeros(size(capt,1),1)),"," )+'};',70)]
    Code_in=[Code_in;
             cformatline('  void *inptr[]={'+...
                  strcat(string(zeros(size(capt,1),1)),"," )+'};',70)]
  end
  if Code_in<>[] then
    Code=[Code;
          '  /* Inputs of sensors */'
          Code_in]
  else
    Code=[Code;
          '  /* Inputs of sensors */'
          '  int nin=0;'
          '  int *typin=NULL;'
          '  void **inptr=NULL;']
  end

  Code_out=[]
  if size(actt,1)>0 then
    Code_out='  int nout='+string(size(actt,1))+';'
    Code_out=[Code_out;
              cformatline('  int typout[]={'+...
                  strcat(string(zeros(size(actt,1),1)),"," )+'};',70)]
    Code_out=[Code_out;
             cformatline('  void *outptr[]={'+...
                  strcat(string(zeros(size(actt,1),1)),"," )+'};',70)]
  end
  if Code_out<>[] then
    Code=[Code;
          '  /* Outputs of actuators */'
          Code_out]
  else
    Code=[Code;
          '  /* Outputs of actuators */'
          '  int nout=0;'
          '  int *typout=NULL;'
          '  void **outptr=NULL;']
  end

  Code=[Code;
        '  /**/'
        '  char * progname = argv[0];'
        '  /* local counter variable */'
        '  int c,i;'
        '  /* error handling variable */'
        '  int ierr;'
        ''
        '  /* init in/output files */'
        '  strcpy(input,'"'");'
        '  strcpy(output,'"'");'
        ''
        '  /* check rhs args */'
        '  while ((c = getopt(argc , argv, '"i:o:d:t:e:s:hv'")) != -1)'
        '    switch (c) {'
        '    case ''i'':'
        '      strcpy(input,argv[optind-1]);'
        '      break;'
        '    case ''o'':'
        '      strcpy(output,argv[optind-1]);'
        '      break;'
        '    case ''d'':'
        '      dt=strtod(argv[optind-1],p);'
        '      break;'
        '    case ''t'':'
        '      tf=strtod(argv[optind-1],p);'
        '      break;'
        '    case ''e'':'
        '      h=strtod(argv[optind-1],p);'
        '      break;'
        '    case ''s'':'
        '      solver=(int) strtod(argv[optind-1],p);'
        '      break;'
        '    case ''h'':'
        '      usage(progname);'
        '      return 0;'
        '      break;'
        '    case ''v'':'
        '      printf(""Generated by Code_Generation toolbox of Scicos ""'
        '             ""with '+get_scicos_version()+'\n"");'
        '      return 0;'
        '      break;'
        '    case ''?'':'
        '      usage(progname);'
        '      return 0;'
        '      break;'
        '    }'
        '']

  Code=[Code;
        '  /* adjust in/out of sensors/actuators */'
        '  if (strlen(input) > 0) {'
        '    for(i=0;i<nin;i++) {'
        '      typin[i]= 1;'
        '      inptr[i]= (void *) input;'
        '    }'
        '  }'
        '  if (strlen(output)> 0) {'
        '    for(i=0;i<nout;i++) {'
        '      typout[i]= 1;'
        '      outptr[i]= (void *) output;'
        '    }'
        '  }'
        '']

  Code=[Code;
        '  /* call simulation function */'
        '  ierr='+rdnom+'_sim(tf,dt,h,solver,typin,inptr,typout,outptr);'
        ''
        '  /* display error message */'
        '  if (ierr!=0) {'
        '    get_err_msg(ierr,err_msg);'
        '    fprintf(stderr,""Simulation fails with error number %d :'+...
             '\n%s\n"",ierr,err_msg);'
        '  }'
        ''
        '  return ierr;'
        '}'
        '']

  Code=[Code;
        '/* Error table function */'
        'void get_err_msg(int ierr,char *err_msg)'
        '{'
        '  switch (ierr)'
        '  {'
        '   case 1  : strcpy(err_msg,""scheduling problem"");'
        '             break;'
        ''
        '   case 2  : strcpy(err_msg,""input to zero-crossing stuck on zero"");'
        '             break;'
        ''
        '   case 3  : strcpy(err_msg,""event conflict"");'
        '             break;'
        ''
        '   case 4  : strcpy(err_msg,""algrebraic loop detected"");'
        '             break;'
        ''
        '   case 5  : strcpy(err_msg,""cannot allocate memory"");'
        '             break;'
        ''
        '   case 6  : strcpy(err_msg,""a block has been called with input out of its domain"");'
        '             break;'
        ''
        '   case 7  : strcpy(err_msg,""singularity in a block"");'
        '             break;'
        ''
        '   case 8  : strcpy(err_msg,""block produces an internal error"");'
        '             break;'
        ''
        '   case 10 : break;'
        ''
        '   /* other scicos error should be done */'
        ''
        '   default : strcpy(err_msg,""undefined error"");'
        '             break;'
        '  }'
        '}'
        '']

  Code=[Code;
        'static void usage(prog)'
        '       char *prog;'
        '{'
        '  fprintf(stderr, ""Usage: %s [-h] [-v] [-i arg] [-o arg] ""'
        '                  ""[-d arg] [-t arg] [-e arg] [-s arg]\n"", prog);'
        '  fprintf(stderr, ""Options : \n"");'
        '  fprintf(stderr, ""     -h for the help  \n"");'
        '  fprintf(stderr, ""     -v for printing the Scilab Version \n"");'
        '  fprintf(stderr, ""     -i for input file name, by default is Terminal \n"");'
        '  fprintf(stderr, ""     -o for output file name, by default is Terminal \n"");'
        '  fprintf(stderr, ""     -d for the clock period, by default is 0.1 \n"");'
        '  fprintf(stderr, ""     -t for the final time, by default is 30 \n"");'
        '  fprintf(stderr, ""     -e for the solvers step size, by default is 0.001 \n"");'
        '  fprintf(stderr, ""     -s integer parameter for select the numerical solver : \n"");']

  if impl_blk then
    Code=[Code;
          '  fprintf(stderr, ""        1 for a dae solver... \n"");']
  else
    Code=[Code;
          '  fprintf(stderr, ""        1 for Euler''s method \n"");'
          '  fprintf(stderr, ""        2 for Heun''s method \n"");'
          '  fprintf(stderr, ""        3 (default value) for the Fourth-Order Runge-Kutta'+...
           ' (RK4) Formula \n"");']
  end
  Code=[Code;
        '}'
        '']

  Code=[Code
        '/*'+part('-',ones(1,40))+'  External simulation function */'
        'int '+rdnom+'_sim(tf,dt,h,solver,typin,inptr,typout,outptr)'
        ''
        '   double tf,dt,h;'
        '   int solver;'
        '   int *typin,*typout;'
        '   void **inptr,**outptr;'
        '{'
        '  double t;']

  if evs<>[] then
    Code=[Code
          '  int nevprt=1;']
  end

  Code=[Code
        '  int local_flag;'
        '  int nport;']

  if with_synchro | impl_blk then
    Code=[Code
          '  int i;'
          '']
  else
    Code=[Code
         '']
  end

  if (with_nrd & with_nrd2) then
    Code=[Code;
          '  /* Variables for constant values */'
          '  int nrd_1, nrd_2;'
          ''
          '  double *args[100];'
          '']
  end

  if (x <> []) then
    Code=[Code
          '  double tout;'
          '  double he=0.1;'
          '']
  end

  //## set a variable to trace error of block
  Code=[Code
        '  int err=0;'
        ''
        '  /* Initial values */'
        '']

  //### Real parameters ###//
  if size(rpar,1) <> 0 then
    Code=[Code;
          '  /* Real parameters declaration */']
          //'static double RPAR1[ ] = {'];

    for i=1:(length(rpptr)-1)
      if rpptr(i+1)-rpptr(i)>0  then

        //** Add comments **//

        //## Modelica block
        if type(corinv(i))==15 then
          //## 09/04/08, Alan : we can extract here all informations
          //## from original scicos blocks with corinv : TODO
          Code($+1)='  /* MODELICA BLK RPAR COMMENTS : TODO */';
        else
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

          Code($+1)='  /* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
          Code($+1)='   * Gui name of block: '+strcat(string(OO.gui));
          Code($+1)='   * Compiled structure index: '+strcat(string(i));

          if stripblanks(OO.model.label)~=emptystr() then
            Code=[Code;
                  '  '+cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
          end
          if stripblanks(OO.graphics.exprs(1))~=emptystr() then
            Code=[Code;
                  '  '+cformatline(' * Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
          end
          if stripblanks(OO.graphics.id)~=emptystr() then
            Code=[Code;
                  '  '+cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
          end
          //txt=[txt;' * rpar='];
          Code($+1)='   */';
        end
        //******************//

        txt=cformatline(strcat(msprintf('%.16g,\n',rpar(rpptr(i):rpptr(i+1)-1))),70);

        txt(1)='double rpar_'+string(i)+'[]={'+txt(1);
        for j=2:size(txt,1)
          txt(j)=get_blank('double rpar_'+string(i)+'[]')+txt(j);
        end
        txt($)=part(txt($),1:length(txt($))-1)+'};'
        Code=[Code;
              '  '+txt
              '']
      end
    end
  end
  //#######################//

  //### Integer parameters ###//
  if size(ipar,1) <> 0 then
    Code=[Code;
          '  /* Integers parameters declaration */']

    for i=1:(length(ipptr)-1)
      if ipptr(i+1)-ipptr(i)>0  then

        //** Add comments **//

        //## Modelica block
        if type(corinv(i))==15 then
          //## 09/04/08, Alan : we can extract here all informations
          //## from original scicos blocks with corinv : TODO
          Code($+1)='  /* MODELICA BLK IPAR COMMENTS : TODO */';
        else
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

          Code($+1)='  /* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
          Code($+1)='   * Gui name of block: '+strcat(string(OO.gui));
          Code($+1)='   * Compiled structure index: '+strcat(string(i));
          if stripblanks(OO.model.label)~=emptystr() then
            Code=[Code;
                  '  '+cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
          end

          if stripblanks(OO.graphics.exprs(1))~=emptystr() then
            Code=[Code;
                  '  '+cformatline(' * Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
          end
          if stripblanks(OO.graphics.id)~=emptystr() then
            Code=[Code;
                  '  '+cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
          end
          Code=[Code;
                '  '+cformatline(' * ipar= {'+strcat(string(ipar(ipptr(i):ipptr(i+1)-1)),",")+'};',70)];
          Code($+1)='   */';
        end
        //******************//

        txt=cformatline(strcat(string(ipar(ipptr(i):ipptr(i+1)-1))+','),70);

        txt(1)='int ipar_'+string(i)+'[]={'+txt(1);
        for j=2:size(txt,1)
          txt(j)=get_blank('int ipar_'+string(i)+'[]')+txt(j);
        end
        txt($)=part(txt($),1:length(txt($))-1)+'};'
        Code=[Code;
              '  '+txt
              '']
      end
    end
  end
  //##########################//

  //### Object parameters ###//

  //** declaration of opar
  Code_opar = [];
  Code_ooparsz=[];
  Code_oopartyp=[];
  Code_oparptr=[];

  for i=1:(length(opptr)-1)
    nopar = opptr(i+1)-opptr(i)
    if nopar>0  then
      //** Add comments **//

      //## Modelica block
      if type(corinv(i))==15 then
        //## 09/04/08, Alan : we can extract here all informations
        //## from original scicos blocks with corinv : TODO
        Code_opar($+1)='  /* MODELICA BLK OPAR COMMENTS : TODO */';
      else
        //@@ 04/11/08, disable generation of comment for opar
        //@@ for m_frequ because of sample clock
        if funs(i)=='m_frequ' then
        else
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

          Code_opar($+1)='  /* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
          Code_opar($+1)='   * Gui name of block: '+strcat(string(OO.gui));
          Code_opar($+1)='   * Compiled structure index: '+strcat(string(i));
          if stripblanks(OO.model.label)~=emptystr() then
            Code_opar=[Code_opar;
                  '  '+cformatline(' * Label: '+strcat(string(OO.model.label)),70)];
          end
          if stripblanks(OO.graphics.id)~=emptystr() then
            Code_opar=[Code_opar;
                  '  '+cformatline(' * Identification: '+strcat(string(OO.graphics.id)),70)];
          end
          Code_opar($+1)='   */';
        end
      end
      //******************//

      for j=1:nopar
        if mat2scs_c_nb(opar(opptr(i)+j-1)) <> 11 then
          Code_opar =[Code_opar;
                 '  '+cformatline(mat2c_typ(opar(opptr(i)+j-1)) +...
                         ' opar_'+string(opptr(i)+j-1) + '[]={'+...
                             strcat(string(opar(opptr(i)+j-1)),',')+'};',70)]
        else //** cmplx test
          Code_opar =[Code_opar;
                 '  '+cformatline(mat2c_typ(opar(opptr(i)+j-1)) +...
                         ' opar_'+string(opptr(i)+j-1) + '[]={'+...
                             strcat(string([real(opar(opptr(i)+j-1)(:));
                                            imag(opar(opptr(i)+j-1)(:))]),',')+'};',70)]
        end
      end
      Code_opar($+1)='';

      //## size
      Code_oparsz   = []
      //** 1st dim **//
      for j=1:nopar
        Code_oparsz=[Code_oparsz
                     string(size(opar(opptr(i)+j-1),1))]
      end
      //** 2dn dim **//
      for j=1:nopar
        Code_oparsz=[Code_oparsz
                     string(size(opar(opptr(i)+j-1),2))]
      end
      Code_tooparsz=cformatline(strcat(Code_oparsz,','),70);
      Code_tooparsz(1)='int oparsz_'+string(i)+'[]={'+Code_tooparsz(1);
      for j=2:size(Code_tooparsz,1)
        Code_tooparsz(j)=get_blank('int oparsz_'+string(i)+'[]')+Code_tooparsz(j);
      end
      Code_tooparsz($)=Code_tooparsz($)+'};'
      Code_ooparsz=[Code_ooparsz;
                    Code_tooparsz];

      //## typ
      Code_opartyp   = []
      for j=1:nopar
        Code_opartyp=[Code_opartyp
                      mat2scs_c_typ(opar(opptr(i)+j-1))]
      end
      Code_toopartyp=cformatline(strcat(Code_opartyp,','),70);
      Code_toopartyp(1)='int opartyp_'+string(i)+'[]={'+Code_toopartyp(1);
      for j=2:size(Code_toopartyp,1)
        Code_toopartyp(j)=get_blank('int opartyp_'+string(i)+'[]')+Code_toopartyp(j);
      end
      Code_toopartyp($)=Code_toopartyp($)+'};'
      Code_oopartyp=[Code_oopartyp;
                     Code_toopartyp];

      //## ptr
      Code_tooparptr=cformatline(strcat(string(zeros(1,nopar)),','),70);
      Code_tooparptr(1)='void *oparptr_'+string(i)+'[]={'+Code_tooparptr(1);
      for j=2:size(Code_tooparptr,1)
        Code_tooparptr(j)=get_blank('void *oparptr_'+string(i)+'[]')+Code_tooparptr(j);
      end
      Code_tooparptr($)=Code_tooparptr($)+'};'
      Code_oparptr=[Code_oparptr
                    Code_tooparptr]

    end
  end

  if Code_opar <> [] then
    Code=[Code;
          '  /* Object parameters declaration */'
          Code_opar
          ''
          '  '+Code_ooparsz
          ''
          '  '+Code_oopartyp
          ''
          '  '+Code_oparptr
          '']
  end

  //##########################//

  //*** Continuous state ***//
  if x <> [] then
   //## implicit block
   if impl_blk then
     Code=[Code;
           '  /* Continuous states declaration */'
           cformatline('  double x[]={'+strcat(string(x(1:nX/2)),',')+'};',70)
           cformatline('  double xd[]={'+strcat(string(zeros(nX/2+1:nX)),',')+'};',70)
           cformatline('  double res[]={'+strcat(string(zeros(1,nX/2)),',')+'};',70)
           ''
           '/* def xproperty */'
           cformatline('     int xprop[]={'+strcat(string(ones(1:nX/2)),',')+'};',70)
           '']

   //## explicit block
   else
     Code=[Code;
           '  /* Continuous states declaration */'
           cformatline('  double x[]={'+strcat(string(x),',')+'};',70)
           cformatline('  double xd[]={'+strcat(string(zeros(1,nX)),',')+'};',70)
           '']
   end
  end
  //************************//

  //### discrete states ###//
  if size(z,1) <> 0 then
    Code=[Code;
          '  /* Discrete states declaration */']
    for i=1:(length(zptr)-1)
      if zptr(i+1)-zptr(i)>0 then

        //** Add comments **//

        //## Modelica block
        if type(corinv(i))==15 then
          //## 09/04/08, Alan : we can extract here all informations
          //## from original scicos blocks with corinv : TODO
          Code($+1)='  /* MODELICA BLK Z COMMENTS : TODO ';
        else
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
          end
        end
        Code($+1)='  */';
        Code=[Code;
              cformatline('  double z_'+string(i)+'[]={'+...
              strcat(string(z(zptr(i):zptr(i+1)-1)),",")+'};',70)]
        Code($+1)='';
      end
      //******************//
    end
  end
  //#######################//

  //** declaration of work
  Code_work=[]
  for i=1:size(with_work,1)
    if with_work(i)==1 then
       Code_work=[Code_work
                  '  void *work_'+string(i)+'[]={0};']
    end
  end

  if Code_work<>[] then
    Code=[Code
          ''
          '  /* Work array declaration */'
          Code_work
          '']
  end

  //### Object state ###//
  //** declaration of oz
  Code_oz = [];
  Code_oozsz=[];
  Code_ooztyp=[];
  Code_ozptr=[];

  for i=1:(length(ozptr)-1)
    noz = ozptr(i+1)-ozptr(i)
    if noz>0 then

      for j=1:noz
        if mat2scs_c_nb(oz(ozptr(i)+j-1)) <> 11 then
          Code_oz=[Code_oz;
                   cformatline('  '+mat2c_typ(oz(ozptr(i)+j-1))+...
                               ' oz_'+string(ozptr(i)+j-1)+'[]={'+...
                               strcat(string(oz(ozptr(i)+j-1)(:)),',')+'};',70)]
        else //** cmplx test
          Code_oz=[Code_oz;
                   cformatline('  '+mat2c_typ(oz(ozptr(i)+j-1))+...
                               ' oz_'+string(ozptr(i)+j-1)+'[]={'+...
                               strcat(string([real(oz(ozptr(i)+j-1)(:));
                                              imag(oz(ozptr(i)+j-1)(:))]),',')+'};',70)]
        end
      end

      //## size
      Code_ozsz   = []
      //** 1st dim **//
      for j=1:noz
        Code_ozsz=[Code_ozsz
                     string(size(oz(ozptr(i)+j-1),1))]
      end
      //** 2dn dim **//
      for j=1:noz
        Code_ozsz=[Code_ozsz
                     string(size(oz(ozptr(i)+j-1),2))]
      end
      Code_toozsz=cformatline(strcat(Code_ozsz,','),70);
      Code_toozsz(1)='int ozsz_'+string(i)+'[]={'+Code_toozsz(1);
      for j=2:size(Code_toozsz,1)
        Code_toozsz(j)=get_blank('int ozsz_'+string(i)+'[]')+Code_toozsz(j);
      end
      Code_toozsz($)=Code_toozsz($)+'};'
      Code_oozsz=[Code_oozsz;
                    Code_toozsz];

      //## typ
      Code_oztyp   = []
      for j=1:noz
        Code_oztyp=[Code_oztyp
                      mat2scs_c_typ(oz(ozptr(i)+j-1))]
      end
      Code_tooztyp=cformatline(strcat(Code_oztyp,','),70);
      Code_tooztyp(1)='int oztyp_'+string(i)+'[]={'+Code_tooztyp(1);
      for j=2:size(Code_tooztyp,1)
        Code_tooztyp(j)=get_blank('int oztyp_'+string(i)+'[]')+Code_tooztyp(j);
      end
      Code_tooztyp($)=Code_tooztyp($)+'};'
      Code_ooztyp=[Code_ooztyp;
                     Code_tooztyp];

      //## ptr
      Code_toozptr=cformatline(strcat(string(zeros(1,noz)),','),70);
      Code_toozptr(1)='void *ozptr_'+string(i)+'[]={'+Code_toozptr(1);
      for j=2:size(Code_toozptr,1)
        Code_toozptr(j)=get_blank('void *ozptr_'+string(i)+'[]')+Code_toozptr(j);
      end
      Code_toozptr($)=Code_toozptr($)+'};'
      Code_ozptr=[Code_ozptr
                    Code_toozptr]

    end
  end

  if Code_oz <> [] then
    Code=[Code;
          '  /* Object discrete states declaration */'
          Code_oz
          ''
          '  '+Code_oozsz
          ''
          '  '+Code_ooztyp
          ''
          '  '+Code_ozptr]
  end
  //#######################//

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
          '  /* Output declaration */'
          Code_outtb
          '']
  end

  Code_outtbptr=[];
  for i=1:lstsize(outtb)
    Code_outtbptr=[Code_outtbptr;
                   '  '+rdnom+'_block_outtbptr['+...
                    string(i-1)+'] = (void *) outtb_'+string(i)+';'];
  end

  //##### insz/outsz #####//
  Code_iinsz=[];
  Code_inptr=[];
  Code_ooutsz=[];
  Code_outptr=[];
  for kf=1:nblk
    nin=inpptr(kf+1)-inpptr(kf);  //** number of input ports
    Code_insz=[];

    //########
    //## insz
    //########

    //## case sensor ##//
    if or(kf==capt(:,1)) then
      ind=find(kf==capt(:,1))
      //Code_insz = 'typin['+string(ind-1)+']'
    //## other blocks ##//
    elseif nin<>0 then
      //** 1st dim **//
      for kk=1:nin
         lprt=inplnk(inpptr(kf)-1+kk);
         Code_insz=[Code_insz
                    string(size(outtb(lprt),1))]
      end
      //** 2dn dim **//
      for kk=1:nin
         lprt=inplnk(inpptr(kf)-1+kk);
         Code_insz=[Code_insz
                    string(size(outtb(lprt),2))]
      end
      //** typ **//
      for kk=1:nin
         lprt=inplnk(inpptr(kf)-1+kk);
         Code_insz=[Code_insz
                    mat2scs_c_typ(outtb(lprt))]
      end
    end
    if Code_insz<>[] then
      Code_toinsz=cformatline(strcat(Code_insz,','),70);
      Code_toinsz(1)='int insz_'+string(kf)+'[]={'+Code_toinsz(1);
      for j=2:size(Code_toinsz,1)
        Code_toinsz(j)=get_blank('int insz_'+string(kf)+'[]')+Code_toinsz(j);
      end
      Code_toinsz($)=Code_toinsz($)+'};'
      Code_iinsz=[Code_iinsz
                  Code_toinsz]
    end

    //########
    //## inptr
    //########

    //## case sensor ##//
    if or(kf==capt(:,1)) then
      Code_inptr=[Code_inptr;
                  'void *inptr_'+string(kf)+'[]={0};';]
    //## other blocks ##//
    elseif nin<>0 then
      Code_toinptr=cformatline(strcat(string(zeros(1,nin)),','),70);
      Code_toinptr(1)='void *inptr_'+string(kf)+'[]={'+Code_toinptr(1);
      for j=2:size(Code_toinptr,1)
        Code_toinptr(j)=get_blank('void *inptr_'+string(kf)+'[]')+Code_toinptr(j);
      end
      Code_toinptr($)=Code_toinptr($)+'};'
      Code_inptr=[Code_inptr
                  Code_toinptr]
    end

    nout=outptr(kf+1)-outptr(kf); //** number of output ports
    Code_outsz=[];

    //########
    //## outsz
    //########

    //## case actuators ##//
    if or(kf==actt(:,1)) then
      ind=find(kf==actt(:,1))
      //Code_outsz = 'typout['+string(ind-1)+']'
    //## other blocks ##//
    elseif nout<>0 then
      //** 1st dim **//
      for kk=1:nout
         lprt=outlnk(outptr(kf)-1+kk);
         Code_outsz=[Code_outsz
                     string(size(outtb(lprt),1))]
      end
      //** 2dn dim **//
      for kk=1:nout
         lprt=outlnk(outptr(kf)-1+kk);
         Code_outsz=[Code_outsz
                     string(size(outtb(lprt),2))]
      end
      //** typ **//
      for kk=1:nout
         lprt=outlnk(outptr(kf)-1+kk);
         Code_outsz=[Code_outsz
                     mat2scs_c_typ(outtb(lprt))]
      end
    end
    if Code_outsz<>[] then
      Code_tooustz=cformatline(strcat(Code_outsz,','),70);
      Code_tooustz(1)='int outsz_'+string(kf)+'[]={'+Code_tooustz(1);
      for j=2:size(Code_tooustz,1)
        Code_tooustz(j)=get_blank('int outsz_'+string(kf)+'[]')+Code_tooustz(j);
      end
      Code_tooustz($)=Code_tooustz($)+'};'
      Code_ooutsz=[Code_ooutsz
                   Code_tooustz]
    end

    //#########
    //## outptr
    //#########

    //## case actuators ##//
    if or(kf==actt(:,1)) then
      Code_outptr=[Code_outptr;
                   'void *outptr_'+string(kf)+'[]={0};';]
    //## other blocks ##//
    elseif nout<>0 then
      Code_tooutptr=cformatline(strcat(string(zeros(1,nout)),','),70);
      Code_tooutptr(1)='void *outptr_'+string(kf)+'[]={'+Code_tooutptr(1);
      for j=2:size(Code_tooutptr,1)
        Code_tooutptr(j)=get_blank('void *outptr_'+string(kf)+'[]')+Code_tooutptr(j);
      end
      Code_tooutptr($)=Code_tooutptr($)+'};'
      Code_outptr=[Code_outptr
                   Code_tooutptr]
    end
  end

  if Code_iinsz<>[] then
     Code=[Code;
          '  /* Inputs */'
          '  '+Code_iinsz
          ''];
  end
  if Code_inptr<>[] then
     Code=[Code;
          '  '+Code_inptr
          ''];
  end
  if Code_ooutsz<>[] then
     Code=[Code;
          '  /* Outputs */'
          '  '+Code_ooutsz
          ''];
  end
  if Code_outptr<>[] then
     Code=[Code;
          '  '+Code_outptr
          ''];
  end
  //######################//

  //##### out events #####//
  Code_evout=[];
  for kf=1:nblk
    if funs(kf)<>'bidon' then
      nevout=clkptr(kf+1)-clkptr(kf);
      if nevout <> 0 then
        Code_toevout=cformatline(strcat(string(cpr.state.evtspt((clkptr(kf):clkptr(kf+1)-1))),','),70);
        Code_toevout(1)='double evout_'+string(kf)+'[]={'+Code_toevout(1);
        for j=2:size(Code_toevout,1)
          Code_toevout(j)=get_blank('double evout_'+string(kf)+'[]')+Code_toevout(j);
        end
        Code_toevout($)=Code_toevout($)+'};';
        Code_evout=[Code_evout
                    Code_toevout];
      end
    end
  end
  if Code_evout<>[] then
     Code=[Code;
          '  /* Outputs event declaration */'
          '  '+Code_evout
          ''];
  end
  //################//

  //## input connection to outtb
  Code_inptr=[]
  for kf=1:nblk
    nin=inpptr(kf+1)-inpptr(kf);  //** number of input ports
    //## case sensor ##//
    if or(kf==capt(:,1)) then
      ind=find(kf==capt(:,1))
      Code_inptr=[Code_inptr;
                  '  inptr_'+string(kf)+'[0] = inptr['+string(ind-1)+'];';]
    //## other blocks ##//
    elseif nin<>0 then
      for k=1:nin
        lprt=inplnk(inpptr(kf)-1+k);
        Code_inptr=[Code_inptr
                    '  inptr_'+string(kf)+'['+string(k-1)+'] = (void *) outtb_'+string(lprt)+';']
      end
    end
  end
  if Code_inptr<>[] then
    Code=[Code;
          '  /* Affectation of inptr */';
          Code_inptr;
          ''];
  end

  //## output connection to outtb
  Code_outptr=[]
  for kf=1:nblk
    nout=outptr(kf+1)-outptr(kf); //** number of output ports
    //## case actuators ##//
    if or(kf==actt(:,1)) then
    ind=find(kf==actt(:,1))
    Code_outptr=[Code_outptr;
                 '  outptr_'+string(kf)+'[0] = outptr['+string(ind-1)+'];';]
    //## other blocks ##//
    elseif nout<>0 then
      for k=1:nout
        lprt=outlnk(outptr(kf)-1+k);
        Code_outptr=[Code_outptr
                    '  outptr_'+string(kf)+'['+string(k-1)+'] = (void *) outtb_'+string(lprt)+';']
      end
    end
  end
  if Code_outptr<>[] then
    Code=[Code;
          '  /* Affectation of outptr */';
          Code_outptr;
          ''];
  end

  //## affectation of oparptr
  Code_oparptr=[]
  for kf=1:nblk
    nopar=opptr(kf+1)-opptr(kf); //** number of object parameters
    if nopar<>0 then
      for k=1:nopar
        Code_oparptr=[Code_oparptr
                    '  oparptr_'+string(kf)+'['+string(k-1)+'] = (void *) opar_'+string(opptr(kf)+k-1)+';']
      end
    end
  end
  if Code_oparptr<>[] then
    Code=[Code;
          '  /* Affectation of oparptr */';
          Code_oparptr;
          ''];
  end

  //## affectation of ozptr
  Code_ozptr=[]
  for kf=1:nblk
    noz=ozptr(kf+1)-ozptr(kf); //** number of object states
    if noz<>0 then
      for k=1:noz
        Code_ozptr=[Code_ozptr
                    '  ozptr_'+string(kf)+'['+string(k-1)+'] = (void *) oz_'+string(ozptr(kf)+k-1)+';']
      end
    end
  end
  if Code_ozptr<>[] then
    Code=[Code;
          '  /* Affectation of ozptr */';
          Code_ozptr;
          ''];
  end

  //## fields of each scicos structure
  for kf=1:nblk
    if funs(kf)<>'bidon' then
      nx=xptr(kf+1)-xptr(kf);         //** number of continuous state
      nz=zptr(kf+1)-zptr(kf);         //** number of discrete state
      nin=inpptr(kf+1)-inpptr(kf);    //** number of input ports
      nout=outptr(kf+1)-outptr(kf);   //** number of output ports
      nevout=clkptr(kf+1)-clkptr(kf); //** number of event output ports

      //** add comment
      txt=[get_comment('set_blk',list(funs(kf),funtyp(kf),kf));]

      Code=[Code;
            '  '+txt];

      Code=[Code;
            '  block_'+rdnom+'['+string(kf-1)+'].type    = '+string(funtyp(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].ztyp    = '+string(ztyp(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].ng      = '+string(zcptr(kf+1)-zcptr(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].nz      = '+string(zptr(kf+1)-zptr(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].nx      = '+string(nx)+';';
            '  block_'+rdnom+'['+string(kf-1)+'].noz     = '+string(ozptr(kf+1)-ozptr(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].nrpar   = '+string(rpptr(kf+1)-rpptr(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].nopar   = '+string(opptr(kf+1)-opptr(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].nipar   = '+string(ipptr(kf+1)-ipptr(kf))+';'
            '  block_'+rdnom+'['+string(kf-1)+'].nin     = '+string(inpptr(kf+1)-inpptr(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].nout    = '+string(outptr(kf+1)-outptr(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].nevout  = '+string(clkptr(kf+1)-clkptr(kf))+';';
            '  block_'+rdnom+'['+string(kf-1)+'].nmode   = '+string(modptr(kf+1)-modptr(kf))+';';]

      if nx <> 0 then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].x       = &(x['+string(xptr(kf)-1)+']);'
              '  block_'+rdnom+'['+string(kf-1)+'].xd      = &(xd['+string(xptr(kf)-1)+']);']
        if impl_blk then
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].res     = &(res['+string(xptr(kf)-1)+']);'
                '  block_'+rdnom+'['+string(kf-1)+'].xprop   = &(xprop['+string(xptr(kf)-1)+']);'];
        end
      end

      //** evout **//
      if nevout<>0 then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].evout   = evout_'+string(kf)+';']
      end

      //***************************** input port *****************************//
      //## case sensor ##//
      if or(kf==capt(:,1)) then
        ind=find(kf==capt(:,1))
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].inptr   = inptr_'+string(kf)+';'
              '  block_'+rdnom+'['+string(kf-1)+'].insz    = &typin['+string(ind-1)+'];']
//              '  block_'+rdnom+'['+string(kf-1)+'].insz    = insz_'+string(kf)+';']
      //## other blocks ##//
      elseif nin<>0 then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].inptr   = inptr_'+string(kf)+';'
              '  block_'+rdnom+'['+string(kf-1)+'].insz    = insz_'+string(kf)+';']
      end
      //**********************************************************************//

      //***************************** output port *****************************//
      //## case actuators ##//
      if or(kf==actt(:,1)) then
        ind=find(kf==actt(:,1))
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].outptr  = outptr_'+string(kf)+';'
              '  block_'+rdnom+'['+string(kf-1)+'].outsz   = &typout['+string(ind-1)+'];']
//              '  block_'+rdnom+'['+string(kf-1)+'].outsz   = outsz_'+string(kf)+';']
      //## other blocks ##//
      elseif nout<>0 then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].outptr  = outptr_'+string(kf)+';'
              '  block_'+rdnom+'['+string(kf-1)+'].outsz   = outsz_'+string(kf)+';']
      end
      //**********************************************************************//

      //## discrete states ##//
      if (nz>0) then
        Code=[Code
              '  block_'+rdnom+'['+string(kf-1)+...
              '].z       = z_'+string(kf)+';']
      end

      //** rpar **//
      if (rpptr(kf+1)-rpptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+...
              '].rpar    = rpar_'+string(kf)+';']
      end

      //** ipar **//
      if (ipptr(kf+1)-ipptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+...
              '].ipar    = ipar_'+string(kf)+';']
      end

      //** opar **//
      if (opptr(kf+1)-opptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].oparptr = oparptr_'+string(kf)+';'
              '  block_'+rdnom+'['+string(kf-1)+'].oparsz  = oparsz_'+string(kf)+';'
              '  block_'+rdnom+'['+string(kf-1)+'].opartyp = opartyp_'+string(kf)+';'
              ]
      end

      //** oz **//
      if (ozptr(kf+1)-ozptr(kf)>0) then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].ozptr   = ozptr_'+string(kf)+';'
              '  block_'+rdnom+'['+string(kf-1)+'].ozsz    = ozsz_'+string(kf)+';'
              '  block_'+rdnom+'['+string(kf-1)+'].oztyp   = oztyp_'+string(kf)+';'
              ]
      end

      //** work **/
      if with_work(kf)==1 then
        Code=[Code;
              '  block_'+rdnom+'['+string(kf-1)+'].work    = work_'+string(kf)+';'
              ]
      end

      //** TODO label **//

      Code=[Code;
            '']
    end
  end

  Code=[Code;
        '  /* set a variable to trace error of blocks */'
        '  block_error=&err; /*GLOBAL*/'
        ''
        '  /* set initial time */'
        '  t=0.0;'
        ''
        '  /* set initial phase simulation */'
        '  phase=1;'
        '']

  if impl_blk then
    Code=[Code;
          '  /* default jacob_param */'
          '  CJJ=1/h;'
          '']
  end

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
          '  /* Initial blocks must be called with flag 1 */'
          txt];
  end

  //## reinidoit
  if x <> [] then
    //## implicit block
    if impl_blk then
      txt=[write_code_reinitdoit(1) //** first pass
           write_code_reinitdoit(7) //** second pass
          ]

      if txt<>[] then
        Code=[Code;
              '  /* Initial derivative computation */'
              txt];
      end
    end
  end

  //** begin input main loop on time
  Code=[Code;
        ''
        '  while (t<=tf) {';
        '    /* */'
        '    scicos_time=t;'
        '']

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
          '      switch (solver) {']

    if impl_blk then
      Code=[Code
            '      case 1:'
            '        err=dae1('+rdnom+'simblk_imp,x,xd,res,tout,h);'
            '        break;'
            '      default :'
            '        err=dae1('+rdnom+'simblk_imp,x,xd,res,tout,h);'
            '        break;'
            '      }']
    else
      Code=[Code
            '      case 1:'
            '        err=ode1('+rdnom+'simblk,x,xd,tout,h);'
            '        break;'
            '      case 2:'
            '        err=ode2('+rdnom+'simblk,x,xd,tout,h);'
            '        break;'
            '      case 3:'
            '        err=ode4('+rdnom+'simblk,x,xd,tout,h);'
            '        break;'
            '      default :'
            '        err=ode4('+rdnom+'simblk,x,xd,tout,h);'
            '        break;'
            '      }']
    end
    Code=[Code
          '      if (err!=0) return err;']

    Code=[Code
          '       tout=tout+h;'
          '    }'
          ''
          '    /* integration for the remainder piece of time */'
          '    he=t+dt-tout;'
          '    switch (solver) {']

    if impl_blk then
      Code=[Code
            '    case 1:'
            '      err=dae1('+rdnom+'simblk_imp,x,xd,res,tout,he);'
            '      break;'
            '    default :'
            '      err=dae1('+rdnom+'simblk_imp,x,xd,res,tout,he);'
            '      break;'
            '      }']
    else
      Code=[Code
            '    case 1:'
            '      err=ode1('+rdnom+'simblk,x,xd,tout,he);'
            '      break;'
            '    case 2:'
            '      err=ode2('+rdnom+'simblk,x,xd,tout,he);'
            '      break;'
            '    case 3:'
            '      err=ode4('+rdnom+'simblk,x,xd,tout,he);'
            '      break;'
            '    default :'
            '      err=ode4('+rdnom+'simblk,x,xd,tout,he);'
            '      break;'
            '    }']
    end
    Code=[Code
          '    if (err!=0) return err;']
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
        if impl_blk then
          Code=[Code;
                '    block_'+rdnom+'['+string(kf-1)+'].res = '+...
                 '&(res['+string(xptr(kf)-1)+']);']
        end
      end
    end
  end

  Code=[Code
        ''
        '    /* update current time */'
        '    t=t+dt;'
        ''
        '    /* set phase simulation */'
        '    phase=1;'
        '  }']

  //** flag 5
//   Code=[Code;
//         Code_end]

  Code=[Code
        ''
        '  Cosend();'
        '}']

  Code=[Code
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
        '  *block_error = err;'
        '  return;'
        '}'
        ''
        'int get_block_error()'
        '{'
        '  return *block_error;'
        '}'
        ''
        'int get_phase_simulation()'
        '{'
        '  return phase;'
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
        '  return scicos_time;'
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
        '}']

 Code=[Code
        ''
        '#if WIN32'
        ' #ifndef vsnprintf'
        '   #define vsnprintf _vsnprintf'
        ' #endif'
        '#endif'
        ''
        '#ifdef __STDC__'
        'void Coserror (char *fmt,...)'
        '#else'
        'void Coserror(va_alist) va_dcl'
        '#endif'
        '{'
        ' int retval;'
        ' va_list ap;'
        ''
        '#ifdef __STDC__'
        ' va_start(ap,fmt);'
        '#else'
        ''
        ' char *fmt;'
        ' va_start(ap);'
        ''
        ' fmt = va_arg(ap, char *);'
        '#endif'
        ''
        '#if defined (vsnprintf) || defined (linux)'
        ' retval= vsnprintf(err_msg,4095, fmt, ap);'
        '#else'
        ' retval= vsprintf(err_msg,fmt, ap);'
        '#endif'
        ''
        ' if (retval == -1) {'
        '   err_msg[0]=''\0'';'
        ' }'
        ''
        ' va_end(ap);'
        ''
        ' /* coserror use error number 10 */'
        ' *block_error=-5;'
        ''
        ' return;'
        '}'
        '']

  if impl_blk then
    Code=[Code;
          'void Set_Jacobian_flag(int flag)'
          '{'
          '  Jacobian_Flag=flag;'
          '  return;'
          '}'
          ''
          'double Get_Jacobian_parameter(void)'
          '{'
          '  return CJJ;'
          '}'
          ''
          'double Get_Scicos_SQUR(void)'
          '{'
          '  return  SQuround;'
          '}'
         ]
  end

  Code=[Code;
        'int getopt (int argc, char *argv[], char *optstring)'
        '{'
        '  char *group, option, *sopt;'
        '  char *optarg;'
        '  int len;'
        '  int offset = 0;'
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
        '                 fprintf ( stderr, '"%s: illegal option -- %c \n'",'
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
        '         fprintf ( stderr, '"%s: illegal option -- %c \n'", argv[0], option );'
        '         option = ''?'';'
        '         break;'
        '        }'
        '    }'
        '  return ( option );'
        '}'
        '']

  if (x <> []) then

    //## implicit case
    if impl_blk then
      Code=[Code;
            'int '+rdnom+'simblk_imp(t, x, xd, res)'
            ''
            '   double t, *x, *xd, *res;'
            ''
            '     /*'
            '      *  !purpose'
            '      *  compute state derivative of the continuous part'
            '      *  !calling sequence'
            '      *  NEQ   : a defined integer : the size of the  continuous state'
            '      *  t     : current time'
            '      *  x     : double precision vector whose contains the continuous state'
            '      *  xd    : double precision vector whose contains the computed derivative'
            '      *          of the state'
            '      *  res   : double precision vector whose contains the computed residual'
            '      *          of the state'
            '      */'
            '{'
            '  /* local variables used to call block */'
            '  int local_flag;']

      if act<>[] | cap<>[] then
        Code=[Code;
              '  int nport;']
      end

      Code=[Code;
            ''
            '  /* counter local variable */'
            '  int i;'
            '']

      if with_nrd then
        //## look at for block of type 0 (no captor)
        ind=find(funtyp==0)
        if ind<>[] then
          with_nrd2=%t
        else
          with_nrd2=%f
        end
//         with_nrd2=%f;
//         for k=1:size(ind,2)
//           if ~or(oord([ind(k)],1)==cap) then
//             with_nrd2=%t;
//             break;
//           end
//         end
        if with_nrd2 then
          Code=[Code;
                '  /* Variables for constant values */'
                '  int nrd_1, nrd_2;'
                ''
                '  double *args[100];'
                '']
        end
      end

      Code=[Code;
            '  /* set phase simulation */'
            '  phase=2;'
            ''
            '  /* initialization of residual */'
            '  for(i=0;i<NEQ;i++) res[i]=xd[i];'
            '']

      Code=[Code;
            '  '+get_comment('update_xd',list())]

      for kf=1:nblk
        if (xptr(kf+1)-xptr(kf)) > 0 then
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].x='+...
                  '&(x['+string(xptr(kf)-1)+']);'
                '  block_'+rdnom+'['+string(kf-1)+'].xd='+...
                  '&(xd['+string(xptr(kf)-1)+']);'
                '  block_'+rdnom+'['+string(kf-1)+'].res='+...
                  '&(res['+string(xptr(kf)-1)+']);']
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
            '/* DAE Method */'
            'int dae1(f,x,xd,res,t,h)'
            '  int (*f) ();'
            '  double *x,*xd,*res;'
            '  double t, h;'
            '{'
            '  int i;'
            '  int ierr;'
            ''
            '  /**/'
            '  ierr=(*f)(t,x, xd, res);'
            '  if (ierr!=0) return ierr;'
            ''
            '  for (i=0;i<NEQ;i++) {'
            '   x[i]=x[i]+h*xd[i];'
            '  }'
            ''
            '  return 0;'
            '}']
    //## explicit case
    else
      Code=[Code;
            'int '+rdnom+'simblk(t, x, xd)'
            ''
            '   double t, *x, *xd;'
            ''
            '     /*'
            '      *  !purpose'
            '      *  compute state derivative of the continuous part'
            '      *  !calling sequence'
            '      *  NEQ   : a defined integer : the size of the  continuous state'
            '      *  t     : current time'
            '      *  x     : double precision vector whose contains the continuous state'
            '      *  xd    : double precision vector whose contains the computed derivative'
            '      *          of the state'
            '      */'
            '{'
            '  /* local variables used to call block */'
            '  int local_flag;']

      if act<>[] | cap<>[] then
        Code=[Code;
              '  int nport;']
      end

      Code=[Code;
            ''
            '  /* counter local variable */'
            '  int i;'
            '']

      if with_nrd then
        //## look at for block of type 0 (no captor)
        ind=find(funtyp==0)
        if ind<>[] then
          with_nrd2=%t
        else
          with_nrd2=%f
        end
//         with_nrd2=%f;
//         for k=1:size(ind,2)
//           if ~or(oord([ind(k)],1)==cap) then
//             with_nrd2=%t;
//             break;
//           end
//         end
        if with_nrd2 then
          Code=[Code;
                '  /* Variables for constant values */'
                '  int nrd_1, nrd_2;'
                ''
                '  double *args[100];'
                '']
        end
      end

      Code=[Code;
            '  /* set phase simulation */'
            '  phase=2;'
            ''
            '  /* initialization of derivatives */'
            '  for(i=0;i<NEQ;i++) xd[i]=0.;'
            '']

      Code=[Code;
            '  '+get_comment('update_xd',list())]

      for kf=1:nblk
        if (xptr(kf+1)-xptr(kf)) > 0 then
          Code=[Code;
                '  block_'+rdnom+'['+string(kf-1)+'].x='+...
                  '&(x['+string(xptr(kf)-1)+']);'
                '  block_'+rdnom+'['+string(kf-1)+'].xd='+...
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
            'int ode1(f,x,xd,t,h)'
            '  int (*f) ();'
            '  double *x,*xd;'
            '  double t, h;'
            '{'
            '  int i;'
            '  int ierr;'
            ''
            '  /**/'
            '  ierr=(*f)(t,x, xd);'
            '  if (ierr!=0) return ierr;'
            ''
            '  for (i=0;i<NEQ;i++) {'
            '   x[i]=x[i]+h*xd[i];'
            '  }'
            ''
            '  return 0;'
            '}'
            ''
            '/* Heun''s Method */'
            'int ode2(f,x,xd,t,h)'
            '  int (*f) ();'
            '  double *x,*xd;'
            '  double t, h;'
            '{'
            '  int i;'
            '  int ierr;'
            '  double y['+string(nX)+'],yh['+string(nX)+'],temp,f0['+string(nX)+'],th;'
            ''
            '  /**/'
            '  memcpy(y,x,NEQ*sizeof(double));'
            '  memcpy(f0,xd,NEQ*sizeof(double));'
            ''
            '  /**/'
            '  ierr=(*f)(t,y, f0);'
            '  if (ierr!=0) return ierr;'
            ''
            '  /**/'
            '  for (i=0;i<NEQ;i++) {'
            '    x[i]=y[i]+h*f0[i];'
            '  }'
            '  th=t+h;'
            '  for (i=0;i<NEQ;i++) {'
            '    yh[i]=y[i]+h*f0[i];'
            '  }'
            '  ierr=(*f)(th,yh, xd);'
            '  if (ierr!=0) return ierr;'
            ''
            '  /**/'
            '  temp=0.5*h;'
            '  for (i=0;i<NEQ;i++) {'
            '    x[i]=y[i]+temp*(f0[i]+xd[i]);'
            '  }'
            ''
            '  return 0;'
            '}'
            ''
            '/* Fourth-Order Runge-Kutta (RK4) Formula */'
            'int ode4(f,x,xd,t,h)'
            '  int (*f) ();'
            '  double *x,*xd;'
            '  double t, h;'
            '{'
            '  int i;'
            '  int ierr;'
            '  double y['+string(nX)+'],yh['+string(nX)+'],'+...
              'temp,f0['+string(nX)+'],th,th2,'+...
              'f1['+string(nX)+'],f2['+string(nX)+'];'
            ''
            '  /**/'
            '  memcpy(y,x,NEQ*sizeof(double));'
            '  memcpy(f0,xd,NEQ*sizeof(double));'
            ''
            '  /**/'
            '  ierr=(*f)(t,y, f0);'
            '  if (ierr!=0) return ierr;'
            ''
            '  /**/'
            '  for (i=0;i<NEQ;i++) {'
            '    x[i]=y[i]+h*f0[i];'
            '  }'
            '  th2=t+h/2;'
            '  for (i=0;i<NEQ;i++) {'
            '    yh[i]=y[i]+(h/2)*f0[i];'
            '  }'
            '  ierr=(*f)(th2,yh, f1);'
            '  if (ierr!=0) return ierr;'
            ''
            '  /**/'
            '  temp=0.5*h;'
            '  for (i=0;i<NEQ;i++) {'
            '    x[i]=y[i]+temp*f1[i];'
            '  }'
            '  for (i=0;i<NEQ;i++) {'
            '    yh[i]=y[i]+(h/2)*f1[i];'
            '  }'
            '  ierr=(*f)(th2,yh, f2);'
            '  if (ierr!=0) return ierr;'
            ''
            '  /**/'
            '  for (i=0;i<NEQ;i++) {'
            '    x[i]=y[i]+h*f2[i];'
            '  }'
            '  th=t+h;'
            '  for (i=0;i<NEQ;i++) {'
            '    yh[i]=y[i]+h*f2[i];'
            '  }'
            '  ierr=(*f)(th2,yh, xd);'
            '  if (ierr!=0) return ierr;'
            ''
            '  /**/'
            '  temp=h/6;'
            '  for (i=0;i<NEQ;i++) {'
            '    x[i]=y[i]+temp*(f0[i]+2.0*f1[i]+2.0*f2[i]+xd[i]);'
            '  }'
            ''
            '  return 0;'
            '}']
    end
  end
endfunction

//generates  static table definitions
//
//Author : Rachid Djenidi
//Copyright INRIA
function txt=make_static_standalone42()
  txt=['static int optind = 1;'
       'static void usage(char *);'
       ''];

endfunction

//** Generates the C code for sensors/actuators
//** of the scilab interface
//
//08/01/08 Alan Layec
//Copyright INRIA
function Code=make_void_io()

  //## for debug
  Code=['#include<stdio.h>'
        '#include<stdlib.h>'
        '#include ""stack-c.h""'
        '#include ""mex.h""']

  //## type of in/out structure definition
  Code=[Code;
        ''
        '/* structure definition of in/out sensors/actuators */'
        'typedef struct {'
        '  int typ;      /* data type */'
        '  int ndims;    /* number of dims */'
        '  int ndata;    /* number of data */'
        '  int *dims;    /* size of data (length ndims) */'
        '  double *time; /* date of data (length ndata) */'
        '  void *data;   /* data (length ndata*prod(dims)) */'
        '} scicos_inout;'
        '']

  //## Actuators
  Code=[Code
        '/*---------------------------------------- Actuators */'
        'void '+rdnom+'_actuator(flag,nport,nevprt,t,u,nu1,nu2,ut,typout,outptr)'
        '     int *flag,*nevprt,*nport;'
        '     int *nu1,*nu2,*ut;'
        ''
        '     int typout;'
        '     void *outptr;'
        ''
        '     double *t;'
        '     void *u;'
        '{'
        '  /*int k,l;*/']

  //## declaration of scicos_inout variables for output of actuators
  if size(actt,1)<>0 then
    Code=[Code
          '  /* declaration of scicos_inout variable for output of actuator */'
          '  scicos_inout *out;'
          '']
  end

  //## declaration of static counter variable for actuators (state)
  if size(actt,1)<>0 then
    Code=[Code
          '  /* static counter variable */']
    for i=1:size(actt,1)
      Code=[Code
            '  static int cnt_'+string(i)+';']
    end
  end

  //## declaration of a local counter variable
  if size(actt,1)<>0 then
    Code=[Code
          '  int cnt=0;'
          '']

    //## update the local counter variable value
    Code=[Code
          '  /* update cnt */']

    for i=1:size(actt,1)
      if i==1 then
        Code=[Code
              '  if(*nport=='+string(i)+') {'
              '    cnt=cnt_'+string(i)+';']
      else
        Code=[Code
              '  }'
              '  else if(*nport=='+string(i)+') {'
              '    cnt=cnt_'+string(i)+';']
      end
    end
    Code=[Code
          '  }'
          '']
  end

  //## affectation of output structure of actuators
  if size(actt,1)<>0 then
    Code=[Code
          '  /* affectation of output structure of actuators*/'
          '  out=(scicos_inout *)outptr;'
          '']
  end

  Code=[Code
        '  switch (*flag) {'
        ''
        '    case 4 : /* actuator initialisation */']
  if size(actt,1)<>0 then
    Code=[Code
          '      /* initialisation of counter variable */'
          '      cnt=0;']
  end
  Code=[Code
        '      break;'
        '']

  if szclkIN==[]&ALWAYS_ACTIVE then
    Code=[Code;
          '    case 1 :']
  else
    Code=[Code;
          '    case 2 :']
  end

  Code=[Code
        '      switch (*ut) {'
        '      case 10 :']

  if size(actt,1)<>0 then
    Code=[Code
          '        memcpy(((double *) out->data + cnt*(*nu1)*(*nu2)), \'
          '               ((double *) u), (*nu1)*(*nu2)*sizeof(double));'
          '        /* *((double *) out->data + cnt)=*((double *) u); */'
          '        out->time[cnt]=*t;'
          '        /*fprintf(stderr,""actuator %d : cnt = %d\n"",*nport,cnt);*/']
  end

  Code=[Code
        '        break;'
        ''
        '      case 11 :']

  if size(actt,1)<>0 then
    Code=[Code
          '        memcpy(((double *) out->data + cnt*(*nu1)*(*nu2)), \'
          '               ((double *) u), (*nu1)*(*nu2)*sizeof(double));'
          '        memcpy(((double *) out->data + cnt*(*nu1)*(*nu2) + out->ndata), \'
          '               ((double *) u + (*nu1)*(*nu2)), (*nu1)*(*nu2)*sizeof(double));'
          '        out->time[cnt]=*t;']
  end

  Code=[Code
        '        break;'
        ''
        '      case 81 :'
        '        break;'
        ''
        '      case 82 :'
        '        break;'
        ''
        '      case 84 :'
        '        break;'
        ''
        '      case 811 :'
        '        break;'
        ''
        '      case 812 :'
        '        break;'
        ''
        '      case 814 :'
        '        break;'
        '      }'
        '']
  //## increase the local counter variable value
  if size(actt,1)<>0 then
    Code=[Code
          '      /* increase counter variable */'
          '      /*fprintf(stderr,""out->ndata=%d\n"",out->ndata);*/'
          '      cnt++;'
          ''
          '      /* check and realloc out->data/out->time if needed */'
          '      if (cnt==out->ndata) {'
          '        out->ndata=2*out->ndata;']

    Code=[Code
          '        if ((*ut)==11) {'
          '          if ((out->data = (double *) realloc(out->data, \'
          '               out->ndata*(*nu1)*(*nu2)*2*sizeof(double)))==NULL) {'
          '            set_block_error(-16);'
          '            return;'
          '          }'
          '          memcpy(((double *) out->data + (out->ndata/2)*(*nu1)*(*nu2)), \'
          '                 ((double *) out->data + out->ndata*(*nu1)*(*nu2)), \'
          '                  (out->ndata/2)*(*nu1)*(*nu2)*sizeof(double));'
          '        }'
          '        else {'
          '          if ((out->data = (double *) realloc(out->data, \'
          '               out->ndata*(*nu1)*(*nu2)*sizeof(double)))==NULL) {'
          '            set_block_error(-16);'
          '            return;'
          '          }'
          '        }']

    Code=[Code
          '        if ((out->time = (double *) realloc(out->time, \'
          '             out->ndata*sizeof(double)))==NULL) {'
          '          set_block_error(-16);'
          '          return;'
          '        }'
          '      }']
  end
  Code=[Code
        '      break;'
        ''
        '    case 5 : /* actuator ending */']

  if size(actt,1)<>0 then
    Code=[Code
          '      switch (*ut) {'
          '        case 11 :'
          '          memcpy(((double *) out->data + (cnt)*(*nu1)*(*nu2)), \'
          '                 ((double *) out->data + out->ndata*(*nu1)*(*nu2)), \'
          '                  (cnt)*(*nu1)*(*nu2)*sizeof(double));'
          '      }'
          '      out->ndata=cnt;']
  end

  Code=[Code
        '      break;'
        '  }'
        '']

  //## update the static counter variables with the
  //## local counter variable value
  if size(actt,1)<>0 then

    Code=[Code
          '  /* update cnt */']

    for i=1:size(actt,1)
      if i==1 then
        Code=[Code
              '  if(*nport=='+string(i)+') {'
              '    cnt_'+string(i)+'=cnt;']
      else
        Code=[Code
              '  }'
              '  else if(*nport=='+string(i)+') {'
              '    cnt_'+string(i)+'=cnt;']
      end
    end
    Code=[Code
          '  }']
  end

  Code=[Code
        '}'
        '']

  //## Sensors
  Code=[Code
        '/*---------------------------------------- Sensors */'
        'void '+rdnom+'_sensor(flag,nport,nevprt,t,y,ny1,ny2,yt,typin,inptr)'
        '     int *flag,*nevprt,*nport;'
        '     int *ny1,*ny2,*yt;'
        ''
        '     int typin;'
        '     void *inptr;'
        ''
        '     double *t;'
        '     void *y;'
        '{'
        '  /*int k,l;*/']

  //## declaration of scicos_inout variables for input of sensors
  if size(capt,1)<>0 then
    Code=[Code
          '  /* declaration of scicos_inout variable for input of sensors */'
          '  scicos_inout *in;'
          '']
  end

  //## declaration of static counter variable for actuators (state)
  if size(capt,1)<>0 then
    Code=[Code
          '  /* static counter variable */']
    for i=1:size(capt,1)
      Code=[Code
            '  static int cnt_'+string(i)+';']
    end
  end

  //## declaration of a local counter variable
  if size(capt,1)<>0 then
    Code=[Code
          '  int cnt=0;'
          '']

    Code=[Code
          '  /* update cnt */']

    for i=1:size(capt,1)
      if i==1 then
        Code=[Code
              '  if(*nport=='+string(i)+') {'
              '    cnt=cnt_'+string(i)+';']
      else
        Code=[Code
              '  }'
              '  else if(*nport=='+string(i)+') {'
              '    cnt=cnt_'+string(i)+';']
      end
    end
    Code=[Code
          '  }']
  end

  //## affectation of input structure for sensors
  if size(capt,1)<>0 then
    Code=[Code
          '  /* affectation of intput structure of sensors */'
          '  in=(scicos_inout *)inptr;'
          '']
  end

  Code=[Code
        '  switch (*flag) {'
        ''
        '    case 4 : /* sensor initialisation */']
  if size(capt,1)<>0 then
    Code=[Code
          '      /* initialisation of counter variable */'
          '      cnt=0;']
  end
  Code=[Code
        '      break;'
        ''
        '    case 1 :'
        '      switch (*yt) {'
        '      case 10 :']

  if size(capt,1)<>0 then
    Code=[Code
          '        memcpy((double *) y, \'
          '              ((double *) in->data + cnt*(*ny1)*(*ny2)), \'
          '              (*ny1)*(*ny2)*sizeof(double));'
          '        /* *((double *)y)=*((double *)in->data + cnt); */'
          '        /*fprintf(stderr,""sensor %d : cnt = %d\n"",*nport,cnt);*/']
  end

  Code=[Code
        '        break;'
        ''
        '      case 11 :']

  if size(capt,1)<>0 then
    Code=[Code
          '        memcpy((double *) y, \'
          '              ((double *) in->data + cnt*(*ny1)*(*ny2)), \'
          '              (*ny1)*(*ny2)*sizeof(double));'
          '        memcpy((double *) y+(*ny1)*(*ny2), \'
          '              ((double *) in->data + cnt*(*ny1)*(*ny2)) + (*ny1)*(*ny2)*in->ndata, \'
          '              (*ny1)*(*ny2)*sizeof(double));']
  end

  Code=[Code
        '        break;'
        ''
        '      case 81 :'
        '        break;'
        ''
        '      case 82 :'
        '        break;'
        ''
        '      case 84 :'
        '        break;'
        ''
        '      case 811 :'
        '        break;'
        ''
        '      case 812 :'
        '        break;'
        ''
        '      case 814 :'
        '        break;'
        '      }'
        '']
  //## increase the local counter variable value
  if size(capt,1)<>0 then
    Code=[Code
          '      /* increase counter variables */'
          '      cnt++;'
          ''
          '      /* check value of cnt */'
          '      if (cnt==in->ndata) {'
          '        cnt--;'
          '      }']
  end
  Code=[Code
        '      break;'
        ''
        '    case 5 : /* sensor ending */'
        '      break;'
        '  }'
        '']

  //## update the static counter variables with the
  //## local counter variable value
  if size(capt,1)<>0 then

    Code=[Code
          '  /* update cnt */']

    //## update the local counter variable value
    for i=1:size(capt,1)
      if i==1 then
        Code=[Code
              '  if(*nport=='+string(i)+') {'
              '    cnt_'+string(i)+'=cnt;']
      else
        Code=[Code
              '  }'
              '  else if(*nport=='+string(i)+') {'
              '    cnt_'+string(i)+'=cnt;']
      end
    end
    Code=[Code
          '  }']
  end

  Code=[Code
        '}']

endfunction

function ccmat=adj_clkconnect_dep(blklst,ccmat)
//this part was taken from c_pass2 and put in c_pass1;!!
nbl=size(blklst)
fff=ones(nbl,1)==1
clkptr=zeros(nbl+1,1);clkptr(1)=1; typ_l=fff;typ_t=fff;
for i=1:nbl
  ll=blklst(i);
  clkptr(i+1)=clkptr(i)+size(ll.evtout,'*');
  //tblock(i)=ll.dep_ut($);
  typ_l(i)=ll.blocktype=='l';
  typ_t(i)=ll.dep_ut($)
end
all_out=[]
for k=1:size(clkptr,1)-1
  if ~typ_l(k) then
    kk=[1:(clkptr(k+1)-clkptr(k))]'
    all_out=[all_out;[k*ones(kk),kk]]
  end
end
all_out=[all_out;[0,0]]
ind=find(typ_t==%t)
ind=ind(:);
for k=ind'
  ccmat=[ccmat;[all_out,ones(all_out)*[k,0;0,0]]]
end
endfunction


// remove synchro block from agenda
function new_agenda=adjust_agenda(evts,clkptr,funtyp)
k=1
new_agenda=[]
for i=1:size(clkptr,1)
  if i<>size(clkptr,1)
    j = clkptr(i+1) - clkptr(i)
    if j<>0 then
      if funtyp(i)<>-1 & funtyp(i)<>-2 then
        new_agenda = [new_agenda;evts(k:k+j-1)]
      end
      k=k+j
    end
  end
end
endfunction

// remove synchro block from clkptr
function clkptr=adjust_clkptr(clkptr,funtyp)
j=0
for i=1:size(clkptr,1)
  clkptr(i)=clkptr(i)-j
  if i<>size(clkptr,1)
    if funtyp(i)==-1 | funtyp(i)==-2 then
       j = clkptr(i+1) - clkptr(i)
    end
  end
end
endfunction


//@@ function to adjust negative and positive id of scicos scopes
function [bllst,ok]=adjust_id_scopes(list_of_scopes,bllst)

  ok=%t
  pos_win=[]
  pos_i=[]
  pos_ind=[]
  for i=1:lstsize(bllst)
    ind = find(bllst(i).sim(1)==list_of_scopes(:,1))
    if ind<>[] then
      ierr=execstr('win=bllst(i).'+list_of_scopes(ind,2),'errcatch');
      if ierr<>0 then
        ok=%f, return;
      end
      if win<0 then
        win = 30000 + i
        execstr('bllst(i).'+list_of_scopes(ind,2)+'='+string(win))
      else
        pos_win=[pos_win;win]
        pos_i=[pos_i;i]
        pos_ind=[pos_ind;ind]
      end
    end
  end

  if pos_win<>[] & size(pos_win,1)<>1 then
    ko=%t;
    while ko
      for j=1:size(pos_win,1)
        if find(pos_win(j)==pos_win(j+1:$))<>[] then
          pos_win(j)=pos_win(j)+1
          ko=%t
          break
        end
        ko=%f
      end
    end
    for i=1:size(pos_ind,1)
      execstr('bllst(pos_i(i)).'+list_of_scopes(pos_ind(i),2)+'='+string(pos_win(i)))
    end
  end

endfunction
// remove synchro block from clkptr
function new_pointi=adjust_pointi(pointi,clkptr,funtyp)
j=0
new_pointi=pointi
for i=1:size(clkptr,1)
  if clkptr(i)>pointi then break, end;
  if i<>size(clkptr,1)
    if funtyp(i)==-1 | funtyp(i)==-2 then
       j = clkptr(i+1) - clkptr(i)
       new_pointi=new_pointi-j
    end
  end
end
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

function scs_m = draw_sampleclock(scs_m,XX,k,flgcdgen, szclkINTemp, freof)
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

//get index of adjusted event
function ind=get_ind_clkptr(bk,clkptr,funtyp)
  clkptr=adjust_clkptr(clkptr,funtyp)
  ind=clkptr(bk)
endfunction


function scs_m = goto_target_scs_m(scs_m)
  //## look if we want generate a sblock
  //## contained in a sblock
  kk=super_path

  //## scs_temp becomes the scs_m of the upper-level sblock
  if size(kk,'*')>1 then
    while size(kk,'*')>1 do
      scs_m=scs_m.objs(kk(1)).model.rpar
      kk(1)=[];
    end
    scs_m=scs_m.objs(kk).model.rpar
  elseif size(kk,'*')>0 then
    scs_m=scs_m.objs(kk).model.rpar
  end
endfunction

function [depu_mat,ok]=incidence_mat(bllst,connectmat,clkconnect,cor,corinv)
  ok=%t,In_blocks=[];OUt_blocks=[];depu_mat=[];
  for i=1:lstsize(bllst)
    sim=bllst(i).sim;sim=sim(1);
    if type(sim)==10 then
      if part(sim,1:10)=='actionneur' then
	OUt_blocks(bllst(i).ipar)=i
      elseif part(sim,1:7)=='capteur' then
	In_blocks(bllst(i).ipar)=i
      end
    end
  end
  %mprt=funcprot()
  funcprot(0) 
  deff('messagebox(txt)',' ')
  funcprot(%mprt)
  in=0
  for i=In_blocks'
    in=in+1
    out=0
    for j=OUt_blocks'
      out=out+1
      if is_dep(i,j,bllst,connectmat,clkconnect,cor,corinv) then
	depu_mat(in,out)=1
      else 
	depu_mat(in,out)=0
      end
    end
  end
endfunction

function dep=is_dep(i,j,bllst,connectmat,clkconnect,cor,corinv) 
  bllst(i).dep_ut = [%t,%t]
  bllst(i).in=1;bllst(i).in2=1;bllst(i).intyp=1
  bllst(j).dep_ut = [%t,%t]
  bllst(j).out=1;bllst(j).out2=1;bllst(j).outtyp=1
  connectmat=[connectmat;j 1 i 1];
  clkconnect=adj_clkconnect_dep(bllst,clkconnect)
  cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv,"silent")
  dep= (cpr==list())
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

function str=string_to_c_string(a)
  str=string(a)
  if strindex(str,"D-")<>[] then
    str=strsubst(str,"D-","e-");
  elseif strindex(str,"D+")<>[] then
    str=strsubst(str,"D+","e+");
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
//Copyright INRIA
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
        if stalone then
          tmp_='*(('+TYPE+' *)outtb_'+string(ix)+')'
        else
          tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        end
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
        if stalone then
          tmp_='*(('+TYPE+' *)outtb_'+string(ix)+')'
        else
          tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        end
        txt=[txt;
             '    i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);'
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
        if flag==1 | pt>0 then
          txt2=call_block42(bk,pt,flag);
        else
          txt2=[]
        end
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
        if stalone then
          tmp_='*(('+TYPE+' *)outtb_'+string(ix)+')'
        else
          tmp_ = '*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        end
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
        if stalone then
          tmp_='*(('+TYPE+' *)outtb_'+string(ix)+')'
        else
          tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        end
        //** C **//
        txt=[txt;
             '    i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);'
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
//Copyright INRIA
function [txt]=write_code_idoit()
  txt=[];

  for j=1:niord
    bk=iord(j,1);
    pt=iord(j,2);
    //** blk
    if funtyp(bk)>-1 then
      if or(bk==act) | or(bk==cap) then
        if stalone then
          txt2=call_block42(bk,pt,1);
          if txt2<>[] then
            txt=[txt;
                 '  '+txt2
                 ''];
          end
        end
      else
        txt2=call_block42(bk,pt,1);
        if txt2<>[] then
          txt=[txt;
               '  '+txt2
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
             '  '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_ = '*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '  if('+tmp_+'>0) {']
        //*******//
        txt=[txt;
             Indent+thentxt];
        if elsetxt<>[] then
           //** C **//
           txt=[txt;
                '  }';
                '  else {';]
           //*******//
           txt=[txt;
                Indent+elsetxt];
        end
        //** C **//
        txt=[txt;
             '  }']
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
             '  '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '  i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);']
        txt=[txt;
             '  switch(i)'
             '  {']
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '   case '+string(i)+' :';]
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
             '  }'];
        //*******//
      end
    //** Unknown block
    else
      error('Unknown block type '+string(bk));
    end
  end

endfunction

//write_code_initdoit : generate body of the code for
//                      ordering calls of blocks during
//                      implicit solver initialization
//
//input : ev  : evt number for block's call
//       flag : flag number for block's call
//
//output : txt
//
//09/04/08 AL
//Copyright INRIA
function [txt]=write_code_initdoit(ev,flag)
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
                 '  '+txt2
                 ''];
          end
        end
      else
        txt2=call_block42(bk,pt,flag);
        if txt2<>[] then
          txt=[txt;
               '  '+txt2
               ''];
        end
      end
    //** ifthenelse blk
    elseif funtyp(bk)==-1 then
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      thentxt=write_code_initdoit(clkptr(bk),flag);
      elsetxt=write_code_initdoit(clkptr(bk)+1,flag);
      if thentxt<>[] | elsetxt<>[] then
        txt=[txt;
             '  '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_ = '*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '  if('+tmp_+'>0) {']
        //*******//
        txt=[txt;
             Indent+thentxt]
        if elsetxt<>[] then
           //** C **//
           txt=[txt;
                '  }';
                '  else {';]
           //*******//
           txt=[txt;
                Indent+elsetxt];
        end
        //** C **//
        txt=[txt;
             '  }']
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
             '  '+get_comment('evtselect_blk',list(bk));]
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        //** C **//
        txt=[txt;
             '  i=max(min((int) '+...
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);'
             '  switch(i)'
             '  {']
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '   case '+string(i)+' :';]
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
             '  }']
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
//Copyright INRIA
function [txt]=write_code_odoit(flag)
  txt=[];

  for j=1:noord
    bk=oord(j,1);
    pt=oord(j,2);
    //** blk
    if funtyp(bk)>-1 then
      if or(bk==cap) then
        if stalone then
          txt2=call_block42(bk,pt,flag);
          if txt2<>[] then
            txt=[txt;
                 '  '+txt2
                 ''];
          end
        end
      else
        txt2=call_block42(bk,pt,flag);
        if txt2<>[] then
          txt=[txt;
               '  '+txt2
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
             '  '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        if stalone then
          tmp_='*(('+TYPE+' *)outtb_'+string(ix)+')'
        else
          tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        end
        txt=[txt;
             '  if ((block_'+rdnom+'['+string(bk-1)+'].nmode<0'+...
              ' && '+tmp_+'>0)'+...
              ' || \'
             '      (block_'+rdnom+'['+string(bk-1)+'].nmode>0'+...
              ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==1)) {']
        //*******//
        txt=[txt;
             Indent+thentxt]
        //** C **//
        txt=[txt;
             '  }'];
        //*******//
        if elsetxt<>[] then
          //** C **//
          txt=[txt;
               '  else if  ((block_'+rdnom+'['+string(bk-1)+'].nmode<0'+...
                ' && '+tmp_+'<=0)'+...
                ' || \'
               '            (block_'+rdnom+'['+string(bk-1)+'].nmode>0'+...
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
             '  '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        if stalone then
          tmp_='*(('+TYPE+' *)outtb_'+string(ix)+')'
        else
          tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        end
        txt=[txt;
             '  if (block_'+rdnom+'['+string(bk-1)+'].nmode<0) {';
             '    i=max(min((int) '+...
                tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);'
             '  }'
             '  else {'
             '    i=block_'+rdnom+'['+string(bk-1)+'].mode[0];'
             '  }']
        txt=[txt;
             '  switch(i)'
             '  {'];
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '   case '+string(i)+' :';]
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
             '  }'];
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
//Copyright INRIA
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
              tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);'
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

//write_code_reinitdoit : generate body of the code for
//                        implicit solver reinitialization
//
//input : flag : flag number for block's call
//
//output : txt
//
//09/04/08 AL
//Copyright INRIA
function [txt]=write_code_reinitdoit(flag)
  txt=[];

  for j=1:noord
    bk=oord(j,1);
    pt=oord(j,2);
    //** blk
    if funtyp(bk)>-1 then
      if or(bk==cap) then
        if stalone then
          txt2=call_block42(bk,pt,flag);
          if txt2<>[] then
            txt=[txt;
                 '  '+txt2
                 ''];
          end
        end
      else
        txt2=call_block42(bk,pt,flag);
        if txt2<>[] then
          txt=[txt;
               '  '+txt2
               ''];
        end
      end
    //** ifthenelse blk
    elseif funtyp(bk)==-1 then
      ix=-1+inplnk(inpptr(bk));
      TYPE=mat2c_typ(outtb(ix+1)); //** scilab index start from 1
      thentxt=write_code_initdoit(clkptr(bk),flag);
      elsetxt=write_code_initdoit(clkptr(bk)+1,flag);
      if thentxt<>[] | elsetxt<>[] then
        txt=[txt;
             '  '+get_comment('ifthenelse_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '  if ((block_'+rdnom+'['+string(bk-1)+'].nmode<0'+...
              ' && '+tmp_+'>0)'+...
              ' || \'
             '      (block_'+rdnom+'['+string(bk-1)+'].nmode>0'+...
              ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==1)) {']
        //*******//
        txt=[txt;
             Indent+thentxt]
        //** C **//
        txt=[txt;
             '  }'];
        //*******//
        if elsetxt<>[] then
          //** C **//
          txt=[txt;
               '  else if  ((block_'+rdnom+'['+string(bk-1)+'].nmode<0'+...
                ' && '+tmp_+'<=0)'+...
                ' || \'
               '            (block_'+rdnom+'['+string(bk-1)+'].nmode>0'+...
                ' && block_'+rdnom+'['+string(bk-1)+'].mode[0]==2)) {';]
          //*******//
          txt=[txt;
               Indent+elsetxt]
          //** C **//
          txt=[txt;
               '  }'];
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
             '  '+get_comment('evtselect_blk',list(bk));]
        //** C **//
        tmp_='*(('+TYPE+' *)'+rdnom+'_block_outtbptr['+string(ix)+'])'
        txt=[txt;
             '  if (block_'+rdnom+'['+string(bk-1)+'].nmode<0) {';
             '    i=max(min((int) '+...
                tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);'
             '  }'
             '  else {'
             '    i=block_'+rdnom+'['+string(bk-1)+'].mode[0];'
             '  }']
        txt=[txt;
             '  switch(i)'
             '  {'];
        //*******//
        for i=II
          //** C **//
          txt=[txt;
               '   case '+string(i)+' :';]
          //*******//
          txt=[txt;
               BigIndent+write_code_initdoit(clkptr(bk)+i-1,flag);]
          //** C **//
          txt=[txt;
               BigIndent+'break;']
          //*******//
        end
        //** C **//
        txt=[txt;
             '  }'];
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
//Copyright INRIA
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
               tmp_+',block_'+rdnom+'['+string(bk-1)+'].nevout),1);'
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

function ok=WriteCodeToFile(Code,path)
//  utility function for writing file
  ierr=execstr('mputl(Code,path)','errcatch')
  if ierr<>0 then
    messagebox([msprintf(_('Error when writing the %s file.\n'),path);
		lasterror()],'modal','error')
    ok=%f
  else
    ok=%t
  end
endfunction
