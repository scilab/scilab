function convTxt = m2sciTestConversionNoExe(functionName)

mfilesPath = "SCI/modules/m2sci/tests/unit_tests/mfilesnoexec/";

exec("SCI/modules/m2sci/tests/unit_tests/utils/sci_m2sciUnknownDims.sci", -1);
exec("SCI/modules/m2sci/tests/unit_tests/utils/sci_m2sciUnknownType.sci", -1);

mfile2sci(mfilesPath + functionName + ".m", TMPDIR, %F, %F, -1, %T);

convTxt = mgetl(TMPDIR + filesep() + functionName + ".sci" );

endfunction
