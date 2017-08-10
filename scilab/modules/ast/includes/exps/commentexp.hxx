/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
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

#ifndef AST_COMMENTEXP_HXX
#define AST_COMMENTEXP_HXX

#include "constexp.hxx"

namespace ast
{
/** \brief Abstract an string Expression node.
**
** \b Example: string*/
class CommentExp : public ConstExp
{
public:
    CommentExp (const Location& location,
                std::wstring* comment)
        : ConstExp (location),
          _comment (comment)
    {
    }
    /** \brief Destroy an string Exp node.
    **
    ** Delete value (see constructor). */
    virtual ~CommentExp ()
    {
        delete _comment;
    }
    /** \} */

    virtual CommentExp* clone()
    {
        CommentExp* cloned = new CommentExp(getLocation(), new std::wstring(getComment()));
        cloned->setVerbose(isVerbose());
        return cloned;
    }

    virtual bool equal(const Exp & e) const
    {
        return e.getType() == COMMENTEXP;
    }

    /** \name Visitors entry point.
    ** \{ */
public:
    /** \brief Accept a const visitor \a v. */
    virtual void accept (Visitor& v)
    {
        v.visit (*this);
    }
    /** \brief Accept a non-const visitor \a v. */
    virtual void accept (ConstVisitor& v) const
    {
        v.visit (*this);
    }
    /** \} */


    /** \name Accessors.
    ** \{ */
public:
    /** \brief Return the comment (read only). */
    std::wstring &getComment() const
    {
        return *_comment;
    }
    /** \} */

    virtual ExpType getType() const
    {
        return COMMENTEXP;
    }
    inline bool isCommentExp() const
    {
        return true;
    }
protected:
    std::wstring* _comment;
};

} // namespace ast

#endif // !AST_COMMENTEXP_HXX
