// exec("bug584.sci")



format('e',32);
result=logspace( 3, 3, 1 );

trueValue=1.0000000000000006821210263E+03;

if result == trueValue then ;

 affich_result(%T,584);

else
affich_result(%F,584);
end

format('v',10);