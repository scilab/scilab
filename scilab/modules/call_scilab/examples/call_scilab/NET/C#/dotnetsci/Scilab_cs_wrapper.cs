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
using System.Runtime.InteropServices;
//=============================================================================
namespace DotNetScilab
{
    class Scilab_cs_wrapper
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public unsafe struct api_Ctx  
        {
            public String pstName; /**< Function name */
        }
        //=============================================================================
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public unsafe struct api_Err
        {
            public int iErr;
            public int iMsgCount;
            public fixed int pstructMsg[5];
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
        public static extern api_Err createNamedMatrixOfString([In]IntPtr pvApiCtx, [In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfWideString from C (see api_string.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Unicode)]
        public static extern api_Err createNamedMatrixOfWideString([In]IntPtr pvApiCtx,
                                                            [In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        [return: MarshalAs(UnmanagedType.Struct)]
        public static extern api_Err createNamedMatrixOfDouble([In]IntPtr pvApiCtx, [In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] double[] _pdblReal);

        
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfBoolean from C (see api_boolean.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public static extern api_Err createNamedMatrixOfBoolean([In]IntPtr pvApiCtx, [In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] int[] _piBool);
        //=============================================================================
        /// <summary>
        /// import createNamedMatrixOfInteger32 from C (see api_int.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern api_Err createNamedMatrixOfInteger32([In]IntPtr pvApiCtx, [In] String _pstName,
                                                           [In] int _iRows, [In] int _iCols,
                                                           [In] int[] _piData);
        //=============================================================================
        /// <summary>
        /// import createNamedComplexMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern api_Err createNamedComplexMatrixOfDouble([In]IntPtr pvApiCtx, [In] String _pstName,
                                                            [In] int _iRows, [In] int _iCols,
                                                            [In] double[] _pdblReal,
                                                            [In] double[] _pdblImg);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfString from C (see api_string.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern api_Err readNamedMatrixOfString([In]IntPtr pvApiCtx, [In] String _pstName,
                                                          [Out]  Int32* _piRows, [Out]  Int32* _piCols,
                                                          [In, Out] int[] _piLength,
                                                          [In, Out] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfWideString from C (see api_string.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Unicode)]
        public unsafe static extern api_Err readNamedMatrixOfWideString([In]IntPtr pvApiCtx, [In] String _pstName,
                                                          [Out]  Int32* _piRows, [Out]  Int32* _piCols,
                                                          [In, Out] int[] _piLength,
                                                          [In, Out] String[] _pstStrings);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern api_Err readNamedMatrixOfDouble([In]IntPtr pvApiCtx, [In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] Double[] _pdblReal);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfBoolean from C (see api_boolean.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern api_Err readNamedMatrixOfBoolean([In]IntPtr pvApiCtx, [In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] int[] _piBool);
        //=============================================================================
        /// <summary>
        /// import readNamedMatrixOfInteger32 from C (see api_int.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern api_Err readNamedMatrixOfInteger32([In]IntPtr pvApiCtx, [In] String _pstName,
                                                          [Out] Int32* _piRows, [Out] Int32* _piCols,
                                                          [In, Out] int[] _piData);

        //=============================================================================
        /// <summary>
        /// import readNamedComplexMatrixOfDouble from C (see api_double.h)
        /// </summary>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern api_Err readNamedComplexMatrixOfDouble([In]IntPtr pvApiCtx, [In] String _pstName,
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
        public unsafe static extern api_Err getVarAddressFromName([In]IntPtr pvApiCtx, [In] String _pstName,
                                                               [Out] Int32** _piAddress);
        //=============================================================================
        /// <summary>
        /// get Variable type in scilab stack from name
        /// </summary>
        /// <param name="_pstName">variable name</param>
        /// <returns>type or -1</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern api_Err getNamedVarType([In]IntPtr pvApiCtx, [In] String _pstName, [Out]Int32* _piType);
        //=============================================================================
        /// <summary>
        /// get variable type with adress in scilab stack
        /// used for getNamedMatrixType (internal)
        /// </summary>
        /// <param name="_piAddress"> stack address</param>
        /// <returns>scilab type, 0 fails</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern api_Err getVarType([In]IntPtr pvApiCtx, [In] Int32* _piAddress, [Out]Int32* _piType);
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
        public unsafe static extern api_Err getNamedVarDimension([In]IntPtr pvApiCtx, [In] String _pstName,
                                   [Out] Int32* _piRows, [Out] Int32* _piCols);
        //=============================================================================
        /// <summary>
        ///  Get named complex information
        /// </summary>
        /// import isNamedVarComplex from C (see api_common.h)
        /// <returns>int if complex 1 otherwise 0</returns>
        [DllImport(API_SCILAB_DLL, CharSet = CharSet.Ansi)]
        public unsafe static extern int isNamedVarComplex([In]IntPtr pvApiCtx, [In] String _pstName);
        //=============================================================================

    }
}
//=============================================================================
