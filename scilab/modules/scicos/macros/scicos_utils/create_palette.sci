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

function [routines,IntFunc] = create_palette(Path)
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

  scicos_ver = get_scicos_version();
  rhs = argn(2)

  if rhs==1 & type(Path)==4 then // generate scicos palettes
    rhs = 0 ; 
    gener = Path ; 
  elseif rhs==0 then 
    gener = %t
  end

  //** Regenerate a specific palette 
  if rhs==1 then
    Path = pathconvert(Path,%t,%t)
    PalName = basename(part(Path,1:length(Path)-1))
    to_del = [];
    lisf = listfiles(Path+'*.sci')
    for i=1:size(lisf,'*')
      fil=lisf(i)
      ierror=execstr('exec(fil)','errcatch')
      if ierror <>0 then
	to_del=[to_del i];
      end
    end
    lisf(to_del)=[];
    routines=build_palette(lisf,Path,PalName);IntFunc=lisf
  
  else //** regenerate ALL the palettes 
  
    [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ..
    %scicos_display_mode, modelica_libs,scicos_pal_libs,..
    %scicos_gif,%scicos_contrib] = initial_scicos_tables();
 
    clear %scicos_menu  %scicos_short %scicos_help %scicos_display_mode modelica_libs

    exec(loadpallibs,-1)

    path = "SCI/modules/scicos/macros/scicos_scicos/" ; //** ... for the moment, the palettes stay here (for compatibility). 

    routines = [];
    IntFunc  = [];
    for txt = scicos_pal(:,1)'
      disp('Constructing '+txt)
      if txt=='Sources' then
	lisf=['CONST_m.sci';'GENSQR_f.sci';'RAMP.sci';
	      'RAND_m.sci';'RFILE_f.sci';
	      'CLKINV_f.sci'; 'CURV_f.sci';  'INIMPL_f.sci'; 'READAU_f.sci';
	      'SAWTOOTH_f.sci'; 'STEP_FUNCTION.sci';
	      'CLOCK_c.sci'; 'GENSIN_f.sci'; 'IN_f.sci';   'READC_f.sci';
	      'TIME_f.sci'; 'Modulo_Count.sci';'Sigbuilder.sci';'Counter.sci';
              'SampleCLK.sci';'TKSCALE.sci';'FROMWSB.sci']

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

      elseif txt=='Non_linear' then
	lisf=['ABS_VALUE.sci'; 'TrigFun.sci';
	      'EXPBLK_m.sci';  'INTRP2BLK_f.sci';
	      'INTRPLBLK_f.sci'; 'INVBLK.sci';
	      'LOGBLK_f.sci'; 'LOOKUP_f.sci'; 'MAXMIN.sci';
	      'POWBLK_f.sci'; 'PROD_f.sci';
	      'PRODUCT.sci';  'QUANT_f.sci';'EXPRESSION.sci';
	      'SATURATION.sci'; 'SIGNUM.sci' ]

      elseif txt=='Events' then
	lisf=['ANDBLK.sci';'HALT_f.sci';'freq_div.sci';
	      'ANDLOG_f.sci';'EVTDLY_f.sci';'IFTHEL_f.sci';'ESELECT_f.sci';
	      'CLKSOMV_f.sci';'CLOCK_c.sci';'EVTGEN_f.sci';'EVTVARDLY.sci';
              'M_freq.sci';'SampleCLK.sci']

      elseif txt=='Threshold' then
	lisf=[  'NEGTOPOS_f.sci';  'POSTONEG_f.sci';  'ZCROSS_f.sci']

      elseif txt=='Others' then
	lisf=['c_block.sci';'fortran_block.sci';
	      'SUPER_f.sci';'scifunc_block_m.sci';
	      'TEXT_f.sci';'CBLOCK.sci';'RATELIMITER.sci';
	      'BACKLASH.sci';'DEADBAND.sci';'EXPRESSION.sci';
	      'CONSTRAINT_f.sci';'HYSTHERESIS.sci';'DEBUG_SCICOS.sci';
	      'LOGICAL_OP.sci';'RELATIONALOP.sci';'generic_block3.sci';
	      'MBLOCK.sci';'PDE.sci';'ENDBLK.sci';'AUTOMAT.sci']

      elseif txt=='Linear' then
	lisf=['DLR.sci';'TCLSS.sci';'DOLLAR_m.sci';
	      'CLINDUMMY_f.sci';'DLSS.sci';'REGISTER.sci';'TIME_DELAY.sci';
	      'CLR.sci';'GAINBLK.sci';'SAMPHOLD_m.sci';'VARIABLE_DELAY.sci';
	      'CLSS.sci';'SUMMATION.sci';'INTEGRAL_m.sci';'SUM_f.sci';
              'DERIV.sci';'PID.sci']

      elseif txt=='OldBlocks' then
	lisf=['CLOCK_f.sci';'ABSBLK_f.sci';
	      'MAX_f.sci'; 'MIN_f.sci';'SAT_f.sci'; 'MEMORY_f.sci';
	      'CLKSOM_f.sci';'TRASH_f.sci';'GENERAL_f.sci';'DIFF_f.sci';
	      'BIGSOM_f.sci';'INTEGRAL_f.sci';'GAINBLK_f.sci';
	      'DELAYV_f.sci';'DELAY_f.sci'; 'DEMUX_f.sci';'MUX_f.sci';
	      'MFCLCK_f.sci';'MCLOCK_f.sci';'COSBLK_f.sci';   'DLRADAPT_f.sci';
	      'SINBLK_f.sci'; 'TANBLK_f.sci';'generic_block.sci';'RAND_f.sci';
	      'DOLLAR_f.sci']

      elseif txt=='DemoBlocks' then
	lisf=['BOUNCE.sci';'BOUNCEXY.sci';'BPLATFORM.sci']

      elseif txt=='Electrical' then
        lisf=['Capacitor.sci';'Ground.sci';'VVsourceAC.sci';'ConstantVoltage.sci';
              'Inductor.sci';'PotentialSensor.sci';'VariableResistor.sci';'CurrentSensor.sci';
              'Resistor.sci';'VoltageSensor.sci';'Diode.sci';'VsourceAC.sci';
              'NPN.sci';'PNP.sci';'SineVoltage.sci';'Switch.sci'; ...
	      'OpAmp.sci';'PMOS.sci';
	      'NMOS.sci';'CCS.sci';'CVS.sci';'IdealTransformer.sci';'Gyrator.sci']

      elseif txt=='ThermoHydraulics' then
	lisf=['Bache.sci';'VanneReglante.sci';'PerteDP.sci';'PuitsP.sci';'SourceP.sci';]

      elseif txt=='Matrix' then
	lisf=['MATMUL.sci';'MATTRAN.sci';'MATSING.sci';'MATRESH.sci';'MATDIAG.sci';
              'MATEIG.sci';'MATMAGPHI.sci';'EXTRACT.sci';'MATEXPM.sci';'MATDET.sci';
              'MATPINV.sci';'EXTTRI.sci';'RICC.sci';'ROOTCOEF.sci';'MATCATH.sci';
              'MATLU.sci';'MATDIV.sci';'MATZCONJ.sci';'MATZREIM.sci';'SUBMAT.sci';
              'MATBKSL.sci';'MATINV.sci';'MATCATV.sci';'MATSUM.sci';'CUMSUM.sci';'SQRT.sci']

      elseif txt=='Integer' then
	lisf=['BITCLEAR.sci';'BITSET.sci';'CONVERT.sci';'EXTRACTBITS.sci';'INTMUL.sci';
	      'SHIFT.sci';'LOGIC.sci';'DLATCH.sci';'DFLIPFLOP.sci';'JKFLIPFLOP.sci';
	      'SRFLIPFLOP.sci']
      else
	error('Palette '+txt+' does not exists')
      end
      IntFunc = [IntFunc;lisf];
      if gener then
          routines = [routines;build_palette(lisf,path,txt)];
      end
    end
  end
  routines = unique(routines);
  IntFunc = unique(IntFunc);
