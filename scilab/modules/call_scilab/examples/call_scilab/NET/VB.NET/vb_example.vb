'
' * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
' * Copyright (C) 2009 - DIGITEO - Allan CORNET
' *
' * Copyright (C) 2012 - 2016 - Scilab Enterprises
' *
' * This file is hereby licensed under the terms of the GNU GPL v2.0,
' * pursuant to article 5.3.4 of the CeCILL v.2.1.
' * This file was originally licensed under the terms of the CeCILL v2.1,
' * and continues to be available under such terms.
' * For more information, see the COPYING file which you should have received
' * along with this program.
' *
'//=============================================================================

' See SCI/modules/call_scilab/examples/NET/C# about scilab .NET Component
' You need to add Scilab (C#) .NET Component as reference
' easier way to test is to launch SCI/bin/VB.NET.exe
' or VB.NET.exe must known libscilab.dll library path

Module vb_example
    '//=============================================================================
    Sub example()
        Dim ObjScilab As New DotNetScilab.Scilab

        ' Send a command to scilab
        ' Here , we want to display SCI variable
        ObjScilab.SendScilabJob("disp('SCI = ');")
        ObjScilab.SendScilabJob("disp(SCI)")

        Dim A() As Double = {1, 2, 3, 4, 5, 6}
        Dim mA As Long = 2
        Dim nA As Long = 3


        ObjScilab.createNamedMatrixOfDouble("A", mA, nA, A)
        ObjScilab.SendScilabJob("disp('A = ');")
        ObjScilab.SendScilabJob("disp(A)")

        ObjScilab.SendScilabJob("A = A + 1;")
        ObjScilab.SendScilabJob("disp('A modified = ');")
        ObjScilab.SendScilabJob("disp(A)")

        Dim A_modified() As Double
        A_modified = ObjScilab.readNamedMatrixOfDouble("A")

        Dim dimension_A_modified(2) As Integer
        dimension_A_modified = ObjScilab.getNamedVarDimension("A")

        Console.WriteLine("")
        Console.WriteLine("(C#) A modified =")

        Dim i As Integer = 0
        Dim j As Integer = 0
        Dim elem As Double = 0

        For i = 0 To dimension_A_modified(0) - 1
            For j = 0 To dimension_A_modified(1) - 1

                elem = A_modified(j * dimension_A_modified(0) + i)
                Console.Write(elem.ToString + " ")
            Next j
            Console.WriteLine("")
        Next i

    End Sub
    '//=============================================================================
    Sub Main()
        example()
    End Sub
    '//=============================================================================
End Module
'//=============================================================================

