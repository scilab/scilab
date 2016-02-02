/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#ifndef __DESERIALIZER_HXX__
#define __DESERIALIZER_HXX__

#include <list>
#include <all.hxx>

namespace ast
{
class DeserializeVisitor
{
private :

    unsigned char *initial_buf;
    unsigned char *buf;


    unsigned int get_uint8(void)
    {
        return *buf++;
    }

    unsigned int get_uint32(void)
    {
        unsigned int c0 = get_uint8();
        unsigned int c1 = get_uint8();
        unsigned int c2 = get_uint8();
        unsigned int c3 = get_uint8();
        return c0 + ((c1 + ((c2 + (c3 << 8)) << 8 )) << 8 );
    }

    unsigned long long get_uint64(void)
    {
        unsigned long long c0 = get_uint32();
        unsigned long long c1 = get_uint32();
        return c0 + (c1 << 32 );
    }

    int get_int32(void)
    {
        return (int)get_uint32();
    }

    bool get_bool(void)
    {
        return get_uint8() == 1;
    }

    Location get_location(void)
    {
        Location loc;
        loc.first_line = get_uint32();
        loc.first_column = get_uint32();
        loc.last_line = get_uint32();
        loc.last_column = get_uint32();
        return loc;
    }

    exps_t* get_exps(void)
    {
        int nitems = get_uint32();
        exps_t *list = new exps_t;
        for (int i = 0; i < nitems; i++)
        {
            Exp* exp = get_exp();
            list->push_back(exp);
        }
        return list;
    }

    exps_t* get_MatrixLines(void)
    {
        int nitems = get_uint32();
        exps_t* list = new exps_t;
        for (int i = 0; i < nitems; i++)
        {
            Location line_loc = get_location();
            MatrixLineExp* line = new MatrixLineExp(line_loc, *get_exps());
            list->push_back(line);
        }
        return list;
    }

    ast::exps_t* get_vars(void)
    {
        int nitems = get_uint32();
        ast::exps_t* list = new ast::exps_t;
        for (int i = 0; i < nitems; i++)
        {
            list->push_back(get_exp());
        }
        return list;
    }

    OpExp::Oper get_OpExp_Oper(void)
    {
        int code = get_uint8();
        switch (code)
        {
            case 1 :
                return  OpExp::plus;
            case 2 :
                return  OpExp::minus;
            case 3 :
                return  OpExp::times;
            case 4 :
                return  OpExp::rdivide;
            case 5 :
                return  OpExp::ldivide;
            case 6 :
                return  OpExp::power;

            case 7 :
                return  OpExp::dottimes;
            case 8 :
                return  OpExp::dotrdivide;
            case 9 :
                return  OpExp::dotldivide;
            case 10 :
                return  OpExp::dotpower;

            case 11 :
                return  OpExp::krontimes;
            case 12 :
                return  OpExp::kronrdivide;
            case 13 :
                return  OpExp::kronldivide;

            case 14 :
                return  OpExp::controltimes;
            case 15 :
                return  OpExp::controlrdivide;
            case 16 :
                return  OpExp::controlldivide;

            case 17 :
                return  OpExp::eq;
            case 18 :
                return  OpExp::ne;
            case 19 :
                return  OpExp::lt;
            case 20 :
                return  OpExp::le;
            case 21 :
                return  OpExp::gt;
            case 22 :
                return  OpExp::ge;

            case 23 :
                return  OpExp::unaryMinus;

            case 24 :
                return  OpExp::logicalAnd;
            case 25 :
                return  OpExp::logicalOr;
            case 26 :
                return  OpExp::logicalShortCutAnd;
            case 27 :
                return  OpExp::logicalShortCutOr;
        }
        std::cerr << "Unknown get_OpExp_Oper code " << code << std::endl;
        exit(2);
    }

    TransposeExp::Kind get_TransposeExp_Kind(void)
    {
        int code = get_uint8();
        switch (code)
        {
            case 1 :
                return TransposeExp::_Conjugate_;
            case 2 :
                return TransposeExp::_NonConjugate_;
        }
        std::cerr << "Unknown get_TransposeExp_Kind code " << code << std::endl;
        exit(2);
    }

    std::wstring* get_wstring(void)
    {
        unsigned int size = get_uint32();
        char* ss = (char*)buf;
        std::string s(ss, size / sizeof(char));
        wchar_t* ws = to_wide_string(s.data());
        std::wstring* w = new std::wstring(ws);
        FREE(ws);
        buf += size;
        return w;
    }

