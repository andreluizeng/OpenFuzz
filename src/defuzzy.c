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

#include "defuzzy.h"
#include "fisutils.h"

double DeFuzzy (double *fuzzy_values, struct SSets *output_set, int method)
{
    double sum1;
    double sum2;
    double value;

    double current_max;
    double first_max;
    double last_max;
    int first_max_pos;
    int last_max_pos;

    long int i;
    long int nmax;

    double pos;
    
    value = 0;
    
    switch (method)
    {
        case COA:   value = 0;
                    sum1 = 0;
		     sum2 = 0;


			for (i = 0; i < output_set[0].npoints; i++)
	               {
				pos = ConvDiscPos (i, output_set[0].npoints, output_set[0].start_uod, output_set[0].stop_uod);
				sum1 = sum1 + (fuzzy_values[i] * pos);
				sum2 = sum2 + fuzzy_values[i];
                    	}

			if (! sum2) value = 0;
			else value = sum1 / sum2;

                    	break;


        case MOM:	value = 0;
			sum1 = 0;
			nmax = 0;

			for (i = 0; i < output_set[0].npoints; i++)
			{
                       	value = Maximum (value, fuzzy_values[i]);

                       	if (fuzzy_values[i] == value)
	                       {
        	                    sum1 = sum1 + ConvDiscPos (i, output_set[0].npoints, output_set[0].start_uod, output_set[0].stop_uod);
        	                    nmax++;
        	               }

			}

			if (! nmax) value = 0;
			value = sum1 / nmax;

        		break;

	case LOM:	value = 0;
			current_max = 0;
			first_max = 0;
			first_max_pos = 0;
			last_max = 0;
			last_max_pos = 0;

			for (i = 0; i < output_set[0].npoints; i++)
			{
				current_max = Maximum (first_max, fuzzy_values[i]);
		                if (current_max > first_max)
			        {
					if (current_max > last_max)
					{
						first_max = last_max;
						first_max_pos = last_max_pos;
						last_max = current_max;
						last_max_pos = i;
					}
					else if (current_max < last_max)
					{
						first_max = current_max;
						first_max_pos = i;
					}
			        }
			}

			value = ConvDiscPos  (last_max_pos, output_set[0].npoints, output_set[0].start_uod, output_set[0].stop_uod);

                   	break;

	case FOM:	value = 0;
			current_max = 0;
			first_max = 0;
			first_max_pos = 0;
			last_max = 0;
			last_max_pos = 0;

			for (i = 0; i < output_set[0].npoints; i++)
			{
				current_max = Maximum (first_max, fuzzy_values[i]);

		                if (current_max > first_max)
			        {
					if (current_max > last_max)
					{
						first_max = last_max;
						first_max_pos = last_max_pos;

						last_max = current_max;
						last_max_pos = i;
					}
					else if (current_max < last_max)
					{
						first_max = current_max;
						first_max_pos = i;
					}
			        }
                   	}

			value = ConvDiscPos  (first_max_pos, output_set[0].npoints, output_set[0].start_uod, output_set[0].stop_uod);

	               break;
    }

    return value;
}
