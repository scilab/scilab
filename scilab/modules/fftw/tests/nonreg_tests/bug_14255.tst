// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 14255 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14255
//
// <-- Short Description -->
// fftw without MKL leaks in withMKL function.


//check memory usage before and after run test to check that Scilab does not used more. ( ~ 200/250 Mbytes )
function y = myfun(x,WIZ) 
    set_fftw_wisdom(WIZ);
    y = fft(x,-1);
    y = abs(y);
endfunction

ntrc = 10000;
nsamp = 1024; 
dum = fft([1:nsamp],-1);
WIZ = get_fftw_wisdom();


fd_1 = mopen('TMPDIR/junk.bin','wb');
for ii =1:ntrc
    x = rand(1,nsamp);
    mput(x,'fb',fd_1);
end
mclose(fd_1);


timer();
fd_1 = mopen('TMPDIR/junk.bin','rb');
fd_2 = mopen('TMPDIR/junkot.bin','wb');
for ii =1:ntrc
    x = mget(nsamp,'fb',fd_1);
    y = myfun(x,WIZ);
mput(y,'fb',fd_2); 
end

timer()
mclose(fd_1);
mclose(fd_2);

deletefile('TMPDIR/junk.bin');
deletefile('TMPDIR/junkot.bin');
