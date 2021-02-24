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

#ifndef __fisutils_h__
#define __fisutils_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "openfuzz.h"

#pragma once

#define TRIANGULAR      0
#define TRAPEZOIDAL     1
#define GAUSSIAN		2

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
int InitializeSets (struct SSets **sets, int nsets, long npoints, double start_uod, double stop_uod, double value);

/**
 * 	Creates the membership functions (wrapper for the Fuzzification function, use this one instead)
 * 	@param type	membership function type (TRIANGULAR, TRAPEZOIDAL, GAUSSIAN)
 * 	@param npoints  number of discretization points
 * 	@param start_uod universe of discourse start value
 * 	@param stop_uod universe of discourse stop value
 *  @n
 *	@n if type == TRIANGULAR
 *	@param x1 first triangle point
 *	@param x2 middle triangle point
 *	@param x3 end triangle point
 *  @n
 *  @n if type == TRAPEZOIDAL
 *	@param x1 first trapezoidal point
 *	@param x2 second trapezoidal point
 *	@param x3 third trapezoidal point
 *	@param x4 fourth trapezoidal point
 *  @n
 *	@n if type == GAUSSIAN
 *	@param center center of the gaussian function
 * 	@param sigma standart deviation
 *  @return the vector (size of discretization points) with fuzzified values
 *  @note  This function is a wrapper of the OpenFIS library. Usage:
 *	@code
 *  // ranges for cold temperature membership (half of a triangle starting from the middle point and stopping at the stop point)
 *  #define START_COLD	5.0
 *  #define MID_COLD	5.0
 *  #define END_COLD	28.0
 *
 *  // ranges for warm temperature membership (trapezoidal)
 *  #define START_WARM	25.0
 *  #define MID1_WARM	28.0
 *  #define MID2_WARM	32.0
 *  #define END_WARM	35.0
 *
 *  // ranges for hot temperature membership (half of a triangle starting from the start point and stopping at the middle point)
 *  #define START_HOT	30.0
 *  #define MID_HOT		45.0
 *  #define END_HOT		45.0
 *
 *	// discrete points (bigger this value is, more precise will be the output, but the performance (speed) will be decreased
 *	#define DISCRETE_PTS 10000
 *
 *	// must be float point
 *	#define START_UOD 5.0
 *	#define STOP_UOD  45.0
 *
 *  int main (void)
 *	{
 *  	int ret;
 * 		double *fuzzified_values_cold;
 * 		double *fuzzified_values_warm;
 * 		double *fuzzified_values_hot;
 *
 *	 	// membership functions fuzzyfication
 *		fuzzified_values_cold = MembershipFunction (TRIANGULAR, DISCRETE_POINTS, START_UOD, STOP_UOD, START_COLD, MID_COLD, END_COLD);
 *		fuzzified_values_warm = MembershipFunction (TRAPEZOIDAL, DISCRETE_POINTS, START_UOD, STOP_UOD, START_WARM, MID1_WARM, MID2_WARM, END_WARM);
 *		fuzzified_values_hot = MembershipFunction (TRIANGULAR, DISCRETE_POINTS, START_UOD, STOP_UOD, START_HOT,  MID_HOT, END_HOT);
 *
 *   	return 0;
 *	}
 *	@endcode
 */
double *MembershipFunction (int type, ...);

/**
 * 	Fuzzifies the membership functions
 *	@param sets	fuzzy sets object pointer
 * 	@param type	membership function type (TRIANGULAR, TRAPEZOIDAL, GAUSSIAN)
 * 	@param npoints  number of discretization points
 * 	@param start_uod universe of discourse start value
 * 	@param stop_uod universe of discourse stop value
 *  @n
 *	@n if type == TRIANGULAR
 *	@param x1 first triangle point
 *	@param x2 middle triangle point
 *	@param x3 end triangle point
 *  @n
 *  @n if type == TRAPEZOIDAL
 *	@param x1 first trapezoidal point
 *	@param x2 second trapezoidal point
 *	@param x3 third trapezoidal point
 *	@param x4 fourth trapezoidal point
 *  @n
 *	@n if type == GAUSSIAN
 *	@param center center of the gaussian function
 * 	@param sigma standart deviation
 *  @return nothing
 *  @note	Usage:
 *	@code
 *  // limit values for fuzzy memberships
 *  #define TEMP_COLD	0
 *  #define TEMP_WARM	1
 *  #define TEMP_HOT	2
 *
 *  // ranges for cold temperature membership (half of a triangle starting from the middle point and stopping at the stop point)
 *  #define START_COLD	5.0
 *  #define MID_COLD	5.0
 *  #define END_COLD	28.0
 *
 *  // ranges for warm temperature membership (trapezoidal)
 *  #define START_WARM	25.0
 *  #define MID1_WARM	28.0
 *  #define MID2_WARM	32.0
 *  #define END_WARM	35.0
 *
 *  // ranges for hot temperature membership (half of a triangle starting from the start point and stopping at the middle point)
 *  #define START_HOT	30.0
 *  #define MID_HOT		45.0
 *  #define END_HOT		45.0
 *
 *	// discrete points (bigger this value is, more precise will be the output, but the performance (speed) will be decreased
 *	#define DISCRETE_PTS 10000
 *
 *	// must be float point
 *	#define START_UOD 5.0
 *	#define STOP_UOD  45.0
 *
 *  int main (void)
 *	{
 *  	struct SSets *temperature;
 *  	int ret;
 *
 *		ret = InitializeSets (&temperature, NSETS, DISCRETE_PTS, START_UOD, STOP_UOD, 0.0);
 *		if (! ret)
 *	 		return 0;
 *
 *	 	// membership functions fuzzyfication
 *		Fuzzyfication (&temperature[TEMP_COLD],	TRIANGULAR, START_COLD, MID_COLD, END_COLD);
 * 		Fuzzyfication (&temperature[TEMP_WARM],	TRAPEZOIDAL, START_WARM, MID1_WARM, MID2_WARM, END_WARM);
 *		Fuzzyfication (&temperature[TEMP_HOT],	TRIANGULAR, START_HOT,  MID_HOT,  END_HOT);
 *
 *   	return 0;
 *	}
 *	@endcode
 */
