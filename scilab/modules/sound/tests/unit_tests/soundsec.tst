// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Unit tests for macro soundsec.sci -->

//==============================================================================
//Check abnormal entries
non_real_time = msprintf(_("%s: Wrong type for argument #%d: Real matrix expected.\n"),"soundsec",1);
non_real_freq = msprintf(_("%s: Wrong type for argument #%d: Real matrix expected.\n"),"soundsec",2);

non_positive_time=msprintf(_("%s: Wrong value for input argument #%d: Must be >= %d.\n"),"soundsec",1,0);
non_positive_freq=msprintf(_("%s: Wrong value for input argument #%d: Must be >= %d.\n"),"soundsec",2,0);

non_scalar_time=msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"soundsec",1,1,1);
non_scalar_freq=msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"soundsec",2,1,1);


//Soundsec must be called with real scalar values
assert_checkerror("soundsec(""test"",1)",non_real_time);
assert_checkerror("soundsec(%i,1)",non_real_time);
assert_checkerror("soundsec(1,""test"")",non_real_freq);
assert_checkerror("soundsec(1,%i)",non_real_freq);

assert_checkerror("soundsec(0:10,1)",non_scalar_time);
assert_checkerror("soundsec(1,0:10)",non_scalar_freq);

//Soundsec must be called with positive parameters
assert_checkerror("soundsec(-1,10)",non_positive_time);
assert_checkerror("soundsec(1,-1)",non_positive_freq);


//==============================================================================
//Check behaviour

//Soundsec returns empty value for a 0 sec sample
assert_checkequal(soundsec(0, 100), []);
//Soundsec returns empty value for a 0 frequency sample
assert_checkequal(soundsec(100, 0), []);

//Checks default behaviour for rate
assert_checkalmostequal( soundsec(10/22050), [0:9]./22050);

//Check 1s of samples at 10hz excluding the last is [0:9]
samples_t = soundsec(1, 10);
assert_checkalmostequal(samples_t, (0:9)/10);

//Check call on non integer time
samples_t = soundsec(0.11, 100); //eleven samples 0 0.01 ... 0.09 0.1
assert_checkalmostequal(samples_t, (0:10)/100);

//Check call on non integer frequency
samples_t = soundsec(1,5.2); //samples 0 1/5.2 2/5.2 ... 5/5.2
assert_checkalmostequal(samples_t, (0:5)/5.2);


//Check call on non integer frequency and time
samples_t = soundsec(1.2324,50.75);
//samples 0 1/50.75 ... 50/50.75 51/51.75 ... 62/50.75
//note that 1.2324 > 62/50.75 and 1.2324 <= 63/50.75
assert_checkalmostequal(samples_t, (0:62)/50.75);



