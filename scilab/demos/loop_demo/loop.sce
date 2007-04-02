//------------------------------------------------------------
// Authors Serge Steer, Jean-Baptiste Silvy Copyright INRIA
// Update 16/11/2005
//------------------------------------------------------------

// Default language
global lang
lang='FR'

// default window size;
wSize = [850,920];

// window size for the image demo
wSize_Small = [850,492] ;

// set firefox has help viewer
global %browsehelp
%browsehelp = 'firefox'

global INDEX
INDEX = make_help_index()

titles=['Scilab','Arbre genealogique', ..
	'Visualisation scientifique',..
	'Visualisation de surface Riemann','Surfaces 3D',..
        'Traitement d''image','Complex Functions', ..
        'Traitement du signal','Stationnement d''une voiture', ..
        'Stationnement d''un semi remorque',..
	'Modelisation et simulation d''une chaine', ..
        'Modelisation et simulation d''un pendule glissant', ..
        'Programmation lineaire 2', ..
        'Optimisation non lineaire','Spline'];

demos=['demo_scilab();','demo_geneal();',..
       'demo_isoSurf()',..
       'demo_riemann();','demo_3dplot()',..
       'demo_image()','demo_cmplxfun();', ..
       'demo_signal();','demo_car();', ..
       'demo_truck();','demo_pendulum();', ..
       'demo_sliding_pendulum();', ..
       'demo_optloc();', ..
       'demo_datafit();','demo_spline();'];

filesdemos=['demo_scilab.sci','Geneal.sci', ..
	    'demo_isoSurf.sci',..
	    'riemann.sci','demo_3dplot.sci',..
            'demo_image.sci','cmplxfun.sci', ..
            'demo_signal.sci','car.sci', ..
            'truck.sci','n_pendulum.sci', ..
	    'sliding_pendulum.sci', ..
	    'optloc.sci', ..
            'datafit.sci','spline.sci'];

nbDemos = size(filesdemos);
nbDemos = nbDemos(2) ;

//------------------------------------------------------------
function demo_help(key)
  browsehelp=browsehelp;
  global lang;
  lang=lang;
  //if MSDOS then
    browsehelp(gethelpfile(key+'_'+lang),key)
  //else
    //tcltk_help(gethelpfile(key),key,'demo')
  //end
  // wait for people to read
  realtimeinit(1.0);
  for i=1:1
//    realtime(i);
  end ;
endfunction
//------------------------------------------------------------
function demotodo(demoname,title)
  clc();
  printf('Demo '+title+' \n');
  execstr(demoname);
  xdel();
  clear ;
  clearglobal() ;
endfunction
//------------------------------------------------------------
function loopdemos(n)

// Max=size(demos,"*");

 for i=1:n do
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
mode(-1);

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
   exec(filesdemos(i));
 end

//while %t
  loopdemos(nbDemos);
//  loopnumber=loopnumber+1;
//end
exit
