// Bench 1
// =============================================================================

str_test_mat =  ["abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
				"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"; ..
				"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz", ..
				"abscefghijklmnopqrstuvxyz","abscefghijklmnopqrstuvxyz"];

timer();

for i=1:10000000
	ascii(str_test_mat);
end

duree = timer();

printf("\nDUREE 1 = %d seconds\n\n",duree);

// Bench 2
// =============================================================================

text = mgetl(SCI+"/modules/string/tests/benchmarks/ascii_text.txt");

timer();
for i=1:100000
	ascii(text);
end
duree = timer();

printf("\nDUREE 2 = %d seconds\n\n",duree);


// Bench 3
// =============================================================================

exec(SCI+"/modules/string/tests/benchmarks/ascii_code.txt");

timer();
for i=1:100000
	ascii(ref);
end
duree = timer();

printf("\nDUREE 3 = %d seconds\n\n",duree);
