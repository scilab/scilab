// =============================================================================
// convstr benchmark
// exec(SCI+'/modules/STRING_V2/tests/benchmarks/bench_convstr.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_convstr_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
    a = convstr(['ABC';'x'],'l');
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench convstr\n");
printf("Time bench 1 = %f seconds.\n",bench_convstr_1(runs));
// =============================================================================

