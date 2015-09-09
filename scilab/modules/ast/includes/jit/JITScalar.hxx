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

#ifndef __JIT_SCALAR_HXX__
#define __JIT_SCALAR_HXX__

#include "JITVisitor.hxx"
#include "JITScilabVal.hxx"
#include "Cast.hxx"

namespace jit
{
    class JITScalar : public JITScilabVal
    {

    protected:
	
        llvm::Value * data;
        bool allocated;

    public:

	JITScalar() : data(nullptr) { }
	
        JITScalar(llvm::Value * _data, const bool _allocated, const std::string & name) : data(_data), allocated(_allocated)
            {
		if (!name.empty())
		{
		    data->setName(name);
		}
	    }

	bool isValid() const override
	    { 
		return data != nullptr;
	    }
	
        llvm::Value * loadRows(JITVisitor & jit) const override
            {
                return nullptr;
            }

        llvm::Value * loadCols(JITVisitor & jit) const override
            {
                return nullptr;
            }

        llvm::Value * getRows(JITVisitor & jit) const override
            {
                return nullptr;
            }

        llvm::Value * getCols(JITVisitor & jit) const override
            {
                return nullptr;
            }

	void setRefCount(llvm::Value * _refCount) override
	    {
	    }
	
	void setData(llvm::Value * _data) override
	    {
		data = _data;
	    }

	void setRows(llvm::Value * _rows) override
	    {
	    }

	void setCols(llvm::Value * _cols) override
	    {
	    }

        void storeRows(JITVisitor & jit, llvm::Value * rows) override
            {

            }

        void storeCols(JITVisitor & jit, llvm::Value * cols) override
            {

            }

        bool isScalar() const override
            {
                return true;
            }

        llvm::Value * loadData(JITVisitor & jit) override
            {
                if (allocated)
                {
                    return jit.getBuilder().CreateAlignedLoad(data, jit.getTySizeInBytes(data));
                }
                else
                {
                    return data;
                }
            }

        llvm::Value * getData(JITVisitor & jit) const override
            {
                return data;
            }

        void storeData(JITVisitor & jit, llvm::Value * _data) override
            {
                if (allocated)
                {
                    jit.getBuilder().CreateAlignedStore(_data, data, jit.getTySizeInBytes(data));
                }
                else
                {
                    assert(false && "storeData mustn't be called");
                }
            }

        /*types::InternalType::ScilabId getScilabId() const override
          {

          }

          void setScilabId(const types::InternalType::ScilabId id) override
          {

          }
        */

        llvm::Value * loadRefCount(JITVisitor & jit) const override
            {
                return nullptr;
            }

        llvm::Value * getRefCount(JITVisitor & jit) const override
            {
                return nullptr;
            }

        void incRefCount(JITVisitor & jit) override
            {

            }

        void decRefCount(JITVisitor & jit) override
            {

            }
    };
}

#endif // __JIT_SCALAR_HXX__
