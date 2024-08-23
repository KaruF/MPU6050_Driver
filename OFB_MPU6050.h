/*
 * OFB_MPU6050.h
 *
 *  Created on: May 22, 2024
 *      Author: m.omerfarukbulut@gmail.com
 */

#ifndef SRC_SENSOR_LAYER_OFB_MPU6050_H_
#define SRC_SENSOR_LAYER_OFB_MPU6050_H_

#include <stdint.h>
#include <stdbool.h>
#include "OFB_MPU6050_Registers.h"

////****MACROS****////
#define MIN_SELF_TEST_RESPONSE	-14.0f
#define MAX_SELF_TEST_RESPONSE	14.0f


////****MPU6050 ENUMS****////

enum MPU6050_STAT{

	MPU6050_INSTANCE_ALLOCATION_ERR,

	MPU6050_INIT_OK,
	MPU6050_INIT_ERR,

	MPU6050_DEINIT_OK,
	MPU6050_DEINIT_ERR,

	MPU6050_FT_CALCULATION_OK,

	MPU6050_SELF_TEST_OK,
	MPU6050_SELF_TEST_XA_ERR,
	MPU6050_SELF_TEST_YA_ERR,
	MPU6050_SELF_TEST_ZA_ERR,
	MPU6050_SELF_TEST_XG_ERR,
	MPU6050_SELF_TEST_YG_ERR,
	MPU6050_SELF_TEST_ZG_ERR,
	MPU6050_GYRO_FT_OK,
	MPU6050_GYRO_FT_ERR,

	MPU6050_SEND_OK,
	MPU6050_RECEIVE_OK,

	MPU6050_HEAP_NULL_ERR,
};


////****MPU6050 STRUCTS****////

struct MPU6050_MEMORY_MAP{

	uint8_t mpu6050_id;






};

struct MPU6050_DATA_STR{


	uint16_t gyro_x;
	uint16_t gyro_y;
	uint16_t gyro_z;

	uint8_t gyro_x_selftest;
	uint8_t gyro_y_selftest;
	uint8_t gyro_z_selftest;

	uint16_t accel_x;
	uint16_t accel_y;
	uint16_t accel_z;

	uint8_t accel_x_selftest;
	uint8_t accel_y_selftest;
	uint8_t accel_z_selftest;

	float FT_gyro_x;
	float FT_gyro_y;
	float FT_gyro_z;

	float FT_accel_x;
	float FT_accel_y;
	float FT_accel_z;

	float temperature;
};

struct MPU6050_COMM_INIT{

	uint8_t i2c_slave_addr;
	uint8_t (*mpu6050_send)(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t size);
	uint8_t (*mpu6050_receive)(uint8_t slave_addr, uint8_t reg_addr, uint8_t* data, uint32_t data_size);
};

struct MPU6050_HANDLE{

	struct MPU6050_COMM_INIT mpu6050_comm_init;
	void (*mpu6050_err_callback)(enum MPU6050_STAT mpu6050_err);

};

struct MPU6050_STRUCT{

	struct MPU6050_MEMORY_MAP mpu6050_memory_map;
	struct MPU6050_DATA_STR mpu6050_data_str;
	struct MPU6050_HANDLE mpu6050_handle;
};

typedef struct MPU6050_STRUCT MPU6050_STR; //Usage for implementation layer




////***REGISTERS VALUES***////


///GYRO_CONFIG

#define FS_SEL_BIT_POS				0x03
#define ZG_ST_BIT_POS				0x05
#define YG_ST_BIT_POS				0x06
#define XG_ST_BIT_POS				0x07
#define ALL_GYRO_ST_BIT_POS			0x05

#define FS_SEL_BIT_MASK				(0x03 << FS_SEL_BIT_POS)
#define ZG_ST_BIT_MASK				(0x01 << ZG_ST_BIT_POS)
#define YG_ST_BIT_MASK				(0x01 << YG_ST_BIT_POS)
#define XG_ST_BIT_MASK				(0x01 << XG_ST_BIT_POS)
#define ALL_GYRO_ST_BIT_MASK		(0x07 << ALL_GYRO_ST_BIT_POS)


#define FS_SEL_0					0x00 	//+-250
#define FS_SEL_1					0x01  	//+-500
#define FS_SEL_2					0x02	//+-1000
#define FS_SEL_3					0x03	//+-2000

