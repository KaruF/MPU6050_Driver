# MPU6050_Driver
Complete driver for MPU6050 IMU 


Hi everyone,

- It is going to be complete driver for mpu6050 and it won't be for any particular architecture. So you can easly implement it to your hardware!
- You just must write wrapper function to struct MPU6050_COMM_INIT send, receive function pointer. Like that:
```
void MPU6050_ERR_CALLBACK(enum MPU6050_STAT mpu6050_err){

	while(1);

}

uint8_t I2C_WRAPPER_WRITE(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t size){

	HAL_I2C_Mem_Write(&hi2c1, slave_addr, reg_addr, 1, data, size, 50);

	return 0;
}

uint8_t I2C_WRAPPER_READ(uint8_t slave_addr, uint8_t reg_addr, uint8_t* data, uint32_t data_size){

	HAL_I2C_Mem_Read(&hi2c1, slave_addr, reg_addr, 1, data, data_size, 50);

	return 0;
}
```
- It is not at the end yet but i will update it when it will get some new functions or new feature.

- Probably it won't be included DMP functionality due to lack of documentation.

- At some point, i might publish linux version under this repository.


Keep in touch!
