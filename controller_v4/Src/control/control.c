/*
 * control.c
 *
 *  Created on: Jan 8, 2020
 *      Author: jolen
 */
#include "control/control.h"
#include "control/mc_params.h"
#include "control/torque.h"
#include "main.h"
#include "control/inner.h"
#include "control/outer.h"
#include "control/PWM.h"
#include "modules/log.h"
#include "sensors/PGA411.h"
#include "sensors/amc1106.h"
#include "sensors/HO250S.h"
#include "sensors/thermistor.h"

static void mc_params_init(void);
static void mc_vars_init(void);
static void check_gate_driver_for_fault(void);
static void check_for_faults(void);
/*
 * 	CONTROL SYSTEM INTERRUPT
 * 		This interrupt is called at 16kHz when controller is in enabled state. The code must be
 * 		compiled with speed optimization in order to run the control system at 16kHz. To do this
 * 		go to project settings (right click on project in project explorer) > C/C++ Build >
 * 		Settings. Click on the Tool Settings tab. Choose Optimization under the MCU GCC Complier.
 * 		Change the optimization level to Optimize for speed (-Ofast).
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim == &htim3) {
		// update sensors
		start_iabc_conversions(); 	// start phase current conversion on external ADC
		update_rdc_values(); 		// update wElectrical and motor angle
		update_iabc();		  		// update phase currents
		check_for_iabc_zeros();		// if Iabc < 0.5A, make them zero (remove noise when not switching)

		// control loop
		Torque_step();
		Outer_step();
		Inner_step();
		PWM_TswCalculations();

		// log data
		log_push(&mc_vars);

		// check for gate driver faulats (the other faults are check as you read the sensors)
		check_gate_driver_for_fault();
		if (!mc_vars.fault) {
			HAL_TIM_Base_Start_IT(&htim3);
		}
	}
}

/*
 *  Initializes sensors, parameters, variables, and checks for faults. Used before enabling the controller.
 */
void control_init(void) {
	ho250S_init();
	amc1106_init();
	pga411_Init();
	mc_params_init();
	mc_vars_init();
	Inner_initialize();
	check_for_faults();
}

/*
 * Changes the state of the controller from idle to enable. Starts the PWM outputs to the IGBTs and starts
 * the 16kHz interrupt for the control system.
 */
void control_enable(void) {
	if (!mc_vars.fault) {
		mc_vars.mc_enable = TRUE;
		PWM_initialize(); // initializes gate outputs
		HAL_Delay(1);
		HAL_TIM_Base_Start_IT(&htim3); // starts control system interrupts
	}
}

/*
 * Checks the enable switch from the user to be able to go from idle to enabled state.
 */
void check_enable_switch(void) {
	if (mc_vars.mc_enable == FALSE) {
		/* If in idle state, check if the switch goes from low to high. This ensure that if you start the motor
		 * controller with the switch high that the motor controller won't start without user input.
		 */
		mc_vars.enable_switch_states[PREV_STATE] = mc_vars.enable_switch_states[PRESENT_STATE];
		mc_vars.enable_switch_states[PRESENT_STATE] = HAL_GPIO_ReadPin(MC_ENBL_IN_GPIO_Port, MC_ENBL_IN_Pin);
		if (mc_vars.enable_switch_states[PREV_STATE] == LOW && mc_vars.enable_switch_states[PRESENT_STATE] == HIGH) {
		  check_for_faults();
		  Inner_initialize();
		  control_enable();
		  if (mc_vars.mc_enable==TRUE) {
			  // change LED to enabled (green)
			  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, HIGH);
			  HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, LOW);
		  }
		}
	} else {
		mc_vars.enable_switch_states[PRESENT_STATE] = HAL_GPIO_ReadPin(MC_ENBL_IN_GPIO_Port, MC_ENBL_IN_Pin);
		if (mc_vars.enable_switch_states[1] == LOW) {
			/* If in enabled state, check if the switch is low. If it is the controller will stop PWM outputs to
			 * the gate driver shutting off the motor controller output.
			 */
			mc_vars.mc_enable = FALSE;
		    control_disable();
		    if (!mc_vars.fault) {
		    	// change LED to idle (blue)
		    	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, LOW);
		    	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, HIGH);
		    }
		}
	}
}

/*
 * Disable the controller by stopping the PWM outputs to the gate drivers and stopping the 16kHZ control system
 * interrupt. Used when going from enabled to idle state (by switching the enable switch off) or if a fault is
 * detected when in the enable state.
 */
