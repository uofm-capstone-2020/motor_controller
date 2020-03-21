typedef struct {
	float error;
	float integral;
	float output;
	float du;
	float Kp;
	float Ki;
	float Kaw;
} PI_variables;

void Inner_initialize(void);
void Inner_step(void);
