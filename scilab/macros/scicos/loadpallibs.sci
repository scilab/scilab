function loadpallibs()
  //to be called by exec(loadpallibs)
    for d=scicos_pal_libs,execstr(['if exists(''scs'+convstr(d)+'lib'')==0 then'
              '  load(''SCI/macros/scicos_blocks/'+d+'/lib'');'
	      'end']); end
endfunction
