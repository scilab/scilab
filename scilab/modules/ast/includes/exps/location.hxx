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

/**
** \file location.hxx
** Define the Location class.
*/

#ifndef __LOCATION_HXX__
#define __LOCATION_HXX__

#include <iostream>
#include <string>
#include <sstream>

/** \brief Abstract a Location. */
class Location
{
    /** \name Ctor & dtor.
    ** \{ */
public:
    /** \brief Construct a Location. */
    Location (void)
    {
        first_line = last_line = 0;
        first_column = last_column = 0;
    }
    /** \} */

    Location(int _first_line, int _last_line, int _first_column, int _last_column) : first_line(_first_line), first_column(_first_column), last_line(_last_line), last_column(_last_column)
    {
    }

    Location(const Location & l) : first_line(l.first_line), first_column(l.first_column), last_line(l.last_line), last_column(l.last_column)
    {
    }

    Location* clone()
    {
        Location* ret = new Location();
        ret->first_line = first_line;
        ret->first_column = first_column;
        ret->last_line = last_line;
        ret->last_column = last_column;
        return ret;
    }

    /** \name Line and Column related manipulators
    ** \{ */
public:
    /** \brief Reset initial location to final location. */
    inline void step (void)
    {
        first_line = last_line;
        first_column = last_column;
    }

    /** \brief Extend the current location to the COUNT next columns. */
    inline void columns (unsigned int count = 1)
    {
        last_column += count;
    }

    /** \brief Extend the current location to the COUNT next lines. */
    inline void lines (unsigned int count = 1)
    {
        last_line += count;
    }
    /** \} */

    inline bool operator<(const Location & loc) const
    {
        return first_line < loc.first_line || (first_line == loc.first_line && first_column < loc.first_column);
    }

    inline bool operator==(const Location & loc) const
    {
        return first_line == loc.first_line && first_column == loc.first_column && last_line == loc.last_line && last_column == loc.last_column;
    }

    inline bool operator!=(const Location & loc) const
    {
        return !(*this == loc);
    }

    std::wstring getLocationString() const
    {
        std::wostringstream os;
        os << L" (" << first_line << L"," << first_column << L")";
        return os.str();
    }

    friend std::wostream & operator<<(std::wostream & out, const Location & loc)
    {
        out << L"(" << loc.first_line << L"," << loc.first_column << L") to (" << loc.last_line << L"," << loc.last_column << L")";
        return out;
    }

public:
    /** \brief Beginning of the located region. */
    int first_line;
    int first_column;
    /** \brief End of the located region. */
    int last_line;
    int last_column;
};

#endif //! __LOCATION_HXX__
