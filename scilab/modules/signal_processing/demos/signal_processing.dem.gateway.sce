// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("signal_processing.dem.gateway.sce");
    add_demo(gettext("Signal Processing"), demopath + "signal_processing.dem.gateway.sce");

    subdemolist = [_("Spectral Estimation")               , "spect.dem.sce"
    _("IIR filter design")                 , "iir.dem.sce"
    _("Minimax FIR filter design")         , "remezfilt.dem.sce"
    _("Discrete IIR filter")               , "iirfilt.dem.sce"
    _("Wiener filter")                     , "wiener.dem.sce"
    _("Bode plots")                        , "bode.dem.sce"
    _("Window FIR filters")                , "winfilt.dem.sce"
    _("1-D convolution")                   , "conv.dem.sce"
    _("2-D convolution")                   , "conv2.dem.sce"
    _("Arma simulation and identification"), "arma/arma1.dem.sce"
    _("Arma, bidimensional version")       , "arma/arma2.dem.sce"
    _("Arma, Spectral power estimation")   , "arma/arma3.dem.sce"];

    subdemolist(:,2) = demopath + subdemolist(:,2);

endfunction

subdemolist = demo_gateway();
clear demo_gateway;
