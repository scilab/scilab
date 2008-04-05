function [x] = erfinv(y)
//   X = erfinv(Y) is the inverse error function for each element of Y.
//   The inverse error function satisfies y = erf(x), for -1 <= y <= 1
//   and -inf <= x <= inf.

  if ~isreal(y) then error("erfinv:argument must be real.");end;
  x = zeros(y)

  // rational approximations.
  // Central range: -0.7 <= y <= 0.7
  R_central=rlist(poly([0.886226899,-1.645349621,0.914624893,-0.140543331],'z','c'),..
		  poly([1 -2.1183777250000002,1.442710462,-0.329097515,0.012229801],'z','c'))    
  //Left and  Right ranges : 0.7 < abs(y )< 1,
  R_out=rlist(poly([-1.970840454,-1.624906493,3.429567803,1.641345311],'z','c'),..
	      poly([1 3.5438892000000002,1.6370678000000001],'z','c'))   

  // Central range: -0.7 <= y <= 0.7
  y0 = 0.7;
  k = find(abs(y)<=y0);
  if k<>[] then
    x(k)=y(k) .* horner(R_central,y(k) .* y(k));
  end

  //Right range : 0.7 < y < 1
  k = find(y0<y & y<1);
  if k<>[] then
    x(k) = horner(R_out,sqrt(-log((1-y(k))/2)));
  end

  //Left range :  -1 < y < 0.7
  k = find(-y0>y & y>-1);
  if k<>[] then
    x(k) = -horner(R_out,sqrt(-log((1+y(k))/2)));
  end

  // Two steps of Newton-Raphson correction to full accuracy.
  // Without these steps, erfinv(y) would be about 3 times
  // faster to compute, but accurate to only about 6 digits.
  
  x = x-(erf(x)-y) ./ (2/sqrt(%pi)*exp(-x.^2));
  x = x-(erf(x)-y) ./ (2/sqrt(%pi)*exp(-x.^2));


  // Limit cases
  x(y == -1) = -%inf;
  x(y == 1) = %inf;
  x(abs(y) > 1|isnan(y)) =  %nan;

endfunction
