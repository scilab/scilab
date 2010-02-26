// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

load('SCI/modules/scicos/macros/scicos_scicos/lib');
load('SCI/modules/scicos/macros/scicos_utils/lib');
load('SCI/modules/scicos/macros/scicos_menus/lib');
if ( ~isdef("scicos_pal") | ~isdef("%scicos_menu") | ..
       ~isdef("%scicos_short") | ~isdef("%scicos_help") | ..
       ~isdef("%scicos_display_mode") | ~isdef("modelica_libs") | ..
       ~isdef("scicos_pal_libs") ) then
    
     [scicos_pal, %scicos_menu, %scicos_short, modelica_libs, scicos_pal_libs,...
     %scicos_lhb_list, %CmenuTypeOneVector, %scicos_gif,%scicos_contrib, ..
     %scicos_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables();
end
  
exec(loadpallibs,-1) 

exec('SCI/modules/scicos/tests/unit_tests/scicos_tests.sci');

options=default_options();
//build the block set
blockslib='scs'+['Branching','Events','Misc','Sinks','Threshold','Linear', ...
	  'NonLinear','Sources','Electrical','Hydraulics']+'lib';
Blocs=[]
for blocklib=blockslib
  B=string(blocklib);Blocs=[Blocs;B(2:$)];
end
Blocs(Blocs=="m_sin")=[];

nb=size(Blocs,1);
if checkdefine() then pause,end
if checkinputs() then pause,end
if checkoutputs() then pause,end
if checkorigin() then pause,end
if checkdeput() then pause,end
if checkfiring() then pause,end
if check_define_ports() then pause,end
if check_set_ports() then pause,end
