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

#include "fmprb_poly.h"


int main()
{
    long iter;
    flint_rand_t state;

    printf("revert_series_newton....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 1000; iter++)
    {
        long qbits1, rbits1, rbits2, n;
        fmpq_poly_t A, B;
        fmprb_poly_t a, b, c;

        qbits1 = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);
        n = 2 + n_randint(state, 25);

        fmpq_poly_init(A);
        fmpq_poly_init(B);

        fmprb_poly_init(a);
        fmprb_poly_init(b);
        fmprb_poly_init(c);

        do {
            fmpq_poly_randtest(A, state, 1 + n_randint(state, 25), qbits1);
            fmpq_poly_set_coeff_ui(A, 0, 0);
        } while (A->length < 2 || fmpz_is_zero(A->coeffs + 1));

        fmpq_poly_revert_series(B, A, n);

        fmprb_poly_set_fmpq_poly(a, A, rbits1);
        fmprb_poly_revert_series_newton(b, a, n, rbits2);

        if (!fmprb_poly_contains_fmpq_poly(b, B))
        {
            printf("FAIL\n\n");
            printf("n = %ld, bits2 = %ld\n", n, rbits2);

            printf("A = "); fmpq_poly_print(A); printf("\n\n");
            printf("B = "); fmpq_poly_print(B); printf("\n\n");

            printf("a = "); fmprb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmprb_poly_printd(b, 15); printf("\n\n");

            abort();
        }

        fmprb_poly_set(c, a);
        fmprb_poly_revert_series_newton(c, c, n, rbits2);
        if (!fmprb_poly_equal(c, b))
        {
            printf("FAIL (aliasing)\n\n");
            abort();
        }

        fmpq_poly_clear(A);
        fmpq_poly_clear(B);

        fmprb_poly_clear(a);
        fmprb_poly_clear(b);
        fmprb_poly_clear(c);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
