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
//
//**
//** INRIA
//** Comments by Simone Mannori
//** 04 Sep 2006 : updated file with the last "scicos_new" mods
//**
//** 30 August 2007: some data structure are moved from scicos.sci here.
//**
//** 23 May 2008 : final porting on Scilab 5  
//**
//** 04 Nov 2008 : the "Grid" function is temporally disabled 
//**
//** N.B. : This function is called from "SCI/modules/scicos/etc/scicos.start" 
//**        at Scilab's startup IF the "scicos" module is active in
//**        "SCI/etc/modules.xml" file :)
//** 24 june 2009 Serge Steer
//    Cut declared as TypeOne menu (according to Cut_.sci modifications)      
//** september 2009 Serge Steer
//    helps pushed to to standard help system
//    menu definition pushed to set_initial_xcos_menus
function [scicos_pal, %scicos_menu, %scicos_short, modelica_libs,scicos_pal_libs, ...
          %scicos_lhb_list, %CmenuTypeOneVector,%scicos_gif, ...
          %scicos_contrib,%scicos_libs,%scicos_with_grid,..
	  %scs_wgrid] = initial_scicos_tables()

//**        This function is called from "scicos" "scicos_simulate" "lincos" "steadycos"


  %scicos_with_grid = %f; //** define grid
  %scs_wgrid = [10;10;12]; //**define aspect of grid

  //Scicos palettes names ============================================================
    pal_names = ['Sources'    ;'Sinks'         ;'Events'    ;'Branching';
		 'Linear'     ;'Non_linear'    ;'Matrix'    ;'Integer'  ;
		 'Modelica'   ;'Lookup_Tables' ;'Threshold' ;'Others'   ;
		 'DemoBlocks' ;'OldBlocks'];
    scicos_pal = [pal_names  'SCI/modules/scicos/palettes/'+pal_names+'.cosf']
   
  //Scicos block icons location ====================================================
  %scicos_gif = SCI+"/modules/scicos/help/scicos_doc/man/gif_icons/";
  
  //Scicos contribs ====================================================
  %scicos_contrib = [];
 
  %scicos_libs = [];

  //Scicos palettes libs list ========================================================
   scicos_pal_libs = ['Branching','Events','Misc','Sinks','Threshold','Linear', ...
		       'MatrixOp','NonLinear','Sources','Electrical','Hydraulics','PDE',...
		       'IntegerOp'];
  
  //Scicos Menu definitions==========================================================
  [%scicos_menu,%scicos_lhb_list, %scicos_short,%CmenuTypeOneVector]= set_initial_xcos_menus()

  //Scicos Modelica librabry path definitions========================================
    modelica_libs='SCI/modules/scicos_blocks/macros/'+['Electrical','Hydraulics'];

  

  //add TMPDIR/Modelica for generic modelica blocks
  status=mkdir(TMPDIR,'Modelica');
  if isdir(TMPDIR+'/Modelica') then
    modelica_libs=[modelica_libs,TMPDIR+'/Modelica'];
  end

 
  //** This is the END, the END my friend". By The Doors, Apocalypse Now.
endfunction
