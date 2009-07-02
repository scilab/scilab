/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
//=============================================================================
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DotNetScilab;
//=============================================================================
namespace example
{
    class example
    {
        /*
         * A small example to call scilab from C#
         * read & write matrix of double and matrix of string
         */
        static void example_readwriteMatrixOfDouble(Scilab _objScilab)
        {
            //=============================================================================
            // Send a command to scilab
            // Here , we want to display SCI variable
            _objScilab.sendScilabJob("disp(\'SCI = \');");
            _objScilab.sendScilabJob("disp(SCI);");
            //=============================================================================
            double[] A = new double[] { 1, 2, 3, 4, 5, 6 };
            int mA = 2, nA = 3;

            // Write a matrix of double named in scilab
            _objScilab.setNamedMatrixOfDouble("A", mA, nA, A);

            // display matrix of double by scilab
            _objScilab.sendScilabJob("disp(\'A =\');");
            _objScilab.sendScilabJob("disp(A);");
            //=============================================================================
            _objScilab.sendScilabJob("B = A + 1;");

            // get dimensions of a named matrix of double
            int[] DimB = _objScilab.getNamedMatrixOfDoubleDimensions("B");

            // get named matrix of double
            double[] B = _objScilab.getNamedMatrixOfDouble("B");

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
            _objScilab.sendScilabJob("disp(\'B =\');");
            _objScilab.sendScilabJob("disp(B);");
            //=============================================================================
        }

        static void example_readwriteMatrixOfString(Scilab _objScilab)
        {
            //=============================================================================
            string[] strA = new string[] { "Scilab", "The", "open",
                                        "source", "for", "numerical",
                                        "computation" , ":", ")"};
            int mstrA = 3, nstrA = 3;

            // Write a matrix of string named in scilab
            _objScilab.setNamedMatrixOfString("string_A", mstrA, nstrA, strA);

            // display matrix of string by scilab
            _objScilab.sendScilabJob("disp(\'string_A =\');");
            _objScilab.sendScilabJob("disp(string_A);");
            //=============================================================================
            _objScilab.sendScilabJob("string_B = convstr(string_A,\'u\');");

            // get dimensions of a named matrix of string
            int[] DimstrB = _objScilab.getNamedMatrixOfStringDimensions("string_B");

            // get named matrix of string
            string[] strB = _objScilab.getNamedMatrixOfString("string_B");

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
            _objScilab.sendScilabJob("disp(\'string_B =\');");
            _objScilab.sendScilabJob("disp(string_B);");
            //=============================================================================
        }
        //=============================================================================
        static void Main(string[] args)
        {
            // start scilab engine
            Scilab m_oSCilab = new Scilab();

            example_readwriteMatrixOfDouble(m_oSCilab);
            example_readwriteMatrixOfString(m_oSCilab);

            // close scilab engine
            m_oSCilab.finish();
        }
        //=============================================================================
    }
}
//=============================================================================
