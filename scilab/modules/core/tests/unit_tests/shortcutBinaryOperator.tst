//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// | and & operators are shortcuted within if test
// this test will check this behaviour based on some
// printed messages (and not on standard test_run/bugme process).

function r = T()
  disp("Call return [%T] ...");
  r = %t;
endfunction

function r = F()
  disp("Call return [%F] ...");
  r = %f;
endfunction

function r = TT()
    disp("Call return [%T %T] ...");
  r = [%t %t];
endfunction

function r = TF()
    disp("Call return [%T %F] ...");
  r = [%t %f];
endfunction

function r = FF()
    disp("Call return [%F %F] ...");
  r = [%f %f];
endfunction

//
// ** OR
//

// normal
T() | F()
T() | T()
F() | T()
F() | F()

TT() | TT()
TT() | TF()
TT() | FF()
TF() | TT()
TF() | TF()
TF() | FF()
FF() | TT()
FF() | TF()
FF() | FF()

// Shortcuted.
if T() | F() then disp("Success"); else disp("Failed"); end
if T() | T() then disp("Success"); else disp("Failed"); end
// Double evaluation.
if F() | T() then disp("Success"); else disp("Failed"); end
if F() | F() then disp("Failed"); else disp("Success"); end

//Shortcuted.
if TT() | TT() then disp("Success"); else disp("Failed"); end
if TT() | TF() then disp("Success"); else disp("Failed"); end
if TT() | FF() then disp("Success"); else disp("Failed"); end

// Double evaluation.
if TF() | TT() then disp("Success"); else disp("Failed"); end
if TF() | TF() then disp("Failed"); else disp("Success"); end
if TF() | FF() then disp("Failed"); else disp("Success"); end
if FF() | TT() then disp("Success"); else disp("Failed"); end
if FF() | TF() then disp("Failed"); else disp("Success"); end
if FF() | FF() then disp("Failed"); else disp("Success"); end


//
// ** AND
//

// normal
T() & F()
T() & T()
F() & T()
F() & F()

TT() & TT()
TT() & TF()
TT() & FF()
TF() & TT()
TF() & TF()
TF() & FF()
FF() & TT()
FF() & TF()
FF() & FF()

// Shortcuted.
if F() & T() then disp("Failed"); else disp("Success"); end
if F() & F() then disp("Failed"); else disp("Success"); end
// Double evaluation.
if T() & F() then disp("Failed"); else disp("Success"); end
if T() & T() then disp("Success"); else disp("Failed"); end

// Shortcuted.
if FF() & TT() then disp("Failed"); else disp("Success"); end
if FF() & TF() then disp("Failed"); else disp("Success"); end
if FF() & FF() then disp("Failed"); else disp("Success"); end
if TF() & TT() then disp("Failed"); else disp("Success"); end
if TF() & TF() then disp("Failed"); else disp("Success"); end
if TF() & FF() then disp("Failed"); else disp("Success"); end

// Double evaluation.
if TT() & TT() then disp("Success"); else disp("Failed"); end
if TT() & TF() then disp("Failed"); else disp("Success"); end
if TT() & FF() then disp("Failed"); else disp("Success"); end

clear T;
clear F;
clear TT;
clear TF;
clear FF;