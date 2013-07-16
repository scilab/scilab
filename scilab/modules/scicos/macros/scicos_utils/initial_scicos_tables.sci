
function [modelica_libs, scicos_pal_libs, ..
    %scicos_with_grid, %scs_wgrid] = initial_scicos_tables()

    //**        This function is called from "scicos" "scicos_simulate" "lincos" "steadycos"


    %scicos_with_grid = %f;//** define grid
    %scs_wgrid = [10;10;12];//**define aspect of grid


    //Scicos palettes libs list ========================================================
    scicos_pal_libs = ["Branching","Events","Misc","Sinks","Threshold","Linear","MatrixOp","NonLinear","Sources","Electrical","Hydraulics","PDE","IntegerOp"];

    //Scicos Modelica librabry path definitions========================================
    modelica_libs = getModelicaPath();

    //** This is the END, the END my friend". By The Doors, Apocalypse Now.
endfunction

