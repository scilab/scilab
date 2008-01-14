// ====================================================================
// Allan CORNET
// Copyright INRIA 2008
// ====================================================================
try   
 version_scilab = getversion('scilab');
catch  
 error('Scilab 5.0 or more is required.');  
end;  
// ====================================================================
root_tlbx = get_absolute_file_path('loader.sce');
exec(root_tlbx+'etc\'+'toolbox_skeleton.start');
// ====================================================================
clear version_scilab;
clear root_tlbx;
// ====================================================================
