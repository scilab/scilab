// <-- Non-regression test for bug 629 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=629
//
// <-- Short Description -->
//    When trying to use the wavread function I get these errors
//    that make this function totally unuseful, either calling it
//    with ONE parameter (a) or calling it with two parameters.
//    I made myself sure that the parameters would be of the form
//    indicated by help on line.


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