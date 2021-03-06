/*
    Copyright (C) 2007 David Howden
    Copyright (C) 2007, 2008, 2009, 2010 William Hart
    Copyright (C) 2008 Richard Howell-Peak
    Copyright (C) 2011 Fredrik Johansson
    Copyright (C) 2012 Lina Kulakova

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
#include "fmpz_vec.h"
#include "fmpz_mod_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int iter;
    FLINT_TEST_INIT(state);
    

    flint_printf("factor_squarefree....");
    fflush(stdout);

    for (iter = 0; iter < 30 * flint_test_multiplier(); iter++)
    {
        int result = 1;
        fmpz_mod_poly_t pol1, poly, quot, rem;
        fmpz_mod_poly_factor_t res;
        fmpz_t modulus;
        slong exp[5], prod1;
        slong length, i, j, num;

        fmpz_init_set_ui(modulus, n_randtest_prime(state, 0));

        fmpz_mod_poly_init(pol1, modulus);
        fmpz_mod_poly_init(poly, modulus);
        fmpz_mod_poly_init(quot, modulus);
        fmpz_mod_poly_init(rem, modulus);

        fmpz_mod_poly_zero(pol1);
        fmpz_mod_poly_set_coeff_ui(pol1, 0, 1);

        length = n_randint(state, 7) + 2;

        do
        {
            fmpz_mod_poly_randtest(poly, state, length);
            fmpz_mod_poly_make_monic(poly, poly);
        }
        while ((!fmpz_mod_poly_is_irreducible(poly)) || (poly->length < 2));
        exp[0] = n_randprime(state, 5, 0);

        prod1 = exp[0];
        for (i = 0; i < exp[0]; i++)
            fmpz_mod_poly_mul(pol1, pol1, poly);

        num = n_randint(state, 5) + 1;
        for (i = 1; i < num; i++)
        {
            do
            {
                length = n_randint(state, 7) + 2;
                fmpz_mod_poly_randtest(poly, state, length);
                if (poly->length)
                {
                    fmpz_mod_poly_make_monic(poly, poly);
                    fmpz_mod_poly_divrem(quot, rem, pol1, poly);
                }
            }
            while ((!fmpz_mod_poly_is_irreducible(poly)) ||
                   (poly->length < 2) || (rem->length == 0));

            do
                exp[i] = n_randprime(state, 5, 0);
            while (prod1 % exp[i] == 0);

            prod1 *= exp[i];
            for (j = 0; j < exp[i]; j++)
                fmpz_mod_poly_mul(pol1, pol1, poly);
        }

        fmpz_mod_poly_factor_init(res);
        fmpz_mod_poly_factor_squarefree(res, pol1);

        result &= (res->num == num);
        if (result)
        {
            ulong prod2 = 1;
            for (i = 0; i < num; i++)
                prod2 *= res->exp[i];
            result &= (prod1 == prod2);
        }

        if (!result)
        {
            flint_printf("Error: exp don't match. Modulus = ");
            fmpz_print(modulus);
            flint_printf("\n");
            for (i = 0; i < res->num; i++)
                flint_printf("%wd ", res->exp[i]);
            flint_printf("\n");
            for (i = 0; i < num; i++)
                flint_printf("%wd ", exp[i]);
            flint_printf("\n");
            abort();
        }

        fmpz_clear(modulus);
        fmpz_mod_poly_clear(quot);
        fmpz_mod_poly_clear(rem);
        fmpz_mod_poly_clear(pol1);
        fmpz_mod_poly_clear(poly);
        fmpz_mod_poly_factor_clear(res);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