#define ZG_ST_ENABLE				0x01	//Gyro-Z axis selftest enable
#define ZG_ST_DISABLE				0x00	//Gyro-Z axis selftest disable

#define YG_ST_ENABLE				0x01	//Gyro-Y axis selftest enable
#define YG_ST_DISABLE				0x00	//Gyro-Y axis selftest disable

#define XG_ST_ENABLE				0x01	//Gyro-X axis selftest enable
#define XG_ST_DISABLE				0x00	//Gyro-X axis selftest disable

#define ALL_GYRO_AXIS_ST_ENABLE		0x07	//All axis selftest enabled
#define ALL_GYRO_AXIS_ST_DISABLE	0x00	//All axis selftest disabled


///ACCEL_CONFIG
#define AFS_SEL_BIT_POS				0x03
#define ZA_ST_BIT_POS				0x05
#define YA_ST_BIT_POS				0x06
#define XA_ST_BIT_POS				0x07
#define ALL_ACCEL_ST_BIT_POS		0x05

#define AFS_SEL_BIT_MASK			(0x03 << AFS_SEL_BIT_POS)
#define ZA_ST_BIT_MASK				(0x01 << ZA_ST_BIT_POS)
#define YA_ST_BIT_MASK				(0x01 << YA_ST_BIT_POS)
#define XA_ST_BIT_MASK				(0x01 << XA_ST_BIT_POS)
#define ALL_ACCEL_ST_BIT_MASK		(0x07 << ALL_ACCEL_ST_BIT_POS)


#define AFS_SEL_0					0x00 	//+-2g
#define AFS_SEL_1					0x01  	//+-4g
#define AFS_SEL_2					0x02	//+-8g
#define AFS_SEL_3					0x03	//+-16g

#define ZA_ST_ENABLE				0x01	//Accel-Z axis selftest enable
#define ZA_ST_DISABLE				0x00	//Accel-Z axis selftest disable

#define YA_ST_ENABLE				0x01	//Accel-Y axis selftest enable
#define YA_ST_DISABLE				0x00	//Accel-Y axis selftest disable

#define XA_ST_ENABLE				0x01	//Accel-X axis selftest enable
#define XA_ST_DISABLE				0x00	//Accel-X axis selftest disable

#define ALL_ACCEL_AXIS_ST_ENABLE	0x07	//All axis selftest enabled
#define ALL_ACCEL_AXIS_ST_DISABLE	0x00	//All axis selftest disabled


////FUNCTION DECLARATIONS////
struct MPU6050_STRUCT *MPU6050_Create_Instance();
enum MPU6050_STAT MPU6050_Init(struct MPU6050_STRUCT *mpu6050_str);
enum MPU6050_STAT MPU6050_DeInit(struct MPU6050_STRUCT *mpu6050_str);
enum MPU6050_STAT MPU6050_Self_Test(struct MPU6050_STRUCT *mpu6050_str);


enum MPU6050_STAT MPU6050_Read_Gyro(struct MPU6050_STRUCT *mpu6050_str);
enum MPU6050_STAT MPU6050_Read_Gyro_SelfTest(struct MPU6050_STRUCT *mpu6050_str);

enum MPU6050_STAT MPU6050_Read_Accel(struct MPU6050_STRUCT *mpu6050_str);
enum MPU6050_STAT MPU6050_Read_Accel_SelfTest(struct MPU6050_STRUCT *mpu6050_str);

enum MPU6050_STAT MPU6050_Set_Gyro_Full_Scale(struct MPU6050_STRUCT *mpu6050_str, uint8_t fullscale_val);
enum MPU6050_STAT MPU6050_Enable_Gyro_Selftest(struct MPU6050_STRUCT *mpu6050_str, bool selftest_enable);

enum MPU6050_STAT MPU6050_Set_Accel_Full_Scale(struct MPU6050_STRUCT *mpu6050_str, uint8_t fullscale_val);
enum MPU6050_STAT MPU6050_Enable_Accel_Selftest(struct MPU6050_STRUCT *mpu6050_str, bool selftest_enable);


enum MPU6050_STAT MPU6050_Read_ID(struct MPU6050_STRUCT *mpu6050_str);

#endif /* SRC_SENSOR_LAYER_OFB_MPU6050_H_ */
