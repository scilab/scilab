// =============================================================================
// stripblanks benchmark
// exec(SCI+'/modules/string_V2/tests/benchmarks/bench_stripblanks.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_stripblanks_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
    a = stripblanks(' S C I L A B ');
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench stripblanks\n");
printf("Time bench 1 = %f seconds.\n",bench_stripblanks_1(runs));
// =============================================================================

