// =============================================================================
// part benchmark
// exec(SCI+'/modules/STRING_V2/tests/benchmarks/bench_part.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_part_1(runs)
  cumulate = 0;
  for i = 1:runs
    timer();
    a = part(['a','abc','abcd'],[1,1,2]);
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate');; clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBench part\n");
printf("Time bench 1 = %f seconds.\n",bench_part_1(runs));
// =============================================================================

