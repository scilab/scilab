// accessing a Scilab matrix inside an interface 
//----------------------------

param=1:10;
z=ex9c_1();
if norm(z-param) > %eps then pause,end

//--------------------------------------------------
// creation of a matrix: the variable C 
ex9c_2();
if C<>[10,20,30] then pause,end

//--------------------------------------------------
// accessing a Scilab string inside an interface 
Mystr='My string';
ex9c_3()

//--------------------------------------------------
// creation of a Scilab string in an interface 
ex9c_4();
if Str<> "Pilgrim said he, where can it be this land of Eldorado" then pause;end

// Fortran interfaces 
// accessing a Scilab matrix inside an interface 
//----------------------------

param=1:10;
z=ex9f_1();
if norm(z-param) > %eps then pause,end

//--------------------------------------------------
// creation of a matrix: the variable C 
ex9f_2();
if C<>[10,20,30] then pause,end

//--------------------------------------------------
// accessing a Scilab string inside an interface 
Mystr='My string';
ex9f_3()

//--------------------------------------------------
// creation of a Scilab string in an interface 
ex9f_4();
if Str<> "Pilgrim said he, where can it be this land of Eldorado" then pause;end


