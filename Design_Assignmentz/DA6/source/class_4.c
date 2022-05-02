#include "ICM20948.h"
    
float c_getResultantG(xyzFloat gVal)
{
	float resultant = 0.0;
	resultant = sqrt(pow(gVal.x, 2) + pow(gVal.y, 2) + pow(gVal.z, 2));
	return resultant;
}


////~~~~


float c_getTemperature()
{
	int16_t rawTemp = (int16_t)(((vars.buffer[12]) << 8) | vars.buffer[13]);
	float tmp = (rawTemp*1.0 - ICM20948_ROOM_TEMP_OFFSET)/ICM20948_T_SENSITIVITY + 21.0;
	return tmp;
}


////~~~~


xyzFloat c_getCorrectedGyrRawValues()
{
	xyzFloat gyrRawVal = c_getGyrRawValues();
	gyrRawVal = c_correctGyrRawValues(gyrRawVal);
	return gyrRawVal;
}


////~~~~


xyzFloat c_correctGyrRawValues(xyzFloat gyrRawVal)
{
	gyrRawVal.x -= (vars.gyrOffsetVal.x / vars.gyrRangeFactor);
	gyrRawVal.y -= (vars.gyrOffsetVal.y / vars.gyrRangeFactor);
	gyrRawVal.z -= (vars.gyrOffsetVal.z / vars.gyrRangeFactor);
	
	return gyrRawVal;
}


////~~~~


xyzFloat c_getGyrValues()
{
	xyzFloat gyrVal = c_getCorrectedGyrRawValues();
	
	gyrVal.x = gyrVal.x * vars.gyrRangeFactor * 250.0 / 32768.0;
	gyrVal.y = gyrVal.y * vars.gyrRangeFactor * 250.0 / 32768.0;
	gyrVal.z = gyrVal.z * vars.gyrRangeFactor * 250.0 / 32768.0;
	
	return gyrVal;
}


////~~~~


xyzFloat c_getGyrValuesFromFifo()
{
	xyzFloat gyrVal;
	xyzFloat gyrRawVal = c_readICM20948xyzValFromFifo();
	
	gyrRawVal = c_correctGyrRawValues(gyrRawVal);
	gyrVal.x = gyrRawVal.x * vars.gyrRangeFactor * 250.0 / 32768.0;
	gyrVal.y = gyrRawVal.y * vars.gyrRangeFactor * 250.0 / 32768.0;
	gyrVal.z = gyrRawVal.z * vars.gyrRangeFactor * 250.0 / 32768.0;
	
	return gyrVal;
}


////~~~~


xyzFloat c_getMagValues()
{
	int16_t x,y,z;
	xyzFloat mag;
	
	x = (int16_t)((vars.buffer[15]) << 8) | vars.buffer[14];
	y = (int16_t)((vars.buffer[17]) << 8) | vars.buffer[16];
	z = (int16_t)((vars.buffer[19]) << 8) | vars.buffer[18];
	
	mag.x = x * AK09916_MAG_LSB;
	mag.y = y * AK09916_MAG_LSB;
	mag.z = z * AK09916_MAG_LSB;
	
	return mag;
}


////~~~~


void c_enableCycle(ICM20948_cycle cycle)
{
	vars.regVal = c_readRegister8(0, ICM20948_LP_CONFIG);
	vars.regVal &= 0x0F;
	vars.regVal |= cycle;
	
	c_writeRegister8(0, ICM20948_LP_CONFIG, vars.regVal);
}


////~~~~


void c_enableLowPower(bool enLP)
{
	vars.regVal = c_readRegister8(0, ICM20948_PWR_MGMT_1);
	if(enLP)
	{
		vars.regVal |= ICM20948_LP_EN;
	}
	else
	{
		vars.regVal &= ~ICM20948_LP_EN;
	}
	c_writeRegister8(0, ICM20948_PWR_MGMT_1, vars.regVal);
}


////~~~~


void c_setGyrAverageInCycleMode(ICM20948_gyroAvgLowPower avg)
{
	c_writeRegister8(2, ICM20948_GYRO_CONFIG_2, avg);
}


////~~~~


void c_setAccAverageInCycleMode(ICM20948_accAvgLowPower avg)
{
	c_writeRegister8(2, ICM20948_ACCEL_CONFIG_2, avg);
}


////~~~~


xyzFloat c_getAngles()
{
	xyzFloat angleVal;
	xyzFloat gVal = c_getGValues();
	if(gVal.x > 1.0)
	{
		gVal.x = 1.0;
	}
	else if(gVal.x < -1.0)
	{
		gVal.x = -1.0;
	}
	angleVal.x = (asin(gVal.x)) * 57.296;
	//
	if(gVal.y > 1.0)
	{
		gVal.y = 1.0;
	}
	else if(gVal.y < -1.0)
	{
		gVal.y = -1.0;
	}
	angleVal.y = (asin(gVal.y)) * 57.296;
	//
	if(gVal.z > 1.0)
	{
		gVal.z = 1.0;
	}
	else if(gVal.z < -1.0)
	{
		gVal.z = -1.0;
	}
	angleVal.z = (asin(gVal.z)) * 57.296;
	//
	return angleVal;
}


////~~~~


ICM20948_orientation c_getOrientation()
{
	xyzFloat angleVal = c_getAngles();
	ICM20948_orientation orientation = ICM20948_FLAT;
	if(abs(angleVal.x) < 45){      // |x| < 45
		if(abs(angleVal.y) < 45){      // |y| < 45
			if(angleVal.z > 0){          //  z  > 0
				orientation = ICM20948_FLAT;
			}
			else{                        //  z  < 0
				orientation = ICM20948_FLAT_1;
			}
		}
		else{                         // |y| > 45
			if(angleVal.y > 0){         //  y  > 0
				orientation = ICM20948_XY;
			}
			else{                       //  y  < 0
				orientation = ICM20948_XY_1;
			}
		}
	}
	else{                           // |x| >= 45
		if(angleVal.x > 0){           //  x  >  0
			orientation = ICM20948_YX;
		}
		else{                       //  x  <  0
			orientation = ICM20948_YX_1;
		}
	}
	return orientation;
}


////~~~~


void c_getOrientationAsString()
{
	ICM20948_orientation orientation = c_getOrientation();
	memset(vars.str_orientation, '\0', BUF_SIZ);
	switch(orientation){
		case ICM20948_FLAT   : snprintf(vars.str_orientation, BUF_SIZ-1, "z up");    break;
		case ICM20948_FLAT_1 : snprintf(vars.str_orientation, BUF_SIZ-1, "z down");  break;
		case ICM20948_XY     : snprintf(vars.str_orientation, BUF_SIZ-1, "y up");    break;
		case ICM20948_XY_1   : snprintf(vars.str_orientation, BUF_SIZ-1, "y down");  break;
		case ICM20948_YX     : snprintf(vars.str_orientation, BUF_SIZ-1, "x up");    break;
		case ICM20948_YX_1   : snprintf(vars.str_orientation, BUF_SIZ-1, "x down");  break;
		default              : snprintf(vars.str_orientation, BUF_SIZ-1, "unknown"); break;
	}
}


////////~~~~~~~~END>  free_candy_4.c
