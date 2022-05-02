/*
	cpe301, da6, task: 1, 2, 3
	1) Read the IMU
	2) Read IMU, smooth, and plot
	3) Reading the IMU, determine: roll, pitch, yaw -> plot
	
	ICM[gnd] --> MCU[gnd]    // and tie SD0 to gnd also, fixes address
	ICM[VIN] --> MCU[5V]     // package will safely reduce 5V to ICM logic levels
	ICM[SCL] --> MCU[PC5]
	ICM[SDA] --> MCU[PC4]
	
	the "object" is a global instance of a struct, see "ICM20948.h"
 */ 

#include "ICM20948.h" 

bool toggle;
#define PASS_TROUGH 0
#define PRINT_DATA_ONLY 1
#define PLOT_DATA_ONLY 2
#define ROLL_PITCH_YAW 3
int selector = 2;    // pick one

#define SMOOTH 20
xyzFloat smooth_acl[SMOOTH];
xyzFloat smooth_gyr[SMOOTH];
xyzFloat smooth_mag[SMOOTH];

void obj_init();
void print_data_max();
void send_9();
void send_rpy();


int main(void)
{
	toggle = true;
	PORTC |= (1 << 5) | (1 << 4);              // enable pull ups for TWI pins
	i2c_init();							       // initialize TWI
	USART_init(BAUD_PRESCALLER);		       // initialize USART
	USART_tx_string("UART Connected!\r\n");
	_delay_ms(PAUSE_RESET_MS);
	
	// initialize the "object" , calibrates, whoAmI checks
	obj_init();                               
		
	while(1)
	{
		hold :
		if (toggle == false) {goto hold;}
			
		switch (selector)
		{
			case (PRINT_DATA_ONLY) : 
				print_data_max();
				_delay_ms(1000);
				break;
			case (PLOT_DATA_ONLY) :
				send_9();
				break;
			case (ROLL_PITCH_YAW) :
				send_rpy();
				break;
			default :
				toggle = false;
				break;
		}
	}
}


////~~~~


void obj_init()
{
	_delay_ms(1000);
	c_init();                                                                    // the old constructor and initializer
	c_initMagnetometer();                                                        // old magnetometer initializer
	//c_setAccOffsets(-16330.0, 16450.0, -16600.0, 16180.0, -16520.0, 16690.0);    // calibrates
	_delay_ms(1000);                                                             // wait to compare new measurement
	c_autoOffsets();                                                             // user holds keeps device flat and still
	c_setAccRange(ICM20948_ACC_RANGE_2G);                                        // the most reliable, also default
	c_setAccDLPF(ICM20948_DLPF_6);                                               // moderate noise filter
	c_setGyrDLPF(ICM20948_DLPF_6);                                               // same filter for gyro
	c_setTempDLPF(ICM20948_DLPF_6);                                              // same filter for temperature
	c_setMagOpMode(AK09916_CONT_MODE_100HZ);                                     // set the magnetometer
	_delay_ms(1000);
	USART_tx_string("all initialization and calibration is complete\r\n");       // device is ready, automatic averaging
	USART_tx_string("\r\n");
}


////~~~~


void print_data_max()
{
	c_readSensor();
	vars.acl = c_getGValues();
	vars.gyr = c_getGyrValues();
	vars.mag = c_getMagValues();
	vars.celsius = c_getTemperature();
	vars.g_result = c_getResultantG(vars.acl);
	c_getOrientationAsString();
	snprintf(vars.helper, BUF_SIZ-1, "acceleration (g)  [x, y, z]  :  %0.3f , %0.3f , %0.3f  , resultant:  %0.3f m/s^2\r\n",
		vars.acl.x, vars.acl.y, vars.acl.z, 9.8*vars.g_result);
	USART_tx_string(vars.helper);
	snprintf(vars.helper, BUF_SIZ-1, "gyroscope (deg)   [x, y, z]  :  %0.3f , %0.3f , %0.3f\r\n", vars.gyr.x, vars.gyr.y, vars.gyr.z);
	USART_tx_string(vars.helper);
	snprintf(vars.helper, BUF_SIZ-1, "magnetometer (uT) [x, y, z]  :  %0.3f , %0.3f , %0.3f\r\n", vars.mag.x, vars.mag.y, vars.mag.z);
	USART_tx_string(vars.helper);
	snprintf(vars.helper, BUF_SIZ-1, "temperature (celsius)        :  %0.3f\r\n", vars.celsius);
	USART_tx_string(vars.helper);
	snprintf(vars.helper, BUF_SIZ-1, "orientation                  :  %s\r\n", vars.str_orientation);
	USART_tx_string(vars.helper);
	USART_tx_string("\r\n");
}


