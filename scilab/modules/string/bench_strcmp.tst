timer();

for i=1:10000000
	strcmp("abcdefghijkl","abcdefghijkl");
end

t1 = timer();

timer();

for i=1:10000000
	'abcdefghijkl' == 'abcdefghijkl';
end

t2 = timer();

printf(' T1 = %d sec \n",t1);
printf(' T2 = %d sec \n",t2);
