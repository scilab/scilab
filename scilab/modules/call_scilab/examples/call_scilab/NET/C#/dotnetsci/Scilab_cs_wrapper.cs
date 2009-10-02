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
using System.Runtime.InteropServices;
//=============================================================================
namespace DotNetScilab
{
    class Scilab_cs_wrapper
    {
        //=============================================================================
        [StructLayout(LayoutKind.Sequential)]
        public struct api_Err
        {
            int iErr;
            int iMsgCount;
            String pstMsg;
        }
        //=============================================================================
        private const string CALL_SCILAB_DLL = "call_scilab.dll";
        private const string API_SCILAB_DLL = "api_scilab.dll";
        private const string GRAPHICS_DLL = "graphics.dll";
        private const string OUTPUT_STREAM_DLL = "scioutput_stream.dll";
        //=============================================================================
        /// <summary>
        /// import SendScilabJob from C (see call_scilab.h)
        /// </summary>
        [DllImport(CALL_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public static extern int SendScilabJob([In]String job);
        //=============================================================================
        /// <summary>
        /// import StartScilab from C (see call_scilab.h)
        /// </summary>
        [DllImport(CALL_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public static extern int StartScilab([In] String SCIpath,
                                              [In] String ScilabStartup,
                                              [In] Int32[] Stacksize);
        //=============================================================================
        /// <summary>
        /// import TerminateScilab from C (see call_scilab.h)
        /// </summary>
        [DllImport(CALL_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public static extern int TerminateScilab([In] String ScilabQuit);
        //=============================================================================
        /// <summary>
        /// import DisableInteractiveMode from C (see call_scilab.h)
        /// </summary>
        [DllImport(CALL_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public static extern void DisableInteractiveMode();
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfString from C (see api_string.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public static extern int createNamedMatrixOfString([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfWideString from C (see api_string.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Unicode)]
        public static extern int createNamedMatrixOfWideString([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public static extern int createNamedMatrixOfDouble([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] double[] _pdblReal);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfBoolean from C (see api_boolean.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public static extern int createNamedMatrixOfBoolean([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] int[] _piBool);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfInteger32 from C (see api_int.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int createNamedMatrixOfInteger32([In] String _pstName,
                                                           [In] int _iRows, [In] int _iCols,
                                                           [In] int[] _piData);
        //=============================================================================
        /// <summary>
        /// import createNamedComplexMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int createNamedComplexMatrixOfDouble([In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] double[] _pdblReal,
                                                            [In] double[] _pdblImg);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfString from C (see api_string.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int readNamedMatrixOfString([In] String _pstName,
                                                          [Out]  Int32* _piRows, [Out]  Int32* _piCols,
                                                          [In, Out] int[] _piLength,
                                                          [In, Out] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfWideString from C (see api_string.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Unicode)]
        public unsafe static extern int readNamedMatrixOfWideString([In] String _pstName,
                                                          [Out]  Int32* _piRows, [Out]  Int32* _piCols,
                                                          [In, Out] int[] _piLength,
                                                          [In, Out] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int readNamedMatrixOfDouble([In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] Double[] _pdblReal);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfBoolean from C (see api_boolean.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int readNamedMatrixOfBoolean([In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] int[] _piBool);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfInteger32 from C (see api_int.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int readNamedMatrixOfInteger32([In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] int[] _piData);

        //=============================================================================
        /// <summary>
        /// import readNamedComplexMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int readNamedComplexMatrixOfDouble([In] String _pstName,
                                                        [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                        [In, Out] double[] _pdblReal,
                                                        [In, Out] double[] _pdblImg);
        //=============================================================================
        /// <summary>
        /// get Variable Adress in scilab stack from name
        /// used for getNamedMatrixType (internal)
        /// </summary>
        /// <param name="_pstName">variable name</param>
        /// <param name="_piAddress"> stack address</param>
        /// <returns>1 if ok</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int getVarAddressFromName([In] String _pstName,
                                                               [Out] Int32** _piAddress);
        //=============================================================================
        /// <summary>
        /// get Variable type in scilab stack from name
        /// </summary>
        /// <param name="_pstName">variable name</param>
        /// <returns>type or -1</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int getNamedVarType([In] String _pstName);
        //=============================================================================
        /// <summary>
        /// get variable type with adress in scilab stack
        /// used for getNamedMatrixType (internal)
        /// </summary>
        /// <param name="_piAddress"> stack address</param>
        /// <returns>scilab type, 0 fails</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int getVarType([In] Int32* _piAddress);
        //=============================================================================
        /// <summary>
        ///  Detect if a Scilab graphic window is opened
        /// </summary>
        /// <returns>0 (FALSE) or 1 (TRUE)</returns>
        [DllImport(GRAPHICS_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int sciHasFigures();
        //=============================================================================
        /// <summary>
        ///  get last error code
        /// </summary>
        /// <returns>last error code</returns>
        [DllImport(OUTPUT_STREAM_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int GetLastErrorCode();
        //=============================================================================
        /// <summary>
        ///  Get variable dimension
        /// </summary>
        /// import getNamedVarDimension from C (see api_common.h)
        /// <returns>int last error code</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int getNamedVarDimension([In] String _pstName,
                                   [Out] Int32* _piRows, [Out] Int32* _piCols);
        //=============================================================================
        /// <summary>
        ///  Get named complex information
        /// </summary>
        /// import isNamedVarComplex from C (see api_common.h)
        /// <returns>int if complex 1 otherwise 0</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int isNamedVarComplex([In] String _pstName);
        //=============================================================================

    }
}
//=============================================================================
