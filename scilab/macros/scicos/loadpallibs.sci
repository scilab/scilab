function loadpallibs()
  //to be called by exec(loadpallibs)
    Dirs=['Branching','Events','Misc','Sinks','Threshold','Linear', ...
	  'NonLinear','Sources'];
    for d=Dirs,execstr(['if exists(''scs'+convstr(d)+'lib'')==0 then'
              '  load(''SCI/macros/scicos_blocks/'+d+'/lib'');'
	      'end']); end
endfunction
