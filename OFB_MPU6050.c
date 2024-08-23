/*
 * OFB_MPU6050.c
 *
 *  Created on: May 22, 2024
 *      Author: m.omerfarukbulut@gmail.com
 */

#include <math.h>
#include <stdlib.h>

#include "OFB_MPU6050.h"



////****STATIC FUNCTIONS DECLARATIONS****////
static enum MPU6050_STAT MPU6050_Read_Gyro_Factory_Trim(struct MPU6050_STRUCT *mpu6050_str);



////*************************************////

struct MPU6050_STRUCT *MPU6050_Create_Instance(){

	struct MPU6050_STRUCT *mpu6050_str = calloc(1,sizeof(struct MPU6050_STRUCT));
	if(mpu6050_str == NULL)  return MPU6050_INSTANCE_ALLOCATION_ERR;


	return mpu6050_str;
}


enum MPU6050_STAT MPU6050_Init(struct MPU6050_STRUCT *mpu6050_str){





	return MPU6050_INIT_OK;
}

enum MPU6050_STAT MPU6050_DeInit(struct MPU6050_STRUCT *mpu6050_str){

	free(mpu6050_str);
	mpu6050_str = NULL;

	return MPU6050_DEINIT_OK;
}


enum MPU6050_STAT MPU6050_Self_Test(struct MPU6050_STRUCT *mpu6050_str){

	//since SelfTest_Response = gyroout_selftest_enabled - gyroout_selftest_disabled
	//change from factory trim of the self-test response must be within from %-14 to %14

	MPU6050_Read_Gyro(mpu6050_str);
	MPU6050_Read_Gyro_SelfTest(mpu6050_str);

	MPU6050_Read_Accel(mpu6050_str);
	MPU6050_Read_Accel_SelfTest(mpu6050_str);

	MPU6050_Read_Gyro_Factory_Trim(mpu6050_str);


	float gyroX_change_trim = ((float)(mpu6050_str->mpu6050_data_str.gyro_x_selftest - mpu6050_str->mpu6050_data_str.gyro_x) - mpu6050_str->mpu6050_data_str.FT_gyro_x) / mpu6050_str->mpu6050_data_str.FT_gyro_x;
	float gyroY_change_trim = ((float)(mpu6050_str->mpu6050_data_str.gyro_y_selftest - mpu6050_str->mpu6050_data_str.gyro_y) - mpu6050_str->mpu6050_data_str.FT_gyro_y) / mpu6050_str->mpu6050_data_str.FT_gyro_y;
	float gyroZ_change_trim = ((float)(mpu6050_str->mpu6050_data_str.gyro_z_selftest - mpu6050_str->mpu6050_data_str.gyro_z) - mpu6050_str->mpu6050_data_str.FT_gyro_z) / mpu6050_str->mpu6050_data_str.FT_gyro_z;


	if(gyroX_change_trim < MIN_SELF_TEST_RESPONSE || gyroX_change_trim > MAX_SELF_TEST_RESPONSE) mpu6050_str->mpu6050_handle.mpu6050_err_callback(MPU6050_SELF_TEST_XG_ERR);
	if(gyroY_change_trim < MIN_SELF_TEST_RESPONSE || gyroY_change_trim > MAX_SELF_TEST_RESPONSE) mpu6050_str->mpu6050_handle.mpu6050_err_callback(MPU6050_SELF_TEST_YG_ERR);
	if(gyroZ_change_trim < MIN_SELF_TEST_RESPONSE || gyroZ_change_trim > MAX_SELF_TEST_RESPONSE) mpu6050_str->mpu6050_handle.mpu6050_err_callback(MPU6050_SELF_TEST_ZG_ERR);


	return MPU6050_SELF_TEST_OK;
}


static enum MPU6050_STAT MPU6050_Read_Gyro_Factory_Trim(struct MPU6050_STRUCT *mpu6050_str){

