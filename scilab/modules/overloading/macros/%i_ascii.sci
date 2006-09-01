function x=%i_ascii(x)
if type(x)==10
x=int32(ascii(x))
else
x=ascii(double(x))
end
endfunction
