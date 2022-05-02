#include "ICM20948.h"
    

bool c_checkInterrupt(uint8_t source, ICM20948_intType type)
{
	source &= type;
	return source;
}


////~~~~


void c_setWakeOnMotionThreshold(uint8_t womThresh, ICM20948_womCompEn womCompEn)
{
	vars.regVal = c_readRegister8(2, ICM20948_ACCEL_INTEL_CTRL);
	if(womCompEn){
		vars.regVal |= 0x01;
	}
	else{
		vars.regVal &= ~(0x01);
	}
	c_writeRegister8(2, ICM20948_ACCEL_INTEL_CTRL, vars.regVal);
	c_writeRegister8(2, ICM20948_ACCEL_WOM_THR, womThresh);
}


////~~~~


void c_enableFifo(bool fifo)
{
	vars.regVal = c_readRegister8(0, ICM20948_USER_CTRL);
	if(fifo){
		vars.regVal |= ICM20948_FIFO_EN;
	}
	else{
		vars.regVal &= ~ICM20948_FIFO_EN;
	}
	c_writeRegister8(0, ICM20948_USER_CTRL, vars.regVal);
}


////~~~~


void c_setFifoMode(ICM20948_fifoMode mode)
{
	if(mode){
		vars.regVal = 0x01;
	}
	else{
		vars.regVal = 0x00;
	}
	c_writeRegister8(0, ICM20948_FIFO_MODE, vars.regVal);
}


////~~~~


void c_startFifo(ICM20948_fifoType fifo)
{
	vars.fifoType = fifo;
	c_writeRegister8(0, ICM20948_FIFO_EN_2, vars.fifoType);
}


////~~~~


void c_stopFifo()
{
	c_writeRegister8(0, ICM20948_FIFO_EN_2, 0);
}


////~~~~


void c_resetFifo()
{
	c_writeRegister8(0, ICM20948_FIFO_RST, 0x01);
	c_writeRegister8(0, ICM20948_FIFO_RST, 0x00);
}


////~~~~


int16_t c_getFifoCount()
{
	int16_t regVal16 = (int16_t) c_readRegister16(0, ICM20948_FIFO_COUNT);
	return regVal16;
}


////~~~~


int16_t c_readRegister16(uint8_t bank, uint8_t reg)
{
	c_switchBank(bank);
	uint8_t MSByte = 0, LSByte = 0;
	int16_t reg16Val = 0;
	
	MSByte = c_readRegister8(bank, reg);
	LSByte = c_readRegister8(bank, reg+1);
	reg16Val = (MSByte<<8) + LSByte;
	return reg16Val;
}


////~~~~


int16_t c_getNumberOfFifoDataSets()
{
	int16_t numberOfSets = c_getFifoCount();
	if((vars.fifoType == ICM20948_FIFO_ACC) || (vars.fifoType == ICM20948_FIFO_GYR)){
		numberOfSets /= 6;
	}
	else if(vars.fifoType==ICM20948_FIFO_ACC_GYR){
		numberOfSets /= 12;
	}
	return numberOfSets;
}


////~~~~


void c_findFifoBegin()
{
	uint16_t count = c_getFifoCount();
	int16_t start = 0;
	
	if((vars.fifoType == ICM20948_FIFO_ACC) || (vars.fifoType == ICM20948_FIFO_GYR)){
		start = count%6;
		for(int i=0; i<start; i++){
			c_readRegister8(0, ICM20948_FIFO_R_W);
		}
	}
	else if(vars.fifoType==ICM20948_FIFO_ACC_GYR){
		start = count%12;
		for(int i=0; i<start; i++){
			c_readRegister8(0, ICM20948_FIFO_R_W);
		}
	}
}


////~~~~
////~~~~
////~~~~
////~~~~
////~~~~
////~~~~
////~~~~
////////~~~~~~~~END>  free_candy_6.c


/*

// Magnetometer 

bool ICM20948_WE::initMagnetometer(){
    enableI2CMaster();
    resetMag();
    reset_ICM20948();
    sleep(false);
    writeRegister8(2, ICM20948_ODR_ALIGN_EN, 1); // aligns ODR 
    delay(10);
    enableI2CMaster();
    delay(10);
    
    int16_t whoAmI = whoAmIMag();
    if(! ((whoAmI == AK09916_WHO_AM_I_1) || (whoAmI == AK09916_WHO_AM_I_2))){
        return false;
    }
    setMagOpMode(AK09916_CONT_MODE_100HZ); 
   
    return true;
}

int16_t ICM20948_WE::whoAmIMag(){
    return readAK09916Register16(AK09916_WIA_1);
}

void ICM20948_WE::setMagOpMode(AK09916_opMode opMode){
    writeAK09916Register8(AK09916_CNTL_2, opMode);
    delay(10);
    if(opMode!=AK09916_PWR_DOWN){
        enableMagDataRead(AK09916_HXL, 0x08);
    }
}

void ICM20948_WE::resetMag(){
    writeAK09916Register8(AK09916_CNTL_3, 0x01);
    delay(100);
}


     //Private Functions

void ICM20948_WE::setClockToAutoSelect(){
    regVal = readRegister8(0, ICM20948_PWR_MGMT_1);
    regVal |= 0x01;
    writeRegister8(0, ICM20948_PWR_MGMT_1, regVal);
    delay(10);
}



void ICM20948_WE::writeAK09916Register8(uint8_t reg, uint8_t val){
    writeRegister8(3, ICM20948_I2C_SLV0_ADDR, AK09916_ADDRESS); // write AK09916
    writeRegister8(3, ICM20948_I2C_SLV0_REG, reg); // define AK09916 register to be written to
    writeRegister8(3, ICM20948_I2C_SLV0_DO, val);
}


uint8_t ICM20948_WE::readAK09916Register8(uint8_t reg){
    enableMagDataRead(reg, 0x01);
    enableMagDataRead(AK09916_HXL, 0x08);
    regVal = readRegister8(0, ICM20948_EXT_SLV_SENS_DATA_00);
    return regVal;
}

int16_t ICM20948_WE::readAK09916Register16(uint8_t reg){
    int16_t regValue = 0;
    enableMagDataRead(reg, 0x02);
    regValue = readRegister16(0, ICM20948_EXT_SLV_SENS_DATA_00);
    enableMagDataRead(AK09916_HXL, 0x08);
    return regValue;
}



void ICM20948_WE::enableI2CMaster(){
    writeRegister8(0, ICM20948_USER_CTRL, ICM20948_I2C_MST_EN); //enable I2C master
    writeRegister8(3, ICM20948_I2C_MST_CTRL, 0x07); // set I2C clock to 345.60 kHz
    delay(10);
}

void ICM20948_WE::enableMagDataRead(uint8_t reg, uint8_t bytes){
    writeRegister8(3, ICM20948_I2C_SLV0_ADDR, AK09916_ADDRESS | AK09916_READ); // read AK09916
    writeRegister8(3, ICM20948_I2C_SLV0_REG, reg); // define AK09916 register to be read
    writeRegister8(3, ICM20948_I2C_SLV0_CTRL, 0x80 | bytes); //enable read | number of byte
    delay(10);
}
  
*/
////////~~~~~~~~END>  ICM20948.c
