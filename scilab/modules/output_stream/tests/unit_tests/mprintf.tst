// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2020 - Samuel GOUGEON - Le Mans Université
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for msprintf function
// =============================================================================

// format '%f'
// =============================================================================

mprintf("==>%f<==\n\n",-35);

mprintf("==>%f<==\n\n",35.55);
mprintf("==>%f<==\n\n",0.00433);
mprintf("==>%f<==\n\n",0.0000000345456);
mprintf("==>%f<==\n\n",1112423453);

mprintf("==>%15f<==\n\n",-35);
mprintf("==>%15f<==\n\n",0.00433);
mprintf("==>%15f<==\n\n",0.0000000345456);
mprintf("==>%15f<==\n\n",1112423453);

mprintf("==>%.1f<==\n\n" ,-35);
mprintf("==>%.0f<==\n\n" ,-35);
mprintf("==>%#.0f<==\n\n",-35);
mprintf("==>%.1f<==\n\n" ,0.00433);
mprintf("==>%.15f<==\n\n",0.0000000345456);
mprintf("==>%.1f<==\n\n" ,11124234534545);

// format '%g'
// =============================================================================

mprintf("==>%g<==\n\n" ,-35);
mprintf("==>%g<==\n\n" ,35.55);
mprintf("==>%g<==\n\n" ,35.551234567890);
mprintf("==>%+g<==\n\n",35.551234567890);
mprintf("==>%g<==\n\n" ,0.00433);
mprintf("==>%g<==\n\n" ,0.0000000345456);
mprintf("==>%g<==\n\n" ,11124234534545);

mprintf("==>%15g<==\n\n",-35);
mprintf("==>%15g<==\n\n",0.00433);
mprintf("==>%15g<==\n\n",0.0000000345456);
mprintf("==>%15g<==\n\n",11124234534545);

mprintf("==>%.1g<==\n\n",-35.1);
mprintf("==>%.0g<==\n\n",-35.1);
mprintf("==>%#.0g<==\n\n",-35.1);
mprintf("==>%#.0G<==\n\n",-35.1);
mprintf("==>%.1g<==\n\n",0.00433);
mprintf("==>%.15g<==\n\n",0.0000000345456);
mprintf("==>%.1g<==\n\n",11124234534545);

// format '%e'
// =============================================================================

mprintf("==>%e<==\n\n",-35);
mprintf("==>%e<==\n\n",35.55);
mprintf("==>%+e<==\n\n",35.55);
mprintf("==>%e<==\n\n",35.551234567890);
mprintf("==>%e<==\n\n",0.00433);
mprintf("==>%e<==\n\n",0.0000000345456);
mprintf("==>%e<==\n\n",11124234534545);
mprintf("==>%E<==\n\n",11124234534545);

mprintf("==>%15e<==\n\n",-35);
mprintf("==>%15e<==\n\n",0.00433);
mprintf("==>%15e<==\n\n",0.0000000345456);
mprintf("==>%+15e<==\n\n",0.0000000345456);
mprintf("==>%15e<==\n\n",11124234534545);

mprintf("==>%.1e<==\n\n",-35);
mprintf("==>%.0e<==\n\n",-35.1);
mprintf("==>%#.0e<==\n\n",-35.1);
mprintf("==>%.1e<==\n\n",0.00433);
mprintf("==>%.15e<==\n\n",0.0000000345456);
mprintf("==>%.1e<==\n\n",11124234534545);

// format '%c'
// =============================================================================

mprintf("==>%c<==\n\n","t");
mprintf("==>%10c<==\n\n","t");
mprintf("==>%10.3c<==\n\n","t");
mprintf("==>%-10c<==\n\n","t");

// format '%s'
// =============================================================================

mprintf("==>%s<==\n\n","text");
mprintf("==>%10s<==\n\n","text");
mprintf("==>%10.3s<==\n\n","text");
mprintf("==>%-10s<==\n\n","text");
mprintf("==>%s<==\n\n","t");

// format '%x'
// =============================================================================

mprintf("==>%x<==\n\n",123);
mprintf("==>%.10x<==\n\n",123);
mprintf("==>%x<==\n\n",-123);
mprintf("==>%X<==\n\n",-123);
mprintf("==>%#.3X<==\n\n",12);

// Vectorisation
// =============================================================================

// test 1

A = "row "+string(1:10)';
B = [ ..
    50.153416    28.06498     40.948255 ; ..
    43.685876    12.800585    87.841258 ; ..
    26.931248    77.831286    11.383597 ; ..
    63.257449    21.190304    19.983377 ; ..
    40.51954     11.213547    56.186607 ; ..
    91.847078    68.56896     58.961773 ; ..
    4.3733433    15.312167    68.539797 ; ..
    48.185089    69.708506    89.062247 ; ..
    26.39556     84.155184    50.422128 ; ..
    41.481037    40.620248    34.936154 ];

mprintf("==>%10s : %08.4f %08.4f %08.4f<==\n\n",A,B);

// test 2

C = [ ..
    50.153416    28.06498  ; ..
    43.685876    12.800585 ; ..
    26.931248    77.831286 ; ..
    63.257449    21.190304 ; ..
    40.51954     11.213547 ; ..
    91.847078    68.56896  ; ..
    4.3733433    15.312167 ; ..
    48.185089    69.708506 ; ..
    26.39556     84.155184 ; ..
    41.481037    40.620248 ];


D = [ ..
    40.948255 ; ..
    87.841258 ; ..
    11.383597 ; ..
    19.983377 ; ..
    56.186607 ; ..
    58.961773 ; ..
    68.539797 ; ..
    89.062247 ; ..
    50.422128 ; ..
    34.936154 ];

mprintf("==>%10s : %08.4f %08.4f %08.4f<==\n\n",A,C,D);

// Booleans
// =============================================================================
n = [%pi ; %e];
b = [%T ; %F];
for f = ["d" "i" "u" "o" "x" "X" "g" "G"]
    mprintf("%"+f+"\n", b);
    mprintf("%"+f+" %d\n", b, n);
    if and(f <> ["u" "o" "x" "X"])  // http://bugzilla.scilab.org/16563
        mprintf("%2$"+f+" %1$d\n", n, b);
    end
end
// %f
msprintf("%f\n", b);
mprintf("%f %d\n", b, n);
mprintf("%2$f %1$d\n", n, b);
// %e
mprintf("%e\n", b);
mprintf("%e %d\n", b, n);
mprintf("%2$e %1$d\n", n, b);
// %E
mprintf("%E\n", b);
mprintf("%E %d\n", b, n);
mprintf("%2$E %1$d\n", n, b);
// %s, %c
for f = ["s" "c"]
    mprintf("%"+f+"\n", b);
    mprintf("%"+f+" %d\n", b, n);
    mprintf("%2$"+f+" %1$d\n", n, b);
end
