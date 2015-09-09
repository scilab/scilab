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

#ifndef __JIT_GATEWAY_CALL_HXX__
#define __JIT_GATEWAY_CALL_HXX__


#include "JITCall.hxx"

namespace jit
{
    
    class JITGatewayCall : public JITCall
    {
    
    public:
	
        JITGatewayCall() : JITCall() { }

	virtual bool invoke(const std::vector<analysis::TIType> & typesIn, const std::vector<JITScilabPtr> & in, const std::vector<analysis::TIType> & typesOut, const std::vector<JITScilabPtr> & out, JITVisitor & jit);

    };

} // namespace jit

#endif // __JIT_GATEWAY_CALL_HXX__