void control_disable(void) {
	// Disable PWM outputs to gate drivers
	HAL_TIM_Base_Stop(&htim1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
	// Disable control system interrupt
	HAL_TIM_Base_Stop_IT(&htim3);
}

/* Initialize parameters using the parameters uploaded by the GUI.
 * Currently there is not default parameters at these address. Since during testing
 * we are continuously reprogramming the STM32, these parameters are required to be
 * rewritten everytime with the GUI. For now we won't use the addresses and just
 * write the configuration parameters in below.
 */
static void mc_params_init(void){

	mc_params.Pmax = 100; // *((__IO uint32_t *)PMAX_ADDRESS);
	mc_params.Tmax = 75; //225;// *((__IO uint32_t *)TMAX_ADDRESS);
	mc_params.Ld = 76e-6; //*((__IO float *)LD_ADDRESS);
	mc_params.Lq = 79e-6; // *((__IO float *)LQ_ADDRESS);
	mc_params.L0 = 0; //*((__IO float *)L0_ADDRESS);
	mc_params.Rs = 8e-3; //*((__IO float *)RS_ADDRESS);
	mc_params.psim = 0.0355; //*((__IO float *)PSIM_ADDRESS);
	mc_params.p = 10.0;//*((__IO uint32_t *)P_ADDRESS);
	mc_params.motorTemp_max = 120; //*((__IO uint32_t *)MOTORTEMP_MAX_ADDRESS);
	mc_params.motorTemp_corner = 100; //*((__IO uint32_t *)MOTORTEMP_CORNER_ADDRESS);
	mc_params.rpm_max = 6500; //*((__IO uint32_t *)RPM_MAX_ADDRESS);
	mc_params.ke_ll_rpm = 0.0478; //*((__IO float *)KE_LL_RPM_ADDRESS);
	mc_params.fsw = 16.0e3; //*((__IO uint32_t *)FSW_ADDRESS);
	mc_params.bw_idq_hz = 1600; //*((__IO uint32_t *)BW_IDQ_HZ_ADDRESS);
	mc_params.n_rpm = 14; //*((__IO uint32_t *)N_RPM_ADDRESS);
	mc_params.n_tq = 5.5; //*((__IO uint32_t *)N_TQ_ADDRESS);
	mc_params.t_ramp = 0.05; //*((__IO float *)T_RAMP_ADDRESS);
	mc_params.igbtTemp_max = 100; //150; //*((__IO uint32_t *)IGBTTEMP_MAX_ADDRESS);
	mc_params.igbtTemp_corner = 80; //100; //*((__IO uint32_t *)IGBTTEMP_CORNER_ADDRESS);
	mc_params.wi = 628.32; // *((__IO float *)WI_ADDRESS);
	mc_params.wt = 6.2832; //*((__IO float *)WT_ADDRESS);
	mc_params.Vhf_mag = 20; // *((__IO uint32_t *)VHF_MAG_ADDRESS);
	mc_params.Tq_trigger = 100; //*((__IO uint32_t *)TQ_TRIGGER_ADDRESS);
	mc_params.rpm_trigger = 1000; // *((__IO uint32_t *)RPM_TRIGGER_ADDRESS);

	mc_params.Tsw = (float) 1/mc_params.fsw;
	mc_params.Tswh = (float) mc_params.Tsw * 0.5;
	mc_params.Tsw_int = (uint32_t) (mc_params.Tsw * 1.0e9);
	mc_params.Tswh_int = (uint32_t) (mc_params.Tswh * 1.0e9);
	mc_params.Ls = (double) (mc_params.Ld + mc_params.Lq) / 2;
	mc_params.Tsi = (float) mc_params.Tsw/10;
	//mc_params.bw_idq_hz = (int) 1/mc_params.Tsi/100; // check with troy
	mc_params.bw_idq = (float) mc_params.bw_idq_hz * 2 * PI;
	mc_params.wMechanical_Sat = mc_params.Pmax * 1000 / mc_params.Tmax;
	mc_params.PWM_Multiplier = mc_params.fsw / 1e9 * 6749;
	mc_params.resolver_offset = 0.06815331309; // rads
}

/*
 * Initializes variables used in the control system.
 */
static void mc_vars_init(void){
	// init vars to 0 or false
	mc_vars.idq[0] = 0;
	mc_vars.idq[1] = 0;
	mc_vars.idq_ref[0] = 0;
	mc_vars.idq_ref[1] = 0;
	mc_vars.vdq_ref[0] = 0;
	mc_vars.vdq_ref[1] = 0;
	mc_vars.mc_enable = FALSE;
	mc_vars.fault = FALSE;
	mc_vars.fault_reg1 = 0;
	mc_vars.enable_switch_states[PRESENT_STATE] = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9);
}

/*
 * Checks fault pin on the gate driver
 */
static void check_gate_driver_for_fault(void) {
	// gate driver pin is high with no faults
	if(!HAL_GPIO_ReadPin(GATE_DRIVER_FAULT_GPIO_Port, GATE_DRIVER_FAULT_Pin)) {
		mc_vars.fault_reg1 |= 1 << GATE_DRIVER_FAULT_BIT;
	}
}

/*
 * Checks for faults on the things that don't check themselves when read (PGA411 fault registers & gate drivers)
 */
static void check_for_faults(void) {
	pga411_checkFaults();
	check_gate_driver_for_fault();
}
