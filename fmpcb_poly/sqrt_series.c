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

    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "fmpcb_poly.h"

void
_fmpcb_poly_sqrt_series(fmpcb_ptr g,
    fmpcb_srcptr h, long hlen, long len, long prec)
{
    hlen = FLINT_MIN(hlen, len);

    if (hlen == 1)
    {
        fmpcb_sqrt(g, h, prec);
        _fmpcb_vec_zero(g + 1, len - 1);
    }
    else if (len == 2)
    {
        fmpcb_sqrt(g, h, prec);
        fmpcb_div(g + 1, h + 1, h, prec);
        fmpcb_mul(g + 1, g + 1, g, prec);
        fmpcb_mul_2exp_si(g + 1, g + 1, -1);
    }
    else
    {
        fmpcb_ptr t;
        t = _fmpcb_vec_init(len);
        _fmpcb_poly_rsqrt_series(t, h, hlen, len, prec);
        _fmpcb_poly_mullow(g, t, len, h, hlen, len, prec);
        _fmpcb_vec_clear(t, len);
    }
}

void
fmpcb_poly_sqrt_series(fmpcb_poly_t g, const fmpcb_poly_t h, long n, long prec)
{
    if (n == 0)
    {
        fmpcb_poly_zero(g);
        return;
    }

    if (g == h)
    {
        fmpcb_poly_t t;
        fmpcb_poly_init(t);
        fmpcb_poly_sqrt_series(t, h, n, prec);
        fmpcb_poly_swap(g, t);
        fmpcb_poly_clear(t);
        return;
    }

    fmpcb_poly_fit_length(g, n);
    if (h->length == 0)
        _fmpcb_vec_indeterminate(g->coeffs, n);
    else
        _fmpcb_poly_sqrt_series(g->coeffs, h->coeffs, h->length, n, prec);
    _fmpcb_poly_set_length(g, n);
    _fmpcb_poly_normalise(g);
}