    symbol::Symbol* get_Symbol(void)
    {
        std::wstring* s = get_wstring();
        symbol::Symbol *sym = new symbol::Symbol(*s);
        delete s;
        return sym;
    }

    double get_double(void)
    {
        double d = *(double*)buf;
        buf += 8;
        return d;
    }

    VarDec* get_VarDec(Location & vardec_location)
    {
        symbol::Symbol *name = get_Symbol();
        Exp *init = get_exp();
        VarDec* vardec = new VarDec(vardec_location, *name, *init);
        delete name;
        return vardec;
    }


    Exp* get_exp(void)
    {
        Exp* exp;
        int code = get_uint8();
        size_t nodeNumber = get_uint64();
        Location loc = get_location();
        bool isVerbose = get_bool();

        switch (code)
        {
            case 1:
            {
                exps_t* l_body = get_exps();
                exp = new SeqExp(loc, *l_body);
                break;
            }
            case 2:
            {
                std::wstring* s = get_wstring();
                exp = new StringExp(loc, *s);
                delete s;
                break;
            }
            case 3:
            {
                std::wstring* s = get_wstring();
                exp = new CommentExp(loc, s);
                //delete s;
                break;
            }
            case 6:
            {
                double d = get_double();
                exp = new DoubleExp(loc, d);
                break;
            }
            case 7:
            {
                bool b = get_bool();
                exp = new BoolExp(loc, b);
                break;
            }
            case 8:
            {
                exp = new NilExp(loc);
                break;
            }
            case 9:
            {
                symbol::Symbol* sym = get_Symbol();
                exp = new SimpleVar(loc, *sym);
                delete sym;
                break;
            }
            case 10:
            {
                exp = new ColonVar(loc);
                break;
            }
            case 11:
            {
                exp = new DollarVar(loc);
                break;
            }
            case 12:
            {
                exp = new ArrayListVar(loc, *get_vars());
                break;
            }
            case 13:
            {
                Exp *head = get_exp();
                Exp *tail = get_exp();
                exp = new FieldExp(loc, *head, *tail);
                break;
            }
            case 14:
            {
                bool hasElse = get_bool();
                Exp* test = get_exp();
                Exp* _then = get_exp();
                IfExp* ifexp;
                if ( hasElse )
                {
                    Exp* _else = get_exp();
                    ifexp = new IfExp(loc, *test, *_then->getAs<SeqExp>(), *_else->getAs<SeqExp>());
                }
                else
                {
                    ifexp = new IfExp(loc, *test, *_then);
                }
                exp = ifexp;
                break;
            }
            case 15:
            {
                Location try_location = get_location();
                Location catch_location = get_location();
                exps_t* try_exps = get_exps();
                exps_t* catch_exps = get_exps();
                SeqExp *tryexp = new SeqExp(try_location, *try_exps);
                SeqExp *catchexp = new SeqExp(catch_location, *catch_exps);
                exp = new TryCatchExp(loc, *tryexp, *catchexp);
                break;
            }
            case 16:
            {
                Exp* test = get_exp();
                Exp* body = get_exp();
                exp = new WhileExp(loc, *test, *body->getAs<SeqExp>());
                break;
            }
            case 17:
            {
                Location vardec_location = get_location();
                VarDec* vardec = get_VarDec(vardec_location);
                Exp* body = get_exp();
                exp = new ForExp(loc, *vardec, *body->getAs<SeqExp>());
                break;
            }
            case 18:
            {
                exp = new BreakExp(loc);
                break;
            }
            case 19:
            {
                exp = new ContinueExp(loc);
                break;
            }
            case 20:
            {
                bool is_global = get_bool();
                if ( is_global )
                {
                    exp = new ReturnExp(loc);
                }
                else
                {
                    Exp* returnExp_exp = get_exp();
                    exp = new ReturnExp(loc, returnExp_exp);
                }
                break;
            }
            case 21:
            {
                bool has_default = get_bool();
                SeqExp * default_case = NULL;
                if ( has_default )
                {
                    Location default_case_location = get_location();
                    exps_t* default_case_exps = get_exps();
                    default_case = new SeqExp(default_case_location,
                                              *default_case_exps);
                }
                Exp* select = get_exp();

                int nitems = get_uint32();
                exps_t* cases = new  exps_t;
                for (int i = 0; i < nitems; i++)
                {

                    Location case_location = get_location();
                    Location body_location = get_location();
                    Exp* test = get_exp();
                    exps_t* body_exps = get_exps();
                    SeqExp *body = new SeqExp(body_location,  *body_exps);

                    CaseExp* _case = new CaseExp(case_location, *test, *body);
                    cases->push_back(_case);
                }


                if ( has_default )
                {
                    exp = new SelectExp(loc, *select, *cases, *default_case);
                }
                else
                {
                    exp = new SelectExp(loc, *select, *cases);
                }
                break;
            }
            /* SHOULD NEVER HAPPEN
            case 22: {
            exp = new CaseExp(*loc);
            break;
            }
            */
            case 23:
            {
                ast::exps_t* lines = get_MatrixLines();
                exp = new CellExp(loc, *lines);
                break;
            }
            case 24:
            {
                exps_t* exps = get_exps();
                exp = new ArrayListExp(loc, *exps);
                break;
            }
            case 25:
            {
                exps_t* exps = get_exps();
                exp = new AssignListExp(loc, *exps);
                break;
            }
            case 26:
            {
                Exp* notexp = get_exp();
                exp = new NotExp(loc, *notexp);
                break;
            }
            case 27:
            {
                TransposeExp::Kind kind = get_TransposeExp_Kind();
                Exp* _exp = get_exp();
                exp = new TransposeExp(loc, *_exp, kind);
                break;
            }
            case 28:
            {
                exp = get_VarDec(loc);
                break;
            }
            case 29:
            {
                symbol::Symbol* name = get_Symbol();
                Location args_loc = get_location();
                Location returns_loc = get_location();
                Exp* body = get_exp();
                exps_t* args_list = get_vars();
                exps_t* returns_list = get_vars();
                ArrayListVar *args = new ArrayListVar(args_loc, *args_list);
                ArrayListVar *returns = new ArrayListVar(returns_loc, *returns_list);
                exp = new FunctionDec(loc, *name, *args, *returns, *body->getAs<SeqExp>());
                delete name;
                break;
            }
            case 30:
            {
                Exp* _start = get_exp();
                Exp* _step = get_exp();
                Exp* _end = get_exp();
                exp = new ListExp(loc, *_start, *_step, *_end);
                break;
            }
            case 31:
            {
                Exp* _left = get_exp();
                Exp* _right = get_exp();
                exp = new AssignExp(loc, *_left, *_right);
                break;
            }
            case 32:
            {
                OpExp::Oper oper = get_OpExp_Oper();
                Exp *left = get_exp();
                Exp *right = get_exp();
                OpExp *_opexp  = new OpExp(loc, *left, oper, *right);
                exp = _opexp;
                break;
            }
            case 33:
            {
                OpExp::Oper oper = get_OpExp_Oper();
                Exp *left = get_exp();
                Exp *right = get_exp();
                LogicalOpExp *_opexp  =
                    new LogicalOpExp(loc, *left, oper, *right);
                exp = _opexp;
                break;
            }
            case 34:
            {
                exps_t* lines = get_MatrixLines();
                exp = new MatrixExp(loc, *lines);
                break;
            }
            case 35:
            {
                Exp* name = get_exp();
                exps_t* args = get_exps();
                exp = new CallExp(loc, *name, *args);
                break;
            }
            /* SHOULD NEVER HAPPEN
            case 36: {
            exp = new MatrixLineExp(*loc);
            break;
            }
            */
            case 37:
            {
                Exp* name = get_exp();
                exps_t* args = get_exps();
                exp = new CellCallExp(loc, *name, *args);
                break;
            }
            default:
                std::cerr << "Unknown code " << code << std::endl;
                exit(2);
        }

        exp->setVerbose(isVerbose);
        if (nodeNumber != 0)
        {
            exp->setNodeNumber(nodeNumber);
        }

        return exp;
    }

public :
    DeserializeVisitor(unsigned char* buffer) : initial_buf(buffer), buf(buffer) {};

    Exp* deserialize()
    {

        // scilabVersion and size unused
        // but get_uintxx used to deserialize macros
        /*unsigned int size = */get_uint32();
        // serialization version
        /*unsigned char scilabVersion[4];
        scilabVersion[0] = */
        get_uint8();
        /*scilabVersion[1] = */
        get_uint8();
        /*scilabVersion[2] = */
        get_uint8();
        /*scilabVersion[3] = */
        get_uint8();

        return get_exp();
    }
};
}
#endif /* !__DESERIALIZER_HXX__  */
