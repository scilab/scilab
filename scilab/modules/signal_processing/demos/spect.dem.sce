// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

////////////////////////
//SPECTRAL ESTIMATION///
////////////////////////

function demo_spectral()

    // generate white data
    rand("normal");
    rand("seed", 0);
    x = rand(1:1024-33+1);

    // make low-pass filter with eqfir
    nf = 33;
    bedge = [0 .1;.125 .5];
    des = [1 0];
    wate = [1 1];
    h = eqfir(nf, bedge, des, wate);

    //filter white data to obtain colored data
    h1 = [h 0*ones(1:max(size(x))-1)];
    x1 = [x 0*ones(1:max(size(h))-1)];
    hf = fft(h1, -1);
    xf = fft(x1, -1);
    yf = hf.*xf;
    y = real(fft(yf, 1));

    // plot magnitude of filter
    h2 = [h 0*ones(1:167)];
    hf2 = fft(h2, -1);
    hf2 = real(hf2.*conj(hf2));
    hsize = max(size(hf2));
    fr = (1:hsize)/hsize;

    my_handle = scf(100001);
    clf(my_handle, "reset");

    plot2d(fr', log(hf2)');
    xtitle(_("Data spectrum"), _("frequency"), _("magnitude"));
    halt(_("Press Return to continue ... \n"));

    if is_handle_valid(my_handle) == %f then
        return
    end

    // pspect example
    [sm1] = pspect(100, 200, "tr", y);
    smsize = max(size(sm1));
    fr = (1:smsize)/smsize;
    clf(my_handle, "reset");
    plot2d(fr', log(sm1)');
    xtitle(_("Spectral estimation"), _("frequency"), _("spectral power"));
    halt(_("Press Return to continue ... \n"));

    if is_handle_valid(my_handle) == %f then
        return
    end

    // cspect example
    [sm2] = cspect(100, 200, "tr", y);
    smsize = max(size(sm2));
    fr = (1:smsize)/smsize;
    clf(my_handle, "reset");
    demo_viewCode("spect.dem.sce");
    plot2d(fr', log(sm2)');
    xtitle(["Spectral estimation ; periodogram method"], " " , " ")


endfunction

demo_spectral()
clear demo_spectral;