////~~~


void send_9()
{
	vars.acl.x = 0;
	vars.acl.y = 0;
	vars.acl.z = 0;
	vars.gyr.x = 0;
	vars.gyr.y = 0;
	vars.gyr.z = 0;
	vars.mag.x = 0;
	vars.mag.y = 0;
	vars.mag.z = 0;
	for (int ii = 0; ii < SMOOTH; ii++)
	{
		c_readSensor();
		smooth_acl[ii] = c_getGValues();
		smooth_gyr[ii] = c_getGyrValues();
		smooth_mag[ii] = c_getMagValues();
		vars.acl.x = vars.acl.x + smooth_acl[ii].x;
		vars.acl.y = vars.acl.y + smooth_acl[ii].y;
		vars.acl.z = vars.acl.z + smooth_acl[ii].z;
		vars.gyr.x = vars.gyr.x + smooth_gyr[ii].x;
		vars.gyr.y = vars.gyr.y + smooth_gyr[ii].y;
		vars.gyr.z = vars.gyr.z + smooth_gyr[ii].z;
		vars.mag.x = vars.mag.x + smooth_mag[ii].x;
		vars.mag.y = vars.mag.y + smooth_mag[ii].y;
		vars.mag.z = vars.mag.z + smooth_mag[ii].z;
	}
	vars.acl.x = vars.acl.x / SMOOTH;
	vars.acl.y = vars.acl.y / SMOOTH;
	vars.acl.z = vars.acl.z / SMOOTH;
	vars.gyr.x = vars.gyr.x / SMOOTH;
	vars.gyr.y = vars.gyr.y / SMOOTH;
	vars.gyr.z = vars.gyr.z / SMOOTH;
	vars.mag.x = vars.mag.x / SMOOTH;
	vars.mag.y = vars.mag.y / SMOOTH;
	vars.mag.z = vars.mag.z / SMOOTH;
	
	snprintf(vars.helper, BUF_SIZ-1, "%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n",
		vars.acl.x, vars.acl.y, vars.acl.z,
		vars.gyr.x, vars.gyr.y, vars.gyr.z,
		vars.mag.x, vars.mag.y, vars.mag.z);
	USART_tx_string(vars.helper);
}


////~~~~


void send_rpy()
{
	float catch_roll[SMOOTH];
	float catch_pitch[SMOOTH];
	float catch_yaw[SMOOTH];
	float h_x;
	float h_y;
	float sum_roll = 0;
	float sum_pitch = 0;
	float sum_yaw = 0;
	
	for (int ii = 0; ii < SMOOTH; ii++)
	{
		c_readSensor();
		catch_roll[ii] = c_getRoll();
		catch_pitch[ii] = c_getPitch();
		smooth_mag[ii] = c_getMagValues();
		
		h_y = (smooth_mag[ii].y * cos(catch_roll[ii]) - 
			smooth_mag[ii].z * sin(catch_roll[ii]));
		
		h_x = (smooth_mag[ii].x * cos(catch_pitch[ii]) + 
			smooth_mag[ii].y * sin(catch_roll[ii]) * sin(catch_pitch[ii]) + 
			smooth_mag[ii].z * cos(catch_roll[ii]) * sin(catch_pitch[ii]));
		
		catch_yaw[ii] = 57.3 * atan2(h_y, h_x);
		sum_roll = sum_roll + catch_roll[ii];
		sum_pitch = sum_pitch + catch_pitch[ii];
		sum_yaw = sum_yaw + catch_yaw[ii];
	}
	vars.roll = sum_roll / SMOOTH;
	vars.pitch = sum_pitch / SMOOTH;
	vars.yaw = sum_yaw / SMOOTH;
	
	snprintf(vars.helper, BUF_SIZ-1, "%f,%f,%f\r\n",
	vars.roll, vars.pitch, vars.yaw);
	USART_tx_string(vars.helper);
}


////////~~~~~~~~END>  main.c
