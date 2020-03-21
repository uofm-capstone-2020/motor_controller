#include "main.h"
#include "control/mc_params.h"
#include "control/torque.h"

static float tq_saturation(float Tq);
static float rate_limiter(void);

/*
 * Control system torque step. Takes in torque requests and limits them in two way. First torque requests
 * are limited if the IGBT or motor temperatures are beyond their corner temperatures. Secondly it limits
 * how fast the torque can increase by t_ramp, a parameter set in the GUI.
 */
void Torque_step(void)
{
  float Tq_limited;

  // Rate Limiter
  Tq_limited = rate_limiter();

  // Saturation
  mc_vars.TqRef = tq_saturation(Tq_limited);

  mc_vars.TqRequest_prev = mc_vars.TqRef;
}

/*
 * Limit how fast the torque can increase
 */
float rate_limiter(void) {
	float TqRate;

	TqRate = mc_vars.TqRequest - mc_vars.TqRequest_prev;
	TqRate = fSaturate(TqRate, -mc_params.t_ramp,mc_params.t_ramp);

	return mc_vars.TqRequest_prev + TqRate;
}

/*
 * Limit torque depending on motor and IGBT temperatures.
 */
float tq_saturation(float Tq) {
	float motorTemp_diff, motorTemp_diff_max;
	float igbtTemp_diff, igbtTemp_diff_max;
	float T1, T2;

	motorTemp_diff = mc_vars.motorTemp - mc_params.motorTemp_corner;
	motorTemp_diff_max = mc_params.motorTemp_max-mc_params.motorTemp_corner;
	motorTemp_diff = fSaturate(motorTemp_diff, 0, motorTemp_diff_max);
	T1 = 1 - motorTemp_diff / motorTemp_diff_max;

	igbtTemp_diff = mc_vars.igbtTemp - mc_params.igbtTemp_corner;
	igbtTemp_diff_max = mc_params.igbtTemp_max-mc_params.igbtTemp_corner;
	igbtTemp_diff = fSaturate(igbtTemp_diff, 0, igbtTemp_diff_max);
	T2 = 1 - igbtTemp_diff / igbtTemp_diff_max;

	mc_vars.TqMax = T1*T2*mc_params.Tmax;

	return fSaturate(Tq,-mc_vars.TqMax,mc_vars.TqMax);
}

