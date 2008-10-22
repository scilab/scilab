function [mu]=lin2mu(y)
//Utility fct: converts linear signal to mu-law encoding.
//  mu = lin2mu(y) converts linear audio signal amplitudes
//   in the range  -1 <= y <= 1 to mu-law in the range 0 <= mu <= 255.
  SCALE = 32768;
  BIAS = 132;
  CLIP = 32635;
  OFFSET = 335;
  y = SCALE*y;
  sig = sign(y)+bool2s(y==0);
  y = min(abs(y),CLIP);
  [f,e] = frexp(y+BIAS);
  mu = 64*sig-16*e-fix(32*f)+OFFSET;
endfunction
