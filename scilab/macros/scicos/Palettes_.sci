function Palettes_()
//**INRIA
//**
//** Comments by Simone Mannori 
//**     
 Cmenu = []
 %pt=[]
 if super_block then
    Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
		     'Cmenu='"Palettes'";%scicos_navig=[]';
		     '%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1']
 else
    [palettes, windows] = do_palettes(palettes, windows)
 end
endfunction
