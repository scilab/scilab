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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ./license.txt
//

function [routines,IntFunc]=create_palette(Path)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// This function generates a palette if Path is a string indicating
// the directoy in which the interfacing functions of the blocks are
// to be found. 
// If Path is not present or if it is %t, then standard scicos
// palettes are regenerated. If %f, then only IntFunc (list of
// interfacing functions) is returned. List of routines is empty in
// this case.

  load SCI/modules/scicos/macros/scicos_scicos/lib;
  load SCI/modules/scicos/macros/scicos_auto/lib;
  load SCI/modules/scicos/macros/scicos_utils/lib;
  exec SCI/modules/scicos_blocks/macros/Misc/PAL_f.sci;

  scicos_ver=get_scicos_version();
  rhs=argn(2)
  if rhs==1 & type(Path)==4 then // generate scicos palettes
    rhs=0
    gener=Path
  elseif rhs==0 then
    gener=%t
  end
  if rhs==1 then
    Path=pathconvert(Path,%t,%t)
    PalName=basename(part(Path,1:length(Path)-1))
    to_del=[]
    lisf=listfiles(Path+'*.sci')
    for i=1:size(lisf,'*')
      fil=lisf(i)
      ierror=execstr('exec(fil)','errcatch')
      if ierror <>0 then
	to_del=[to_del i];
      end
    end
    lisf(to_del)=[];
    routines=build_palette(lisf,Path,PalName);
    IntFunc=lisf
  else
    [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ..
     modelica_libs,scicos_pal_libs,..
     %scicos_gif,%scicos_contrib,%scicos_libs] = initial_scicos_tables()
    clear %scicos_menu  %scicos_short %scicos_help modelica_libs
    exec(loadpallibs,-1)
    path = "SCI/modules/scicos/palettes/" ;

    routines=[];IntFunc=[];
    for txt=scicos_pal(:,1)'
      disp('Constructing '+txt)
      if txt=='Sources' then
	lisf=['CONST_m.sci';'GENSQR_f.sci';'RAMP.sci';  
	      'RAND_m.sci';'RFILE_f.sci';
	      'CLKINV_f.sci'; 'CURV_f.sci';  'INIMPL_f.sci'; 'READAU_f.sci';
	      'SAWTOOTH_f.sci'; 'STEP_FUNCTION.sci';
	      'CLOCK_c.sci'; 'GENSIN_f.sci'; 'IN_f.sci';   'READC_f.sci';
	      'TIME_f.sci'; 'Modulo_Count.sci';'Sigbuilder.sci';'Counter.sci';
              'SampleCLK.sci';'TKSCALE.sci';'FROMWSB.sci';'Ground_g.sci';
	      'PULSE_SC.sci';'GEN_SQR.sci']
        gr_i=[]
        id=[]
        PalName=txt

      elseif txt=='Sinks' then
	lisf=['AFFICH_m.sci';   'CMSCOPE.sci';
	      'CSCOPXY.sci';   'WRITEC_f.sci';
	      'CANIMXY.sci';   'CSCOPE.sci';
	      'OUTIMPL_f.sci'; 
	      'CLKOUTV_f.sci';  'CEVENTSCOPE.sci';
	      'OUT_f.sci';      'WFILE_f.sci';
	      'CFSCOPE.sci';   'WRITEAU_f.sci';
	      'CSCOPXY3D.sci';   'CANIMXY3D.sci';
	      'CMATVIEW.sci';	'CMAT3D.sci'; 
              'TOWS_c.sci']
        gr_i=[]
        id=[]
        PalName=txt
	
      elseif txt=='Branching' then
	lisf=['DEMUX.sci';
	      'MUX.sci'; 'NRMSOM_f.sci';  'EXTRACTOR.sci';
	      'SELECT_m.sci';'ISELECT_m.sci';
	      'RELAY_f.sci';'SWITCH2_m.sci';'IFTHEL_f.sci';
	      'ESELECT_f.sci';'M_SWITCH.sci';
	      'SCALAR2VECTOR.sci';'SWITCH_f.sci';'EDGE_TRIGGER.sci';
	      'Extract_Activation.sci';'GOTO.sci';'FROM.sci';
	      'GotoTagVisibility.sci';'CLKGOTO.sci';'CLKFROM.sci';
	      'CLKGotoTagVisibility.sci';'GOTOMO.sci';'FROMMO.sci';
	      'GotoTagVisibilityMO.sci']
        gr_i=[]
        id=[]
        PalName=txt
	
      elseif txt=='Non_linear' then
	lisf=['ABS_VALUE.sci'; 'TrigFun.sci';
	      'EXPBLK_m.sci';  'INVBLK.sci';
	      'LOGBLK_f.sci'; 'LOOKUP_f.sci'; 'MAXMIN.sci';
	      'POWBLK_f.sci'; 'PROD_f.sci';
	      'PRODUCT.sci';  'QUANT_f.sci';'EXPRESSION.sci';
	      'SATURATION.sci'; 'SIGNUM.sci';'CONSTRAINT_c.sci' ]
        gr_i=[]
        id=[]
        PalName=txt

      elseif txt=='Lookup_Tables' then
	lisf=['LOOKUP_c.sci';'LOOKUP2D.sci' ; 'INTRPLBLK_f.sci'; 'INTRP2BLK_f.sci']
        gr_i=[]
        id=[]
        PalName=txt
	
      elseif txt=='Events' then
	lisf=['ANDBLK.sci';'HALT_f.sci';'freq_div.sci';
	      'ANDLOG_f.sci';'EVTDLY_f.sci';'IFTHEL_f.sci';'ESELECT_f.sci';
	      'CLKSOMV_f.sci';'CLOCK_c.sci';'EVTGEN_f.sci';'EVTVARDLY.sci';
              'M_freq.sci';'SampleCLK.sci';'VirtualCLK0.sci']
        gr_i=[]
        id=[]
        PalName=txt

      elseif txt=='Threshold' then
	lisf=[  'NEGTOPOS_f.sci';  'POSTONEG_f.sci';  'ZCROSS_f.sci']
        gr_i=[]
        id=[]
        PalName=txt
	
      elseif txt=='Others' then
	lisf=['fortran_block.sci';
	      'SUPER_f.sci';'scifunc_block_m.sci';
	      'TEXT_f.sci';'CBLOCK4.sci';'RATELIMITER.sci';
	      'BACKLASH.sci';'DEADBAND.sci';'EXPRESSION.sci';
	      'HYSTHERESIS.sci';'DEBUG_SCICOS.sci';
	      'LOGICAL_OP.sci';'RELATIONALOP.sci';'generic_block3.sci';
	      'PDE.sci';'ENDBLK.sci';'AUTOMAT.sci';
              'PAL_f.sci']
        gr_i=[]
        id=[]
        PalName=txt
	
      elseif txt=='Linear' then
	lisf=['DLR.sci';'TCLSS.sci';'DOLLAR_m.sci';
	      'CLINDUMMY_f.sci';'DLSS.sci';'REGISTER.sci';'TIME_DELAY.sci';
	      'CLR.sci';'GAINBLK.sci';'SAMPHOLD_m.sci';'VARIABLE_DELAY.sci';
	      'CLSS.sci';'SUMMATION.sci';'INTEGRAL_m.sci';'SUM_f.sci';
              'DERIV.sci';'PID.sci';'DIFF_c.sci']
        gr_i=[]
        id=[]
        PalName=txt
	
      elseif txt=='OldBlocks' then
	lisf=['CLOCK_f.sci';'ABSBLK_f.sci';    
	      'MAX_f.sci'; 'MIN_f.sci';'SAT_f.sci'; 'MEMORY_f.sci';
	      'CLKSOM_f.sci';'TRASH_f.sci';'GENERAL_f.sci';'DIFF_f.sci';
	      'BIGSOM_f.sci';'INTEGRAL_f.sci';'GAINBLK_f.sci';
	      'DELAYV_f.sci';'DELAY_f.sci'; 'DEMUX_f.sci';'MUX_f.sci';
	      'MFCLCK_f.sci';'MCLOCK_f.sci';'COSBLK_f.sci';   'DLRADAPT_f.sci';
	      'SINBLK_f.sci'; 'TANBLK_f.sci';'generic_block.sci';'RAND_f.sci';
	      'DOLLAR_f.sci';'CBLOCK.sci';'c_block.sci']
        gr_i=[]
        id=[]
        PalName=txt

      elseif txt=='DemoBlocks' then
	lisf=['BOUNCE.sci';'BOUNCEXY.sci';'BPLATFORM.sci';'PENDULUM_ANIM.sci']
        gr_i=[]
        id=[]
        PalName=txt

      elseif txt=='Modelica' then
        lisf=list('MBLOCK.sci',...
                  list(['Capacitor.sci';'Ground.sci';'VVsourceAC.sci'; //## Electrical
                        'ConstantVoltage.sci';'Inductor.sci';'PotentialSensor.sci';
                        'VariableResistor.sci';'CurrentSensor.sci';'Resistor.sci';
                        'VoltageSensor.sci';'Diode.sci';'VsourceAC.sci';
                        'NPN.sci';'PNP.sci';'SineVoltage.sci';'Switch.sci';
                        'OpAmp.sci';'PMOS.sci';'NMOS.sci';'CCS.sci';'CVS.sci';
                        'IdealTransformer.sci';'Gyrator.sci']),...
                  list(['Bache.sci';'VanneReglante.sci';'PerteDP.sci'; //## ThermoHydraulics
                        'PuitsP.sci';'SourceP.sci';'Flowmeter.sci']))
        //## TODO Masoud
        gr_i=list([],...  //## main
                  [],...  //## Elec
                  [])     //## Thermo
        id=list([],...  //## main
                [],...  //## Elec
                'Hydraulics') //## Thermo
        path=list(path,path,path)
        PalName=list(txt,'Electrical','ThermoHydraulics')

