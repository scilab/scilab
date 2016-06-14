/*
 *  Scilab (http://www.scilab.org/) - This file is part of Scilab
 *  Copyright (C) 2012-2013 - OCAMLPRO INRIA - Fabrice LE FESSANT
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __SERIALIZER_HXX__
#define __SERIALIZER_HXX__

#include "dummyvisitor.hxx"
#include "deserializervisitor.hxx"
#include "timer.hxx"
#include "charEncoding.h"
#include "version.h"

#define FAGMENT_SIZE 65536

namespace ast
{
class SerializeVisitor : public DummyVisitor
{
private :
    Exp* ast;
    unsigned char *buf;
    int buflen;
    int bufsize;
    bool saveNodeNumber;
    bool saveLocation;

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
        if (saveLocation)
        {
            add_uint32(loc.first_line);
            add_uint32(loc.first_column);
            add_uint32(loc.last_line);
            add_uint32(loc.last_column);
        }
        else
        {
            add_uint32(0);
            add_uint32(0);
            add_uint32(0);
            add_uint32(0);
        }
    }
    void add_ast(unsigned int code, const Exp& e)
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
    }

    /** @{ Low-level append to the buffer functions */

    /* ensure that we have [size] bytes in the buffer */
    void need(int size)
    {
        if (bufsize - buflen < size)
        {
            bufsize = 2 * bufsize + size + FAGMENT_SIZE;
            unsigned char *newbuf = (unsigned char*) malloc(bufsize * sizeof(unsigned char));
            // std::cerr << "malloc " << (void*) newbuf << " " << bufsize << " " << (void*) buf << " " << buflen << std::endl;
            if (buflen > 0)
            {
                // std::cerr << "memcpy " << (void*) newbuf << " " << bufsize << " " << (void*) buf << " " << buflen << std::endl;
                memcpy(newbuf, buf, buflen);
            }
            if (buf != NULL)
            {
                // std::cerr << "free " << (void*) newbuf << " " << bufsize << " " << (void*) buf << " " << buflen << std::endl;
                free(buf);
            }
            else
            {
                buflen = 8;    /* Header length. Header =  final size of buf (4 bytes) + scilab version (4 bytes)*/
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
        char *c_str = wide_string_to_UTF8(w.c_str());
        int size = strlen(c_str);
        int final_size = size * sizeof(char);
        add_uint32(final_size);
        need(final_size);
        memcpy(buf + buflen, c_str, final_size);
        FREE(c_str);
        buflen += final_size;
    }

    /** @} */

    void add_exps(const exps_t& exps)
    {
        add_uint32((unsigned int)exps.size());
        if (exps.size() != 0)
        {
            for (auto exp : exps)
            {
                exp->getOriginal()->accept(*this);
            }
        }
    }

    void add_vars(const ArrayListVar& var)
    {
        exps_t vars = var.getVars();
        add_uint32((unsigned int)vars.size());
        for (exps_t::const_iterator it = vars.begin (), itEnd = vars.end(); it != itEnd; ++it)
        {
            (*it)->getOriginal()->accept(*this);
        }
    }

    void add_Symbol(const symbol::Symbol& e)
    {
        add_wstring(e.getName());
    }

    void add_exp(const Exp* e)
    {
        e->getOriginal()->accept(*this);
    }

    void add_exp(const Exp& e)
    {
        e.getOriginal()->accept(*this);
    }

    void add_OpExp_Oper(const OpExp::Oper oper)
    {
        int code = 253;
        switch (oper)
        {
            case OpExp::plus :
                code = (1);
                break;
            case OpExp::minus:
                code = (2);
                break;
            case OpExp::times:
                code = (3);
                break;
            case OpExp::rdivide:
                code = (4);
                break;
            case OpExp::ldivide:
                code = (5);
                break;
            case OpExp::power:
                code = (6);
                break;

            case OpExp::dottimes:
                code = (7);
                break;
            case OpExp::dotrdivide:
                code = (8);
                break;
            case OpExp::dotldivide:
                code = (9);
                break;
            case OpExp::dotpower:
                code = (10);
                break;

            case OpExp::krontimes:
                code = (11);
                break;
            case OpExp::kronrdivide:
                code = (12);
                break;
            case OpExp::kronldivide:
                code = (13);
                break;

            case OpExp::controltimes:
                code = (14);
                break;
            case OpExp::controlrdivide:
                code = (15);
                break;
            case OpExp::controlldivide:
                code = (16);
                break;

            case OpExp::eq:
                code = (17);
                break;
            case OpExp::ne:
                code = (18);
                break;
            case OpExp::lt:
                code = (19);
                break;
            case OpExp::le:
                code = (20);
                break;
            case OpExp::gt:
                code = (21);
                break;
            case OpExp::ge:
                code = (22);
                break;

            case OpExp::unaryMinus:
                code = (23);
                break;

            case OpExp::logicalAnd:
                code = (24);
                break;
            case OpExp::logicalOr:
                code = (25);
                break;
            case OpExp::logicalShortCutAnd:
                code = (26);
                break;
            case OpExp::logicalShortCutOr:
                code = (27);
                break;
        }
        add_uint8(code);
    }

    void add_TransposeExp_Kind(const TransposeExp::Kind kind)
    {
        int code = 249;
        switch (kind)
        {
            case TransposeExp::_Conjugate_ :
                code = (1);
                break;
            case TransposeExp::_NonConjugate_:
                code = (2);
                break;
        }
        add_uint8(code);
    }

    void add_bool(bool b)
    {
        add_uint8(b);
    }

    void add_varDec(const VarDec& varDec)
    {
        add_Symbol(varDec.getSymbol());
        add_exp(varDec.getInit());
    }

    void add_MatrixLines(const exps_t* lines)
    {
        add_uint32((unsigned int)lines->size());
        for (exps_t::const_iterator it = lines->begin(), itEnd = lines->end(); it != itEnd ; ++it)
        {
            add_location((*it)->getLocation());
            add_exps((*it)->getAs<MatrixLineExp>()->getColumns());
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
        if (hasElse)
        {
            add_exp(& e.getElse());
        }
    }
    void visit(const TryCatchExp& e)  /* done */
    {
        add_ast(15, e);
        add_location(e.getTry().getLocation());
        add_location(e.getCatch().getLocation());
        add_exps(e.getTry().getAs<SeqExp>()->getExps());
        add_exps(e.getCatch().getAs<SeqExp>()->getExps());
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
        add_varDec(*e.getAs<ForExp>()->getVardec().getAs<VarDec>());
        add_exp(&e.getBody());
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
        if (!is_global) /* otherwise exp is NULL */
        {
            add_exp(& e.getExp());
        }
    }
    void visit(const SelectExp& e)
    {
        add_ast(21, e);
        Exp *default_case = e.getDefaultCase();
        add_bool(e.hasDefault());
        if (e.hasDefault())
        {
            add_location(default_case->getLocation());
            add_exps(default_case->getAs<SeqExp>()->getExps());
        }
        add_exp(e.getSelect());

        exps_t cases = e.getCases();
        add_uint32((unsigned int)cases.size());

        for (auto exp : cases)
        {
            const CaseExp *ce = exp->getAs<CaseExp>();
            add_location(ce->getLocation());
            add_location(ce->getBody()->getLocation());
            add_exp(ce->getTest());
            add_exps(ce->getBody()->getAs<SeqExp>()->getExps());
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
        add_exp(e.getExp());
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
        add_vars(*e.getArgs().getAs<ArrayListVar>());
        add_vars(*e.getReturns().getAs<ArrayListVar>());
    }
    void visit(const ListExp& e)  /* done */
    {
        add_ast(30, e);
        add_exp(e.getStart());
        add_exp(e.getStep());
        add_exp(e.getEnd());
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
        e.getLeft().getOriginal()->accept(*this);
        e.getRight().getOriginal()->accept(*this);
    }
    void visit(const LogicalOpExp& e)  /* done */
    {
        add_ast(33, e);
        add_OpExp_Oper(e.getOper());
        e.getLeft().getOriginal()->accept(*this);
        e.getRight().getOriginal()->accept(*this);
    }
    void visit(const MatrixExp& e) /* done */
    {
        add_ast(34, e);
        add_MatrixLines(& e.getLines());
    }
    void visit(const CallExp& e)  /* done */
    {
        add_ast(35, e);
        add_exp(e.getName());
        ast::exps_t args = e.getArgs();
        add_exps(args);
    }
    void visit(const MatrixLineExp& e)  /* SHOULD NEVER HAPPEN */
    {
        add_ast(36, e);
    }
    void visit(const CellCallExp& e)  /* done */
    {
        add_ast(37, e);
        add_exp(e.getName());
        ast::exps_t args = e.getArgs();
        add_exps(args);
    }

    /* optimized */
    void visit(const OptimizedExp& e)
    {
        e.getOriginal()->accept(*this);
    }

    void visit(const MemfillExp& e)
    {
        e.getOriginal()->accept(*this);
    }

    void visit(const DAXPYExp& e)
    {
        e.getOriginal()->accept(*this);
    }

    void visit(const IntSelectExp& e)
    {
        e.getOriginal()->accept(*this);
    }

    void visit(const StringSelectExp& e)
    {
        e.getOriginal()->accept(*this);
    }

public :
    SerializeVisitor(Exp* _ast) : ast(_ast), buf(NULL), buflen(0), bufsize(0), saveNodeNumber(true), saveLocation(true) {}

    SerializeVisitor* clone()
    {
        return new SerializeVisitor(ast);
    }

    unsigned char* serialize(bool _saveNodeNumber = true, bool _saveLocation = true)
    {
        saveNodeNumber = _saveNodeNumber;
        saveLocation = _saveLocation;
        ast->getOriginal()->accept(*this);
        return get_buf();
    }
};
}

#endif /* !__SERIALIZER_HXX__ */
