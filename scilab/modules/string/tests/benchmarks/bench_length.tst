// =============================================================================
// length benchmark
// exec(SCI+'/modules/STRING_V2/tests/benchmarks/bench_length.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_length_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
    a = length("Scilab");
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench length\n");
printf("Time bench 1 = %f seconds.\n",bench_length_1(runs));
// =============================================================================

