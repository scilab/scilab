

A = rand(10,10,'n')>=0 ;

B=ex3c_1(A);

if (~B)<>A then pause,end

B=ex3c_2(A);

if (~B)<>A then pause,end

B=ex3c_3(A);

if B<>A then pause,end

