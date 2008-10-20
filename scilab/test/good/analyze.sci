function analyze (w,fmin,fmax,rate,points)
// Make a frequency plot of the signal w with sampling rate.
// The data must be at least points long.
// The maximal frequency plotted will be fmax, the minimal fmin.
// Copyright INRIA
[lhs,rhs]=argn(0);
	if ( rhs <= 4 ) then points=8192 ; end ;
	if ( rhs <= 3 ) then rate=0; end ;
	if ( rhs <= 2 ) then fmax=1500; end ;
	if ( rhs <= 1 ) then fmin=10;  end ;
	defaultrate=22050;
        if rate==0; rate=defaultrate; end;
        v=w(1:points);
        f=abs(fft(v,1));
        i=fmin/rate*points:fmax/rate*points;
        fr=i/points*rate;
        plot2d(fr',f(i)');
endfunction
