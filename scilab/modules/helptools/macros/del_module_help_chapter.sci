// ====================================================================
// Allan CORNET
// INRIA 2007
// ====================================================================
function del_module_help_chapter(modulename)
  path_removechapter = SCI+"/modules/"+modulename+"/help/"+getlanguage()+'/removechapter.sce';
  if ( fileinfo(path_removechapter) == [] ) then
    path_removechapter = SCI+"/modules/"+modulename+"/help/"+getdefaultlanguage()+'/removechapter.sce';
  end
  try
    exec(path_removechapter);
  catch
    warning(' incorrect module path '+path_removechapter);
  end
endfunction
// ====================================================================
