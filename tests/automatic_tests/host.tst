getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/host_data.ref','rb');
//create a getdir function based on host
function wd=getdir()
        if MSDOS then
          host('cd>'+TMPDIR+'\path');
        else
          host('pwd>'+TMPDIR+'/path');
        end
        wd=read(TMPDIR+'/path',1,1,'(a)')
      
endfunction
if load_ref('%ans') then   pause,end,

//call it
wd = getdir();
if load_ref_nocheck('wd') then   bugmes();quit;end,

xdel_run(winsid());

mclose(%U);
