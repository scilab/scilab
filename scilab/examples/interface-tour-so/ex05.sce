Bref =[ "Un","Trois","Cinq"; "Deux", "Quatre","Six" ];

// first example 

[A,B]=ex5c_1(string(1:10));
if A<>string(1:10) then pause,end
if B<>Bref then pause,end

// second example 

I= ex5c_2("Cinq",Bref) ;
if I<>5 then pause,end


