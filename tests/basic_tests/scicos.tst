load SCI/macros/scicos/lib;
exec(loadpallibs,-1) 

getf SCI/tests/scicos_tests.sci;
options=default_options();
//build the block set
blockslib='scs'+['Branching','Events','Misc','Sinks','Threshold','Linear', ...
	  'NonLinear','Sources','Electrical','Hydraulics']+'lib';
Blocs=[]
for blocklib=blockslib
  B=string(blocklib);Blocs=[Blocs;B(2:$)];
end
Blocs(Blocs=="m_sin")=[];

nb=size(Blocs,1);
if checkdefine() then pause,end
if checkdraw() then pause,end
if checkinputs() then pause,end
if checkoutputs() then pause,end
if checkorigin() then pause,end
if checkset() then pause,end
if checkdeput() then pause,end
if checkfiring() then pause,end
if check_define_ports() then pause,end
if check_set_ports() then pause,end
    


