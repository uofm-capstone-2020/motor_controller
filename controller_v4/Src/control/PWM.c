#include "control/PWM.h"
#include "control/mc_params.h"
#include "control/math_fcns.h"
#include "stm32f7xx_hal.h"
#include "control/inner.h"

extern TIM_HandleTypeDef htim1;
float Va, Vb, Vc;

/*
 * PWM Calculations. Takes in the voltage references and outputs the PWM duty cycles. This is
 * all done in integers to quicken up the processing time. The floats are multiplied by 1e9 to
 * be able to have enough significant figures to complete the calculations. These are converted
 * back when calculating the duty cycles by multiplying by dividing by 1e9.
 */
void PWM_TswCalculations(void)
{
  int Tas,Tbs,Tcs;
  int Tqmax,Tqmin,Tqoffset;
  int Tga_off_h,Tgb_off_h,Tgc_off_h;
  int Tga_on, Tgb_on, Tgc_on;
  int Tga_off, Tgb_off, Tgc_off;

  inverse_park_transform();

  // imaginary switching time
  Tas = mc_params.Tswh_int*Va;
  Tbs = mc_params.Tswh_int*Vb;
  Tcs = mc_params.Tswh_int*Vc;

  // Find max and min
  Tqmax = max(Tas, Tbs, Tcs);
  Tqmin = min(Tas, Tbs, Tcs);

  // Tqoffset calculation
  Tqoffset = (mc_params.Tswh_int - (Tqmax - Tqmin)) / 2 - Tqmin;

  // OFF sequency gating time within half-cycle (Tsw/2) range
  Tga_off_h = Tas + Tqoffset;
  Tgb_off_h = Tbs + Tqoffset;
  Tgc_off_h = Tcs + Tqoffset;

  // ON sequency gating time within half-cycle (Tsw/2) range, which is also the gating time within whole cycle (Tsw) range
  Tga_on = mc_params.Tswh_int - Tga_off_h;
  Tgb_on = mc_params.Tswh_int - Tgb_off_h;
  Tgc_on = mc_params.Tswh_int - Tgc_off_h;

  // OFF sequency gating time within whole cycle (Tsw) range
  Tga_off = Tga_off_h + mc_params.Tswh_int;
  Tgb_off = Tgb_off_h + mc_params.Tswh_int;
  Tgc_off = Tgc_off_h + mc_params.Tswh_int;

  // over modulation (vabc is large, e.g., 220 V peak vabc vs. 400 Vdc for SPWM)
  // (1). correct ON gating times
  // (2). correct OFF gating times
  // 0 < T_on < Tswh
  Tga_on = iSaturate(Tga_on, 0, mc_params.Tsw_int);
  Tgb_on = iSaturate(Tgb_on, 0, mc_params.Tsw_int);
  Tgc_on = iSaturate(Tgc_on, 0, mc_params.Tsw_int);

  // Tswh < T_off < Tsw
  Tga_off = iSaturate(Tga_off,mc_params.Tswh_int,mc_params.Tsw_int);
  Tgb_off = iSaturate(Tgb_off,mc_params.Tswh_int,mc_params.Tsw_int);
  Tgc_off = iSaturate(Tgc_off,mc_params.Tswh_int,mc_params.Tsw_int);

  // calculate duty cycles
  mc_vars.duty_cycles[0] = (uint32_t) (Tga_off - Tga_on) * mc_params.PWM_Multiplier;
  mc_vars.duty_cycles[1] = (uint32_t) (Tgb_off - Tgb_on) * mc_params.PWM_Multiplier;
  mc_vars.duty_cycles[2] = (uint32_t) (Tgc_off - Tgc_on) * mc_params.PWM_Multiplier;


  // update compare registers
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, mc_vars.duty_cycles[0]);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, mc_vars.duty_cycles[1]);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, mc_vars.duty_cycles[2]);

}

/*
 * Initialize PWM module
 */
void PWM_initialize(void)
{
  	// Start PWM outputs
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

	// Set duty cycle to 50%
	mc_vars.duty_cycles[0] = 3375;
	mc_vars.duty_cycles[1] = 3375;
	mc_vars.duty_cycles[2] = 3375;

	// Update compare registers
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, mc_vars.duty_cycles[0]);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, mc_vars.duty_cycles[1]);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, mc_vars.duty_cycles[2]);
}

/*
 * Inverse Park Transform
 */
void inverse_park_transform(void) {
	float theta_plus_2PIdiv3,theta_minus_2PIdiv3;
	float sinArray[3];
	float cosArray[3];
	float Vdc;

	theta_plus_2PIdiv3 = mc_vars.theta_e + TWOPIDIV3;
	if (theta_plus_2PIdiv3 > TWOPI) {
		theta_plus_2PIdiv3 -= TWOPI;
	}
	theta_minus_2PIdiv3 = mc_vars.theta_e - TWOPIDIV3;
	if (theta_minus_2PIdiv3 <0) {
		theta_minus_2PIdiv3 += TWOPI;
	}

	sinArray[0] = sin_interpolation(mc_vars.theta_e);
	sinArray[1] = sin_interpolation(theta_minus_2PIdiv3);
	sinArray[2] = sin_interpolation(theta_plus_2PIdiv3);

	cosArray[0] = cos_interpolation(mc_vars.theta_e);
	cosArray[1] = cos_interpolation(theta_minus_2PIdiv3);
	cosArray[2] = cos_interpolation(theta_plus_2PIdiv3);

	// Avoid dividing future calculations by 0 by making Vdc really small if Vdc = 0
	Vdc = check_zero_Vdc();

	// Va, Vb, Vc are divided by Vdc here to keep the floating point calculation together
	Va = (sinArray[0] * mc_vars.vdq_ref[0] + cosArray[0] * mc_vars.vdq_ref[1])/Vdc;
	Vb = (sinArray[1] * mc_vars.vdq_ref[0] + cosArray[1] * mc_vars.vdq_ref[1])/Vdc;
	Vc = (sinArray[2] * mc_vars.vdq_ref[0] + cosArray[2] * mc_vars.vdq_ref[1])/Vdc;
}

/*
 * Avoid dividing future calculations by 0 by making Vdc really small if Vdc = 0
 */
float check_zero_Vdc(void) {
	if (mc_vars.Vdc <= VERY_SMALL_NUMBER) {
	return VERY_SMALL_NUMBER; // Vdc = 0
	} else {
	return mc_vars.Vdc;
	}
}
