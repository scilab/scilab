// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10807 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10807
//
// <-- Short Description -->
// It was not possible to overload fft and fftw for list, tlist, mlist type
//

M = mlist(['V', 'name', 'value'], ['a', 'b';'c' 'd'], [1 2; 3 4]);

if %fftw then
  fname = "fftw";
  msgerr = [msprintf(gettext("Function not defined for given argument type(s),\n"));
            msprintf(gettext("  check arguments or define function %s for overloading.\n"), "%V_" + fname)];
  assert_checkerror ("fft(M)", msgerr);
  
  msgerr = [msprintf(gettext("Function not defined for given argument type(s),\n"));
            msprintf(gettext("  check arguments or define function %s for overloading.\n"), "%V_" + fname)];
  assert_checkerror ("fftw(M)", msgerr);
  
  function bOK = %V_fftw(X)
    bOK = %T;
  endfunction
  
  assert_checktrue(fft(M));
  assert_checktrue(fftw(M));
else
  fname = "fft";
  msgerr = [msprintf(gettext("Function not defined for given argument type(s),\n"));
            msprintf(gettext("  check arguments or define function %s for overloading.\n"), "%V_" + fname)];
  assert_checkerror ("fft(M)", msgerr);
  
  function bOK = %V_fft(X)
    bOK = %T;
  endfunction
  assert_checktrue(fft(M));
end