//##       12/07/08, Alan
//##       That's an example of how to call build_palette to recursively build palette
//##       in palette. Please do no remove that comment for that time
//##
//##        lisf=list(list(list(['Capacitor.sci';'Ground.sci';'VVsourceAC.sci'; //## Electrical
//##                             'ConstantVoltage.sci';'Inductor.sci';'PotentialSensor.sci';
//##                             'VariableResistor.sci';'CurrentSensor.sci';'Resistor.sci';
//##                             'VoltageSensor.sci';'Diode.sci';'VsourceAC.sci';
//##                             'NPN.sci';'PNP.sci';'SineVoltage.sci';'Switch.sci';
//##                             'OpAmp.sci';'PMOS.sci';'NMOS.sci';'CCS.sci';'CVS.sci';
//##                             'IdealTransformer.sci';'Gyrator.sci'])),...
//##                  list(['Bache.sci';'VanneReglante.sci';'PerteDP.sci'; //## ThermoHydraulics
//##                        'PuitsP.sci';'SourceP.sci';'Flowmeter.sci']),...
//##                  'MBLOCK.sci')
//##       gr_i=list(['xstringb(orig(1),orig(2),''A'',sz(1),sz(2),''fill'')'],...
//##                  list(['xstringb(orig(1),orig(2),''E'',sz(1),sz(2),''fill'')'],...
//##                        ['xstringb(orig(1),orig(2),''Elec'',sz(1),sz(2),''fill'')']),...
//##                   ['xstringb(orig(1),orig(2),''Thermo'',sz(1),sz(2),''fill'')'])
//##        path='SCI/macros/scicos/'
//##        path=list(path,list(path,path),path)
//##        PalName=list(txt,list('E','Electrical'),'Hydraulics')

      elseif txt=='Matrix' then
	lisf=['MATMUL.sci';'MATTRAN.sci';'MATSING.sci';'MATRESH.sci';'MATDIAG.sci';
              'MATEIG.sci';'MATMAGPHI.sci';'EXTRACT.sci';'MATEXPM.sci';'MATDET.sci';
              'MATPINV.sci';'EXTTRI.sci';'RICC.sci';'ROOTCOEF.sci';'MATCATH.sci';
              'MATLU.sci';'MATDIV.sci';'MATZCONJ.sci';'MATZREIM.sci';'SUBMAT.sci';
              'MATBKSL.sci';'MATINV.sci';'MATCATV.sci';'MATSUM.sci';'CUMSUM.sci';'SQRT.sci']
        gr_i=[]
        id=[]
        PalName=txt

      elseif txt=='Integer' then
	lisf=['BITCLEAR.sci';'BITSET.sci';'CONVERT.sci';'EXTRACTBITS.sci';'INTMUL.sci';
	      'SHIFT.sci';'LOGIC.sci';'DLATCH.sci';'DFLIPFLOP.sci';'JKFLIPFLOP.sci';
	      'SRFLIPFLOP.sci']
        gr_i=[]
        id=[]
        PalName=txt

      else
	error('Palette '+txt+' does not exists')
      end

      IntFunc=Get_IntFunc(IntFunc,lisf)

      if gener then routines=[routines;build_palette(lisf,path,PalName,gr_i,id)];end
    end
  end
  routines=unique(routines);IntFunc=unique(IntFunc);
