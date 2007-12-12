// =============================================================================
// string benchmark
// exec(SCI+'/modules/string_V2/tests/benchmarks/bench_string.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_string_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
    a = string(1.5);
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench string\n");
printf("Time bench 1 = %f seconds.\n",bench_string_1(runs));
// =============================================================================

