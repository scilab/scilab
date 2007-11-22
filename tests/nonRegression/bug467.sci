// <-- Non-regression test for bug 467 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=467
//
// <-- Short Description -->
//    Bug Report Id: 12022200362265140
//    If you don't mind, where are read_dat_pcm or read_wavedat?
//
//
//
//    On Scilab 2-7 with " wavread " function
//    The Error Messages are:
//       !--error     4
//    undefined variable : erro
//    at line      26 of function read_dat_pcm             called by :
//    line     6 of function read_wavedat             called by :
//    line    93 of function wavread                  called by :
//    [x,y] = wavread('c:\b.wav','size'); 
//
//
//     !--error   246
//    function not defined for given argument type(s)
//    Check arguments or define function %s_convstr               for overloading
//    at line      28 of function wavread                  called by :
//    z = wavread('c:\b.wav',100000);   
// ...

correct=%F
result=execstr("[x,y] = wavread(''bug467.wav'',''size'')","errcatch","n")
if result == 0 then 
   result=execstr("z = wavread(""bug467.wav"",10)","errcatch","n")
end
affich_result(result==0, 467)
