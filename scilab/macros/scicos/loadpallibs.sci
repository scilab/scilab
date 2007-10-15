function loadpallibs()
// Copyright INRIA
//to be called by exec(loadpallibs)
    if ~(exists('scicos_pal_libs')) then
      // this is for make tests
      [scicos_pal, %scicos_menu, %scicos_short, %scicos_help, ..
	  %scicos_display_mode, modelica_libs,scicos_pal_libs] = initial_scicos_tables()
    end
    for d=scicos_pal_libs 
        execstr(['if exists(''scs'+convstr(d)+'lib'')==0 then'
                 '  load(''SCI/macros/scicos_blocks/'+d+'/lib'');'
	         'end']);
    end
    clear d
endfunction
