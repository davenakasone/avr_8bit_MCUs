#include "ICM20948.h"
    

void c_autoOffsets()
{
	xyzFloat accRawVal;
	xyzFloat gyrRawVal;
	vars.accOffsetVal.x = 0.0;
	vars.accOffsetVal.y = 0.0;
	vars.accOffsetVal.z = 0.0;
	
	c_setGyrDLPF(ICM20948_DLPF_6); // lowest noise
	c_setGyrRange(ICM20948_GYRO_RANGE_250); // highest resolution
	c_setAccRange(ICM20948_ACC_RANGE_2G);
	c_setAccDLPF(ICM20948_DLPF_6);
	_delay_ms(PAUSE_RESET_MS);
	
	for(int i=0; i<MOV_AVG; i++)
	{
		c_readSensor();
		accRawVal = c_getAccRawValues();
		vars.accOffsetVal.x += accRawVal.x;
		vars.accOffsetVal.y += accRawVal.y;
		vars.accOffsetVal.z += accRawVal.z;
		_delay_ms(PAUSE_RESET_MS);
	}
	vars.accOffsetVal.x /= MOV_AVG;
	vars.accOffsetVal.y /= MOV_AVG;
	vars.accOffsetVal.z /= MOV_AVG;
	vars.accOffsetVal.z -= 16384.0;
	
	for(int i=0; i<MOV_AVG; i++){
		c_readSensor();
		gyrRawVal = c_getGyrRawValues();
		vars.gyrOffsetVal.x += gyrRawVal.x;
		vars.gyrOffsetVal.y += gyrRawVal.y;
		vars.gyrOffsetVal.z += gyrRawVal.z;
		_delay_ms(PAUSE_RESET_MS);
	}
	vars.gyrOffsetVal.x /= MOV_AVG;
	vars.gyrOffsetVal.y /= MOV_AVG;
	vars.gyrOffsetVal.z /= MOV_AVG;
}


////~~~~


void c_readSensor()
{
	c_readAllData(vars.buffer);
}


////~~~~


void c_readAllData(uint8_t* data)
{
	vars.currentBank = 0;
	i2c_start((vars.i2cAddress << 1) + I2C_WRITE);  
	i2c_write(ICM20948_REG_BANK_SEL);                 
	i2c_write(vars.currentBank << 4);           	  
	i2c_stop();    
	
	i2c_start((vars.i2cAddress << 1) + I2C_WRITE);
	i2c_write(ICM20948_ACCEL_OUT);
	i2c_start((vars.i2cAddress << 1) + I2C_READ);                                                       
	for (int ii = 0; ii < UINT8_BUF; ii++)
	{
		if (ii == UINT8_BUF-1)
		{
			data[ii] = i2c_readNak();
		}
		else
		{
			data[ii] = i2c_readAck();	
		}
		//data[ii] = c_readRegister8(0, ICM20948_ACCEL_OUT+ii);
	}
	//i2c_stop();
}


////~~~~


void c_setGyrDLPF(ICM20948_dlpf dlpf)
{
	vars.regVal = c_readRegister8(2, ICM20948_GYRO_CONFIG_1);
	if(dlpf==ICM20948_DLPF_OFF)
	{
		vars.regVal &= 0xFE;
		c_writeRegister8(2, ICM20948_GYRO_CONFIG_1, vars.regVal);
		return;
	}
	else
	{
		vars.regVal |= 0x01;
		vars.regVal &= 0xC7;
		vars.regVal |= (dlpf<<3);
	}
	c_writeRegister8(2, ICM20948_GYRO_CONFIG_1, vars.regVal);
}


////~~~~


void c_setGyrRange(ICM20948_gyroRange gyroRange)
{
	vars.regVal = c_readRegister8(2, ICM20948_GYRO_CONFIG_1);
	vars.regVal &= ~(0x06);
	vars.regVal |= (gyroRange<<1);
	c_writeRegister8(2, ICM20948_GYRO_CONFIG_1, vars.regVal);
	vars.gyrRangeFactor = (1<<gyroRange);
}


////~~~~


void c_setAccRange(ICM20948_accRange accRange)
{
	vars.regVal = c_readRegister8(2, ICM20948_ACCEL_CONFIG);
	vars.regVal &= ~(0x06);
	vars.regVal |= (accRange<<1);
	c_writeRegister8(2, ICM20948_ACCEL_CONFIG, vars.regVal);
	vars.accRangeFactor = (1<<accRange);
}


////~~~~


void c_setAccDLPF(ICM20948_dlpf dlpf)
{
	vars.regVal = c_readRegister8(2, ICM20948_ACCEL_CONFIG);
	if(dlpf==ICM20948_DLPF_OFF)
	{
		vars.regVal &= 0xFE;
		c_writeRegister8(2, ICM20948_ACCEL_CONFIG, vars.regVal);
		return;
	}
	else
	{
		vars.regVal |= 0x01;
		vars.regVal &= 0xC7;
		vars.regVal |= (dlpf<<3);
	}
	c_writeRegister8(2, ICM20948_ACCEL_CONFIG, vars.regVal);
}


////~~~~


xyzFloat c_getAccRawValues()
{
	xyzFloat accRawVal;
	accRawVal.x = (int16_t)(((vars.buffer[0]) << 8) | vars.buffer[1]) * 1.0;
	accRawVal.y = (int16_t)(((vars.buffer[2]) << 8) | vars.buffer[3]) * 1.0;
	accRawVal.z = (int16_t)(((vars.buffer[4]) << 8) | vars.buffer[5]) * 1.0;
	return accRawVal;
}


////~~~~


xyzFloat c_getGyrRawValues()
{
	xyzFloat gyrRawVal;
	gyrRawVal.x = (int16_t)(((vars.buffer[6]) << 8) | vars.buffer[7]) * 1.0;
	gyrRawVal.y = (int16_t)(((vars.buffer[8]) << 8) | vars.buffer[9]) * 1.0;
	gyrRawVal.z = (int16_t)(((vars.buffer[10]) << 8) | vars.buffer[11]) * 1.0;
	return gyrRawVal;
}


////////~~~~~~~~END>  free_candy_2.c
