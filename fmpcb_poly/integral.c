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

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "fmpcb_poly.h"

void
_fmpcb_poly_integral(fmpcb_ptr res, fmpcb_srcptr poly, long len, long prec)
{
    long k = len - 1;

    for (k = len - 1; k > 0; k--)
        fmpcb_div_ui(res + k, poly + k - 1, k, prec);

    fmpcb_zero(res);
}

void
fmpcb_poly_integral(fmpcb_poly_t res, const fmpcb_poly_t poly, long prec)
{
    fmpcb_poly_fit_length(res, poly->length + 1);
    _fmpcb_poly_integral(res->coeffs, poly->coeffs, poly->length + 1, prec);
    _fmpcb_poly_set_length(res, poly->length + 1);
    _fmpcb_poly_normalise(res);
}