endfunction


function [routines] = build_palette(lisf,path,fname)
  scs_m = scicos_diagram()
  X = 0
  Y = 0
  yy  = 0
  sep = 30
  routines = [];
  
  for fil = lisf'
    name = basename(fil)

    //name= part(fil,1:length(fil)-4)
    ierror = execstr('blk='+name+'(''define'')','errcatch')
    if ierror <>0 then
      message(['Error in GUI function';lasterror()] )
      fct=[]
      return
    end

    routines = [routines;blk.model.sim(1)]
    blk.graphics.sz = 20*blk.graphics.sz;
    blk.graphics.orig = [X Y] ;
    X = X+blk.graphics.sz(1)+sep ;
    yy = max(yy,blk.graphics.sz(2)) ;
    if X>400 then 
      X = 0
      Y = Y + yy + sep
      yy= 0
    end

    scs_m.objs($+1) = blk

  end
  [u,err]=file('open',path+fname+'.cosf','unknown','formatted')
  if err<>0 then
    message("File or directory write access denied")
    return
  end
  scs_m.props.title = [fname,path]
  ierr = cos2cosf(u,do_purge(scs_m))
  if ierr<>0 then
    message("Directory write access denied")
    file('close',u)
    return
  end

  file('close',u)
  mprintf('Wrote '+path+fname+'.cosf \n')

endfunction

