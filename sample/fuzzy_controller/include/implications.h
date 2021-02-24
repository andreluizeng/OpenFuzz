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

#ifndef __implications_h__
#define __implications_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "openfuzz.h"

#pragma once

#define MANDANI         0
#define ZADEH           1
#define LARSEN          2

#define AND             0
#define OR              1

//-------------------------------------------------------------------------------------------------
// singleton_input_set	= singleton set of input membership function
// input_set			= input membership function
// output_set			= output membership function
// npoints1				= number of discretization points of input membership function
// npoints2				= number of discretization points of output membership function
// method				= implication method -> MANDANI, LARSEN, ZADEH
/**
 * 	Allocates memory for Fuzzy Sets
 * 	@param sets fuzzy sets object pointer
 * 	@param nsets number of sets
 * 	@param npoints  number of discretization points
 * 	@param start_uod universe of discourse start value
 * 	@param stop_uod universe of discourse stop value
 * 	@param value  initialization value (normally 0 or NULL)
 *  @return TRUE if success or FALSE if it fails
 *  @note	Usage:
 *	@code
 *	// discrete points (bigger this value is, more precise will be the output, but the performance (speed) will be decreased
 *	#define DISCRETE_PTS 10000
 *
 *	// 3 memebership function for this set
 *	#define NSETS 3
 *
 *	// must be float point
 *	#define START_UOD 0.0
 *	#define STOP_UOD  100.0
 *  .
 *  .
 *  .
 *  int main (void)
 *	{
 *  	struct SSets *temperature;
 *  	int ret;
 *
 *		ret = InitializeSets (&temperature, NSETS, DISCRETE_PTS, START_UOD, STOP_UOD, 0.0);
 *		if (! ret)
 *		{
 *	 		printf ("\nError Initializing sets\n");
 *			return 0;
 *		}
 * 		.
 * 		.
 *		return 0;
 *	}
 *	@endcode
 */
double *ImplicationSet (double *singleton_input_set, double *input_set, double *output_set, long npoints1, long npoints2, int method);

//-------------------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------------------
// Rule base for 1 inputs
// input_set1	= input set 1
// membership1	= membership function of input set 1
// value1		= membership value of input set 1
// output_set	= output set
// membership3	= membermship function of input set 3
// method		= implication method  (MANDANI, LARSEN, ZADEH)
// fuzzy_values	= array with implicated values
void  FuzzyIfInput1 (	struct SSets *input_set1, int membership1, double value1,
        						struct SSets *output_set, int membership3, int method, double **fuzzy_values);

//-------------------------------------------------------------------------------------------------//-------------------------------------------------------------------------------------------------
// Rule base for 2 inputs
// input_set1	= input set 1
// membership1	= membership function of input set 1
// value1		= membership value of input set 1
// op			= operator AND or OR
// input_set2	= input set 1
// membership2	= membership function of input set 2
// value2		= membership value of input set 2
// output_set	= output set
// membership3	= membermship function of input set 3
// method		= implication method  (MANDANI, LARSEN, ZADEH)
// fuzzy_values	= array with implicated values
void  FuzzyIfInput2 (	struct SSets *input_set1, int membership1, double value1, int op,
        						struct SSets *input_set2, int membership2, double value2,
	           					struct SSets *output_set, int membership3, int method, double **fuzzy_values);



#endif
