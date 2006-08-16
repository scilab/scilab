function x=ifft(a)
  if type(a)==1 then 
    x=fft(a,1)
  elseif typeof(a)=='hypermat' then
    x=%hm_fft(a,1)
  end
endfunction
