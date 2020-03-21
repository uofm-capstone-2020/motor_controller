#ifndef __MC_PARAMS_H
#define __MC_PARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define HIGH 1
#define LOW 0
#define TRUE 1
#define FALSE 0
#define VERY_SMALL_NUMBER 2.2204460492503131e-16
#define PI 3.14159265359
#define THIRTYDIVPI 9.54929658551
#define TWOPIDIV3 2.09439510239
#define MULTIPLIER 15.7563393661
#define ONEDIVSQRT3 0.57735026919
#define TWOPI 6.28318530718
#define TWOPIDV4096 0.00153398078
#define SQRT3 1.73205080757
#define MAXCHAR 1000
#define ANGLE_MULTIPLIER 0.1943453543

typedef struct MotorController_Parameters {
    float fsw; 		// Switching frequency [Hz]
    uint32_t Pmax;	// Maximum power output [kW]
    uint32_t Tmax;	// Maximum torque to motor [Nm]
    float Ld;		// Stator d-axis inductance [H]
    float Lq;		// Stator q-axis inductance [H]
    float L0;		// Stator zero-sequence inductance [H]
    float Rs;		// Stator resistance per phase [ohm]
    float psim;		// Permanent magnet flux linkage [Wb]
    float p;		// Number of pole pairs of motor
    uint32_t motorTemp_max;		// deg C
    uint32_t motorTemp_corner;	// deg C
    uint32_t rpm_max;			// max rotational speed of the motor [RPM]
    float ke_ll_rpm;			// Back emf constant [Vrmsll/rpm]
    uint32_t bw_idq_hz;			// Inner current loop bandwidth [Hz]
    uint32_t n_rpm;
    uint32_t n_tq;
    float t_ramp;				// maximum torque ramp
    uint32_t igbtTemp_max;
    uint32_t igbtTemp_corner;
    float wi;
    float wt;
    uint32_t Vhf_mag;

    double Ls;
    float Tsw;    				// switching period (1/fsw)
    float Tswh;   				// half of the switching period
    uint32_t Tsw_int;    		// switching period (1/fsw) * 1e9
    uint32_t Tswh_int;   		// half of the switching period * 1e9
    uint32_t Tq_trigger;
    uint32_t rpm_trigger;

    double Tsi; 				// controller interrupt period
    double bw_idq;
    double wMechanical_Sat;		// mechanical frequency saturation [rad/s]
    float PWM_Multiplier;		// PWM multilier to change the int switching times to duty cylcles
    uint32_t resolver_offset;	// resolver zero difference from phase A of motor zero
} MotorController_Parameters;

typedef struct MotorController_Variables {
    // these are already integrated
	float idq_ref[2];		// Id and Iq current references [A]
    float wElectrical;		// Motor electrical frequency [rad/s]
    float iabc[3];			// phase currents [A]
    float theta_e;			// electrical angle [rad]
    float Vdc;				// DC voltage [V]
    float vdq_ref[2];		// Vd and Vq voltage references [V]
    float idq[2];			// Measured Id and Iq currents [A]
    float TqRef;			// Torque reference [Nm]
    float TqLim;			// Torque limit [Nm]
    float TqRequest;		// Torque request from VCU [Nm]
    float wMechanical; 		// Motor mechanical frequency [rad/s]
    float rpm; 				// Motor rpm
    float motorTemp;
    float igbtTemp;
    uint32_t duty_cycles[3];// duty cylcle of pwm outputs to gate drivers
    float TqRequest_prev;	// previous torque request [Nm] used for torque ramp calculations
    float TqMax;			// Maximum torque
    int mc_enable;			// Used to keep track of if MC is in enable state
    int8_t can_watchdog;    // -1 = error, is set to 0 on CAN in. Not currently used
    float power_out;		// not currently used
    uint8_t fault;			// Used to keep track of if MC is in fault state
    uint8_t enable_switch_states[2]; // Used to keep track of previous and current enable switch position
    uint16_t fault_reg1;	// Fault register bits correspond to what fault occured (see below)
    float fault_current[3];	// If a phase current fault occurs, this records the fault current
    float bat_voltage;		// LV battery voltage
    float vll[2];			// line to line voltages. Not currently used
    float theta_plus_2PIdiv3;
    float theta_minus_2PIdiv3;
} MotorController_Variables;

// currently just stores the resolver to digital fault registers
typedef struct MotorController_Faults {
	uint16_t 	rdc_faults_reg1;
	uint16_t 	rdc_faults_reg2;
	uint16_t 	rdc_faults_reg3;
	uint16_t 	rdc_faults_reg4;
} MotorController_Faults;

// mc_vars.fault_reg1
#define HIGH_VDC_FLAG_BIT 				0
#define DFSDM_TIME_OUT_FLAG_BIT			1
#define HIGH_IA_FLAG_BIT 				2
#define HIGH_IB_FLAG_BIT 				3
#define HIGH_IC_FLAG_BIT 				4
#define IABC_SPI_FLAG_BIT				5
#define RDC_CRC_FLAG_BIT				6
#define RDC_FAULT_REG_FLAG_BIT			7
#define HIGH_MOTOR_TEMP_FLAG_BIT 		8
#define HIGH_IGBT_TEMP_FLAG_BIT 		9
#define IGBT_THERMISTOR_FLAG_BIT		10
#define ADC_FLAG_BIT					11
#define GATE_DRIVER_FAULT_BIT	 		12
#define LOW_BATTERY_FAULT_BIT	 		13
#define HARD_FAULT_BIT					14

// not currently used
#define VLL_MAG_HIGH 					15
#define VLL_MAG_LOW 					15

// Flash addresses to store MC parameters written by GUI
#define PMAX_ADDRESS                0x081FFFFB
#define TMAX_ADDRESS                0x081FFFF7
#define LD_ADDRESS                  0x081FFFF3
#define LQ_ADDRESS                  0x081FFFEF
#define L0_ADDRESS                  0x081FFFEB
#define RS_ADDRESS                  0x081FFFE7
#define PSIM_ADDRESS                0x081FFFE3
#define P_ADDRESS                   0x081FFFDF
#define MOTORTEMP_MAX_ADDRESS       0x081FFFDB
#define MOTORTEMP_CORNER_ADDRESS    0x081FFFD7
#define RPM_MAX_ADDRESS             0x081FFFD3
#define KE_LL_RPM_ADDRESS           0x081FFFCF
#define FSW_ADDRESS                 0x081FFFCB
#define BW_IDQ_HZ_ADDRESS           0x081FFFC7
#define N_RPM_ADDRESS               0x081FFFC3
#define N_TQ_ADDRESS                0x081FFFBF
#define T_RAMP_ADDRESS              0x081FFFBB
#define IGBTTEMP_MAX_ADDRESS        0x081FFFB7
#define IGBTTEMP_CORNER_ADDRESS     0x081FFFB3
#define WI_ADDRESS                  0x081FFFAF
#define WT_ADDRESS                  0x081FFFAB
#define VHF_MAG_ADDRESS             0x081FFFA7
#define TQ_TRIGGER_ADDRESS          0x081FFFA3
#define RPM_TRIGGER_ADDRESS         0x081FFF9F


#ifdef __cplusplus
}

#endif

#endif /* __MC_PARAMS_H */


MotorController_Variables mc_vars;
MotorController_Parameters mc_params;
MotorController_Faults mc_faults;
