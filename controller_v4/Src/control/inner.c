#include "control/inner.h"
#include "control/math_fcns.h"
#include "control/mc_params.h"

void parkTransform(void);
void PI_controllers(void);
void vdq_saturation(void);
void init_PI_constants(void);
void getSinValues(float* sinArray);
void getCosValues(float* cosArray);

PI_variables pi_id;
PI_variables pi_iq;

/*
 * Initializes inner control step module
 */
void Inner_initialize(void)
{
	init_PI_constants();

}

/*
 * Inner step module called every 16kHz by the control system interrupt. Takes the current
 * references and outputs the voltage references.
 */
void Inner_step(void) {
	parkTransform();
	PI_controllers();
	vdq_saturation();
}

/*
 * Park Transform function
 */
void parkTransform(void) {
	float sinArray[3], cosArray[3];

	getSinValues(sinArray);
	getCosValues(cosArray);

	mc_vars.idq[0] = (sinArray[0]*mc_vars.iabc[0] + sinArray[1]*mc_vars.iabc[1] +
						sinArray[2]*mc_vars.iabc[2]) * 0.66666666;
	mc_vars.idq[1] = (cosArray[0]*mc_vars.iabc[0] + cosArray[1]*mc_vars.iabc[1] +
						cosArray[2]*mc_vars.iabc[2]) * 0.66666666;
}

/*
 * PI Controller for Id and Iq vectors.
 */
void PI_controllers(void) {
	// PI Id
	pi_id.error = mc_vars.idq_ref[0] - mc_vars.idq[0]; // error of Id
	pi_id.integral = (pi_id.Ki * pi_id.error + pi_id.Kaw * pi_id.du) *
						mc_params.Tsw + pi_id.integral; // Discrete Time Integrator
	pi_id.output = pi_id.Kp * pi_id.error + pi_id.integral;

	// PI Iq
	pi_iq.error = mc_vars.idq_ref[1] - mc_vars.idq[1]; // error of Iq
	pi_iq.integral = (pi_iq.Ki * pi_iq.error + pi_iq.Kaw * pi_iq.du) *
						mc_params.Tsw + pi_iq.integral; // Discrete Time Integrator
	pi_iq.output = pi_iq.Kp * pi_iq.error + pi_iq.integral;

}

/*
 * Vd and Vq saturation
 */
void vdq_saturation(void) {
	float vdFF,vqFF;
	float vdRefUnsat, vqRefUnsat;
	float vdRefMax,vqRefMax, magnitude, VphMax;

	// Feed Forward Pre-control
	vdFF =  mc_vars.idq[1] * mc_params.Lq * mc_vars.wElectrical;
	vqFF =  mc_vars.idq[0] * mc_params.Ld * mc_vars.wElectrical +  mc_params.psim * mc_vars.wElectrical ;
	// SATURATION - note that our model is Q axis prioritized, see https://www.mathworks.com/help/physmod/sps/ref/dqvoltagelimiter.html
	// Vq reference saturation (Saturation v3 in Simulink)
	vdRefUnsat = pi_id.output - vdFF;
	vqRefUnsat = pi_iq.output + vqFF;
	magnitude= sqrt(vqRefUnsat*vqRefUnsat+vdRefUnsat*vdRefUnsat);
	if (magnitude == 0) { // avoid division by 0
		magnitude = 0.001;
	}

	VphMax = ONEDIVSQRT3 * mc_vars.Vdc;
	vdRefMax= absVal(vdRefUnsat)*VphMax/magnitude;
	mc_vars.vdq_ref[0] = fSaturate(vdRefUnsat, -vdRefMax, vdRefMax);

	vqRefMax= absVal(vqRefUnsat)*VphMax/magnitude;
	mc_vars.vdq_ref[1] = fSaturate(vqRefUnsat, -vqRefMax, vqRefMax);

	// Set du's for next inner loop
	pi_id.du = mc_vars.vdq_ref[0] - vdRefUnsat;
	pi_iq.du = mc_vars.vdq_ref[1] - vqRefUnsat;

}

/*
 * Initialize PI constants using parameters written by the GUI. Called upon initializing the
 * control system.
 */
void init_PI_constants(void) {
	//  PI controller values
	float Ka_id = mc_params.Ld * mc_params.bw_idq;
	float Ka_iq = mc_params.Lq * mc_params.bw_idq;
	float Kb_id = mc_params.Rs / mc_params.Ld;
	float Kb_iq = mc_params.Rs / mc_params.Lq;

	//Parallel PI controller values
	pi_id.Kp = Ka_id;     		// Proportional gain id controller
	pi_id.Ki = Kb_id * Ka_id; 	// Integrator gain id controller
	pi_iq.Kp = Ka_iq;     		// Proportional gain iq controller
	pi_iq.Ki = Kb_iq * Ka_iq; 	// Integrator gain iq controller

	pi_id.Kaw = pi_id.Ki;
	pi_iq.Kaw = pi_iq.Ki;

	pi_id.integral = 0.0;
	pi_iq.integral = 0.0;
	pi_id.du = 0.0;
	pi_iq.du = 0.0;
}

/*
 * Get the sin values given the current theta value. Used by the park transform and the inverse
 * park transform. Interpolates between sin values given in a sin look up table of 1000 points.
 */
void getSinValues(float* sinArray) {
	sinArray[0] = sin_interpolation(mc_vars.theta_e);
	sinArray[1] = sin_interpolation(mc_vars.theta_minus_2PIdiv3);
	sinArray[2] = sin_interpolation(mc_vars.theta_plus_2PIdiv3);
}

/*
 * Get the cos values given the current theta value. Used by the park transform and the inverse
 * park transform. Interpolates between cos values given in a cos look up table of 1000 points.
 */
void getCosValues(float* cosArray) {
	cosArray[0] = cos_interpolation(mc_vars.theta_e);
	cosArray[1] = cos_interpolation(mc_vars.theta_minus_2PIdiv3);
	cosArray[2] = cos_interpolation(mc_vars.theta_plus_2PIdiv3);
}
