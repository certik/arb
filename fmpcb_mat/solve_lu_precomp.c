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

#include "fmpcb_mat.h"

void
fmpcb_mat_solve_lu_precomp(fmpcb_mat_t X, const long * perm,
    const fmpcb_mat_t A, const fmpcb_mat_t B, long prec)
{
    long i, j, c, n, m;

    n = fmpcb_mat_nrows(X);
    m = fmpcb_mat_ncols(X);

    if (X == B)
    {
        fmpcb_ptr tmp = flint_malloc(sizeof(fmpcb_struct) * n);

        for (c = 0; c < m; c++)
        {
            for (i = 0; i < n; i++)
                tmp[i] = B->rows[perm[i]][c];
            for (i = 0; i < n; i++)
                X->rows[i][c] = tmp[i];
        }

        flint_free(tmp);
    }
    else
    {
        for (c = 0; c < m; c++)
        {
            for (i = 0; i < n; i++)
            {
                fmpcb_set(fmpcb_mat_entry(X, i, c),
                    fmpcb_mat_entry(B, perm[i], c));
            }
        }
    }

    for (c = 0; c < m; c++)
    {
        /* solve Ly = b */
        for (i = 1; i < n; i++)
        {
            for (j = 0; j < i; j++)
            {
                fmpcb_submul(fmpcb_mat_entry(X, i, c),
                    fmpcb_mat_entry(A, i, j), fmpcb_mat_entry(X, j, c), prec);
            }
        }

        /* solve Ux = y */
        for (i = n - 1; i >= 0; i--)
        {
            for (j = i + 1; j < n; j++)
            {
                fmpcb_submul(fmpcb_mat_entry(X, i, c),
                    fmpcb_mat_entry(A, i, j), fmpcb_mat_entry(X, j, c), prec);
            }

            fmpcb_div(fmpcb_mat_entry(X, i, c), fmpcb_mat_entry(X, i, c),
                fmpcb_mat_entry(A, i, i), prec);
        }
    }
}
