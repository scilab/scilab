// demolist is the list of ALL demos (UNIX and Windows)
  demolist=[
      'Introduction to SCILAB','intro/dem01.dem';
      'Graphics: Introduction','graphics/dessin.dem';
      'Graphics: Primitives','graphics/xdemo.dem';
      'Graphics: Animation','anim/anim.dem';
      'Graphics: Finite Elements ','fec/fec.dem';
      'Graphics: More surfaces ','surface/surfaces.dem';
      'Graphics: Bezier curves and surfaces ','surface/bezier.dem';
      'Graphics: Misc','misc/misc.dem';
      'Inverted pendulum','pendulum/pendule.dem';
      'n-pendulum','npend/npend.dem';
      'Car parking','flat/flat.dem';
      'Wheel simulation ','wheel2/wheel.dem';
      'Bike Simulation (1)','bike/bike.dem';
      'Bike Simulation (2)','bike/bike2.dem';
      'Signal Processing','signal/signal.dem';
      'Dialog','dialog/dialog.dem';
      'Fortran translator','sci2for/demotrad.dem';
      'ODE''S','ode/ode.dem';
      'DAE''S','dae/dae.dem';
      'Arma','arma/arma.dem';
      'Tracking','tracking/track.dem';
      'Robust control','robust/rob.dem';
      'LMITOOL','lmitool/lmi.dem';
      'METANET','metanet/meta.dem';
      'Control examples','control/cont.dem';
      'Scicos','scicos/scicos.dem';
      'Sounds','sound/sound.dem';
      'Random','random/random.dem';
      'Communications with PVM','pvm/pvm.dem';
      'TK/TCL demos','tk/tk.dem'];
demolist(:,2)='SCI/demos/'+demolist(:,2)
