/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DISASSEMBLER_HXX__
#define __DISASSEMBLER_HXX__


namespace jit
{

    class Disassembler
    {

	const llvm::MCDisassembler;
	
    public:

	Disassembler(const Target & target, const std::string & triple, MCSubtargetInfo & STI)
	    {
		target.createMCDisassembler(STI);
		// TODO: on verra ca plus tard
		
	    }

	

int Disassembler::disassemble(const Target &T,
                              const std::string &Triple,
                              MCSubtargetInfo &STI,
                              MCStreamer &Streamer,
                              MemoryBuffer &Buffer,
                              SourceMgr &SM,
                              raw_ostream &Out) {
	
    };
	
}

#endif // __DISASSEMBLER_HXX__
