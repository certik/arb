/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "arb.h"

void
arb_log_ui(arb_t x, ulong n)
{
    mpfr_t t;

    if (n == 0)
    {
        printf("arb_log_ui: log(0)\n");
        abort();
    }

    if (n == 1)
    {
        arb_zero(x);
        return;
    }

    /* power of two */
    if (n > 2 && (n & (n-1UL)) == 0UL)
    {
        arb_log_ui(x, 2);
        arb_mul_si(x, x, FLINT_BIT_COUNT(n) - 1);
        return;
    }

    mpfr_init2(t, FLINT_MAX(arb_prec(x), FLINT_BITS));

    if (n == 2)
    {
        mpfr_const_log2(t, MPFR_RNDN);
    }
    else
    {
        mpfr_set_ui(t, n, MPFR_RNDN);  /* exact */
        mpfr_log(t, t, MPFR_RNDN);
    }

    arb_set_mpfr(x, t, 1);
}