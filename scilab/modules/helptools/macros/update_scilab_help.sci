// ====================================================================
// update modules help chapters
// Copyright INRIA 2007
// Allan CORNET
// ====================================================================
function update_scilab_help()
  clearglobal %helps_modules
  global %helps_modules
  modules=getmodules();
  index=size(modules);
  for i=1:index(1) do 
	  add_module_help_chapter(modules(i));
  end
  make_help_index()
  clear modules index i
endfunction
// ====================================================================
