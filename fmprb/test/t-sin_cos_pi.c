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

    Copyright (C) 2012, 2013 Fredrik Johansson

******************************************************************************/

#include "fmprb.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("sin_cos_pi....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 1000; iter++)
    {
        fmprb_t a, b, c, d, e;
        long prec = 2 + n_randint(state, 200);

        fmprb_init(a);
        fmprb_init(b);
        fmprb_init(c);
        fmprb_init(d);
        fmprb_init(e);

        fmprb_randtest(a, state, 1 + n_randint(state, 200), 10);
        fmprb_randtest(b, state, 1 + n_randint(state, 200), 10);
        fmprb_randtest(c, state, 1 + n_randint(state, 200), 10);
        fmprb_randtest(d, state, 1 + n_randint(state, 200), 10);
        fmprb_randtest(e, state, 1 + n_randint(state, 200), 10);

        fmprb_const_pi(b, prec);
        fmprb_mul(b, b, a, prec);
        fmprb_sin_cos(b, d, b, prec);

        fmprb_sin_cos_pi(c, e, a, prec);

        if (!fmprb_overlaps(b, c) || !fmprb_overlaps(d, e))
        {
            printf("FAIL: overlap\n\n");
            printf("a = "); fmprb_print(a); printf("\n\n");
            printf("b = "); fmprb_print(b); printf("\n\n");
            printf("c = "); fmprb_print(c); printf("\n\n");
            printf("d = "); fmprb_print(d); printf("\n\n");
            printf("e = "); fmprb_print(e); printf("\n\n");
            abort();
        }

        fmprb_clear(a);
        fmprb_clear(b);
        fmprb_clear(c);
        fmprb_clear(d);
        fmprb_clear(e);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

