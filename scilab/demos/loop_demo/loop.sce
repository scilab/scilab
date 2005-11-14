//------------------------------------------------------------
// Author Serge Steer Copyright INRIA
// Update 26/10/2004
//------------------------------------------------------------

// default window size;
wSize = [850,920];

// window size for the image demo
wSize_Small = [850,492] ;

global INDEX
INDEX = make_help_index()

compteur = 0 ;

//titles=['Scilab','Arbre genealogique', ..
//	'Visualisation scientifique',..
//	'Visualisation de surface Riemann','Surfaces 3D',..
//        'Traitement d''image','Complex Functions', ..
//        'Traitement du signal','Stationnement d''une voiture', ..
//        'Stationnement d''un semi remorque','Modelisation et simulation d''une chaine', ..
//        'Modelisation et simulation d''un pendule glissant', ..
//        'Programmation lineaire 2', ..
//        'Optimisation non lineaire','Spline'];
        //'Scicos 1','Scicos 2'

titles=['Visualisation scientifique'];
          
//demos=['demo_scilab();','demo_geneal();',..
//       'demo_isoSurf()',..
//       'demo_riemann();','demo_3dplot()',..
//       'demo_image()','demo_cmplxfun();', ..
//       'demo_signal();','demo_car();', ..
//       'demo_truck();','demo_pendulum();', ..
//       'demo_sliding_pendulum();', ..
//       'demo_optloc();', ..
//       'demo_datafit();','demo_spline();'];
       //'demo_scicos1();','demo_scicos2();'

demos=['demo_geneal();'];
       
//filesdemos=['demo_scilab.sci','Geneal.sci', ..
//	    'demo_isoSurf.sci',..
//	    'riemann.sci','demo_3dplot.sci',..
//            'demo_image.sci','cmplxfun.sci', ..
//            'demo_signal.sci','car.sci', ..
//            'truck.sci','n_pendulum.sci', ..
//            'sliding_pendulum.sci', ..
//            'optloc.sci', ..
//            'datafit.sci','spline.sci'];       
            //'demo_scicos1.sci','demo_scicos2.sci'

filesdemos=['Geneal.sci'];
         
//------------------------------------------------------------
function demo_help(key)
  browsehelp=browsehelp;
  if MSDOS then
    browsehelp(gethelpfile(key),key)
  else
    tcltk_help(gethelpfile(key),key,'demo')
  end
  // wait for people to read
  realtimeinit(1.0);
  for i=1:1
    realtime(i);
  end ;
endfunction
//------------------------------------------------------------
function demotodo(demoname,title)
  //clc();
  printf('Demo '+title+' \n'); 
  execstr(demoname);
  xdel();
  clear ;
  clearglobal() ;
  
  //[fd,err] = mopen('mem.txt','a');
  //Memsize = stacksize() ;
  //mfprintf(fd,'usedSize = %d\n',Memsize(2) ) ;
  //mclose(fd) ;
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
  f=gcf();
  f.figure_position = [0,0];
  //f.figure_size=[400,350]*0.9;
  f.figure_size = wSize ; //[850,950] ;
  toolbar(f.figure_id,'off') ;
endfunction
//------------------------------------------------------------
function SetPosition_old
  xset("wpos",0,0);
  xset('wdim', wSize(1), wSize(2) ) ;
  toolbar(0,'off') ;
endfunction
//------------------------------------------------------------
function SetPosition_Small
  xset("wpos",0,0);
  xset('wdim', wSize_Small(1), wSize_Small(2) ) ;
  toolbar(0,'off') ;
endfunction
//------------------------------------------------------------
function removeBar()
  f = gcf();
  toolbar(f.figure_id,'off') ;
endfunction
//------------------------------------------------------------
loopnumber=0;
//mode(-1);

lines(0);

f= gdf() ;
f.figure_position= [0,0];
f.figure_size = wSize ;

path=get_absolute_file_path("loop.sce")
global %browsehelp;
if MSDOS then
  %browsehelp='Default Windows Browser';
else
  //%browsehelp='mozilla/netscape (gnome-moz-remote)';  
end
%helps=[%helps;path "Demos"];


Maxfiles=size(filesdemos);
 
 for i=1:1:Maxfiles(2) do
   getf(filesdemos(i));
 end

while %t
  loopdemos();
  loopnumber=loopnumber+1;
  disp( loopnumber ) ;
  //pause ;
end
//------------------------------------------------------------
