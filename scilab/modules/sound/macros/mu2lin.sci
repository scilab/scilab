function [y]=mu2lin(mu)
    //Utility fct: converts mu-law encoding to linear signal.
    //   y=mu2lin(mu) converts mu-law encoded 8-bit audio signals,
    //   stored in the range 0 <= mu <= 255, to
    //   linear signal amplitude in the range -s < y < s where
    //   s = 32124/32768 ~= .9803.  The input mu is often obtained
    //   using mget(...,'uc') to read byte-encoded audio files.
    //  Translation of C program by:
    //  Craig Reese: IDA/Supercomputing Research Center
    //  Joe Campbell: Department of Defense
    //  29 September 1989
    SCALE = 1/32768;
    ETAB = [0,132,396,924,1980,4092,8316,16764];
    mu = 255-mu;
    sig = mu>127;
    e = fix(mu/16)-8*bool2s(sig)+1;
    f = mu-fix(mu./16).*16;
    y = f.*2.^(e+2);
    e(:) = ETAB(e);
    y = SCALE*(1-2*bool2s(sig)) .* (e+y);
endfunction
