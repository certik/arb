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


int main()
{
    long iter;
    flint_rand_t state;

    printf("div_series....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000; iter++)
    {
        long m, n, p, qbits, rbits1, rbits2;
        fmpq_poly_t A, B, C;
        fmpcb_poly_t a, b, c, d;

        qbits = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);

        m = 1 + n_randint(state, 20);
        n = 1 + n_randint(state, 20);
        p = 1 + n_randint(state, 20);

        fmpq_poly_init(A);
        fmpq_poly_init(B);
        fmpq_poly_init(C);

        fmpcb_poly_init(a);
        fmpcb_poly_init(b);
        fmpcb_poly_init(c);
        fmpcb_poly_init(d);

        fmpq_poly_randtest(A, state, m, qbits);

        do {
            fmpq_poly_randtest_not_zero(B, state, n, qbits);
        } while (B->coeffs[0] == 0);

        fmpq_poly_div_series(C, A, B, p);
        fmpcb_poly_set_fmpq_poly(a, A, rbits1);
        fmpcb_poly_set_fmpq_poly(b, B, rbits1);
        fmpcb_poly_div_series(c, a, b, p, rbits2);

        if (!fmpcb_poly_contains_fmpq_poly(c, C))
        {
            printf("FAIL\n\n");
            printf("bits2 = %ld\n", rbits2);

            printf("A = "); fmpq_poly_print(A); printf("\n\n");
            printf("B = "); fmpq_poly_print(B); printf("\n\n");
            printf("C = "); fmpq_poly_print(C); printf("\n\n");

            printf("a = "); fmpcb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmpcb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmpcb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        fmpcb_poly_set(d, a);
        fmpcb_poly_div_series(d, d, b, p, rbits2);
        if (!fmpcb_poly_equal(d, c))
        {
            printf("FAIL (aliasing 1)\n\n");
            abort();
        }

        fmpcb_poly_set(d, b);
        fmpcb_poly_div_series(d, a, d, p, rbits2);
        if (!fmpcb_poly_equal(d, c))
        {
            printf("FAIL (aliasing 2)\n\n");
            abort();
        }

        fmpcb_poly_set(d, b);
        fmpcb_poly_div_series(c, d, d, p, rbits2);
        fmpcb_poly_div_series(d, d, d, p, rbits2);
        if (!fmpcb_poly_equal(d, c))
        {
            printf("FAIL (aliasing 3)\n\n");
            abort();
        }

        fmpq_poly_clear(A);
        fmpq_poly_clear(B);
        fmpq_poly_clear(C);

        fmpcb_poly_clear(a);
        fmpcb_poly_clear(b);
        fmpcb_poly_clear(c);
        fmpcb_poly_clear(d);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
