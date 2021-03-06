/* This file is public domain. Author: Fredrik Johansson. */

#include <string.h>
#include "fmpcb.h"
#include "fmpcb_poly.h"
#include "arith.h"
#include "profiler.h"

/* Print roots in a non-random order */
#ifndef __compar_fn_t
typedef int (*__compar_fn_t) (__const void *, __const void *);
#endif

int fmpcb_cmp_pretty(const fmpcb_t a, const fmpcb_t b)
{
    fmprb_t t, u, v;
    int res;
    fmprb_init(t);
    fmprb_init(u);
    fmprb_init(v);
    fmprb_abs(u, fmpcb_imagref(a));
    fmprb_abs(v, fmpcb_imagref(b));
    fmprb_sub(t, u, v, FMPRB_RAD_PREC);
    res = 0;
    if (fmprb_contains_zero(t))
    {
        fmprb_sub(t, fmpcb_realref(a), fmpcb_realref(b), FMPRB_RAD_PREC);
        res = fmprb_is_positive(t) ? 1 : -1;
    }
    else
    {
        res = fmprb_is_positive(t) ? 1 : -1;
    }
    fmprb_clear(t);
    fmprb_clear(u);
    fmprb_clear(v);
    return res;
}

void _fmpcb_vec_sort_pretty(fmpcb_ptr vec, long len)
{
    qsort(vec, len, sizeof(fmpcb_struct), (__compar_fn_t) fmpcb_cmp_pretty);
}

int check_accuracy(fmpcb_ptr vec, long len, long prec)
{
    long i;

    for (i = 0; i < len; i++)
    {
        if (fmpr_cmp_2exp_si(fmprb_radref(fmpcb_realref(vec + i)), -prec) >= 0
         || fmpr_cmp_2exp_si(fmprb_radref(fmpcb_imagref(vec + i)), -prec) >= 0)
            return 0;
    }

    return 1;
}

void
poly_roots(const fmpz_poly_t poly,
    long initial_prec,
    long target_prec,
    long print_digits)
{
    long i, prec, deg, isolated, maxiter;
    fmpcb_poly_t cpoly;
    fmpcb_ptr roots;

    deg = poly->length - 1;

    fmpcb_poly_init(cpoly);
    roots = _fmpcb_vec_init(deg);

    for (prec = initial_prec; ; prec *= 2)
    {
        fmpcb_poly_set_fmpz_poly(cpoly, poly, prec);
        maxiter = FLINT_MIN(deg, prec);

        TIMEIT_ONCE_START
        printf("prec=%ld: ", prec);
        isolated = fmpcb_poly_find_roots(roots, cpoly,
            prec == initial_prec ? NULL : roots, maxiter, prec);
        printf("%ld isolated roots | ", isolated);
        TIMEIT_ONCE_STOP

        if (isolated == deg && check_accuracy(roots, deg, target_prec))
        {
            printf("done!\n");
            break;
        }
    }

    if (print_digits != 0)
    {
        _fmpcb_vec_sort_pretty(roots, deg);

        for (i = 0; i < deg; i++)
        {
            fmpcb_printd(roots + i, print_digits);
            printf("\n");
        }
    }

    fmpcb_poly_clear(cpoly);
    _fmpcb_vec_clear(roots, deg);
}

