//------------------------------------------------------------
// Author Serge Steer Copyright INRIA
// Update 26/10/2004
//------------------------------------------------------------
global INDEX
INDEX = make_help_index()

titles=['Scilab','Visualisation de surface Riemann','Complex Functions', ..
          'Traitement du signal','Stationnement d''une voiture', ..
          'Stationnement d''un semi remorque','Modelisation et simulation d''une chaine', ..
          'Modelisation et simulation d''un pendule glissant', ..
          'Programmation lineaire 1','Programmation lineaire 2', ..
          'Optimisation non lineaire','Spline'];
          //'Scicos 1','Scicos 2'
          
demos=['demo_scilab();','demo_riemann();','demo_cmplxfun();', ..
       'demo_signal();','demo_car();', ..
       'demo_truck();','demo_pendulum();', ..
       'demo_sliding_pendulum();', ..
       'demo_multiflot();','demo_optloc();', ..
       'demo_datafit();','demo_spline();'];
       //'demo_scicos1();','demo_scicos2();'
       
filesdemos=['demo_scilab.sci','riemann.sci','cmplxfun.sci', ..
            'demo_signal.sci','car.sci', ..
            'truck.sci','n_pendulum.sci', ..
            'sliding_pendulum.sci', ..
            'multiflot.sci','optloc.sci', ..
            'datafit.sci','spline.sci'];       
            //'demo_scicos1.sci','demo_scicos2.sci'
         
//------------------------------------------------------------
function demo_help(key)
  browsehelp=browsehelp;
  if MSDOS then
    browsehelp(gethelpfile(key),key)
  else
    tcltk_help(gethelpfile(key),key,'demo')
  end
endfunction
//------------------------------------------------------------
function demotodo(demoname,title)
  clc();
  printf('Demo '+title+' \n'); 
  execstr(demoname);
endfunction
//------------------------------------------------------------
function loopdemos
  
 Max=size(demos);
 
 for i=1:1:Max(2) do
   demotodo(demos(i),titles(i));
 end

endfunction
//------------------------------------------------------------
function SetPosition
  f=gdf();
  f.figure_position= [550,16];
  f.figure_size=[400,350]*0.9;
endfunction
//------------------------------------------------------------
loopnumber=0;
mode(-1);

path=get_absolute_file_path("loop.sce")
global %browsehelp;
if MSDOS then
  %browsehelp='Default Windows Browser';
else
  //%browsehelp='mozilla/netscape (gnome-moz-remote)';  
end
%helps=[%helps;path "Demos"];

SetPosition();

Maxfiles=size(filesdemos);
 
 for i=1:1:Maxfiles(2) do
   getf(filesdemos(i));
 end

while %t
  loopdemos();
  loopnumber=loopnumber+1;
end
//------------------------------------------------------------
