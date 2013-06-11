/*
  Copyright (C) 2006  EDF - Code Saturne
  Copyright (C) 2001 - DIGITEO - Sylvestre LEDRU. Adapted for Scilab

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __BACKTRACE_H__
#define __BACKTRACE_H__

#include <stdarg.h>

/* BFT backtrace descriptor */

typedef struct _sci_backtrace_t sci_backtrace_t;

/* Pointers for backtrace print functions */

typedef void (sci_backtrace_print_t) (int start_depth);

/*============================================================================
 * Public function prototypes
 *============================================================================*/

/*
 * Build a backtrace description structure.
 *
 * @return pointer to sci_backtrace_t backtrace descriptor (NULL in case of
 *   error, or if backtracing is unavailable on this architecture).
 */

sci_backtrace_t *sci_backtrace_create(void);

/*
 * Free a backtrace description structure.
 *
 * @param bt pointer to backtrace description structure.
 * @return NULL pointer.
 */

sci_backtrace_t *sci_backtrace_destroy(sci_backtrace_t * bt);

/*
 * Demangle a backtrace description structure (for C++).
 *
 * @param bt pointer to backtrace description structure.
 */

void sci_backtrace_demangle(sci_backtrace_t * bt);

/*
 * Return the total depth of a backtrace.
 *
 * @param bt pointer to backtrace description structure.
 * @return total backtrace depth.
 */

int sci_backtrace_size(const sci_backtrace_t * bt);

/*
 * Return file name associated with a backtrace at a given depth.
 *
 * @param bt pointer to backtrace description structure.
 * @param depth index in backtrace structure (< sci_backtrace_size(bt)).
 * @return file name at the given depth, or NULL.
 */

const char *sci_backtrace_file(sci_backtrace_t * bt, int depth);

/*
 * Return function name associated with a backtrace at a given depth.
 *
 * @param bt pointer to backtrace description structure.
 * @param depth index in backtrace structure (< sci_backtrace_size(bt)).
 * @return function name at the given depth, or NULL.
 */

const char *sci_backtrace_function(sci_backtrace_t * bt, int depth);

/*
 * Return address associated with a backtrace at a given depth.
 *
 * @param bt pointer to backtrace description structure.
 * @param depth index in backtrace structure (< sci_backtrace_size(bt)).
 * @return address at the given depth, or NULL.
 */

const char *sci_backtrace_address(sci_backtrace_t * bt, int depth);

/*
 * Demangle a function name
 *
 * Have a look at the documentation of abi::__cxa_demangle
 */
char * sci_demangle(const char * __mangled_name, char * __output_buffer, size_t * __length, int * __status);

#endif /* __BACKTRACE_H__ */
