// =============================================================================
// strindex benchmark
// exec(SCI+'/modules/string_V2/tests/benchmarks/bench_strindex.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_strindex_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
		[k,w]=strindex('1+3*abc/2.33',['+','-','*','/'])
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('k');clear('w'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench strindex\n");
printf("Time bench 1 = %f seconds.\n",bench_strindex_1(runs));
// =============================================================================

