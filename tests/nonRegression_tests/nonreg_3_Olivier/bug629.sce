// exec("e:\testNonReg\629.sce");  
// iReturnCode=execstr("[x,fs,bits] = wavread("e:\testNonReg\t1.wav")','errcatch');

iReturnCode=execstr('wavread(''t1.wav'')','errcatch');
//iReturnCode=execstr('wavread(''e:\testNonReg\btnenter.wav'')','errcatch');

//[strError,iReturnCode]=lasterror() 

 if iReturnCode == 0 then

   affich_result(%T,629);
   
  else 
    
    disp (lasterror());
    affich_result(%F,629);
 end