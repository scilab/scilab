path=get_absolute_file_path('loader.sce');

exec(path+"/loader_inc.sce");

functions = [ "umfpack"      ;
              "umf_lufact"   ;
              "umf_lusolve"  ;
              "umf_ludel"    ;
              "umf_luinfo"   ;
              "umf_luget"    ;
              "taucs_chfact" ;
              "taucs_chsolve";
              "taucs_chdel"  ;
              "taucs_chinfo" ;
              "taucs_chget"  ;
              "res_with_prec"] ;



entrypoint = "scispt" ;

addinter( objects , entrypoint , functions )
num_interface = floor(funptr("umfpack")/100);
intppty(num_interface)


[units,typs,nams]=file();
for k=size(nams,'*'):-1:1 
  l=strindex(nams(k),'loader.sce');
  if l<>[] then
    DIR_SCISPT = part(nams(k),1:l($)-1);
    break
  end
end

DIR_SCISPT_DEM = DIR_SCISPT + "examples/";

add_help_chapter("scispt : Interface on umfpack & taucs snmf plus sparse utilities",...
		  path+"manxml");

clear units typs k l entrypoint num_interface nams path

getf(DIR_SCISPT+"sparse_util.sci")

write(%io(2),[" scispt toolbox loaded : enter scisptdemo() for a demo ";...
              "                         (stacksize must be >= 3000000)"]);
