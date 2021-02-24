#ifndef __openfuzz_h__
#define __openfuzz_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>

#pragma once

#define FALSE           0
#define TRUE            1

/**
 * 	Fuzzy set struct
 * 	@param value membership pointer
 * 	@param nsets number of sets
 * 	@param npoints number of discretization points
 * 	@param start_uod start of universe of discourse
 * 	@param stop_uod stop of universe of discourse
 */
extern struct SSets
{
      double *value;	// membership value
      int nsets;			// number of sets
      long npoints;		// number of discretization points
      double start_uod;  // uod = universe of discourse
      double stop_uod;
} InfoSet;

#include "defuzzy.h"
#include "fisutils.h"
#include "implications.h"


#endif
