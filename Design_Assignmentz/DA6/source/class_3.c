#include "ICM20948.h"
    

void c_setAccOffsets(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	vars.accOffsetVal.x = (xMax + xMin) * 0.5;
	vars.accOffsetVal.y = (yMax + yMin) * 0.5;
	vars.accOffsetVal.z = (zMax + zMin) * 0.5;
	vars.accCorrFactor.x = (xMax + abs(xMin)) / 32768.0;
	vars.accCorrFactor.y = (yMax + abs(yMin)) / 32768.0;
	vars.accCorrFactor.z = (zMax + abs(zMin)) / 32768.0 ;
}


////~~~~


void c_setGyrOffsets(float xOffset, float yOffset, float zOffset)
{
	vars.gyrOffsetVal.x = xOffset;
	vars.gyrOffsetVal.y = yOffset;
	vars.gyrOffsetVal.z = zOffset;
}


////~~~~


void c_enableAcc(bool enAcc)
{
	vars.regVal = c_readRegister8(0, ICM20948_PWR_MGMT_2);
	if(enAcc)
	{
		vars.regVal &= ~ICM20948_ACC_EN;
	}
	else
	{
		vars.regVal |= ICM20948_ACC_EN;
	}
	c_writeRegister8(0, ICM20948_PWR_MGMT_2, vars.regVal);
}


////~~~~


void c_setAccSampleRateDivider(uint16_t accSplRateDiv)
{
	c_writeRegister16(2, ICM20948_ACCEL_SMPLRT_DIV_1, accSplRateDiv);
}


////~~~~


void c_writeRegister16(uint8_t bank, uint8_t reg, int16_t val)
{
	c_switchBank(bank);
	int8_t MSByte = (int8_t)((val>>8) & 0xFF);
	uint8_t LSByte = val & 0xFF;
	
	i2c_start((vars.i2cAddress << 1) + I2C_WRITE);    //_wire->beginTransmission(vars.i2cAddress);
	i2c_write(reg);                                   //_wire->write(reg);
	i2c_write(MSByte);     
	i2c_write(LSByte);                             
	i2c_stop();                                       //_wire->endTransmission();
}


////~~~~


void c_enableGyr(bool enGyr)
{
	vars.regVal = c_readRegister8(0, ICM20948_PWR_MGMT_2);
	if(enGyr)
	{
		vars.regVal &= ~ICM20948_GYR_EN;
	}
	else
	{
		vars.regVal |= ICM20948_GYR_EN;
	}
	c_writeRegister8(0, ICM20948_PWR_MGMT_2, vars.regVal);
}


////~~~~


void c_setGyrSampleRateDivider(uint8_t gyrSplRateDiv)
{
	c_writeRegister8(2, ICM20948_GYRO_SMPLRT_DIV, gyrSplRateDiv);
}


////~~~~


void c_setTempDLPF(ICM20948_dlpf dlpf)
{
	c_writeRegister8(2, ICM20948_TEMP_CONFIG, dlpf);
}


////~~~~


void c_setI2CMstSampleRate(uint8_t rateExp)
{
	if(rateExp < 16)
	{
		c_writeRegister8(3, ICM20948_I2C_MST_ODR_CFG, rateExp);
	}
}


////~~~~


xyzFloat c_getCorrectedAccRawValues()
{
	xyzFloat accRawVal = c_getAccRawValues();
	accRawVal = c_correctAccRawValues(accRawVal);
	return accRawVal;
}


////~~~~


xyzFloat c_correctAccRawValues(xyzFloat accRawVal)
{
	accRawVal.x = (accRawVal.x -(vars.accOffsetVal.x / vars.accRangeFactor)) / vars.accCorrFactor.x;
	accRawVal.y = (accRawVal.y -(vars.accOffsetVal.y / vars.accRangeFactor)) / vars.accCorrFactor.y;
	accRawVal.z = (accRawVal.z -(vars.accOffsetVal.z / vars.accRangeFactor)) / vars.accCorrFactor.z;
	return accRawVal;
}


////~~~~


xyzFloat c_getGValues()
{
	xyzFloat gVal;
	xyzFloat accRawVal;
	accRawVal = c_getCorrectedAccRawValues();
	
	gVal.x = accRawVal.x * vars.accRangeFactor / 16384.0;
	gVal.y = accRawVal.y * vars.accRangeFactor / 16384.0;
	gVal.z = accRawVal.z * vars.accRangeFactor / 16384.0;
	return gVal;
}


////~~~~


xyzFloat c_getAccRawValuesFromFifo()
{
	xyzFloat accRawVal = c_readICM20948xyzValFromFifo();
	return accRawVal;
}


////~~~~


xyzFloat c_readICM20948xyzValFromFifo()
{
	uint8_t fifoTriple[6];
	xyzFloat xyzResult = {0.0, 0.0, 0.0};
	
	for (int ii = 0; ii < 6; ii++)
	{
		fifoTriple[ii] = c_readRegister8(0, ICM20948_ACCEL_OUT+ii); // only need the 6
	}
	xyzResult.x = ((int16_t)((fifoTriple[0]<<8) + fifoTriple[1])) * 1.0;
	xyzResult.y = ((int16_t)((fifoTriple[2]<<8) + fifoTriple[3])) * 1.0;
	xyzResult.z = ((int16_t)((fifoTriple[4]<<8) + fifoTriple[5])) * 1.0;
	return xyzResult;
}


////~~~~


xyzFloat c_getCorrectedAccRawValuesFromFifo()
{
	xyzFloat accRawVal = c_getAccRawValuesFromFifo();
	accRawVal = c_correctAccRawValues(accRawVal);
	
	return accRawVal;
}


////~~~~


xyzFloat c_getGValuesFromFifo()
{
	xyzFloat gVal, accRawVal;
	accRawVal = c_getCorrectedAccRawValuesFromFifo();
	
	gVal.x = accRawVal.x * vars.accRangeFactor / 16384.0;
	gVal.y = accRawVal.y * vars.accRangeFactor / 16384.0;
	gVal.z = accRawVal.z * vars.accRangeFactor / 16384.0;
	return gVal;
}


////////~~~~~~~~END>  free_candy_3.c
