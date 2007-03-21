function ww=ascii2string(w1)
//converts a numeric (real or integer) matrix made of ascii codes
//into a one column scilab string matrix and conversely.
ww=[];
for k=1:size(w1,1)
ww=[ww;ascii(w1(k,:))];
end
endfunction