int main(int argc, char *argv[])
{
    fmpz_poly_t f;
    fmpz_t t;
    long compd, printd, i, j;

    if (argc < 2)
    {
        printf("poly_roots [-refine d] [-print d] <poly>\n\n");

        printf("Isolates all the complex roots of a polynomial with\n");
        printf("integer coefficients. For convergence, the input polynomial\n");
        printf("is required to be squarefree.\n\n");

        printf("If -refine d is passed, the roots are refined to an absolute\n");
        printf("tolerance better than 10^(-d). By default, the roots are only\n");
        printf("computed to sufficient accuracy to isolate them.\n");
        printf("The refinement is not currently done efficiently.\n\n");

        printf("If -print d is passed, the computed roots are printed to\n");
        printf("d decimals. By default, the roots are not printed.\n\n");

        printf("The polynomial can be specified by passing the following as <poly>:\n\n");

        printf("a <n>          Easy polynomial 1 + 2x + ... + (n+1)x^n\n");
        printf("t <n>          Chebyshev polynomial T_n\n");
        printf("u <n>          Chebyshev polynomial U_n\n");
        printf("p <n>          Legendre polynomial P_n\n");
        printf("c <n>          Cyclotomic polynomial Phi_n\n");
        printf("s <n>          Swinnerton-Dyer polynomial S_n\n");
        printf("b <n>          Bernoulli polynomial B_n\n");
        printf("w <n>          Wilkinson polynomial W_n\n");
        printf("e <n>          Taylor series of exp(x) truncated to degree n\n");
        printf("m <n> <m>      The Mignotte-like polynomial x^n + (100x+1)^m, n > m\n");
        printf("c0 c1 ... cn   c0 + c1 x + ... + cn x^n where all c:s are specified integers\n");

        return 1;
    }

    compd = 0;
    printd = 0;
    j = 0;

    fmpz_poly_init(f);
    fmpz_init(t);

    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-refine"))
        {
            compd = atol(argv[i+1]);
            i++;
        }
        else if (!strcmp(argv[i], "-print"))
        {
            printd = atol(argv[i+1]);
            i++;
        }
        else if (!strcmp(argv[i], "a"))
        {
            long n = atol(argv[i+1]);
            for (j = 0; j <= n; j++)
                fmpz_poly_set_coeff_ui(f, j, j+1);
            break;
        }
        else if (!strcmp(argv[i], "t"))
        {
            arith_chebyshev_t_polynomial(f, atol(argv[i+1]));
            break;
        }
        else if (!strcmp(argv[i], "u"))
        {
            arith_chebyshev_u_polynomial(f, atol(argv[i+1]));
            break;
        }
        else if (!strcmp(argv[i], "p"))
        {
            fmpq_poly_t g;
            fmpq_poly_init(g);
            arith_legendre_polynomial(g, atol(argv[i+1]));
            fmpq_poly_get_numerator(f, g);
            fmpq_poly_clear(g);
            break;
        }
        else if (!strcmp(argv[i], "c"))
        {
            arith_cyclotomic_polynomial(f, atol(argv[i+1]));
            break;
        }
        else if (!strcmp(argv[i], "s"))
        {
            arith_swinnerton_dyer_polynomial(f, atol(argv[i+1]));
            break;
        }
        else if (!strcmp(argv[i], "b"))
        {
            fmpq_poly_t g;
            fmpq_poly_init(g);
            arith_bernoulli_polynomial(g, atol(argv[i+1]));
            fmpq_poly_get_numerator(f, g);
            fmpq_poly_clear(g);
            break;
        }
        else if (!strcmp(argv[i], "w"))
        {
            long n = atol(argv[i+1]);
            fmpz_poly_fit_length(f, n+2);
            arith_stirling_number_1_vec(f->coeffs, n+1, n+2);
            _fmpz_poly_set_length(f, n+2);
            fmpz_poly_shift_right(f, f, 1);
            break;
        }
        else if (!strcmp(argv[i], "e"))
        {
            fmpq_poly_t g;
            fmpq_poly_init(g);
            fmpq_poly_set_coeff_si(g, 0, 0);
            fmpq_poly_set_coeff_si(g, 1, 1);
            fmpq_poly_exp_series(g, g, atol(argv[i+1]) + 1);
            fmpq_poly_get_numerator(f, g);
            fmpq_poly_clear(g);
            break;
        }
        else if (!strcmp(argv[i], "m"))
        {
            fmpz_poly_set_coeff_ui(f, 0, 1);
            fmpz_poly_set_coeff_ui(f, 1, 100);
            fmpz_poly_pow(f, f,  atol(argv[i+2]));
            fmpz_poly_set_coeff_ui(f, atol(argv[i+1]), 1);
            break;
        }
        else
        {
            fmpz_set_str(t, argv[i], 10);
            fmpz_poly_set_coeff_fmpz(f, j, t);
            j++;
        }
    }

    TIMEIT_ONCE_START
    poly_roots(f, 53, compd * 3.32193 + 2, printd);
    TIMEIT_ONCE_STOP

    fmpz_poly_clear(f);
    fmpz_clear(t);

    flint_cleanup();
    return EXIT_SUCCESS;
}

