
// exec("bug623.sci");

 
 uno=int8(1);

   1==[uno uno];

  
if ans == [ %T %T ]  then


//affich_result(%T,623)
else
affich_result(%F,623)

end;
 

//lasterror()
 
iErrorCode=execstr("uint8(1) & %t","errcatch");
if iErrorCode then
//affich_result(%T,623);
else 
affich_result(%F,623);
 abort()
end

iErrorCode=execstr("int32(1) & %t","errcatch");
if iErrorCode then
//affich_result(%T,623);
else 
affich_result(%F,623);
 abort()
end

iErrorCode=execstr("uint32(1) & %t","errcatch");
if iErrorCode then
//affich_result(%T,623);
else 
affich_result(%F,623);
 abort()
end
 
iErrorCode=execstr("int16(1) & %t","errcatch");
if iErrorCode then
//affich_result(%T,623);
else 
affich_result(%F,623);
 abort()
end
 
iErrorCode=execstr("int8(1) & %f ","errcatch");
if iErrorCode then
affich_result(%T,623);
else 
affich_result(%F,623);
 abort()
end  
    


