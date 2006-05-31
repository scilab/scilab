// Scilab Benchmark 2 (8 March 2003)
// version 2, scaled to get 1 +/- 0.1 sec with R 1.6.2
// using the standard ATLAS library (Rblas.dll)
// on a Pentium IV 1.6 Ghz with 1 Gb Ram on Win XP pro
// Author : Philippe Grosjean
// eMail  : phgrosjean@sciviews.org
// Web    : http://www.sciviews.org
// License: GPL 2 or above at your convenience (see: http://www.gnu.org)
//
// Several tests are adapted from:
//***************************************************************************
//* Matlab Benchmark program version 2.0                                    *
//* Author : Stefan Steinhaus                                               *
//* EMAIL  : stst@informatik.uni-frankfurt.de                               * 
//* This program is public domain. Feel free to copy it freely.             *
//***************************************************************************
// Escoufier's equivalents vectors (III.5) is adapted from Planque & Fromentin, 1996 
// Ref: Escoufier Y., 1970. Echantillonnage dans une population de variables
// aleatoires réelles. Publ. Inst. Statis. Univ. Paris 19 Fasc 4, 1-47.
//
// type exec('c:\<dir>\Scilab2.sce', 0); to start the test

stacksize(30000000)
runs = 3;			// Number of times the tests are executed
times = zeros(5, 3);

mode (-1)

affich_date()
disp("")
disp("")
disp("------------------------------{ DEBUT TEST SUITE BENCHMARK v2 from sciviews DEBUT }");
disp("")
disp("")


disp('   Scilab Benchmark 2')
disp('   ==================')
disp('Number of times each test is run__________________________: ' + string(runs))
disp(' ')


disp('   I. Matrix calculation')
disp('   ---------------------')


// (1)
cumulate = 0; a = 0; b = 0;
for i = 1:runs
  timer();
    a = abs(rand(1500, 1500, 'n')/10);
    b = a';
    a = matrix(b, 750, 3000);
    b = a';
  timing = timer();
  cumulate = cumulate + timing;
end;
timing = cumulate/runs;
times(1, 1) = timing;
disp('Creation, transp., deformation of a 1500x1500 matrix (sec): ' + string(timing));
clear('a'); clear('b');

// (2)
cumulate = 0; b = 0;
for i = 1:runs
  a = abs(rand(800, 800, 'n')/2);
  timer(); 
    b = a.^1000; 
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(2, 1) = timing;
disp('800x800 normal distributed random matrix ^1000______ (sec): ' + string(timing))
clear('a'); clear('b');

// (3)
cumulate = 0; b = 0;
for i = 1:runs
  a = rand(2000000, 1, 'n');
  timer();
    b = -sort(-a);
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(3, 1) = timing;
disp('Sorting of 2,000,000 random values__________________ (sec): ' + string(timing))
clear('a'); clear('b'); 

// (4)
cumulate = 0; b = 0;
for i = 1:runs
  a = rand(700, 700, 'n');
  timer();
    b = a'*a;
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(4, 1) = timing;
disp('700x700 cross-product matrix (b = a'' * a)___________ (sec): ' + string(timing))
clear('a'); clear('b');

// (5)
cumulate = 0; c = 0;
for i = 1:runs
  a = rand(600, 600, 'n');
  b = 1:600;
  timer();
    c = a\b';
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(5, 1) = timing;
disp('Linear regression over a 600x600 matrix (c = a \ b'') (sec): ' + string(timing))
clear('a'); clear('b'); clear('c');

times = sort(times, 'r');
disp('                --------------------------------------------------')
disp('                Trimmed geom. mean (2 extremes eliminated): ' +  string(exp(mean(log(times(2:4,1))))))
disp(' ')

disp('   II. Matrix functions')
disp('   --------------------')


// (1)
cumulate = 0; b = 0;
for i = 1:runs
  a = rand(800000, 1, 'n');
  timer();
    b = fft(a, -1);
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(1, 2) = timing;
disp('FFT over 800,000 random values______________________ (sec): ' +  string(timing))
clear('a'); clear('b');

// (2)
cumulate = 0; b = 0;
for i = 1:runs
  a = rand(320, 320, 'n');
  timer();
    b = spec(a);
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(2, 2) = timing;
disp('Eigenvalues of a 320x320 random matrix______________ (sec): ' + string(timing))
clear('a'); clear('b');

// (3)
cumulate = 0; b = 0;
for i = 1:runs
  a = rand(650, 650, 'n');
  timer();
    b = det(a);
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(3, 2) = timing;
disp('Determinant of a 650x650 random matrix______________ (sec): ' +  string(timing))
clear('a'); clear('b');

// (4)
cumulate = 0; b = 0;
for i = 1:runs
  a = rand(900, 900, 'n');
  a = a'*a;
  timer();
    b = chol(a);
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(4, 2) = timing;
disp('Cholesky decomposition of a 900x900 matrix__________ (sec): ' + string(timing))
clear('a'); clear('b');

// (5)
cumulate = 0; b = 0;
for i = 1:runs
  a = rand(400, 400, 'n');
  timer();
    b = inv(a);
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(5, 2) = timing;
disp('Inverse of a 400x400 random matrix__________________ (sec): ' + string(timing))
clear('a'); clear('b');

times = sort(times, 'r');
disp('                --------------------------------------------------')
disp('                Trimmed geom. mean (2 extremes eliminated): ' +  string(exp(mean(log(times(2:4,2))))))
disp(' ')

