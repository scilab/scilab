/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#include "checkers/StructChecker.hxx"

namespace slint
{
void StructChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isCallExp())
    {
        const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
        if (ce.getName().isSimpleVar())
        {
            const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
            if (name == L"struct")
            {
                // we have a struct definition
                const ast::exps_t args = ce.getArgs();
                if (args.empty())
                {
                    result.report(context, e.getLocation(), *this, _("All the fields in a struct should be defined at the struct definition."));
                }
                else if (args.size() % 2)
                {
                    result.report(context, e.getLocation(), *this, _("Struct definition must contain an even number of arguments."));
                }
                else
                {
                    for (ast::exps_t::const_iterator i = args.begin(), end = args.end(); i != end; i += 2)
                    {
                        const ast::Exp & arg = **i;
                        if (arg.isStringExp())
                        {
                            const std::wstring & field = static_cast<const ast::StringExp &>(arg).getValue();
                            if (!matcher.match(field))
                            {
                                result.report(context, arg.getLocation(), *this, _("Field name doesn\'t match the pattern: %s, %s"), field, matcher.getPattern());
                            }

                            if (minimalLen > 0 && field.length() < minimalLen)
                            {
                                result.report(context, arg.getLocation(), *this, _("Field name length is lower than minimum: %d < %d"), field.length(), minimalLen);
                            }

                            if (maximalLen > 0 && field.length() > maximalLen)
                            {
                                result.report(context, arg.getLocation(), *this, _("Field name length is greater than maximum: %d > %d"), field.length(), minimalLen);
                            }
                            if (fields.find(field) != fields.end())
                            {
                                result.report(context, arg.getLocation(), *this, _("Duplicated field definition: %s."), field);
                            }
                            else
                            {
                                fields.emplace(field);
                            }
                        }
                        else
                        {
                            result.report(context, arg.getLocation(), *this, _("Field should be defined by a constant string."));
                        }
                    }

                    if (ce.getParent()->isAssignExp() && ce.getParent() != context.getLHSExp())
                    {
                        const ast::AssignExp & ae = *static_cast<const ast::AssignExp *>(ce.getParent());
                        if (ae.getLeftExp().isSimpleVar())
                        {
                            const std::wstring & Lname = static_cast<const ast::SimpleVar &>(ae.getLeftExp()).getSymbol().getName();
                            vars[Lname] = fields;
                        }
                        else if (ae.getLeftExp().isArrayListExp())
                        {
                            result.report(context, e.getLocation(), *this, _("Function struct has only one return argument."));
                        }
                    }
                    fields.clear();
                }
            }
        }
    }
    else if (e.isSimpleVar())
    {
        const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e);
        if (const ast::AssignExp * ae = context.getAssignExp())
        {
            if (&ae->getLeftExp() == &e && context.getRHSCallName() != L"struct")
            {
                vars.erase(var.getSymbol().getName());
            }
        }
    }
    else if (e.isFieldExp())
    {
        // a.b.c.d.e.f <==> ((((a.b).c).d).e).f
        const ast::FieldExp & fe = static_cast<const ast::FieldExp &>(e);
        if (fe.getHead()->isSimpleVar() && fe.getTail()->isSimpleVar())
        {
            const std::wstring & sname = static_cast<const ast::SimpleVar *>(fe.getHead())->getSymbol().getName();
            auto i = vars.find(sname);
            if (i != vars.end())
            {
                const std::unordered_set<std::wstring> & sfields = i->second;
                const std::wstring & fname = static_cast<const ast::SimpleVar *>(fe.getTail())->getSymbol().getName();
                if (sfields.find(fname) == sfields.end())
                {
                    if (&e == context.getLHSExp())
                    {
                        result.report(context, e.getLocation(), *this, _("It should be better to define field %s at struct definition."), fname);
                    }
                    else
                    {
                        result.report(context, e.getLocation(), *this, _("Read an undefined field: %s."), fname);
                    }
                }
            }
        }
    }
}

void StructChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string StructChecker::getName() const
{
    return "StructChecker";
}
}
