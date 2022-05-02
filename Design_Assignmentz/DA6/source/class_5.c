#include "ICM20948.h"
    

float c_getPitch()
{
	xyzFloat angleVal = c_getAngles();
	float pitch = (atan2(angleVal.x, sqrt(abs((angleVal.x*angleVal.y + angleVal.z*angleVal.z))))*180.0)/M_PI;
	return pitch;
}


////~~~~


float c_getRoll()
{
	xyzFloat angleVal = c_getAngles();
	float roll = (atan2(angleVal.y, angleVal.z)*180.0)/M_PI;
	return roll;
}


////~~~~


void c_setIntPinPolarity(ICM20948_intPinPol pol)
{
	vars.regVal = c_readRegister8(0, ICM20948_INT_PIN_CFG);
	if(pol){
		vars.regVal |= ICM20948_INT1_ACTL;
	}
	else{
		vars.regVal &= ~ICM20948_INT1_ACTL;
	}
	c_writeRegister8(0, ICM20948_INT_PIN_CFG, vars.regVal);
}


////~~~~


void c_enableIntLatch(bool latch)
{
	vars.regVal = c_readRegister8(0, ICM20948_INT_PIN_CFG);
	if(latch){
		vars.regVal |= ICM20948_INT_1_LATCH_EN;
	}
	else{
		vars.regVal &= ~ICM20948_INT_1_LATCH_EN;
	}
	c_writeRegister8(0, ICM20948_INT_PIN_CFG, vars.regVal);
}


////~~~~


void c_enableClearIntByAnyRead(bool clearByAnyRead)
{
	vars.regVal = c_readRegister8(0, ICM20948_INT_PIN_CFG);
	if(clearByAnyRead){
		vars.regVal |= ICM20948_INT_ANYRD_2CLEAR;
	}
	else{
		vars.regVal &= ~ICM20948_INT_ANYRD_2CLEAR;
	}
	c_writeRegister8(0, ICM20948_INT_PIN_CFG, vars.regVal);
}


////~~~~


void c_setFSyncIntPolarity(ICM20948_intPinPol pol)
{
	vars.regVal = c_readRegister8(0, ICM20948_INT_PIN_CFG);
	if(pol){
		vars.regVal |= ICM20948_ACTL_FSYNC;
	}
	else{
		vars.regVal &= ~ICM20948_ACTL_FSYNC;
	}
	c_writeRegister8(0, ICM20948_INT_PIN_CFG, vars.regVal);
}


////~~~~


void c_enableInterrupt(ICM20948_intType intType)
{
	switch(intType){
		case ICM20948_FSYNC_INT:
		vars.regVal = c_readRegister8(0, ICM20948_INT_PIN_CFG);
		vars.regVal |= ICM20948_FSYNC_INT_MODE_EN;
		c_writeRegister8(0, ICM20948_INT_PIN_CFG, vars.regVal); // enable FSYNC as interrupt pin
		vars.regVal = c_readRegister8(0, ICM20948_INT_ENABLE);
		vars.regVal |= 0x80;
		c_writeRegister8(0, ICM20948_INT_ENABLE, vars.regVal); // enable wake on FSYNC interrupt
		break;
		
		case ICM20948_WOM_INT:
		vars.regVal = c_readRegister8(0, ICM20948_INT_ENABLE);
		vars.regVal |= 0x08;
		c_writeRegister8(0, ICM20948_INT_ENABLE, vars.regVal);
		vars.regVal = c_readRegister8(2, ICM20948_ACCEL_INTEL_CTRL);
		vars.regVal |= 0x02;
		c_writeRegister8(2, ICM20948_ACCEL_INTEL_CTRL, vars.regVal);
		break;
		
		case ICM20948_DMP_INT:
		vars.regVal = c_readRegister8(0, ICM20948_INT_ENABLE);
		vars.regVal |= 0x02;
		c_writeRegister8(0, ICM20948_INT_ENABLE, vars.regVal);
		break;
		
		case ICM20948_DATA_READY_INT:
		c_writeRegister8(0, ICM20948_INT_ENABLE_1, 0x01);
		break;
		
		case ICM20948_FIFO_OVF_INT:
		c_writeRegister8(0, ICM20948_INT_ENABLE_2, 0x01);
		break;
		
		case ICM20948_FIFO_WM_INT:
		c_writeRegister8(0, ICM20948_INT_ENABLE_3, 0x01);
		break;
	}
}


////~~~~


void c_disableInterrupt(ICM20948_intType intType)
{
	switch(intType){
		case ICM20948_FSYNC_INT:
		vars.regVal = c_readRegister8(0, ICM20948_INT_PIN_CFG);
		vars.regVal &= ~ICM20948_FSYNC_INT_MODE_EN;
		c_writeRegister8(0, ICM20948_INT_PIN_CFG, vars.regVal);
		vars.regVal = c_readRegister8(0, ICM20948_INT_ENABLE);
		vars.regVal &= ~(0x80);
		c_writeRegister8(0, ICM20948_INT_ENABLE, vars.regVal);
		break;
		
		case ICM20948_WOM_INT:
		vars.regVal = c_readRegister8(0, ICM20948_INT_ENABLE);
		vars.regVal &= ~(0x08);
		c_writeRegister8(0, ICM20948_INT_ENABLE, vars.regVal);
		vars.regVal = c_readRegister8(2, ICM20948_ACCEL_INTEL_CTRL);
		vars.regVal &= ~(0x02);
		c_writeRegister8(2, ICM20948_ACCEL_INTEL_CTRL, vars.regVal);
		break;
		
		case ICM20948_DMP_INT:
		vars.regVal = c_readRegister8(0, ICM20948_INT_ENABLE);
		vars.regVal &= ~(0x02);
		c_writeRegister8(0, ICM20948_INT_ENABLE, vars.regVal);
		break;
		
		case ICM20948_DATA_READY_INT:
		c_writeRegister8(0, ICM20948_INT_ENABLE_1, 0x00);
		break;
		
		case ICM20948_FIFO_OVF_INT:
		c_writeRegister8(0, ICM20948_INT_ENABLE_2, 0x00);
		break;
		
		case ICM20948_FIFO_WM_INT:
		c_writeRegister8(0, ICM20948_INT_ENABLE_3, 0x00);
		break;
	}
}


////~~~~


uint8_t c_readAndClearInterrupts()
{
	uint8_t intSource = 0;
	vars.regVal = c_readRegister8(0, ICM20948_I2C_MST_STATUS);
	if(vars.regVal & 0x80){
		intSource |= 0x01;
	}
	vars.regVal = c_readRegister8(0, ICM20948_INT_STATUS);
	if(vars.regVal & 0x08){
		intSource |= 0x02;
	}
	if(vars.regVal & 0x02){
		intSource |= 0x04;
	}
	vars.regVal = c_readRegister8(0, ICM20948_INT_STATUS_1);
	if(vars.regVal & 0x01){
		intSource |= 0x08;
	}
	vars.regVal = c_readRegister8(0, ICM20948_INT_STATUS_2);
	if(vars.regVal & 0x01){
		intSource |= 0x10;
	}
	vars.regVal = c_readRegister8(0, ICM20948_INT_STATUS_3);
	if(vars.regVal & 0x01){
		intSource |= 0x20;
	}
	return intSource;
}



////////~~~~~~~~END>  free_candy_5.c