	float *ft_gyro = malloc(sizeof(float)*3); // 0,1,2 are corresponds to X,Y,Z gyro axis respectively!
	if(ft_gyro == NULL) mpu6050_str->mpu6050_handle.mpu6050_err_callback(MPU6050_HEAP_NULL_ERR);


	if(mpu6050_str->mpu6050_data_str.gyro_x_selftest == 0){

		mpu6050_str->mpu6050_data_str.FT_gyro_x = 0;

	}else{

		mpu6050_str->mpu6050_data_str.FT_gyro_x = 25.00*131.00*(pow(1.046,(mpu6050_str->mpu6050_data_str.gyro_x_selftest-1)));

	}

	if(mpu6050_str->mpu6050_data_str.gyro_y_selftest == 0){

		mpu6050_str->mpu6050_data_str.FT_gyro_y = 0;

	}else{

		mpu6050_str->mpu6050_data_str.FT_gyro_y = -25.00*131.00*(pow(1.046,(mpu6050_str->mpu6050_data_str.gyro_x_selftest-1)));

	}

	if(mpu6050_str->mpu6050_data_str.gyro_z_selftest == 0){

		mpu6050_str->mpu6050_data_str.FT_gyro_z = 0;

	}else{

		mpu6050_str->mpu6050_data_str.FT_gyro_z = 25.00*131.00*(pow(1.046,(mpu6050_str->mpu6050_data_str.gyro_x_selftest-1)));

	}


	return MPU6050_FT_CALCULATION_OK;
}


enum MPU6050_STAT MPU6050_Read_Gyro(struct MPU6050_STRUCT *mpu6050_str){

	uint8_t data[2] = {0};

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_XOUT_H_REG, &data[0], 1);
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_XOUT_L_REG, &data[1], 1);

	mpu6050_str->mpu6050_data_str.gyro_x = ( data[0] << 8 ) | data[1];


	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_YOUT_H_REG, &data[0], 1);
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_YOUT_L_REG, &data[1], 1);

	mpu6050_str->mpu6050_data_str.gyro_y = ( data[0] << 8 ) | data[1];


	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_ZOUT_H_REG, &data[0], 1);
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_ZOUT_L_REG, &data[1], 1);

	mpu6050_str->mpu6050_data_str.gyro_z = ( data[0] << 8 ) | data[1];

	return 0;
}



enum MPU6050_STAT MPU6050_Read_Gyro_SelfTest(struct MPU6050_STRUCT *mpu6050_str){

	MPU6050_Set_Gyro_Full_Scale(mpu6050_str, FS_SEL_0);
	MPU6050_Enable_Gyro_Selftest(mpu6050_str, true);

	uint8_t data[1] = {0};

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, SELF_TEST_X_REG, data, 1);
	mpu6050_str->mpu6050_data_str.gyro_x_selftest = data[0] & 0x1F;

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, SELF_TEST_Y_REG, data, 1);
	mpu6050_str->mpu6050_data_str.gyro_y_selftest = data[0] & 0x1F;

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, SELF_TEST_Z_REG, data, 1);
	mpu6050_str->mpu6050_data_str.gyro_z_selftest = data[0] & 0x1F;

	MPU6050_Enable_Gyro_Selftest(mpu6050_str, false);

	return 0;
}


enum MPU6050_STAT MPU6050_Read_Accel(struct MPU6050_STRUCT *mpu6050_str){

	uint8_t data[2] = {0};

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, ACCEL_XOUT_H_REG, &data[0], 1);
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, ACCEL_XOUT_L_REG, &data[1], 1);

	mpu6050_str->mpu6050_data_str.accel_x= ( data[0] << 8 ) | data[1];


	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, ACCEL_YOUT_H_REG, &data[0], 1);
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, ACCEL_YOUT_H_REG, &data[1], 1);

	mpu6050_str->mpu6050_data_str.accel_y = ( data[0] << 8 ) | data[1];


	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, ACCEL_ZOUT_H_REG, &data[0], 1);
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, ACCEL_ZOUT_H_REG, &data[1], 1);

	mpu6050_str->mpu6050_data_str.accel_z = ( data[0] << 8 ) | data[1];


	return 0;
}