endfunction

//## Get_IntFunc recusively return interfacing function name in list
function IntFunc=Get_IntFunc(IntFunc,lisf)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

 if type(lisf)==15 then
   for k=1:lstsize(lisf)
     IntFunc=Get_IntFunc(IntFunc,lisf(k))
   end
 else
   IntFunc=[IntFunc;lisf];
 end
endfunction

//## build_palette : create a cof file anf return list of gui
function [routines]=build_palette(lisf,path,fname,gr_i,id)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  //## init variables
  X=0
  Y=0
  yy=0
  sep=30
  routines=[]
  //## minimal check
  [lhs,rhs] = argn(0);
  if rhs<4 then
    gr_i=[]
    id=[]
  end
  //## blk of PAL_f
  ierr=execstr('blk=PAL_f(''define'')','errcatch')
  if ierr<>0 then
    messagebox('Block palette not found.','modal')
    return
  end
  //## set size graphics of PAL_f block
  blk.graphics.sz=20*blk.graphics.sz
  //## get flat palette
  [routines,scs_m]=get_flatpal(lisf,path,fname,gr_i,id)
  //## encapsulate palettes & blocks in PAL_f
  //## set top level diagram of the palette
  scs_m_top=scicos_diagram()
  //## encapsulate palette in PAL_f
  blk.model.sim=fname(1)
  if gr_i(1)<>[] then
    blk.graphics.gr_i(1)=gr_i(1)
  end
  if id(1)<>[] then
    blk.graphics.id=id(1),
  else
    blk.graphics.id=fname(1)
  end
  blk.model.rpar=scs_m
  scs_m_top.objs(1) = blk
  scs_m=scs_m_top
  //## set main title and path of the top level palette
  scs_m.props.title=[fname(1),path(1)] //## ??
  //## open file
  [u,err]=file('open',path(1)+fname(1)+'.cosf','unknown','formatted')
  if err<>0 then
    messagebox('File or directory write access denied','modal')
    routines=[]
    return
  end
  //## write scs_m in file with cos2cosf
  ierr=cos2cosf(u,do_purge(scs_m))
  if ierr<>0 then
    messagebox('Directory write access denied','modal')
    routines=[]
    file('close',u)
    return
  end
  //## close file
  file('close',u)
  mprintf('Wrote '+path(1)+fname(1)+'.cosf \n')
