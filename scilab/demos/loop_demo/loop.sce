mode(-1)
//Author Serge Steer Copyright INRIA
path=get_absolute_file_path("loop.sce")
function demo_help(key)
  browsehelp=browsehelp;
  tcltk_help(gethelpfile(key),key,'demo')
endfunction

getf demo_scilab.sci
getf car.sci
getf truck.sci
getf n_pendulum.sci
getf riemann.sci
getf demo_signal.sci
getf multiflot.sci
getf optloc.sci
getf sliding_pendulum.sci
getf datafit.sci
getf cmplxfun.sci
getf demo_scicos1.sci
getf demo_scicos2.sci
global %browsehelp;
//%browsehelp='mozilla/netscape (gnome-moz-remote)';
%helps=[%helps;path "Demos"];
f=gdf();f.figure_position= [600,16];f.figure_size=[600,400]*0.9
while %t 
  demo_scilab()

  
  demo_riemann()
  demo_cmplxfun()
  
  demo_signal()
  
  demo_car()
  demo_truck()
  
  demo_scilab()
   
  demo_pendulum()
  demo_sliding_pendulum()

  demo_multiflot()
  demo_optloc()
  
  demo_datafit()
  
  demo_scicos1()
  demo_scicos2()
end

