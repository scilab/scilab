/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016 - Scilab Enterprises - Clement DAVID
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

extern "C" {
#include "dynlib_polynomials.h"
#include "machine.h"            /* C2F */

    POLYNOMIALS_IMPEXP int C2F(rpoly)(double* op, int* degree, double* zeror, double* zeroi, int* fail);
}

#include "find_polynomial_roots_jenkins_traub.h"
#include <Eigen/Core>

/**
 * finds the zeros of a real polynomial (compatible with the original rpoly.f)
 *
 * \param op  double precision vector of coefficients in
 *            order of decreasing powers.
 * \param degree integer degree of polynomial.
 * \param zeror real part of the zeros
 * \param zeroi imaginary part of the zeros
 * \param fail output parameter,
 *             2 if  leading coefficient is zero
 *             1 for non convergence or if rpoly has found fewer than degree
 *               zeros. In the latter case degree is reset to the number of
 *               zeros found.
 *             3 if degree>100
 */
POLYNOMIALS_IMPEXP int C2F(rpoly)(double* op, int* degree, double* zeror, double* zeroi, int* fail)
{
    if (*degree > 100)
    {
        *fail = 3;
        return 0;
    }

    // let's copy there as Map<VectorXd> is not supported yet on rpoly_plus_plus
    Eigen::Map<Eigen::VectorXd> mapped_op(op, *degree + 1);
    Eigen::Map<Eigen::VectorXd> mapped_zeror(zeror, *degree);
    Eigen::Map<Eigen::VectorXd> mapped_zeroi(zeroi, *degree);

    // reverse as the polynomials are used in different ordering
    Eigen::VectorXd polynomial(mapped_op);
    Eigen::VectorXd real_roots(*degree);
    Eigen::VectorXd complex_roots(*degree);

    bool valid = rpoly_plus_plus::FindPolynomialRootsJenkinsTraub(polynomial, &real_roots, &complex_roots);
    if (!valid)
    {
        *fail = 1;
        return 0;
    }

    mapped_zeror = real_roots;
    mapped_zeroi = complex_roots;

    *fail = 0;
    return 0;
}


