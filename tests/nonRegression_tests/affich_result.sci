function affich_result(correct, num)
// affiche FAILED ou PASSED, fonction utiliser dans les tests-suites de non regression
   // This part display result
   if correct then
      disp("Test bug" + string(num)  +" : PASSED");
   else
      disp("*****************************************  Test bug" + string(num)  +" : FAILED ! ");
   end
endfunction

//======================================================================================================================

function affich_date
w=getdate()
disp(date() + "  "+string(w(7))+" h. "+string(w(8))+" mn. "+string(w(9))+" s. ")
endfunction


//======================================================================================================================

