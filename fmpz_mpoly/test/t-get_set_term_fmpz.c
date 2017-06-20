/*
    Copyright (C) 2017 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mpoly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, j, k, result;
    FLINT_TEST_INIT(state);

    flint_printf("get/set_term_fmpz....");
    fflush(stdout);

    /* Set term and get term and compare */
    for (i = 0; i < 1000 * flint_test_multiplier(); i++)
    {
       fmpz_mpoly_ctx_t ctx;
       fmpz_mpoly_t f;
       ordering_t ord;
       fmpz_t c, d;
       slong nvars, len, exp_bound, coeff_bits, exp_bits;

       fmpz_init(c);
       fmpz_init(d);

       ord = mpoly_ordering_randtest(state);
       nvars = n_randint(state, 20) + 1;

       fmpz_mpoly_ctx_init(ctx, nvars, ord);

       fmpz_mpoly_init(f, ctx);

       len = n_randint(state, 100);

       exp_bits = n_randint(state, FLINT_BITS -
                     mpoly_ordering_isdeg(ord)*FLINT_BIT_COUNT(nvars) - 1) + 1;
       exp_bound = n_randbits(state, exp_bits);
       coeff_bits = n_randint(state, 200);

       fmpz_mpoly_randtest(f, state, len, exp_bound, coeff_bits, ctx);

       for (j = 0; j < 10; j++)
       {
          ulong * exp = (ulong *) flint_malloc(nvars*sizeof(ulong));

          for (k = 0; k < nvars; k++)
          {
             slong bits = n_randint(state, FLINT_BITS -
                     mpoly_ordering_isdeg(ord)*FLINT_BIT_COUNT(nvars) - 1) + 1;
             exp[k] = n_randbits(state, bits);
          }

          fmpz_randtest(c, state, n_randint(state, 200));

          fmpz_mpoly_set_term_fmpz(f, exp, c, ctx);

          fmpz_mpoly_get_term_fmpz(d, f, exp, ctx);

          result = fmpz_equal(c, d);

          if (!result)
          {
             printf("FAIL\n");

             printf("ord = "); mpoly_ordering_print(ord);
             printf(", len = %ld, exp_bits = %ld, exp_bound = %lx, "
                                      "coeff_bits = %ld, nvars = %ld\n\n",
                                  len, exp_bits, exp_bound, coeff_bits, nvars);

             printf("c = "); fmpz_print(c); printf("\n");
             printf("d = "); fmpz_print(d); printf("\n");

             flint_abort();
          }

          flint_free(exp);
       }

       fmpz_mpoly_clear(f, ctx);  

       fmpz_clear(c);      
       fmpz_clear(d);      
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}

