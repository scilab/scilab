/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012-2013 - OCAMLPRO INRIA - Fabrice LE FESSANT
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SERIALIZER_HXX__
#define __SERIALIZER_HXX__

#include "dummyvisitor.hxx"
#include "deserializervisitor.hxx"
#include "timer.hxx"
#include "version.h"

#define FAGMENT_SIZE 65536

namespace ast
{
class SerializeVisitor : public DummyVisitor
{
private :
    ast::Exp* ast;
    unsigned char *buf;
    int buflen;
    int bufsize;
    bool saveNodeNumber;

    unsigned char* get_buf(void)
    {
        // set buffer size
        set_uint32(0, buflen);
        // set scilab version
        set_version();
        return buf;
    }

    /** @{ Set the file header without modifying the file size. */

    void set_version()
    {
        set_byte(4, (unsigned char)SCI_VERSION_MAJOR);
        set_byte(5, (unsigned char)SCI_VERSION_MINOR);
        set_byte(6, (unsigned char)SCI_VERSION_MAINTENANCE);
        set_byte(7, (unsigned char)0);
    }

    void set_uint32(unsigned int pos, unsigned int n)
    {
        buf[pos++] = (n & 0xff);
        n >>= 8;
        buf[pos++] = (n & 0xff);
        n >>= 8;
        buf[pos++] = (n & 0xff);
        n >>= 8;
        buf[pos++] = (n & 0xff);
    }

    void set_byte(unsigned int pos, unsigned char n)
    {
        buf[pos] = n;
    }

    /** @} */

    void add_location(const Location& loc)
    {
        add_uint32(loc.first_line);
        add_uint32(loc.first_column);
        add_uint32(loc.last_line);
        add_uint32(loc.last_column);
    }
    void add_ast(unsigned int code, const ast::Exp& e)
    {
        add_uint8(code);
        if (saveNodeNumber)
        {
            add_uint64(e.getNodeNumber());
        }
        else
        {
            add_uint64((unsigned long long)0);
        }
        add_location(e.getLocation());
        add_uint8(e.isVerbose());
        add_uint8(e.isBreak());
        add_uint8(e.isBreakable());
        add_uint8(e.isReturn());
        add_uint8(e.isReturnable());
        add_uint8(e.isContinue());
        add_uint8(e.isContinuable());
    }

    /** @{ Low-level append to the buffer functions */

    /* ensure that we have [size] bytes in the buffer */
    void need(int size)
    {
        if ( bufsize - buflen < size )
        {
            bufsize = 2 * bufsize + size + FAGMENT_SIZE;
            unsigned char *newbuf = (unsigned char*) malloc(bufsize * sizeof(unsigned char));
            // std::cerr << "malloc " << (void*) newbuf << " " << bufsize << " " << (void*) buf << " " << buflen << std::endl;
            if ( buflen > 0 )
            {
                // std::cerr << "memcpy " << (void*) newbuf << " " << bufsize << " " << (void*) buf << " " << buflen << std::endl;
                memcpy(newbuf, buf, buflen);
            }
            if ( buf != NULL)
            {
                // std::cerr << "free " << (void*) newbuf << " " << bufsize << " " << (void*) buf << " " << buflen << std::endl;
                free(buf);
            }
            else
            {
                buflen = 8;    /* Header length. Header =  final size of buf ( 4 bytes ) + scilab version ( 4 bytes )*/
            }
            buf = newbuf;
        }

        // std::cerr << "need " << size << " " << bufsize << " " << (void*) buf << " " << buflen << std::endl;
    }

    void add_byte(unsigned char n)
    {
        buf[buflen++] = n;
    }

    void add_uint8(unsigned char n)
    {
        need(1);
        add_byte(n);
    }

    void add_uint32(unsigned int n)
    {
        need(4);
        add_byte(n & 0xff);
        add_byte((n >>= 8) & 0xff);
        add_byte((n >>= 8) & 0xff);
        add_byte((n >>= 8) & 0xff);
    }

    void add_uint64(unsigned long long n)
    {
        need(8);
        add_byte(n & 0xff);
        add_byte((n >>= 8) & 0xff);
        add_byte((n >>= 8) & 0xff);
        add_byte((n >>= 8) & 0xff);
        add_byte((n >>= 8) & 0xff);
        add_byte((n >>= 8) & 0xff);
        add_byte((n >>= 8) & 0xff);
        add_byte((n >>= 8) & 0xff);
    }

    void add_double(double d)
    {
        need(8);
        *(double*)(buf + buflen) = d;
        buflen += 8;
    }

    void add_wstring(const std::wstring &w)
    {
        int size = (int)w.size();
        const wchar_t *c_str = w.c_str();
        int final_size = size * sizeof(wchar_t);
        add_uint32(final_size);
        need(final_size);
        memcpy(buf + buflen, c_str, final_size);
        buflen += final_size;
    }

    /** @} */

    void add_exps(const std::list<Exp *> exps)
    {
        add_uint32((unsigned int)exps.size());
        std::list<Exp *>::const_iterator it;
        for (it = exps.begin() ; it != exps.end() ; it++)
        {
            (*it)->accept(*this);
        }
    }

