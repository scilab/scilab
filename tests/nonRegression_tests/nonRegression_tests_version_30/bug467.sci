// bug467

mode (-1)
clear

correct=%F
result=execstr("[x,y] = wavread(''bug467.wav'',''size'')","errcatch","n")
if result == 0 then 
   result=execstr("z = wavread(""bug467.wav"",10)","errcatch","n")
end
affich_result(result==0, 467)

clear

// The Error Messages are:
//    !--error     4
// undefined variable : erro
// at line      26 of function read_dat_pcm             called by :
// line     6 of function read_wavedat             called by :
// line    93 of function wavread                  called by :
// [x,y] = wavread('c:\b.wav','size'); 

//  !--error   246
//  function not defined for given argument type(s)
//  Check arguments or define function %s_convstr               for overloading
//  at line      28 of function wavread                  called by :
//  z = wavread('c:\b.wav',100000);   




