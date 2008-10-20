// exec("bug649.sci");

x=rand(5,5,100)+%i;
iCodeError= execstr('mtlb_save gain.mat x','errcatch' );

affich_result(%T,649)
