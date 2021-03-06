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

#include "fmpcb.h"

void
fmpcb_tan_lower_halfplane(fmpcb_t r, const fmpcb_t z, long prec, int pi, int cot)
{
    fmprb_t s, c, t, u, v;
    long wp;

#define a fmpcb_realref(z)
#define b fmpcb_imagref(z)

    fmprb_init(s);
    fmprb_init(c);
    fmprb_init(t);
    fmprb_init(u);
    fmprb_init(v);

    wp = prec + 6;

    fmprb_mul_2exp_si(t, a, 1);
    if (pi)
        fmprb_sin_cos_pi(s, c, t, wp);
    else
        fmprb_sin_cos(s, c, t, wp);

    /* t = exp(4b) - 1 */
    if (pi)
    {
        fmprb_const_pi(t, wp);
        fmprb_mul(t, t, b, wp);
        fmprb_mul_2exp_si(t, t, 2);
    }
    else
    {
        fmprb_mul_2exp_si(t, b, 2);
    }

    fmprb_expm1(t, t, wp);

    /* u = 2exp(2b) (sqrt would be inaccurate when b is very negative) */
    if (pi)
    {
        fmprb_const_pi(u, wp);
        fmprb_mul(u, u, b, wp);
        fmprb_mul_2exp_si(u, u, 1);
    }
    else
    {
        fmprb_mul_2exp_si(u, b, 1);
    }
    fmprb_exp(u, u, wp);
    fmprb_mul_2exp_si(u, u, 1);

    /* im = (exp(4b) - 1) / (2 cos(2a) exp(2b) + (exp(4b) - 1) + 2) */
    fmprb_mul(v, c, u, wp);
    if (cot)
        fmprb_neg(v, v);
    fmprb_add(v, v, t, wp);
    fmprb_add_ui(v, v, 2, wp);
    fmprb_div(fmpcb_imagref(r), t, v, prec);
    if (cot)
        fmprb_neg(fmpcb_imagref(r), fmpcb_imagref(r));

    /* re = 2 exp(2b) sin(2a) / (...) */
    fmprb_mul(s, s, u, wp);
    fmprb_div(fmpcb_realref(r), s, v, prec);

    fmprb_clear(s);
    fmprb_clear(c);
    fmprb_clear(t);
    fmprb_clear(u);
    fmprb_clear(v);

#undef a
#undef b
}

void
fmpcb_tan_near_real(fmpcb_t r, const fmpcb_t z, long prec, int pi, int cot)
{
#define a fmpcb_realref(z)
#define b fmpcb_imagref(z)

    fmprb_t sa, ca, sb, cb;
    long wp;

    fmprb_init(sa);
    fmprb_init(ca);
    fmprb_init(sb);
    fmprb_init(cb);

    wp = prec + 6;

    if (pi)
    {
        fmprb_mul_2exp_si(sa, a, 1);
        fmprb_sin_cos_pi(sa, ca, sa, wp);

        fmprb_const_pi(sb, wp);
        fmprb_mul(sb, sb, b, wp);
        fmprb_mul_2exp_si(sb, sb, 1);
        fmprb_sinh_cosh(sb, cb, sb, wp);
    }
    else
    {
        fmprb_mul_2exp_si(sa, a, 1);
        fmprb_sin_cos(sa, ca, sa, wp);
        fmprb_mul_2exp_si(sb, b, 1);
        fmprb_sinh_cosh(sb, cb, sb, wp);
    }

    if (cot)
    {
        fmprb_sub(ca, ca, cb, wp);
    }
    else
    {
        fmprb_add(ca, ca, cb, wp);
    }

    fmprb_div(fmpcb_realref(r), sa, ca, prec);
    fmprb_div(fmpcb_imagref(r), sb, ca, prec);

    if (cot)
        fmprb_neg(fmpcb_realref(r), fmpcb_realref(r));

    fmprb_clear(sa);
    fmprb_clear(ca);
    fmprb_clear(sb);
    fmprb_clear(cb);

#undef a
#undef b
}

void
fmpcb_tan(fmpcb_t r, const fmpcb_t z, long prec)
{
    if (fmprb_is_zero(fmpcb_imagref(z)))
    {
        fmprb_tan(fmpcb_realref(r), fmpcb_realref(z), prec);
        fmprb_zero(fmpcb_imagref(r));
    }
    else if (fmprb_is_zero(fmpcb_realref(z)))
    {
        fmprb_tanh(fmpcb_imagref(r), fmpcb_imagref(z), prec);
        fmprb_zero(fmpcb_realref(r));
    }
    else
    {
        if (fmpr_cmpabs_2exp_si(fmprb_midref(fmpcb_imagref(z)), 1) < 0)
        {
            fmpcb_tan_near_real(r, z, prec, 0, 0);
        }
        else if (fmpr_sgn(fmprb_midref(fmpcb_imagref(z))) < 0)
        {
            fmpcb_tan_lower_halfplane(r, z, prec, 0, 0);
        }
        else
        {
            fmpcb_neg(r, z);
            fmpcb_tan_lower_halfplane(r, r, prec, 0, 0);
            fmpcb_neg(r, r);
        }
    }
}

void
fmpcb_tan_pi(fmpcb_t r, const fmpcb_t z, long prec)
{
    if (fmprb_is_zero(fmpcb_imagref(z)))
    {
        fmprb_tan_pi(fmpcb_realref(r), fmpcb_realref(z), prec);
        fmprb_zero(fmpcb_imagref(r));
    }
    else if (fmprb_is_zero(fmpcb_realref(z)))
    {
        fmprb_t t;
        fmprb_init(t);
        fmprb_const_pi(t, prec + 4);
        fmprb_mul(t, fmpcb_imagref(z), t, prec + 4);
        fmprb_tanh(fmpcb_imagref(r), t, prec);
        fmprb_zero(fmpcb_realref(r));
        fmprb_clear(t);
    }
    else
    {
        if (fmpr_cmpabs_2exp_si(fmprb_midref(fmpcb_imagref(z)), 1) < 0)
        {
            fmpcb_tan_near_real(r, z, prec, 1, 0);
        }
        else if (fmpr_sgn(fmprb_midref(fmpcb_imagref(z))) < 0)
        {
            fmpcb_tan_lower_halfplane(r, z, prec, 1, 0);
        }
        else
        {
            fmpcb_neg(r, z);
            fmpcb_tan_lower_halfplane(r, r, prec, 1, 0);
            fmpcb_neg(r, r);
        }
    }
}

