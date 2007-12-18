// =============================================================================
// ascii benchmark
// exec(SCI+'/modules/string_V2/tests/benchmarks/bench_ascii.tst');
// =============================================================================

// Bench for
// =============================================================================

function time = bench_for(runs)

	timer();
	
	for i = 1:runs
	end
	
	timing = timer();
	time   = timing / runs;
	
endfunction


// Bench 1
// =============================================================================

function time = bench_ascii_1(runs)

	str_test_mat = ["abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"; ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
					"abcdefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"];
	
	timer();
	
	for i = 1:runs
		a = ascii(str_test_mat);
	end
	
	timing = timer();
	time   = timing / runs;
	
endfunction


// Bench 2
// =============================================================================

function time = bench_ascii_2(runs)
	
	text = mgetl(SCI+"/modules/string/tests/benchmarks/ascii_text.txt");
	
	timer();
	
	for i = 1:runs
		a = ascii(text);
	end
	
	timing = timer();
	time   = timing / runs;
	
endfunction


// Bench 3
// =============================================================================

function time = bench_ascii_3(runs)
	
	exec(SCI+"/modules/string/tests/benchmarks/ascii_code.txt",-1);
	timer();
	
	for i = 1:runs
		a = ascii(ref);
	end
	
	timing = timer();
	time   = timing / runs;
	
endfunction

// Bench 4
// =============================================================================

function time = bench_ascii_4(runs)
	
	code_test_mat = int( 128 * rand(100,100) );
	timer();
	
	for i = 1:runs
		a = ascii(code_test_mat);
	end
	
	timing = timer();
	time   = timing / runs;
	
endfunction

// Display
// ============================================================================

runs = 100000;

printf("\nBenchs ascii\n");

printf("Time for     = %f seconds.\n",bench_for(runs));
printf("Time bench 1 = %f seconds.\n",bench_ascii_1(runs));
printf("Time bench 2 = %f seconds.\n",bench_ascii_2(runs));
printf("Time bench 3 = %f seconds.\n",bench_ascii_3(runs));

