/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
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

#ifndef ADAPTERS_UTILITIES_HXX_
#define ADAPTERS_UTILITIES_HXX_

enum startOrEnd
{
    Start = 0,
    End = 1
};

struct link_t
{
    int block;
    int port;
    enum startOrEnd kind;
};

// partial-link information
struct partial_link_t
{
    link_t from;
    link_t to;
};

typedef std::vector<unsigned int> link_indices_t;

// partial-port information
struct partial_ports_t
{
    link_indices_t graphics_pin;
    link_indices_t graphics_pout;
    link_indices_t graphics_pein;
    link_indices_t graphics_peout;
};

#endif /* ADAPTERS_UTILITIES_HXX_ */
