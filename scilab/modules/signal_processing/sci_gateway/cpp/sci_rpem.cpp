/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
/*--------------------------------------------------------------------------*/

#include "signal_gw.hxx"
#include "list.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"


    extern void C2F(rpem)(double theta[], double p[],
                          int *order,
                          double *u, double *y, double *lambda, double *k, double *c,
                          int *istab2,
                          double *v, double *eps, double *eps1,
                          int *dimension,
                          double phi[], double psi[], double tstab[],
                          double work[], double f[], double g[], double l[]);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_rpem(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double* data    = NULL;
    double* u       = NULL;
    double* y       = NULL;
    double* tstab   = NULL;
    double* work    = NULL;
    double* f       = NULL;
    double* g       = NULL;

    double v        = 0;
    double eps      = 0;
    double eps1     = 0;

    double lambda   = 0.950l;
    double alpha    = 0.990l;
    double beta     = 0.01l;
    double kappa    = 0.000l;
    double mu       = 0.980l;
    double nu       = 0.020l;
    double c        = 1000.0l;

    int order       = 0;
    int dimension   = 0;
    int istab2      = 0;
    int u_length    = 0;

    types::Double* dTheta   = NULL;
    types::Double* dP       = NULL;
    types::Double* dPhi     = NULL;
    types::Double* dPsi     = NULL;
    types::Double* dL       = NULL;

    if (in.size() < 3 || in.size() > 6)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "rpem", 3, 6);
        return types::Function::Error;
    }

    /* arg1: w0 = list(theta, p, l, phi, psi) */
    if ((in[0]->isList() == false) || in[0]->getAs<types::List>()->getSize() != 5)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %d-element list expected.\n"), "rpem", 1, 5);
        return types::Function::Error;
    }

    types::List* w0 = in[0]->getAs<types::List>();
    for (int i = 0; i < 5; i++)
    {
        if (!w0->get(i)->isDouble() || w0->get(i)->getAs<types::Double>()->isComplex())
        {
            Scierror(77, _("%s: Wrong type for element %d of input argument #%d: A matrix of real expected.\n"), "rpem", i + 1, 1);
            return types::Function::Error;
        }
        types::Double* current = w0->get(i)->getAs<types::Double>();
        switch (i)
        {
            case 0:  /* theta: 3n real ranged row vector */
            {
                if (current->getRows() != 1)
                {
                    Scierror(77, _("%s: Wrong size for element %d of input argument #%d: A row vector expected.\n"), "rpem", i + 1, 1);
                    return types::Function::Error;
                }
                if (current->getCols() % 3 != 0)
                {
                    Scierror(77, _("%s: Wrong size for element %d of input argument #%d: Size must be multiple of %d.\n"), "rpem", i + 1, 1, 3);
                    return types::Function::Error;
                }
                dimension = current->getCols();
                order = dimension / 3;
                dTheta = new types::Double(1, dimension);
                dTheta->set(current->get());
            }
            break;
            case 1:  /* p: 3n x 3n real ranged matrix */
            {
                if (current->getRows() != dimension || current->getCols() != dimension)
                {
                    Scierror(77, _("%s: Wrong size for element %d of input argument #%d: A square matrix expected.\n"), "rpem", i + 1, 1);
                    return types::Function::Error;
                }
                dP = new types::Double(dimension, dimension);
                dP->set(current->get());
            }
            break;
            case 2:  /* l: 3n real ranged row vector */
            case 3:  /* phi: 3n real ranged row vector */
            case 4:  /* psi: 3n real ranged row vector */
            {
                if (current->getRows() != 1 || current->getCols() != dimension)
                {
                    Scierror(77, _("%s: Wrong size for element %d of input argument #%d: Same sizes of element %d expected.\n"), "rpem", i + 1, 1, 1);
                    return types::Function::Error;
                }
            }
        }
    }

    dL      = new types::Double(1, dimension);
    dPhi    = new types::Double(1, dimension);
    dPsi    = new types::Double(1, dimension);

    dL->set(w0->get(2)->getAs<types::Double>()->get());
    dPhi->set(w0->get(3)->getAs<types::Double>()->get());
    dPsi->set(w0->get(4)->getAs<types::Double>()->get());

    /* arg2: u0: real ranged row vector */
    if ((in[1]->isDouble() == false) || in[1]->getAs<types::Double>()->getRows() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), "rpem", 2);
        return types::Function::Error;
    }
    u = in[1]->getAs<types::Double>()->get();
    u_length = in[1]->getAs<types::Double>()->getCols();

    /* arg3: y0: real ranged row vector of same length as u0 */
    if ((in[2]->isDouble() == false) || in[2]->getAs<types::Double>()->getRows() != 1)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A row vector expected.\n"), "rpem", 3);
        return types::Function::Error;
    }
    if (in[2]->getAs<types::Double>()->getCols() != u_length)
    {
        Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"), "rpem", 2, 3);
        return types::Function::Error;
    }
    y = in[2]->getAs<types::Double>()->get();

    /* optional arguments */
    switch (in.size())
    {
        case 6: /* c */
        {
            if ((in[5]->isDouble() == false) || !in[5]->getAs<types::Double>()->isScalar())
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "rpem", 6);
                return types::Function::Error;
            }
            c = in[5]->getAs<types::Double>()->get(0);
        }
        case 5: /* [kappa, mu, nu] */
        {
            if ((in[4]->isDouble() == false) || (in[4]->getAs<types::Double>()->getRows() != 1) || (in[4]->getAs<types::Double>()->getCols() != 3))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), "rpem", 5, 1, 3);
                return types::Function::Error;
            }
            data    = in[4]->getAs<types::Double>()->get();
            kappa   = data[0];
            mu      = data[1];
            nu      = data[2];
        }
        case 4: /* [lambda, alpha, beta] */
        {
            if ((in[3]->isDouble() == false) || (in[3]->getAs<types::Double>()->getRows() != 1) || (in[3]->getAs<types::Double>()->getCols() != 3))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A %d-by-%d matrix expected.\n"), "rpem", 4, 1, 3);
                return types::Function::Error;
            }
            data    = in[3]->getAs<types::Double>()->get();
            lambda  = data[0];
            alpha   = data[1];
            beta    = data[2];
        }
    }

    /*** algorithm call ***/

    /* references provided to justify allocation with code relying on it */
    f = (double *) MALLOC((dimension) * sizeof(double));        /* rpem.f l.168 */
    memset(f, 0x00, (dimension) * sizeof(double));
    g = (double *) MALLOC((dimension) * sizeof(double));        /* rpem.f l.169 */
    memset(g, 0x00, (dimension) * sizeof(double));
    tstab = (double *) MALLOC((order + 1) * sizeof(double));    /* rpem.f l.105 */
    memset(tstab, 0x00, (order + 1) * sizeof(double));
    work = (double *) MALLOC((2 * order + 2) * sizeof(double)); /* nstabl.f */
    memset(work, 0x00, (2 * order + 2) * sizeof(double));
    /* (tip: bound variables to determine required memory: nk1 <= ordre + 1) */

    for (int i = 1 ; i < u_length ; ++i)
    {
        C2F(rpem)(  dTheta->get(), dP->get(), &order, &(u[i - 1]), &(y[i]), &lambda, &kappa, &c,
                    &istab2, &v, &eps, &eps1, //output
                    &dimension, dPhi->get(), dPsi->get(),
                    tstab, work, //output
                    f, g, dL->get());

        lambda = alpha * lambda + beta;
        kappa = mu * kappa + nu;
    }

    FREE(work);
    FREE(tstab);
    FREE(g);
    FREE(f);

    /*** output formatting ***/

    types::List* resultList = new types::List();
    resultList->append(dTheta);

    resultList->append(dP);
    resultList->append(dL);
    resultList->append(dPhi);
    resultList->append(dPsi);

    out.push_back(resultList);

    if (_iRetCount == 2)
    {
        types::Double* dV = new types::Double(1, 1);
        dV->set(&v);
        out.push_back(dV);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

