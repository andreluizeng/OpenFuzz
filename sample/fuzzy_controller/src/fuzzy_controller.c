#include <stdio.h>

#include "openfuzz.h"

// limit values for fuzzy memberships
// temperature
#define TEMP_COLD	0
#define TEMP_WARM	1
#define TEMP_HOT	2

// ranges for cold temperature membership
#define START_COLD	5.0
#define MID_COLD	5.0
#define END_COLD	28.0

// ranges for warm temperature membership
#define START_WARM	25.0
#define MID_WARM	28.5
#define END_WARM	35.0

// ranges for hot temperature membership
#define START_HOT	30.0
#define MID_HOT		45.0
#define END_HOT		45.0

// controller
#define	CONTROL_MIN 0
#define	CONTROL_MED 1
#define	CONTROL_MAX 2

// ranges for minimum control membership
#define START_MIN	0.0
#define MID_MIN		0.0
#define END_MIN		20.0

// ranges for medium control membership
#define START_MED	20.0
#define MID_MED		40.0
#define END_MED		70.0

// ranges for maximum control membership
#define START_MAX	50.0
#define MID_MAX		100.0
#define END_MAX		100.0

// temperature set
struct SSets *temperature;
struct SSets *dutycycle_control;

// discrete points
#define DISCRETE_PTS 10000

// discrete fuzzy response
double *fuzzy_resp;

// Menu Function
int Menu (void);

int main (int argc, char **argv)
{
	double temp_value = 0;
	double output_value = 0;
	
	// initialize fuzzy sets   (set name, number of memberships, discrete points, min range value, max range value, initial values for the vector)
    InitializeSets (&temperature,  3, DISCRETE_PTS, 5.0, 45.0, 0.0);
    InitializeSets (&dutycycle_control,  3, DISCRETE_PTS, 0.0, 100.0, 0.0);

	// allocates memory for the fuzzy discretization map
	fuzzy_resp = (double *) malloc (sizeof (double) * DISCRETE_PTS);
    for (int i = 0;  i < DISCRETE_PTS; i++) fuzzy_resp[i] = 0;

	// membership functions fuzzyfication - using triangular function type (could be trapezoidal or gaussian)
	Fuzzification (&temperature[TEMP_COLD],	TRIANGULAR, START_COLD, MID_COLD, END_COLD);
	Fuzzification (&temperature[TEMP_WARM],	TRIANGULAR, START_WARM, MID_WARM, END_WARM);
	Fuzzification (&temperature[TEMP_HOT],	TRIANGULAR, START_HOT,  MID_HOT,  END_HOT);

	Fuzzification (&dutycycle_control[CONTROL_MIN], TRIANGULAR, START_MIN, MID_MIN, END_MIN);
	Fuzzification (&dutycycle_control[CONTROL_MED], TRIANGULAR, START_MED, MID_MED, END_MED);
	Fuzzification (&dutycycle_control[CONTROL_MAX], TRIANGULAR, START_MAX, MID_MAX, END_MAX);

	int menu_resp = 0;
	while (menu_resp != 2)
	{
		menu_resp = Menu ();

		if (menu_resp == 1)
		{
			printf ("\nTemperature: ");
			scanf ("%lf", &temp_value);
			memset ((double *) fuzzy_resp, 0, DISCRETE_PTS * sizeof (double));

			// simple test rules
			// 1 - if temperature is cold, then we do the minimum control (we are not going to warm the water if the user doesn´t want it)
			FuzzyIfInput1 (temperature, TEMP_COLD, temp_value, dutycycle_control, CONTROL_MIN, MANDANI, &fuzzy_resp);

			// 2 - if temperature is warm, then we enable a medium control (warm it a bit =))
			FuzzyIfInput1 (temperature, TEMP_WARM, temp_value, dutycycle_control, CONTROL_MED, MANDANI, &fuzzy_resp);

			// 3 - if user wants a hot temperature, then we enable a max control
			FuzzyIfInput1 (temperature, TEMP_HOT, temp_value, dutycycle_control, CONTROL_MAX, MANDANI, &fuzzy_resp);

			// defuzzy the output
			output_value = DeFuzzy (fuzzy_resp, dutycycle_control, COA);

			printf ("\nControl value: %lf\n", output_value);
		}
		else
		{
			break;
		}
	}

	free (fuzzy_resp);
	return 0;
}

int Menu (void)
{
	int option;

	printf ("\n-=-=-=-=- MENU -=-=-=-=-\n");
	printf ("\n1) calculate control output");
	printf ("\n2) finish application\n");
	printf ("\noption: ");

	scanf ("%d", &option);

	return option;
}

