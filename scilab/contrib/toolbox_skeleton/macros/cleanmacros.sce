// ====================================================================
// Allan CORNET
// DIGITEO 2009
// This file is released under the 3-clause BSD license. See COPYING-BSD.
// ====================================================================
libpath = get_absolute_file_path('cleanmacros.sce');

binfiles = ls(libpath+'/*.bin');
for i = 1:size(binfiles,'*')
  mdelete(binfiles(i));
end

mdelete(libpath+'/names');
mdelete(libpath+'/lib');

// ====================================================================
