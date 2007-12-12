// =============================================================================
// grep benchmark
// exec(SCI+'/modules/string_V2/tests/benchmarks/bench_grep.tst');
// =============================================================================
// Bench 1
// =============================================================================
function time = bench_grep_1(runs)
  text = mgetl(SCI+"/modules/string/tests/unit_tests/text.txt");
  cumulate = 0;
  
  for i = 1:runs
    timer();
	  a = grep(text,"fonction");
	  timing = timer();
    cumulate = cumulate + timing;
  end
  time = cumulate / runs;
  clear('a'); clear('cumulate'); clear('text'); clear('i');
endfunction
// =============================================================================
runs = 100000;
printf("\nBenchs grep\n");
printf("Time bench 1 = %f seconds.\n",bench_grep_1(runs));
// =============================================================================