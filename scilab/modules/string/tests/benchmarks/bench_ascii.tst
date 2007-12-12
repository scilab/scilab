// =============================================================================
// ascii benchmark
// exec(SCI+'/modules/string_V2/tests/benchmarks/bench_ascii.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_ascii_1(runs)
  cumulate = 0;
  str_test_mat =  ["abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
				"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"; ..
				"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
				"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"];

  for i = 1:runs
    timer();
	  a = ascii(str_test_mat);
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate'); clear('str_test_mat'); clear('i');
endfunction

// =============================================================================
// Bench 2
// =============================================================================
function time = bench_ascii_2(runs)
  cumulate = 0;
  text = mgetl(SCI+"/modules/string/tests/benchmarks/ascii_text.txt");

  for i = 1:runs
    timer();
	  a = ascii(text);
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate'); clear('text'); clear('i');
endfunction
// =============================================================================
// Bench 3
// =============================================================================
function time = bench_ascii_3(runs)
  cumulate = 0;
  exec(SCI+"/modules/string/tests/benchmarks/ascii_code.txt",-1);

  for i = 1:runs
    timer();
	  a = ascii(ref);
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate'); clear('ref'); clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBenchs ascii\n");
printf("Time bench 1 = %f seconds.\n",bench_ascii_1(runs));
printf("Time bench 2 = %f seconds.\n",bench_ascii_2(runs));
printf("Time bench 3 = %f seconds.\n",bench_ascii_3(runs));
// =============================================================================