    void add_vars(const ast::ArrayListVar& var)
    {
        add_uint32((unsigned int)var.getVars().size());
        std::list<Var *>::const_iterator it;
        for (it = var.getVars().begin() ; it != var.getVars().end() ; it++)
        {
            (*it)->accept(*this);
        }
    }

    void add_Symbol(const symbol::Symbol& e)
    {
        add_wstring(e.getName());
    }

    void add_exp(const ast::Exp* e)
    {
        e->accept(*this);
    }

    void add_exp(const ast::Exp& e)
    {
        e.accept(*this);
    }

    void add_OpExp_Oper(const ast::OpExp::Oper oper)
    {
        int code = 253;
        switch (oper)
        {
            case ast::OpExp::plus :
                code = (1);
                break;
            case ast::OpExp::minus:
                code = (2);
                break;
            case ast::OpExp::times:
                code = (3);
                break;
            case ast::OpExp::rdivide:
                code = (4);
                break;
            case ast::OpExp::ldivide:
                code = (5);
                break;
            case ast::OpExp::power:
                code = (6);
                break;

            case ast::OpExp::dottimes:
                code = (7);
                break;
            case ast::OpExp::dotrdivide:
                code = (8);
                break;
            case ast::OpExp::dotldivide:
                code = (9);
                break;
            case ast::OpExp::dotpower:
                code = (10);
                break;

            case ast::OpExp::krontimes:
                code = (11);
                break;
            case ast::OpExp::kronrdivide:
                code = (12);
                break;
            case ast::OpExp::kronldivide:
                code = (13);
                break;

            case ast::OpExp::controltimes:
                code = (14);
                break;
            case ast::OpExp::controlrdivide:
                code = (15);
                break;
            case ast::OpExp::controlldivide:
                code = (16);
                break;

            case ast::OpExp::eq:
                code = (17);
                break;
            case ast::OpExp::ne:
                code = (18);
                break;
            case ast::OpExp::lt:
                code = (19);
                break;
            case ast::OpExp::le:
                code = (20);
                break;
            case ast::OpExp::gt:
                code = (21);
                break;
            case ast::OpExp::ge:
                code = (22);
                break;

            case ast::OpExp::unaryMinus:
                code = (23);
                break;

            case ast::OpExp::logicalAnd:
                code = (24);
                break;
            case ast::OpExp::logicalOr:
                code = (25);
                break;
            case ast::OpExp::logicalShortCutAnd:
                code = (26);
                break;
            case ast::OpExp::logicalShortCutOr:
                code = (27);
                break;
        }
        add_uint8(code);
    }

    void add_TransposeExp_Kind(const ast::TransposeExp::Kind kind)
    {
        int code = 249;
        switch (kind)
        {
            case ast::TransposeExp::_Conjugate_ :
                code = (1);
                break;
            case ast::TransposeExp::_NonConjugate_:
                code = (2);
                break;
        }
        add_uint8(code);
    }

    void add_bool(bool b)
    {
        add_uint8(b);
    }

    void add_varDec(const ast::VarDec& varDec)
    {
        add_Symbol(varDec.getSymbol());
        add_exp(varDec.getInit());
    }

    void add_MatrixLines(const  std::list<ast::MatrixLineExp*> *lines)
    {
        add_uint32((unsigned int)lines->size());
        std::list<MatrixLineExp *>::const_iterator it;
        for (it = lines->begin() ; it != lines->end() ; it++)
        {
            add_location((*it)->getLocation());
            add_exps((*it)->getColumns());
        }
    }

