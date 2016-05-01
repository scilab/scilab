// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for escoufier macro
//==============================================================================

// <-- BENCH NB RUN : 10 -->

p     = 0;
q     = 0;
vt    = 0;
vr    = 0;
vrt   = 0;
rvt   = 0;
RV    = 0;
j     = 0;
k     = 0;

x2    = 0;
R     = 0;
Rxx   = 0;
Ryy   = 0;
Rxy   = 0;
Ryx   = 0;
Rvmax = 0;

f     = 0;
c     = 0;
d     = 0;
x     = abs(rand(37,37,"n"));

// <-- BENCH START -->
// Calculation of Escoufier's equivalent vectors
p  = size(x, 2);
vt = [1:p];                                    // Variables to test
vr = [];                                       // Result: ordered variables
RV = [1:p];                                    // Result: correlations

for j = 1:p                                    // loop on the variable number
    Rvmax = 0;
    for k = 1:(p-j+1)                          // loop on the variables
        x2 = [x, x(:, vr), x(:, vt(k))];       // New table to test
        // R = corrcoef(x2);                   // Correlations table
        // Not in scilab, so the 5 following lines do it
        q   = size(x2, 1);
        x2  = x2 - ones (q, 1) * sum (x2, "r") / q;
        c   = conj(x2' * x2 / (q - 1));
        d   = diag(c);
        R   = c ./ sqrt(d * d');
        Ryy = R(1:p, 1:p);
        Rxx = R(p+1:p+j, p+1:p+j);
        Rxy = R(p+1:p+j, 1:p);
        Ryx = Rxy';
        rvt = trace(Ryx*Rxy)/((trace(Ryy^2)*trace(Rxx^2))^0.5); // RV calculation
        if rvt > Rvmax
            Rvmax = rvt;                        // test of RV
            vrt(j) = vt(k);                     // temporary held variable
        end
    end
    vr(j) = vrt(j);                             // Result: variable
    RV(j) = Rvmax;                              // Result: correlation
    f     = find(vt~=vr(j));                    // identify the held variable
    vt    = vt(f);                              // reidentify variables to test
end
// <-- BENCH END -->
