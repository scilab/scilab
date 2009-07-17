// ====================================================================
// Allan CORNET
// DIGITEO 2009
// This file is released into the public domain
// ====================================================================

src_dir = get_absolute_file_path('cleaner_src.sce');

langs = ['c','cpp','fortran'];
for l = langs
  if fileinfo(src_dir +'/' + l + '/cleaner.sce') <> [] then
    exec(src_dir +'/' + l + '/cleaner.sce');
    mdelete(src_dir +'/' + l + '/cleaner.sce');
  end
end

clear src_dir;
