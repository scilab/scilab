// =============================================================================
// str2code benchmark
// exec(SCI+'/modules/STRING_V2/tests/benchmarks/bench_str2code.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_str2code_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
    a = str2code('ABCDEFGHIJKLMNOPQRSTUVWXYZ');
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench str2code\n");
printf("Time bench 1 = %f seconds.\n",bench_str2code_1(runs));
// =============================================================================

