// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain

demopath = get_absolute_file_path("signal.dem.gateway.sce");

subdemolist = ["Spectral Estimation"                , "spect.dem.sce"
               "IIR filter design "                 , "iir.dem.sce"
               "Minimax FIR filter design"          , "remezfilt.dem.sce"
               "Wiegner filter"                     , "wiener.dem.sce"
               "Bode plots"                         , "bode.dem.sce"
               "Arma simulation and identification" , "arma/arma1.dem.sce"
               "Arma, bidimensional version"        , "arma/arma2.dem.sce"
               "Arma, Spectral power estimation"    , "arma/arma3.dem.sce"];

subdemolist(:,2) = demopath + subdemolist(:,2);
