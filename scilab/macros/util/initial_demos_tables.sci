function demolist=initial_demos_tables()
  demolist=[
      'Introduction to SCILAB',                'basic/intro/dem01.dem';
      'Graphics'                               'graphics/graphics.dem';
      'Simulation',                            'simulation/simulation.dem'
      'Control'   ,                            'control/control.dem'
      'Signal Processing',                     'signal/signal.dem';
      'Optimization',                          'optimization/optimization.dem';
      'Graph & Networks',                      'metanet/meta.dem';
      'Scicos',                                'scicos/scicos.dem';
      'Random',                                'random/random.dem';
      'Communications with PVM',               'pvm/pvm.dem';
      'TK/TCL demos',                          'tk/tk.dem'];
demolist(:,2)='SCI/demos/'+demolist(:,2)
endfunction
