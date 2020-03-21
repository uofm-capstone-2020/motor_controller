#include <control/math_fcns.h>
#include "control/outer.h"
#include "control/mc_params.h"

static void torque_limiter(void);

/*
 * Outer step module. Takes the torque requests and outputs current references.
 */
void Outer_step(void)
{
	// get motor values
	mc_vars.wMechanical = absVal(mc_vars.wElectrical / mc_params.p);
	mc_vars.rpm = mc_vars.wMechanical * THIRTYDIVPI;

	// limit the torque if required
	torque_limiter();

	// calculate Id and Iq references
	mc_vars.idq_ref[0] = 0.0;
	mc_vars.idq_ref[1] = mc_vars.TqRef * 0.66666667 / mc_params.p / mc_params.psim;
}

/*
 * Limit the torque request base on maximum torque set by user in GUI
 */
void torque_limiter(void) {
	float motor_torque;

	// Mechanical frequency saturation
	if (mc_vars.wMechanical < mc_params.wMechanical_Sat) {
		mc_vars.wMechanical = mc_params.wMechanical_Sat;
	}

	// Get Motor Torque
	motor_torque = (float) mc_params.Pmax * 1000 / mc_vars.wMechanical ;

	// Get Minimum of Tmax or TqRef
	if (motor_torque > mc_params.Tmax) {
		motor_torque = mc_params.Tmax;
	}

	// Calculated torque limit
	mc_vars.TqLim = motor_torque ;

	// Saturate torque request if required
	mc_vars.TqRef = fSaturate(mc_vars.TqRequest,-mc_vars.TqLim,mc_vars.TqLim);
}


