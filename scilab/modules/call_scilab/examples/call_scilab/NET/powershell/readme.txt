Call scilab from powershell 
(http://wiki.scilab.org/Call_scilab_from_powershell)

A easy example to call scilab from powershell. 

you need : 

-- powershell 1.0 or 2.0 CTP 

-- scilab 5.2 

-- visual studio C# 2008 pro or express (to build donetscilab classlibrary) 

1] build donetscilab classlibrary 

launch SCI/modules/call_scilab/examples/call_scilab/NET/C#/dotnetsci.sln 

and build solution. 

2] configure your powershell environment 

launch powershell :
	$Scilab_bin_directory = "C:\Program Files\scilab-5.2\bin"
	$Env:PATH = $Env:PATH + ";" + $Scilab_bin_directory

3] load .NET classlibrary 
	$Fullpath ="$Scilab_bin_directory\DotNet-Component-Scilab.dll"
	[System.Reflection.Assembly]::LoadFile($Fullpath)
	
4] create a .NET scilab object 
	$objScilab = new-object dotnetscilab.Scilab
	
5] play with methods of scilab object 

send a job to scilab (here , display a string)  :
	$r = $objScilab.SendScilabJob("disp('Hello')")
	$r
	
creates a double matrix in scilab and get result in powershell :
	$r = $objScilab.SendScilabJob("A=ones(5,10)")
	$r

	$objScilab.SendScilabJob("disp(A)")
	$objScilab.readNamedMatrixOfDouble('A')

	$dimA = $objScilab.getNamedVarDimension('A')
	$dimA
	
creates a string in scilab and get result in powershell :
	$objScilab.SendScilabJob("strA = 'test'")
	$objScilab.readNamedMatrixOfString('strA')
	$objScilab.getNamedVarDimension('strA')
	
creates a string matrix in scilab and get result in powershell :
	$objScilab.SendScilabJob("strB =['test';'scilab']")
	$dimstrB = $objScilab.getNamedVarDimension('strB')
	$dimstrB

	$strB = $objScilab.readNamedMatrixOfString('strB')
	$strB
	
send an array of double from powershell, modify by scilab and get result :
	$matDouble = (5.,10.,5.,10.,5.,10.)
	$matDouble

	$objScilab.createNamedMatrixOfDouble('matDouble',3,2,$matDouble)

	$objScilab.SendScilabJob("matDouble = matDouble * 2")
	$objScilab.SendScilabJob("disp(matDouble)")

	$matDouble = $objScilab.readNamedMatrixOfDouble('matDouble')
	$matDouble
	
send an array of string from powershell, modify by scilab and get result :
	$matString = ('test' , 'scilab' , 'from' , 'powershell')
	$matString

	$objScilab.createNamedMatrixOfString('matString',2,2,$matString)
	$objScilab.SendScilabJob("matString = convstr(matString,'u')")
	$objScilab.SendScilabJob("disp(matString)")

	$matString = $objScilab.readNamedMatrixOfString('matString')
	$matString


Allan CORNET - DIGITEO - 2009

