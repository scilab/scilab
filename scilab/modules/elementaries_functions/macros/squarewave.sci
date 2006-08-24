function s = squarewave(t,%)
//square(t) generates a square wave with period 2*Pi 
//square(t) is like sin(t), only it creates a square wave 
//with peaks of +1 to -1 instead of a sine wave.

//square(t,%) generates an unsymetric  square wave: %, is the 
//percent of the period in which the signal is positive.
[lhs,rhs]=argn()
if rhs<2 then %=50,end
s=2*bool2s(modulo(t,2*%pi) < 2*%pi*%/100)-1
endfunction
