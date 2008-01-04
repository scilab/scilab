// ====================================================================
// Allan CORNET
// INRIA 2007
// ====================================================================
function add_module_help_chapter(modulename)
  path_addchapter = SCI+"/modules/"+modulename+"/help/"+getlanguage()+"/addchapter.sce";
  if ( fileinfo(path_addchapter) == [] ) then
    path_addchapter = SCI+"/modules/"+modulename+"/help/"+getdefaultlanguage()+"/addchapter.sce";
  end
//  try
    exec(path_addchapter);
//  catch
//    warning(gettext("Incorrect module path ")+path_addchapter);
//  end
endfunction
// ====================================================================
