load SCI/macros/scicos/lib;
load SCI/macros/scicos_blocks/lib;
getf SCI/tests/scicos_tests.sci;
options=default_options();
Blocs=string(blockslib);Blocs=Blocs(2:$);
Blocs(Blocs=="m_sin")=[];

nb=size(Blocs,1);
if checkdefine() then pause,end
if checkdraw() then pause,end
if checkinputs() then pause,end
if checkoutputs() then pause,end
if checkorigin() then pause,end
//if checkset() then pause,end a revoir avec Ramine
if checkdeput() then pause,end
if checkfiring() then pause,end
if check_define_ports() then pause,end
if check_set_ports() then pause,end
    


