function y = convert_to_float(code, binlen, maxbound, minbound)
Max_Bin = 2^binlen-1;
y       = [];
for i=1:length(maxbound)
  y(i) = (maxbound(i) - minbound(i)) * (bin2dec(part(x,(i-1)*binlen+1:i*binlen)) / Max_Bin) + minbound(i);
end
endfunction
