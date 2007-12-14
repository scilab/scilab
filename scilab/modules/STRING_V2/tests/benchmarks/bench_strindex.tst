// =============================================================================
// tokens benchmark
// exec(SCI+'/modules/string_V2/tests/benchmarks/bench_tokens.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_tokens_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
		a = tokens('This is a character string');
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench tokens\n");
printf("Time bench 1 = %f seconds.\n",bench_tokens_1(runs));
// =============================================================================