endfunction

//## get_flatpal : return a flat diagram of a palette
function [routines,scs_m]=get_flatpal(lisf,path,fname,gr_i,id)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  //## init variables
  X=0
  Y=0
  yy=0
  sep=30
  routines=[]
  //## blk of PAL_f
  ierr=execstr('blk=PAL_f(''define'')','errcatch')
  if ierr<>0 then
    messagebox('Block palette not found.','modal')
    return
  end
  //## set size graphics of PAL_f block
  blk.graphics.sz=20*blk.graphics.sz
  //## flat palette
  scs_m=scicos_diagram()
  //##lisf is a palette
  if type(lisf)==15 then
    nb=lstsize(lisf)
    pal=1
    for j=1:nb
      flisf = lisf(j)
      if type(flisf)==15 then
        //## flat palette
        [routines_j,scs_m_j]=get_flatpal(flisf,path(pal+1),fname(pal+1),gr_i(pal+1),id(pal+1))
        //## encapsulate palette in PAL_f
        blk_j=blk
        blk_j.graphics.orig=[X Y]
        X=X+blk.graphics.sz(1)+sep
        yy=max(yy,blk.graphics.sz(2))
        if X>400 then X=0,Y=Y+yy+sep,yy=0,end
        blk_j.model.sim=fname(pal+1)(1)
        if gr_i(pal+1)(1)<>[] then
          blk_j.graphics.gr_i(1)=gr_i(pal+1)(1)
        end
        if id(pal+1)(1)<>[] then
          blk_j.graphics.id=id(pal+1)(1)
        else
          blk_j.graphics.id=fname(pal+1)(1)
        end
        blk_j.model.rpar=scs_m_j
        scs_m.objs($+1) = blk_j
        routines=[routines;routines_j]
        pal=pal+1
      else
        for fil=flisf'
          name=basename(fil)
          ierror=execstr('blk_j='+name+'(''define'')','errcatch')
          if ierror <>0 then
            messagebox(['Error in GUI function';lasterror()],'modal' )
            return
          end
          routines=[routines;blk_j.model.sim(1)]
          blk_j.graphics.sz=20*blk_j.graphics.sz;
          blk_j.graphics.orig=[X Y]
          X=X+blk_j.graphics.sz(1)+sep
          yy=max(yy,blk_j.graphics.sz(2))
          if X>400 then X=0,Y=Y+yy+sep,yy=0,end
          scs_m.objs($+1)=blk_j
        end
      end
    end
  //##lisf is a vector of string
  else
    //##
    flisf = lisf
    for fil=flisf'
      name=basename(fil)
      ierror=execstr('blk_j='+name+'(''define'')','errcatch')
      if ierror <>0 then
        messagebox(['Error in GUI function';lasterror()],'modal' )
        return
      end
      routines=[routines;blk_j.model.sim(1)]
      blk_j.graphics.sz=20*blk_j.graphics.sz;
      blk_j.graphics.orig=[X Y]
      X=X+blk_j.graphics.sz(1)+sep
      yy=max(yy,blk_j.graphics.sz(2))
      if X>400 then X=0,Y=Y+yy+sep,yy=0,end
      scs_m.objs($+1)=blk_j
    end
  end
  //## set main title and path of the top level palette
  scs_m.props.title=[fname(1),path(1)] //## ??
endfunction
