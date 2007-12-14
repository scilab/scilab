// =============================================================================
// code2str benchmark
// exec(SCI+'/modules/STRING_V2/tests/benchmarks/bench_code2str.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_code2str_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
    a = code2str(-(10:35));
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench code2str\n");
printf("Time bench 1 = %f seconds.\n",bench_code2str_1(runs));
// =============================================================================

