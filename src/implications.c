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

#include "implications.h"
#include "fisutils.h"

// for LARSEN and ZADEH, only
double *ImplicationSet (double *singleton_input_set, double *input_set, double *output_set, long npoints1, long npoints2, int method)
{
    long i;
    long y;
    double minvalue;
    double maxvalue;

    double *aux;

    aux = (double *) malloc (sizeof (double) * npoints2);
    if (! aux)
    {
        printf ("\nError on allocating memory: ImplicationSet ()\n");
        return NULL;
    }

    switch (method)
    {
        case ZADEH:     for (i = 0; i < npoints1; i++)
                        {
			 	minvalue = 1;
				maxvalue = 0;

                            	for (y = 0; y < npoints2; y++)
                            	{
					minvalue = Minimum (singleton_input_set[i], Maximum (1.0 - input_set[i], Minimum (input_set[i], output_set[y])));
					maxvalue = Maximum (minvalue, maxvalue);

                            	}

				aux[i] = maxvalue;
                        }

                        break;

        case LARSEN:    for (i = 0; i < npoints1; i++)
                        {
				minvalue = 1;
				maxvalue = 0;

                            	for (y = 0; y < npoints2; y++)
                            	{
					minvalue = Minimum (singleton_input_set[i], input_set[i] * output_set[y]);
					maxvalue = Maximum (minvalue, maxvalue);

                            	}

				aux[i] = maxvalue;
                        }

                        break;
    }

    return aux;
}

// as MANDANI doesn´t use Singleton values, it was implemented separately in the functions below
void  FuzzyIfInput2 (	struct SSets *input_set1, int membership1, double value1, int op,
			          			struct SSets *input_set2, int membership2, double value2,
					           	struct SSets *output_set, int membership3, int method, double **fuzzy_values)
{
    double *aux_fuzzy_values = 0;
    long int i;

    double *singleton_set1 = 0;
    double *values_set1 = 0;

    double *singleton_set2 = 0;
    double *values_set2 = 0;

    double minmax;
    long int pos1;
    long int pos2;

    if (method == MANDANI)
    {

        pos1 = ConvPosDisc (value1, input_set1[membership1].npoints, input_set1[membership1].start_uod, input_set1[membership1].stop_uod);

        pos2 = ConvPosDisc (value2, input_set2[membership2].npoints, input_set2[membership2].start_uod, input_set2[membership2].stop_uod);

        value1 = input_set1[membership1].value[pos1];
        value2 = input_set2[membership2].value[pos2];

        switch (op)
        {
            case AND :  minmax = 1.0;
                        minmax = Minimum (value1, value2);
                        aux_fuzzy_values = Cut (&output_set[membership3].value, output_set[membership3].npoints, minmax, FALSE);
                        break;

            case OR :   minmax = 0;
                        minmax = Maximum (value1, value2);
                        aux_fuzzy_values = Cut (&output_set[membership3].value, output_set[membership3].npoints, minmax, FALSE);
						break;
        }
    }

    else
    {
        singleton_set1 = SigletonSet (value1, input_set1[membership1].npoints, input_set1[membership1].start_uod,
					input_set1[membership1].stop_uod);
        values_set1 = ImplicationSet (singleton_set1, input_set1[membership1].value, output_set[membership3].value,
					input_set1[membership1].npoints, output_set[membership3].npoints, method);


		singleton_set2 = SigletonSet (value2, input_set2[membership2].npoints, input_set2[membership2].start_uod,
						input_set2[membership1].stop_uod);

        values_set2 = ImplicationSet (singleton_set2, input_set2[membership2].value, output_set[membership3].value,
					input_set2[membership1].npoints, output_set[membership3].npoints, method);


        switch (op)
        {
            case AND :  for (i = 0; i < input_set1[membership1].npoints; i++)
                        {
                            aux_fuzzy_values[i] = Minimum (values_set1[i], values_set2[i]);
                        }

                        break;

            case OR :   for (i = 0; i < input_set1[membership1].npoints; i++)
                        {
                            aux_fuzzy_values[i] = Maximum (values_set1[i], values_set2[i]);
                        }

                        break;
        }

        free (singleton_set1);
		free (singleton_set2);
        free (values_set1);
		free (values_set2);
    }

    for (i = 0; i < output_set[membership3].npoints; i++)
    {
        (* fuzzy_values)[i] = Maximum((* fuzzy_values)[i], aux_fuzzy_values[i]);
    }

    free (aux_fuzzy_values);


    return;
}

void  FuzzyIfInput1 (	struct SSets *input_set1, int membership1, double value1,
				        		struct SSets *output_set, int membership3, int method, double **fuzzy_values)
{
    double *aux_fuzzy_values = 0;
    long int i;

    double *singleton_set1 = 0;
    double *values_set1 = 0;

    long int pos1;

    if (method == MANDANI)
    {

        pos1 = ConvPosDisc (value1, input_set1[membership1].npoints, input_set1[membership1].start_uod, input_set1[membership1].stop_uod);

        value1 = input_set1[membership1].value[pos1];

	aux_fuzzy_values = Cut (&output_set[membership3].value, output_set[membership3].npoints, value1, FALSE);

    }

    else
    {
        singleton_set1 = SigletonSet (value1, input_set1[membership1].npoints, input_set1[membership1].start_uod,
					input_set1[membership1].stop_uod);
        values_set1 = ImplicationSet (singleton_set1, input_set1[membership1].value, output_set[membership3].value,
					input_set1[membership1].npoints, output_set[membership3].npoints, method);

		for (i = 0; i < input_set1[membership1].npoints; i++)
		    aux_fuzzy_values[i] = values_set1[i];

        free (singleton_set1);
        free (values_set1);
    }

    for (i = 0; i < output_set[membership3].npoints; i++)
    {
        (* fuzzy_values)[i] = Maximum((* fuzzy_values)[i], aux_fuzzy_values[i]);
    }

    free (aux_fuzzy_values);


	return;
}
