// =============================================================================
// strcat benchmark
// exec(SCI+'/modules/string_V2/tests/benchmarks/bench_strcat.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_strcat_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
		a = strcat(['abc','abd','aa','bxe']);
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench strcat\n");
printf("Time bench 1 = %f seconds.\n",bench_strcat_1(runs));
// =============================================================================

