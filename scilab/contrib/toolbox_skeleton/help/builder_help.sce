// ====================================================================
// Copyright INRIA 2008
// Allan CORNET
// ====================================================================

help_dir_tlbx_sklt = get_absolute_file_path('builder_help.sce');

backup_dir = pwd();

chdir(help_dir_tlbx_sklt);
if ( isdir(getdefaultlanguage()) ) then 
 chdir(getdefaultlanguage());
 exec('build_help.sce',-1);
else
end

chdir(help_dir_tlbx_sklt);
if ( isdir('fr_FR') ) then 
 chdir('fr_FR');
 exec('build_help.sce',-1);
else
end

chdir(backup_dir);

// ====================================================================
clear help_dir_tlbx_sklt;
clear backup_dir;
// ====================================================================