enum MPU6050_STAT MPU6050_Read_Accel_SelfTest(struct MPU6050_STRUCT *mpu6050_str){

	MPU6050_Set_Accel_Full_Scale(mpu6050_str, FS_SEL_0);
	MPU6050_Enable_Accel_Selftest(mpu6050_str, true);

	uint8_t dataH = 0, dataL = 0;

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, SELF_TEST_A_REG, &dataL, 1);

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, SELF_TEST_X_REG, &dataH, 1);
	mpu6050_str->mpu6050_data_str.accel_x_selftest = ((dataH & 0xE0) >> 0x03) | (dataL >> 0x04);

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, SELF_TEST_Y_REG, &dataH, 1);
	mpu6050_str->mpu6050_data_str.accel_y_selftest = ((dataH & 0xE0) >> 0x03) | (dataL >> 0x02);

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, SELF_TEST_Z_REG, &dataH, 1);
	mpu6050_str->mpu6050_data_str.accel_z_selftest = ((dataH & 0xE0) >> 0x03) | (dataL & 0x03);

	MPU6050_Enable_Accel_Selftest(mpu6050_str, false);

	return 0;
}


enum MPU6050_STAT MPU6050_Set_Gyro_Full_Scale(struct MPU6050_STRUCT *mpu6050_str, uint8_t fullscale_val){

	uint8_t data[1];
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_CONFIG_REG, data, 1);

	data[0] = data[0] & (~(FS_SEL_BIT_MASK) | (fullscale_val << FS_SEL_BIT_POS));

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_send(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_CONFIG_REG, data, 1);

	return 0;
}

enum MPU6050_STAT MPU6050_Enable_Gyro_Selftest(struct MPU6050_STRUCT *mpu6050_str, bool selftest_enable){

	uint8_t data[1];
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_CONFIG_REG, data, 1);

	uint8_t selftest_state = 0;

	if(selftest_enable == true){

		selftest_state = ALL_GYRO_AXIS_ST_ENABLE;

	}else{

		selftest_state = ALL_GYRO_AXIS_ST_DISABLE;
	}

	data[0] = data[0] & (~(ALL_GYRO_ST_BIT_MASK) | (selftest_state << ALL_GYRO_ST_BIT_POS));

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_send(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_CONFIG_REG, data, 1);

	return 0;
}

enum MPU6050_STAT MPU6050_Set_Accel_Full_Scale(struct MPU6050_STRUCT *mpu6050_str, uint8_t fullscale_val){

	uint8_t data[1];
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_CONFIG_REG, data, 1);

	data[0] = data[0] & (~(AFS_SEL_BIT_MASK) | (fullscale_val << AFS_SEL_BIT_POS));

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_send(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_CONFIG_REG, data, 1);

	return 0;
}

enum MPU6050_STAT MPU6050_Enable_Accel_Selftest(struct MPU6050_STRUCT *mpu6050_str, bool selftest_enable){

	uint8_t data[1];
	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_CONFIG_REG, data, 1);

	uint8_t selftest_state = 0;

	if(selftest_enable == true){

		selftest_state = ALL_ACCEL_AXIS_ST_ENABLE;

	}else{

		selftest_state = ALL_ACCEL_AXIS_ST_DISABLE;
	}

	data[0] = data[0] & (~(ALL_ACCEL_ST_BIT_MASK) | (selftest_state << ALL_ACCEL_ST_BIT_POS));

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_send(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, GYRO_CONFIG_REG, data, 1);

	return 0;
}



enum MPU6050_STAT MPU6050_Read_ID(struct MPU6050_STRUCT *mpu6050_str){

	uint8_t data[1] = {0};

	mpu6050_str->mpu6050_handle.mpu6050_comm_init.mpu6050_receive(mpu6050_str->mpu6050_handle.mpu6050_comm_init.i2c_slave_addr, WHO_AM_I_REG, data, 1);
	mpu6050_str->mpu6050_memory_map.mpu6050_id = data[0];

	return 0;
}