    virtual void visit (const SeqExp &e)  /* done */
    {
        add_ast(1, e);
        add_exps(e.getExps());
    }
    void visit(const StringExp& e)  /* done */
    {
        add_ast(2, e);
        add_wstring(e.getValue());
    }
    void visit(const CommentExp& e)  /* done */
    {
        add_ast(3, e);
        add_wstring(e.getComment());
    }
    void visit(const DoubleExp& e)  /* done */
    {
        add_ast(6, e);
        add_double(e.getValue());
    }
    void visit(const BoolExp& e)  /* done */
    {
        add_ast(7, e);
        add_bool(e.getValue());
    }
    void visit(const NilExp& e)  /* done */
    {
        add_ast(8, e);
    }
    void visit(const SimpleVar& e)  /* done */
    {
        add_ast(9, e);
        add_Symbol(e.getSymbol());
    }
    void visit(const ColonVar& e)  /* done */
    {
        add_ast(10, e);
    }
    void visit(const DollarVar& e)  /* done */
    {
        add_ast(11, e);
    }
    void visit(const ArrayListVar& e)  /* done */
    {
        add_ast(12, e);
        add_vars(e);
    }
    void visit(const FieldExp& e)  /* done */
    {
        add_ast(13, e);
        add_exp(e.getHead());
        add_exp(e.getTail());
    }
    void visit(const IfExp& e)  /* done */
    {
        add_ast(14, e);
        bool hasElse = e.hasElse();
        add_bool(hasElse);
        add_exp(& e.getTest());
        add_exp(& e.getThen());
        if ( hasElse )
        {
            add_exp(& e.getElse());
        }
    }
    void visit(const TryCatchExp& e)  /* done */
    {
        add_ast(15, e);
        add_location(e.getTry().getLocation());
        add_location(e.getCatch().getLocation());
        add_exps(e.getTry().getExps());
        add_exps(e.getCatch().getExps());
    }
    void visit(const WhileExp& e)  /* done */
    {
        add_ast(16, e);
        add_exp(& e.getTest());
        add_exp(& e.getBody());
    }
    void visit(const ForExp& e)   /* done */
    {
        add_ast(17, e);
        add_location(e.getVardec().getLocation());
        add_varDec(e.getVardec());
        add_exp(& e.getBody());
    }
    void visit(const BreakExp& e)  /* done */
    {
        add_ast(18, e);
    }
    void visit(const ContinueExp& e)  /* done */
    {
        add_ast(19, e);
    }
    void visit(const ReturnExp& e)  /* done */
    {
        add_ast(20, e);
        bool is_global = e.isGlobal();
        add_bool(is_global);
        if ( !is_global ) /* otherwise exp is NULL */
        {
            add_exp(& e.getExp());
        }
    }
    void visit(const SelectExp& e)
    {
        add_ast(21, e);
        ast::SeqExp *default_case = e.getDefaultCase();
        bool has_default = default_case != NULL;
        add_bool( has_default );
        if ( has_default )
        {
            add_location(default_case->getLocation());
            add_exps(default_case->getExps());
        }
        add_exp(e.getSelect());

        add_uint32((unsigned int)e.getCases()->size());
        cases_t::const_iterator it;
        for (it = e.getCases()->begin() ; it != e.getCases()->end() ; it++)
        {
            const ast::CaseExp *ce = *it;
            add_location(ce->getLocation() );
            add_location(ce->getBody()->getLocation() );
            add_exp(ce->getTest());
            add_exps(ce->getBody()->getExps() );
        }
    }
    void visit(const CellExp& e)  /* done */
    {
        add_ast(23, e);
        add_MatrixLines(& e.getLines());
    }
    void visit(const ArrayListExp& e)  /* done */
    {
        add_ast(24, e);
        add_exps(e.getExps());
    }
    void visit(const AssignListExp& e)  /* done */
    {
        add_ast(25, e);
        add_exps(e.getExps());
    }
    void visit(const NotExp& e)  /* done */
    {
        add_ast(26, e);
        add_exp(e.getExp() );
    }
    void visit(const TransposeExp& e)  /* done */
    {
        add_ast(27, e);
        add_TransposeExp_Kind(e.getConjugate());
        add_exp(e.getExp());
    }
    void visit(const VarDec& e)
    {
        add_ast(28, e);
        add_varDec(e);
    }
    void visit(const FunctionDec& e)  /* done */
    {
        add_ast(29, e);
        add_Symbol(e.getSymbol());
        add_location(e.getArgs().getLocation());
        add_location(e.getReturns().getLocation());
        add_exp(e.getBody());
        add_vars(e.getArgs());
        add_vars(e.getReturns());
    }
    void visit(const ListExp& e)  /* done */
    {
        add_ast(30, e);
        add_exp(e.getStart() );
        add_exp(e.getStep() );
        add_exp(e.getEnd() );
    }
    void visit(const AssignExp& e)
    {
        add_ast(31, e);
        add_exp(e.getLeftExp());
        add_exp(e.getRightExp());
    }
    void visit(const OpExp& e)  /* done */
    {
        add_ast(32, e);
        add_OpExp_Oper(e.getOper());
        e.getLeft().accept(*this);
        e.getRight().accept(*this);
    }
    void visit(const LogicalOpExp& e)  /* done */
    {
        add_ast(33, e);
        add_OpExp_Oper(e.getOper());
        e.getLeft().accept(*this);
        e.getRight().accept(*this);
    }
    void visit(const MatrixExp& e) /* done */
    {
        add_ast(34, e);
        add_MatrixLines(& e.getLines());
    }
    void visit(const CallExp& e)  /* done */
    {
        add_ast(35, e);
        add_exp( e.getName());
        add_exps(e.getArgs());
    }
    void visit(const MatrixLineExp& e)  /* SHOULD NEVER HAPPEN */
    {
        add_ast(36, e);
    }
    void visit(const CellCallExp& e)  /* done */
    {
        add_ast(37, e);
        add_exp( e.getName());
        add_exps(e.getArgs());
    }

public :
    SerializeVisitor(ast::Exp* _ast) : ast(_ast), buf(NULL), buflen(0), bufsize(0), saveNodeNumber(true) {}

    unsigned char* serialize(bool _saveNodeNumber = true)
    {
        saveNodeNumber = _saveNodeNumber;
        ast->accept(*this);
        return get_buf();
    }
};
}

#endif /* !__SERIALIZER_HXX__ */
