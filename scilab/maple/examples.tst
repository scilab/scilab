// example 1
x=1

link('foof1.o','foof1');
getf('foof1.sci');
out=foof1(x)

link('fooc1.o','fooc1');
getf('fooc1.sci');
out=fooc1(x)

getf('foos1.sci')
out=foos1(x)

// example 2

link('foof2.o','foof2');
getf('foof2.sci');
out=foof2()

link('fooc2.o','fooc2');
getf('fooc2.sci');
out=fooc2()

getf('foos2.sci');
out=foos2()

// example 3
x=2

link('fooc3.o','fooc3');
getf('fooc3.sci');
out=fooc3(x)

link('foof3.o','foof3');
getf('foof3.sci');
out=foof3(x)

getf('foos3.sci');
out=foos3(x)

// example 4
x=1
y=2

link('foof4.o','foof4');
getf('foof4.sci');
out=foof4(x,y)

link('fooc4.o','fooc4');
getf('fooc4.sci');
out=fooc4(x,y)

getf('foos4.sci');
out=foos4(x,y)

// example 5
k=2
x=[1,2,3]

link('foof5.o','foof5');
getf('foof5.sci');
out=foof5(k,x)

link('fooc5.o','fooc5');
getf('fooc5.sci');
out=fooc5(k,x)

getf('foos5.sci');
out=foos5(k,x)

// example 6
k=2
x=[1,2,3;4,5,6]

link('foof6.o','foof6');
getf('foof6.sci');
out=foof6(k,x)

link('fooc6.o','fooc6');
getf('fooc6.sci');
out=fooc6(k,x)

getf('foos6.sci');
out=foos6(k,x)

// example 7
k=2
x=[1,2,3;4,5,6]

link('foof7.o','foof7');
getf('foof7.sci');
out=foof7(k,x)

link('fooc7.o','fooc7');
getf('fooc7.sci');
out=fooc7(k,x)

getf('foos7.sci');
out=foos7(k,x)

// example 8

link('foof8.o','foof8');
getf('foof8.sci');
out=foof8()

link('fooc8.o','fooc8');
getf('fooc8.sci');
out=fooc8()

getf('foos8.sci');
out=foos8()
