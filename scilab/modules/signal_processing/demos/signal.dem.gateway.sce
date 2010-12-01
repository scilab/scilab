// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("Spectral Estimation")               , "spect.dem.sce"
               _("IIR filter design")                 , "iir.dem.sce"
               _("Minimax FIR filter design")         , "remezfilt.dem.sce"
               _("Discrete IIR filter")               , "iirfilt.dem.sce"
               _("Wiegner filter")                    , "wiener.dem.sce"
               _("Bode plots")                        , "bode.dem.sce"
               _("Window FIR filters")                , "winfilt.dem.sce"
               _("Arma simulation and identification"), "arma/arma1.dem.sce"
               _("Arma, bidimensional version")       , "arma/arma2.dem.sce"
               _("Arma, Spectral power estimation")   , "arma/arma3.dem.sce"];

subdemolist(:,2) = SCI + "/modules/signal_processing/demos/" + subdemolist(:,2);
