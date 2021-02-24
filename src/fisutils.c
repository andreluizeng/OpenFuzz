/**
 * @file
 * @author  Andre Silva <andreluizeng@yahoo.com.br>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2012, Andre Luiz Vieira da Silva
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the <organization>.
 * 4. Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Andre Silva ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Andre Silva BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 *
 */

#include "fisutils.h"


//-------------------------------------------------------------------------------------------------
int InitializeSets (struct SSets **sets, int nsets, long npoints, double start_uod, double stop_uod, double value)
{
    long i;
    long j;

    struct SSets *aux;

    aux = (struct SSets *) malloc (sizeof (struct SSets) * nsets);
    if (aux == NULL) return FALSE;

    for (i = 0; i < nsets; i++)
    {
        aux[i].value = (double *) malloc (sizeof (double) * npoints);
        if (aux[i].value == NULL) return FALSE;
    }

    for (i = 0; i < nsets; i++)
    {
        for (j = 0; j < npoints; j++)
        {
            aux[i].value[j] = value;
        }
    }

    for (i = 0; i < nsets; i++)
    {
            aux[i].nsets = nsets;
            aux[i].npoints = npoints;
            aux[i].start_uod = start_uod;
            aux[i].stop_uod = stop_uod;
    }

    (* sets) = aux;


    return TRUE;
}
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
double *MembershipFunction (int type, ...)
{
    double *aux;
    long int npoints;
    double start_uod;
    double stop_uod;

    double x1;
    double x2;
    double x3;
    double x4;
    double center;
    double sigma;

    double y;
    double j;
    long int i;


    va_list ap;
    va_start (ap, type);
    npoints = va_arg (ap, long int);
    start_uod = va_arg (ap, double);
    stop_uod = va_arg (ap, double);

    y = (stop_uod - start_uod) / npoints;


    aux = (double *) malloc (sizeof (double) * npoints);
    if (! aux)
    {
        printf ("\nError on allocating memory: MembershipFunction ()\n");
        return FALSE;
    }

    switch (type)
    {
        case TRIANGULAR:    x1 = va_arg (ap, double);
                            x2 = va_arg (ap, double);
                            x3 = va_arg (ap, double);

                            for (i = 0, j = start_uod; i < npoints; i++, j = j + y)
                            {
                                if (j < x1)
                                {
                                    aux[i] = 0;
                                }

                                else if ((j >= x1) && (j < x2))
                                {
                                    aux[i] = (j / (x2 - x1)) - (x1 / (x2 - x1));
                                }

                                else if ((j >= x2) && (j < x3))
                                {
                                    aux[i] = ((-j) / (x3 - x2)) + (x3 / (x3 - x2));
                                }

                                else if (j >= x3)
                                {
                                    aux[i] = 0;
                                }
                            }
                            break;

        case TRAPEZOIDAL:   x1 = va_arg (ap, double);
                            x2 = va_arg (ap, double);
                            x3 = va_arg (ap, double);
                            x4 = va_arg (ap, double);

                            for (i = 0, j = start_uod; i < npoints; i++, j = j + y)
                            {
                                if (j < x1)
                                {
                                    aux[i] = 0;
                                }

                                else if ((j >= x1) && (j < x2))
                                {
                                    aux[i] = (j / (x2 - x1))  - (x1 / (x2 - x1));
                                }

                                else if ((j >= x2) && (j < x3))
                                {
                                    aux[i] = 1;
                                }

                                else if ((j >= x3) && (j < x4))
                                {
                                    aux[i] = ((-j) / (x4 - x3)) + (x4 / (x4 - x3));
                                }

                                else if (j >= x4)
                                {
                                    aux[i] = 0;
                                }
                            }


                            break;

        case GAUSSIAN:		center = va_arg (ap, double);
                            sigma  = va_arg (ap, double);

                            for (i = 0, j = start_uod; i < npoints; i++, j = j + y)
                            {
                                aux[i] = exp ((-1 / pow (sigma, 2)) * pow ((j - center), 2));
                            //f(x) = A e ((-1/sigma^2)*(x - x0)^2)
                            }

                            break;
    }



    return aux;

}
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
void Fuzzification (struct SSets *sets, int type, ...)
{
    double x1;
    double x2;
    double x3;
    double x4;

    double center;
    double sigma;

    va_list ap;
    va_start (ap, type);


    switch (type)
    {
        case TRIANGULAR:    x1 = va_arg (ap, double);
                            x2 = va_arg (ap, double);
                            x3 = va_arg (ap, double);
                            sets->value = MembershipFunction (type, sets->npoints, sets->start_uod, sets->stop_uod, x1, x2, x3);

                            break;

        case TRAPEZOIDAL:   x1 = va_arg (ap, double);
                            x2 = va_arg (ap, double);
                            x3 = va_arg (ap, double);
                            x4 = va_arg (ap, double);
							sets->value = MembershipFunction (type, sets->npoints, sets->start_uod, sets->stop_uod, x1, x2, x3, x4);
                            break;

        case GAUSSIAN:		center = va_arg (ap, double);
                            sigma  = va_arg (ap, double);
                            sets->value = MembershipFunction (type, sets->npoints, sets->start_uod, sets->stop_uod, center, sigma);

                            break;
    }

    return;
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
long ConvPosDisc (double point, long npoints, double start_uod, double stop_uod)
{
    long aprox;
    double x;
//    double aux = 0;

	// i = beginning of universe of discourse
    // f = end of universe of discourse
    // vi = start of array (alwasys 0)
    // vf = end of array (always the number of points)

    // y = ((f - i) / (vf - vi)).x + (f - ((f - i) / (vf - vi)).vf)
    //   shorting...:
    // y = ((f - i) / (vf - vi).x ) + i))

	// where y = analogic value (input of universe of discourse)
    // and x = point of discretization array

	// inverting the equation we have
    // x = ((y - i).(vf)/(f-i)

	// as the counting starts on zero.. numbers of points is = (total - 1)

	x = ((point - start_uod)* (npoints - 1)) / (stop_uod - start_uod);

	aprox = (long) floor (x);
    	x = x - (double) aprox;
//	aux = aprox;
	if (x >= 0.5)
		aprox = aprox + 1;


    return aprox;
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
double ConvDiscPos  (long int disc, long npoints, double start_uod, double stop_uod)
{
    long int i;
    double k;
    double y;
    double value;

    y = (double) (stop_uod - start_uod) / (double) npoints;

    for (i = 0, k = 0; i <= disc; i++, k = k + y);

    value = start_uod + k;

    return value;
}

//------------------------------------------------------------------------------
double *SigletonSet (double point, long npoints, double start_uod, double stop_uod)
{
    double *aux;
    long aprox;


    aux = (double *) malloc (sizeof (double) * npoints);
    if (! aux)
    {
        printf ("\nError on allocating memory: SingletonSet ()\n");
        return NULL;
    }

    aprox = ConvPosDisc (point, npoints, start_uod, stop_uod);
	memset ((double *) aux, 0, npoints * sizeof (double));

    aux[aprox] = 1.0;

    return aux;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double Minimum (double value1, double value2)
{
	if (value1 < value2) return value1;
	else return value2;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double Maximum (double value1, double value2)
{
	if (value1 > value2) return value1;
	else return value2;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double *Cut (double **set, long npoints, double alpha, int flag)
{
    double *aux = NULL;
    long j;

    if (! flag)
    {
	    aux = (double *) malloc (sizeof (double) * npoints);
		if (aux == NULL)
		{
			printf ("\nError on allocating memory: Cut ()");
			return NULL;
		}

        for (j = 0; j < npoints; j++)
        {
            if ((* set)[j] >= alpha)
            {
                aux[j] = alpha;
            }

            else
            {
                aux[j] = (* set)[j];
            }
        }

		return aux;
    }

    else
    {
        for (j = 0; j < npoints; j++)
        {
            if ((*set)[j] >= alpha)
            {
                (* set)[j] = alpha;
            }
        }
    }

	return aux;
}



