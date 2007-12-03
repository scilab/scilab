// Bench 1
// =============================================================================

text = mgetl(SCI+"/modules/string/tests/unit_tests/text.txt");

timer();
for i=1:1000000
	grep(text,"fonction");
end
duree = timer();

printf("\nDUREE = %d seconds\n\n",duree);
