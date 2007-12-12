// =============================================================================
// strsplit benchmark
// exec(SCI+'/modules/string_V2/tests/benchmarks/bench_strsplit.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_strsplit_1(runs)
  cumulate = 0;
  STR_SPLITED = ['strsplit splits';'a string';'into';'a vector of strings'];
  STR = 'strsplit splits a string into a vector of strings';
  INDICES = [15 25 30];
  for i = 1:runs
    timer();
		
		a = stripblanks(strsplit(STR,INDICES));
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate'); clear('i');
  clear('STR_SPLITED'); clear('STR'); clear('INDICES');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench strsplit\n");
printf("Time bench 1 = %f seconds.\n",bench_strsplit_1(runs));
// =============================================================================

