// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
// Copyright (C) 2012 - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y = convert_to_float(code, binlen, maxbound, minbound)
    // Convert a binary string into a real.
    //
    // Calling Sequence
    //   y = convert_to_float(code, binlen, maxbound, minbound)
    //
    // Parameters
    //   code : a 1-by-1 matrix of strings, the binary code. The length of code must be binlen.
    //   binlen : a 1-by-1 matrix of doubles, integer value, positive, the number of bits in the binary string, i.e. the length of code.
    //   maxbound : a 1-by-d or d-by-1 matrix of doubles, the maximum bounds of the parameters
    //   minbound : a 1-by-d or d-by-1 matrix of doubles, the minimum bounds of the parameters
    //   y : a d-by-1 matrix of doubles, the real numbers corresponding to x.
    //
    // Description
    //   Convert the dimension d binary string encoded in code into the real x. 
	//   On output, the argument y is in the range [minbound,maxbound].
	//
	//   The bits in <literal>code</literal> are organized in d blocks of binlen bits. 
	//   In other words, the bits in <literal>code</literal> are the concatenation of 
	//   the bits of each dimensions.
	//
    // Examples
	// // Consider 1 bit of 8 bytes in dimension 1.
    // convert_to_float("00000000",8, 1, 0) // 0.
    // convert_to_float("11111111",8, 1, 0) // 1.
    // convert_to_float("11100111",8, 1, 0) // 0.9058824
	//
	// // In dimensions 2
	// code = "1110111001100011";
	// BinLen = 8;
	// Min = [-1;-1];
	// Max = [1;1];
	// y = convert_to_float(code, BinLen, Max, Min)
	// expected = [0.8666667;-0.2235294]
	// // See the concatenation:
	// // 11101110 : the first block in code
	// convert_to_float("11101110", BinLen, 1, -1)
	// // 01100011 : the second block in code
	// convert_to_float("01100011", BinLen, 1, -1)
	//
	// // Convert these reals into integers in the [0,s] range.
	// BinLen = 8;
	// s=2^BinLen-1;
    // s*convert_to_float("00000000",8, 1, 0) // 0.
    // s*convert_to_float("11111111",8, 1, 0) // 255.
    // s*convert_to_float("11100111",8, 1, 0) // 231.
    //

    Max_Bin = 2^binlen-1
    y       = [];
    for i=1:length(maxbound)
        dec = bin2dec(part(code,(i-1)*binlen+1:i*binlen))
        y(i) = (maxbound(i) - minbound(i)) * (dec / Max_Bin) + minbound(i)
    end
endfunction
