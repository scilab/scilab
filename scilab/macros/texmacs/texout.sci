function texout(v)
DATA_BEGIN=ascii(2);
DATA_END=ascii(5)
mprintf(strsubst(DATA_BEGIN+"latex:$"+texprint(v)+'$'+DATA_END,'\','\\'))
endfunction
