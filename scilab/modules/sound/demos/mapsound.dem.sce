// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function demo_mapsound()

    idfig = 100001;

    // Example #1: Standard
    // ====================
    fs = 22050;
    t = 0:1/fs:1;
    y = 0.3 + [1.5*sin(2*%pi*800*t) sin(2*%pi*1200*t)];
    // Then: build and display its map:
    fig = scf(idfig);
    show_window(fig)
    clf()  // reset
    demo_viewCode("mapsound.dem.sce");
    mapsound(y, 0.04, 1500)
    title "0.3 + [1.5⋅sin(2π⋅800⋅t), sin(2π⋅1200⋅t)]" fontsize 3.2
    // Let's hear it, not too loud:
    playsnd(y/8)
    msg = _(["<html><h4>Example #1: Standard</h4>" ;
          "" ;
          "A standard sound made of 2 pure sine waves" ;
          "of known amplitudes and frequencies is considered." ;
          "We add a constant on demo purpose. It should not be hearable." ;
          "" ;
          "<b>y = 0.3 + [1.5*sin(2*%pi*800*t) sin(2*%pi*1200*t)]</b>" ;
          "playsnd(y/8)" ;
          "<b>mapsound(y, 0.04, 1500)</b>" ;
          "" ;
          "Both main frequencies and amplitudes values are reliably mapped." ;
          "The constant appears at the zero frequency and 2×0.3 expected amplitude"
          ]);
     go? = messagebox(msg, "mapsound","scilab",_(["Next example" ; "Stop"]), "modal");
     if go?==2, close(fig), return, end


    // Example #2: Wobbling
    // ====================
    fs = 22050; // Sampling frequency
    t = 0:1/fs:1;
    f = 500*(1-t) + 2000*t;
    y0 = sin(2*%pi*f.*t);
    y = [y0 y0 y0];
    // Then: build and display its map:
    fig = scf(idfig);
    clf reset
    mapsound(y)
    title "Wobbling: y = sin(2π⋅f(t)⋅t), with f=500→2000→500 Hz" fontsize 3.5
    demo_viewCode("mapsound.dem.sce");
    // Let's hear it, not too loud:
    playsnd(y/8)
    //
    msg = ["<html><h4>Example #2: Wobbling</h4>" ;
          "" ;
          "Here, the sound is a sine wave whose frequency varies linearly" ;
          "in time on the [f0=550, f1=2000] Hz interval." ;
          "This yields some actual higher frequencies, on [f0, 2*f1-f0]." ;
          "We repeat it 3 times one second." ;
          "" ;
          "fs = 22050; // Sampling frequency" ;
          "t = 0:1/fs:1;" ;
          "<b>f = 500*(1-t) + 2000*t</b>; // Actual instantaneous frequency" ;
          "<b>y0 = sin(2*%pi*f.*t)</b>;   // for one second" ;
          "y = [y0 y0 y0];" ;
          "" ;
          "playsnd(y/8)" ;
          "<b>mapsound(y)</b>" ;
          ];
     go? = messagebox(msg, "mapsound","scilab",_(["Next example" "Stop"]), "modal");
     if go?==2, close(fig), return, end


    // Example #3: Chimes
    // ==================
    [y, fs] = wavread('SCI/modules/sound/demos/chimes.wav');
    fig = scf(idfig);
    clf reset
    mapsound(y,,2500,fs,parulacolormap)
    title chimes.wav fontsize 3.5
    demo_viewCode("mapsound.dem.sce");
    playsnd(y)
    msg = _(["<html><h4>Example #3: chimes.wav sound</h4>" ;
          "" ;
          "[y, fs] = wavread(""SCI/modules/sound/demos/chimes.wav"")";
          "playsnd(y)" ;
          "<b>mapsound(y, , 2500, fs, parulacolormap)</b>" ;
          ]);
     b = messagebox(msg, "mapsound","scilab", _(["End" ; "End & Keep the map"]), "modal");
     if b==1, close(fig), end
     // Otherwise: keeping the graphic figure allows to click on "View the code"

endfunction

demo_mapsound();
clear demo_mapsound;
