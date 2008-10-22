/*  Scicos
 *
 *  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * See the file ../license.txt
*/

package Modelica
package Math
  function sin
    input Real u;
    output Real y;
  external "builtin";
  end sin;

  function cos
    input Real u;
    output Real y;
  external "builtin";
  end cos;

  function tan
    input Real u;
    output Real y;
  external "builtin";
  end tan;

  function tanh
    input Real u;
    output Real y;
  external "builtin";
  end tanh;

  function exp
    input Real u;
    output Real y;
  external "builtin";
  end exp;

  function sqrt
    input Real u;
    output Real y;
  external "builtin";
  end sqrt;

  function log
    input Real u;
    output Real y;
  external "builtin";
  end log;

end Math;
end Modelica;

