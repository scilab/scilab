function texout(v)
DATA_BEGIN=ascii(2);
DATA_END=ascii(5)
write(%io(2),DATA_BEGIN+"latex:$"+texprint(v)+'$'+DATA_END,'(a)')
endfunction
