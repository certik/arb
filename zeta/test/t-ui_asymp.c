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

#include "zeta.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("ui_asymp....");
    fflush(stdout);
    flint_randinit(state);

    for (iter = 0; iter < 10000; iter++)
    {
        fmprb_t r;
        ulong n;
        mpfr_t s;
        long prec;

        prec = 2 + n_randint(state, 1 << n_randint(state, 10));

        fmprb_init(r);
        mpfr_init2(s, prec + 100);

        n = 2 + n_randint(state, 1 << n_randint(state, 10));

        zeta_ui_asymp(r, n, prec);
        mpfr_zeta_ui(s, n, MPFR_RNDN);

        if (!fmprb_contains_mpfr(r, s))
        {
            printf("FAIL: containment\n\n");
            printf("n = %lu\n\n", n);
            printf("r = "); fmprb_printd(r, prec / 3.33); printf("\n\n");
            printf("s = "); mpfr_printf("%.275Rf\n", s); printf("\n\n");
            abort();
        }

        fmprb_clear(r);
        mpfr_clear(s);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
