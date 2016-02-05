/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
//=============================================================================
using System;
using System.Collections.Generic;
using System.Text;
using DotNetScilab;
//=============================================================================
namespace example
{
    class cs_example
    {
        //=============================================================================
        /*
         * A small example to call scilab from C#
         * read & write matrix of double, string, boolean, int(32)
         */
        static void example_readwriteMatrixOfDouble(Scilab _objScilab)
        {
            //=============================================================================
            // Send a command to scilab
            // Here , we want to display SCI variable
            _objScilab.SendScilabJob("disp(\'SCI = \');");
            _objScilab.SendScilabJob("disp(SCI);");
            //=============================================================================
            double[] A = new double[] { 1, 2, 3, 4, 5, 6 };
            int mA = 2, nA = 3;

            // Write a matrix of double named in scilab
            _objScilab.createNamedMatrixOfDouble("A", mA, nA, A);

            // display matrix of double by scilab
            _objScilab.SendScilabJob("disp(\'A =\');");
            _objScilab.SendScilabJob("disp(A);");
            //=============================================================================
            if (_objScilab.getNamedVarType("A") == (int)DotNetScilab.ScilabType.sci_matrix)
            {
                Console.WriteLine("A is a matrix of double");
            }
            //=============================================================================
            _objScilab.SendScilabJob("B = A + 1;");

            // get dimensions of a named matrix of double
            int[] DimB = _objScilab.getNamedVarDimension("B");

            // get named matrix of double
            double[] B = _objScilab.readNamedMatrixOfDouble("B");

            // display matrix of double from C#
            Console.WriteLine("");
            Console.WriteLine("(C#) B =");
            for (int i = 0; i < DimB[0]; i++)
            {
                for (int j = 0; j < DimB[1]; j++)
                {
                    Console.Write(B[j * DimB[0] + i] + " ");
                }

                Console.WriteLine("");
            }

            // display matrix of double by scilab
            _objScilab.SendScilabJob("disp(\'B =\');");
            _objScilab.SendScilabJob("disp(B);");
            //=============================================================================
        }
        //=============================================================================
        static void example_readwriteMatrixOfString(Scilab _objScilab)
        {
            //=============================================================================
            string[] strA = new string[] { "Scilab", "The", "open",
                                        "source", "for", "numerical",
                                        "computation" , ":", ")"};
            int mstrA = 3, nstrA = 3;

            // Write a matrix of string named in scilab
            _objScilab.createNamedMatrixOfString("string_A", mstrA, nstrA, strA);

            // display matrix of string by scilab
            _objScilab.SendScilabJob("disp(\'string_A =\');");
            _objScilab.SendScilabJob("disp(string_A);");
            //=============================================================================
            if (_objScilab.getNamedVarType("string_A") == (int)DotNetScilab.ScilabType.sci_strings)
            {
                Console.WriteLine("string_A is a matrix of strings");
            }
            //=============================================================================
            _objScilab.SendScilabJob("string_B = convstr(string_A,\'u\');");

            // get dimensions of a named matrix of string
            int[] DimstrB = _objScilab.getNamedVarDimension("string_B");

            // get named matrix of string
            string[] strB = _objScilab.readNamedMatrixOfString("string_B");

            Console.WriteLine("");
            Console.WriteLine("(C#) strB =");
            for (int i = 0; i < DimstrB[0]; i++)
            {
                for (int j = 0; j < DimstrB[1]; j++)
                {
                    Console.Write(strB[j * DimstrB[0] + i] + " ");
                }

                Console.WriteLine("");
            }

            // display matrix of string by scilab
            _objScilab.SendScilabJob("disp(\'string_B =\');");
            _objScilab.SendScilabJob("disp(string_B);");
            //=============================================================================
        }
        //=============================================================================
        static void example_readwriteMatrixOfBoolean(Scilab _objScilab)
        {
            //=============================================================================
            Boolean[] bA = new Boolean[] { false, true, false,
                                           true, false, true};
            int mbA = 2, nbA = 3;

            // Write a matrix of string named in scilab
            _objScilab.createNamedMatrixOfBoolean("boolean_A", mbA, nbA, bA);

            // display matrix of string by scilab
            _objScilab.SendScilabJob("disp(\'boolean_A =\');");
            _objScilab.SendScilabJob("disp(boolean_A);");
            //=============================================================================
            // check if av
            if (_objScilab.existNamedVariable("boolean_A") == true)
            {
                Console.WriteLine("boolean_A exists in scilab");
            }

            if (_objScilab.existNamedVariable("boolean_B") == false)
            {
                Console.WriteLine("boolean_B does not exist in scilab");
            }
            //=============================================================================
            if (_objScilab.getNamedVarType("boolean_A") == (int)DotNetScilab.ScilabType.sci_boolean)
            {
                Console.WriteLine("boolean_A is a matrix of boolean");
            }
            //=============================================================================
            _objScilab.SendScilabJob("boolean_B = ~boolean_A;");
            // get dimensions of a named matrix of boolean
            int[] DimbB = _objScilab.getNamedVarDimension("boolean_B");

            // get named matrix of boolean
            Boolean[] bB = _objScilab.getNamedMatrixOfBoolean("boolean_B");

            Console.WriteLine("");
            Console.WriteLine("(C#) bB =");
            for (int i = 0; i < DimbB[0]; i++)
            {
                for (int j = 0; j < DimbB[1]; j++)
                {
                    Console.Write(bB[j * DimbB[0] + i] + " ");
                }

                Console.WriteLine("");
            }

            // display matrix of string by scilab
            _objScilab.SendScilabJob("disp(\'boolean_B =\');");
            _objScilab.SendScilabJob("disp(boolean_B);");
            //=============================================================================
        }
        //=============================================================================
        static void example_doplot3d(Scilab _objScilab)
        {
            _objScilab.SendScilabJob("plot3d()");
            while (_objScilab.HaveAGraph())
            {
                _objScilab.doEvent();
            }
        }
        //=============================================================================
        static void example_readwriteMatrixOfInt(Scilab _objScilab)
        {
            //=============================================================================
            int[] A = new int[] { 1, 2, 3, 4, 5, 6 };
            int mA = 2, nA = 3;

            // Write a matrix of double named in scilab
            _objScilab.createNamedMatrixOfInt32("int32_A", mA, nA, A);

            // display matrix of double by scilab
            _objScilab.SendScilabJob("disp(\'int32_A =\');");
            _objScilab.SendScilabJob("disp(int32_A);");
            //=============================================================================
            if (_objScilab.getNamedVarType("int32_A") == (int)DotNetScilab.ScilabType.sci_ints)
            {
                Console.WriteLine("int32_A is a matrix of int(32)");
            }
            //=============================================================================
            _objScilab.SendScilabJob("int32_B = int32_A + 1;");

            // get dimensions of a named matrix of double
            int[] DimB = _objScilab.getNamedVarDimension("int32_B");

            // get named matrix of double
            int[] B = _objScilab.readNamedMatrixOfInt32("int32_B");

            // display matrix of double from C#
            Console.WriteLine("");
            Console.WriteLine("(C#) int32_B =");
            for (int i = 0; i < DimB[0]; i++)
            {
                for (int j = 0; j < DimB[1]; j++)
                {
                    Console.Write(B[j * DimB[0] + i] + " ");
                }

                Console.WriteLine("");
            }

            // display matrix of double by scilab
            _objScilab.SendScilabJob("disp(\'int32_B =\');");
            _objScilab.SendScilabJob("disp(int32_B);");
            //=============================================================================
        }
        //=============================================================================
        static void example_readwriteComplexMatrixOfDouble(Scilab _objScilab)
        {
            //=============================================================================
            double[] realPartA = new double[] { 1, 2, 3, 4, 5, 6 };
            double[] imagPartA = new double[] { 6, 5, 4, 3, 2, 1 };
            int mA = 2, nA = 3;

            // Write a matrix of double named in scilab
            _objScilab.createNamedComplexMatrixOfDouble("cplx_A", mA, nA, realPartA, imagPartA);

            // display matrix of double by scilab
            _objScilab.SendScilabJob("disp(\'cplx_A =\');");
            _objScilab.SendScilabJob("disp(cplx_A);");
            //=============================================================================
            _objScilab.SendScilabJob("cplx_B = cplx_A * 2;");

            // get dimensions of a named matrix of double
            int[] DimB = _objScilab.getNamedVarDimension("cplx_B");

            // get named matrix of double
            double[] realPartB = _objScilab.readNamedComplexMatrixOfDoubleRealPart("cplx_B");
            double[] imagPartB = _objScilab.readNamedComplexMatrixOfDoubleImgPart("cplx_B");

            // display matrix of double from C#
            Console.WriteLine("");
            Console.WriteLine("(C#) cplx_B =");
            for (int i = 0; i < DimB[0]; i++)
            {
                for (int j = 0; j < DimB[1]; j++)
                {
                    Console.Write(realPartB[j * DimB[0] + i] + " + i *" + imagPartB[j * DimB[0] + i] + " ");
                }

                Console.WriteLine("");
            }

            // display matrix of double by scilab
            _objScilab.SendScilabJob("disp(\'cplx_B =\');");
            _objScilab.SendScilabJob("disp(cplx_B);");
            //=============================================================================
        }
        //=============================================================================
        static void Main(string[] args)
        {
            // start scilab engine with graphics
            Scilab m_oSCilab = new Scilab(true);

            example_readwriteMatrixOfDouble(m_oSCilab);

            example_readwriteMatrixOfString(m_oSCilab);

            example_readwriteMatrixOfBoolean(m_oSCilab);

            example_readwriteMatrixOfInt(m_oSCilab);

            example_readwriteComplexMatrixOfDouble(m_oSCilab);

            example_doplot3d(m_oSCilab);
        }
        //=============================================================================
    }
}
//=============================================================================