void Fuzzification (struct SSets *sets, int type, ...);

/**
 * 	change a position in the universe of discourse in a position of discretization
 *	@param point position in the universe of discourse
 * 	@param npoints  number of discretization points
 * 	@param start_uod universe of discourse start value
 * 	@param stop_uod universe of discourse stop value
 *  @return vector index to the vector of discrete points
 *  @note This function is a wrapper of the OpenFIS library. Usage:
 *	@code
 *	// discrete points (bigger this value is, more precise will be the output, but the performance (speed) will be decreased
 *	#define DISCRETE_PTS 10000
 *
 *	// must be float point
 *	#define START_UOD 5.0
 *	#define STOP_UOD 45.0
 *  .
 *  .
 *  .
 *  int main (void)
 *	{
 *		long value;
 *  	.
 *  	.
 * 		.
 *		value = ConvPosDisc (10.5, DISCRETE_PTS, START_UOD, STOP_UOD);
 *  	.
 *  	.
 * 		.
 *   	return 0;
 *	}
 *	@endcode
 */
long ConvPosDisc (double point, long npoints, double start_uod, double stop_uod);

/**
 * 	change a position of discretization in a position in the universe of discourse
 *	@param disc position in the discrete vector
 * 	@param npoints  number of discretization points
 * 	@param start_uod universe of discourse start value
 * 	@param stop_uod universe of discourse stop value
 *  @return a value (float point) of the universe of discourse
 *  @note This function is a wrapper of the OpenFIS library. Usage:
 *	@code
 *	// discrete points (bigger this value is, more precise will be the output, but the performance (speed) will be decreased
 *	#define DISCRETE_PTS 10000
 *
 *	// must be float point
 *	#define START_UOD 5.0
 *	#define STOP_UOD 45.0
 *  .
 *  .
 *  .
 *  int main (void)
 *	{
 *		double value;
 *  	.
 *  	.
 * 		.
 *		value = ConvDiscPos (300, DISCRETE_PTS, START_UOD, STOP_UOD);
 *  	.
 *  	.
 * 		.
 *   	return 0;
 *	}
 *	@endcode
 */
double ConvDiscPos  (long int disc, long npoints, double start_uod, double stop_uod);

/**
 * 	Singleton Set
 *	@param point position in the universe of discourse
 * 	@param npoints  number of discretization points
 * 	@param start_uod universe of discourse start value
 * 	@param stop_uod universe of discourse stop value
 *  @return vector containing the singleton set
 *  @note This function is a wrapper of the OpenFIS library. Usage:
 *	@code
 *	// discrete points (bigger this value is, more precise will be the output, but the performance (speed) will be decreased
 *	#define DISCRETE_PTS 10000
 *
 *	// must be float point
 *	#define START_UOD 5.0
 *	#define STOP_UOD 45.0
 *  .
 *  .
 *  .
 *  int main (void)
 *	{
 *		double *value;
 *  	.
 *  	.
 * 		.
 *		value = SingletonSet (10.5, DISCRETE_PTS, START_UOD, STOP_UOD);
 *  	.
 *  	.
 * 		.
 *   	return 0;
 *	}
 *	@endcode
 */
double *SigletonSet (double point, long npoints, double start_uod, double stop_uod);

/**
 * 	Calculates the minimum between values
 *	@param value1 float point value 1
 *	@param value2 float point value 2
 *  @return the minimum value
 *  @note This function is a wrapper of the OpenFIS library. Usage:
 *	@code
 *	double value1 = 10.5;
 *  double value2 = 15.6
 *	double min;
 *
 *	min = Minimum (value1, value2);
 *
 *	@endcode
 *  result will be 10.5
 */
double Minimum (double value1, double value2);

/**
 * 	Calculates the maximum between values
 *	@param value1 float point value 1
 *	@param value2 float point value 2
 *  @return the maximum value
 *  @note This function is a wrapper of the OpenFIS library. Usage:
 *	@code
 *	double value1 = 10.5;
 *  double value2 = 15.6
 *	double max;
 *
 *	max = Maximum (value1, value2);
 *
 *	@endcode
 *  result will be 15.6
 */
double Maximum (double value1, double value2);

/**
 * 	Pèrforms the cut operation in a vector (calculates the limiar)
 *	@param set object vector pointer
 * 	@param npoints  number of discretization points
 * 	@param alpha  cut threshold
 * 	@param flag 1 overwrite set, 0 returns value
 *  @return the cut set
 *  @note This function is a wrapper of the OpenFIS library. Usage:
 *	@code
 *	#define DISCRETE_PTS 10000
 *
 *	double *cut_set;
 *	double *temperature;
 *  double alpha = 0.7;
 *
 *	cut_set = Cut (&temperature, DISCRETE_PTS, alpha, 0); // doesn´t overwrite the input vector
 *
 *	@endcode
 */
double *Cut (double **set, long npoints, double alpha, int flag);

#endif
