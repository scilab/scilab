//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function demo_scicos2()
  demo_help demo_scicos2
  getf Bounce/BOUNCE4_f.sci
  getf Bounce/BOUNCEXY_f.sci
  if ~c_link('bouncexy') then
    D=getcwd()
    chdir Bounce;
    ilib_for_link('bounce4','bounce4.o',[],'c');
    exec loader.sce;
    ilib_for_link('bouncexy','bouncexy.o',[],'f');
    exec loader.sce;
    chdir(D);
  end
  getf SCI/demos/scicos/scicos_play.sci
  scicos_play('Bounce/bounce.dialogs')
endfunction
