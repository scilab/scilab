function loadpallibs()
  //to be called by exec(loadpallibs)
    if ~(exists('scicos_pal_libs')) then
      // this is for make tests
      scicos_pal_libs=['Branching','Events','Misc','Sinks','Threshold','Linear', ...
	  'NonLinear','Sources','Electrical','Hydraulics'];
    end
    for d=scicos_pal_libs,execstr(['if exists(''scs'+convstr(d)+'lib'')==0 then'
              '  load(''SCI/macros/scicos_blocks/'+d+'/lib'');'
	      'end']); end
endfunction
