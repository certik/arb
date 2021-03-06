/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2011 William Hart
    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "fmprb_poly.h"

void
_fmprb_poly_product_roots(fmprb_ptr poly, fmprb_srcptr xs, long n, long prec)
{
    if (n == 0)
    {
        fmprb_one(poly);
    }
    else if (n == 1)
    {
        fmprb_neg(poly, xs);
        fmprb_one(poly + 1);
    }
    else if (n == 2)
    {
        fmprb_mul(poly, xs + 0, xs + 1, prec);
        fmprb_add(poly + 1, xs + 0, xs + 1, prec);
        fmprb_neg(poly + 1, poly + 1);
        fmprb_one(poly + 2);
    }
    else
    {
        const long m = (n + 1) / 2;
        fmprb_ptr tmp;

        tmp = _fmprb_vec_init(n + 2);

        _fmprb_poly_product_roots(tmp, xs, m, prec);
        _fmprb_poly_product_roots(tmp + m + 1, xs + m, n - m, prec);
        _fmprb_poly_mul_monic(poly, tmp, m + 1, tmp + m + 1, n - m + 1, prec);

        _fmprb_vec_clear(tmp, n + 2);
    }
}

void
fmprb_poly_product_roots(fmprb_poly_t poly, fmprb_srcptr xs, long n, long prec)
{
    fmprb_poly_fit_length(poly, n + 1);
    _fmprb_poly_product_roots(poly->coeffs, xs, n, prec);
    _fmprb_poly_set_length(poly, n + 1);
}
