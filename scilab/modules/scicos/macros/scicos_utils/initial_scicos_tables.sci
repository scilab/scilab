function [scicos_pal, %scicos_menu, %scicos_short, modelica_path, scicos_pal_libs, %scicos_lhb_list, %CmenuTypeOneVector, %scicos_gif, %scicos_contrib, %scicos_libs, %scicos_with_grid, %scs_wgrid]=initial_scicos_tables()
  
  //**        This function is called from "scicos" "scicos_simulate" "lincos" "steadycos"
  
  
  %scicos_with_grid = %f;//** define grid
  %scs_wgrid = [10;10;12];//**define aspect of grid
  
  //Scicos palettes names ============================================================
  pal_names = ['Sources';'Sinks';'Events';'Branching';
    'Linear';'Non_linear';'Matrix';'Integer';
    'Modelica';'Lookup_Tables';'Threshold';'Others';
    'DemoBlocks';'OldBlocks'];
  scicos_pal = [pal_names,'SCI/modules/scicos/palettes/' + pal_names + '.cosf']
  
  //Scicos block icons location ====================================================
  %scicos_gif = SCI + '/modules/scicos/help/scicos_doc/man/gif_icons/';
  
  //Scicos contribs ====================================================
  %scicos_contrib = [];
  
  %scicos_libs = [];
  
  //Scicos palettes libs list ========================================================
  
  
  scicos_pal_libs = ['Branching','Events','Misc','Sinks','Threshold','Linear','MatrixOp','NonLinear','Sources','Electrical','Hydraulics','PDE','IntegerOp'];
  
  //Scicos Menu definitions==========================================================
  [%scicos_menu,%scicos_lhb_list,%scicos_short,%CmenuTypeOneVector] = set_initial_xcos_menus()
  
  //Scicos Modelica librabry path definitions========================================
  modelica_path = getModelicaPath();
  
  //** This is the END, the END my friend". By The Doors, Apocalypse Now.
endfunction
