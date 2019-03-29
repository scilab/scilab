// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// ===================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Resampling accuracy
// -------------------
F0 = 2;                   // Signal frequency
Fs1 = 1000;               // initial sampling frequency
t1 = linspace(0, 5-1/Fs1, Fs1*5);
u1 = sin(2*%pi*F0*t1);
for Fs2 = int(Fs1*(0.5:0.15:1.6))   // targeted resampling frequency
    t2 = linspace(0, 5-1/Fs2, Fs2*5);
    u2 = sin(2*%pi*F0*t2);     // Direct sampling at targeted frequency (as reference)
    u2b = intdec(u1, Fs2/Fs1); // Resampled signal
    assert_checktrue(max(abs(u2b-u2))< 4e-14)
end

// length(intdec(intdec(x, r), 1/r)) == length(x)
// ----------------------------------------------
Fs1 = 1000;               // initial sampling frequency
F0 = 2;                    // Signal frequency
t1 = linspace(0, 5-1/Fs1, 5*Fs1)';
u1 = sin(2*%pi*F0*t1);
L1 = length(u1);
for Fs2 = 695:705     // targeted resampling frequency < Fs1
    r = intdec(intdec(u1, Fs2/Fs1), Fs1/Fs2);
    assert_checkequal(length(r), L1);
end
for Fs2 = 1295:1305     // targeted resampling frequency > Fs1
    r = intdec(intdec(u1, Fs2/Fs1), Fs1/Fs2);
    assert_checkequal(length(r), L1);
end
