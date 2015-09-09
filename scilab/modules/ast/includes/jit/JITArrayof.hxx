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

#ifndef __JIT_ARRAYOF_HXX__
#define __JIT_ARRAYOF_HXX__

#include <cassert>

#include "internal.hxx"
#include "JITVisitor.hxx"
#include "JITScilabVal.hxx"

namespace jit
{
    class JITArrayof : public JITScilabVal
    {

    protected:
	
	llvm::Value * rows;
	llvm::Value * cols;
	llvm::Value * refCount;
	llvm::Value * data;

    public:

	JITArrayof() : data(nullptr) { }
	
	JITArrayof(llvm::Value * const _data, llvm::Value * const _rows, llvm::Value * const _cols, llvm::Value * const _refCount, const std::string & name) : rows(_rows), cols(_cols), refCount(_refCount), data(_data)
	    {
		if (!name.empty())
		{
		    rows->setName(name + "_r");
		    cols->setName(name + "_c");
		    refCount->setName(name + "_rc");
		    data->setName(name);
		}
	    }
	
	JITArrayof(JITVisitor & jit, llvm::Value * const _data, llvm::Value * const _rows, llvm::Value * const _cols, llvm::Value * const _refCount, const std::string & name) : JITArrayof(jit.getAlloca(_data), jit.getAlloca(_rows), jit.getAlloca(_cols), jit.getAlloca(_refCount), name) { }

	JITArrayof(JITVisitor & jit, llvm::Value * const _data, llvm::Value * const _rows, llvm::Value * const _cols, llvm::Value * const _refCount, const bool alloc, const std::string & name) : JITArrayof(jit.getValue(_data, alloc), jit.getValue(_rows, alloc), jit.getValue(_cols, alloc), jit.getValue(_refCount, alloc), name) { }

	bool isValid() const override
	    { 
		return data != nullptr;
	    }

	llvm::Value * loadRows(JITVisitor & jit) const override
	    {
		return jit.getBuilder().CreateAlignedLoad(rows, sizeof(int64_t));
	    }

	llvm::Value * loadCols(JITVisitor & jit) const override
	    {
		return jit.getBuilder().CreateAlignedLoad(cols, sizeof(int64_t));
	    }
	
	llvm::Value * getRows(JITVisitor & jit) const override
	    {
		return rows;
	    }

	llvm::Value * getCols(JITVisitor & jit) const override
	    {
		return cols;
	    }

	void setRefCount(llvm::Value * _refCount) override
	    {
		refCount = _refCount;
	    }
	
	void setData(llvm::Value * _data) override
	    {
		data = _data;
	    }

	void setRows(llvm::Value * _rows) override
	    {
		rows = _rows;
	    }

	void setCols(llvm::Value * _cols) override
	    {
		cols = _cols;
	    }

	void storeRows(JITVisitor & jit, llvm::Value * _rows) override
	    {
		jit.getBuilder().CreateAlignedStore(_rows, rows, sizeof(int64_t));
	    }

	void storeCols(JITVisitor & jit, llvm::Value * _cols) override
	    {
		jit.getBuilder().CreateAlignedStore(_cols, cols, sizeof(int64_t));
	    }

	bool isScalar() const override
	    {
		return false;
	    }

	llvm::Value * getData(JITVisitor & jit) const override
	    {
		return data;
	    }
	
	llvm::Value * loadData(JITVisitor & jit) override
	    {
		return jit.getBuilder().CreateAlignedLoad(data, sizeof(void *));
	    }
	
	void storeData(JITVisitor & jit, llvm::Value * _data) override
	    {
		jit.getBuilder().CreateAlignedStore(_data, data, sizeof(void *));
	    }

	/*types::InternalType::ScilabId getScilabId() const override
	    {

	    }

	void setScilabId(const types::InternalType::ScilabId id) override
	    {
		
	    }
	*/

	llvm::Value * getRefCount(JITVisitor & jit) const override
	    {
		return refCount;
		//return refCount.load(jit);
	    }
	
	llvm::Value * loadRefCount(JITVisitor & jit) const override
	    {
		return nullptr;
		//return refCount.load(jit);
	    }

	void incRefCount(JITVisitor & jit) override
	    {
		/*llvm::Value * rc_i64 = refCount.load(jit);
		llvm::Value * rcp1_i64 = jit.getBuilder().CreateNSWAdd(rc_i64, jit.getConstant<int64_t>(1));
		refCount.store(jit, rcp1_i64);*/
	    }
	
	void decRefCount(JITVisitor & jit) override
	    {
		/*llvm::Value * rc_i64 = refCount.load(jit);
		llvm::Value * rcp1_i64 = jit.getBuilder().CreateNSWSub(rc_i64, jit.getConstant<int64_t>(1));
		refCount.store(jit, rcp1_i64);*/
	    }
    };
}

#endif // __JIT_ARRAYOF_HXX__