disp('   III. Programmation')
disp('   ------------------')

// (1)
cumulate = 0; a = 0; b = 0; phi = 1.6180339887498949;
for i = 1:runs
  a = floor(1000 * rand(750000, 1, 'u'));
  timer();	  
    b = (phi.^a - (-phi).^(-a)) / sqrt(5);
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(1, 3) = timing;
disp('750,000 Fibonacci numbers calculation (vector calc)_ (sec): ' + string(timing))
clear('a'); clear('b'); clear('phi');

// (2)
cumulate = 0; a = 2250; b = 0;
for i = 1:runs
  timer();
    b = ones(a, a)./((1:a)' * ones(1, a) + ones(a, 1) * (0:(a-1)));    
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(2, 3) = timing;
disp('Creation of a 2250x2250 Hilbert matrix (matrix calc) (sec): ' + string(timing))
clear('a'); clear('b');

// (3)
cumulate = 0; c = 0;
deff('[c]=gcd2(a, b)',['if b <= 1.0E-4'; 'c = a;'; 'else'; 'b(b == 0) = a(b == 0);'; 'c = gcd2(b, modulo(a, b));'; 'end'])
for i = 1:runs
  a = ceil(1000 * rand(70000, 1, 'u'));
  b = ceil(1000 * rand(70000, 1, 'u'));
  timer();	  
  c = gcd2(a, b);                            // gcd2 is a recursive function
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(3, 3) = timing;
disp('Grand common divisors of 70,000 pairs (recursion)___ (sec): ' +  string(timing))
clear('a'); clear('b'); clear('c');

// (4)
cumulate = 0; b = 0;
for i = 1:runs
  b = zeros(220, 220);
  timer();
    for j = 1:220
      for k = 1:220
        b(k,j) = abs(j - k) + 1;
      end
    end
  timing = timer();
  cumulate = cumulate + timing;
end
timing = cumulate/runs;
times(4, 3) = timing;
disp('Creation of a 220x220 Toeplitz matrix (loops)_______ (sec): ' + string(timing))
clear('b'); clear('j'); clear('k');

// (5)
cumulate = 0; p = 0; q = 0; vt = 0; vr = 0; vrt = 0; rvt = 0; RV = 0; j = 0; k = 0;
x2 = 0; R = 0; Rxx = 0; Ryy = 0; Rxy = 0; Ryx = 0; Rvmax = 0; f = 0;, c= 0; d = 0;
for i = 1:runs
  x = abs(rand(37, 37, 'n'));
  timer();
    // Calculation of Escoufier's equivalent vectors
    p = size(x, 2);
    vt = [1:p];                                // Variables to test
    vr = [];                                   // Result: ordered variables
    RV = [1:p];                                // Result: correlations
    for j = 1:p                                // loop on the variable number
      Rvmax = 0;
      for k = 1:(p-j+1)                        // loop on the variables
        x2 = [x, x(:, vr), x(:, vt(k))];       // New table to test
        // R = corrcoef(x2);                   // Correlations table
        // Not in scilab, so the 5 following lines do it
        q = size(x2, 1);
        x2 = x2 - ones (q, 1) * sum (x2, 'r') / q;
	c = conj(x2' * x2 / (q - 1));
	d=diag(c);
	R = c ./ sqrt(d * d');
        Ryy = R(1:p, 1:p);
        Rxx = R(p+1:p+j, p+1:p+j);
        Rxy = R(p+1:p+j, 1:p);
        Ryx = Rxy';
        rvt = trace(Ryx*Rxy)/((trace(Ryy^2)*trace(Rxx^2))^0.5); // RV calculation
        if rvt > Rvmax
          Rvmax = rvt;                         // test of RV
          vrt(j) = vt(k);                      // temporary held variable
        end
      end
      vr(j) = vrt(j);                          // Result: variable
      RV(j) = Rvmax;                           // Result: correlation
      f = find(vt~=vr(j));                     // identify the held variable
      vt = vt(f);                              // reidentify variables to test
    end
  timing = timer();
  cumulate = cumulate + timing;
end
times(5, 3) = timing;
disp('Escoufier''s method on a 37x37 matrix (mixed)________ (sec): ' + string(timing))
clear('x'); clear('p'); clear('q'); clear('vt'); clear('vr'); clear('vrt'); clear('rvt'); clear('RV'); clear('j'); clear('k');
clear('x2'); clear('R'); clear('Rxx'); clear('Ryy'); clear('Rxy'); clear('Ryx'); clear('Rvmax'); clear('f'); clear('c'); clear('d'); 

times = sort(times, 'r');
disp('                --------------------------------------------------')
disp('                Trimmed geom. mean (2 extremes eliminated): ' +  string(exp(mean(log(times(2:4,3))))))
disp(' ')

disp(' ')
disp('Total time for all 15 tests_________________________ (sec): ' + string(sum(times)))
disp('Overall mean (sum of I, II and III trimmed means/3)_ (sec): ' + string(exp(mean(log(times(2:4,:))))))
clear('cumulate'); clear('timing'); clear('times'); clear('runs'); clear('i');
disp('                      --- End of test ---')     


disp("")
disp("")
disp("------------------------------{ FIN TEST SUITE BENCHMARK v2 from sciviews FIN }");
disp("")
disp("")
affich_date()
