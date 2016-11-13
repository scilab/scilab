//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
mode(7)
//To quit this demo, enter CTRL+C, then type "abort"

//              INLINE DEFINITION OF FUNCTION
deff("[x]=fact(n)","if n==0 then x=1,else x=n*fact(n-1),end")
10+fact(5)
