/**
  ******************************************************************************
  * @file    app_x-cube-ai.c
  * @author  X-CUBE-AI C code generator
  * @brief   AI program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

 /*
  * Description
  *   v1.0 - Minimum template to show how to use the Embedded Client API
  *          model. Only one input and one output is supported. All
  *          memory resources are allocated statically (AI_NETWORK_XX, defines
  *          are used).
  *          Re-target of the printf function is out-of-scope.
  *   v2.0 - add multiple IO and/or multiple heap support
  *
  *   For more information, see the embeded documentation:
  *
  *       [1] %X_CUBE_AI_DIR%/Documentation/index.html
  *
  *   X_CUBE_AI_DIR indicates the location where the X-CUBE-AI pack is installed
  *   typical : C:\Users\[user_name]\STM32Cube\Repository\STMicroelectronics\X-CUBE-AI\7.1.0
  */

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#if defined ( __ICCARM__ )
#elif defined ( __CC_ARM ) || ( __GNUC__ )
#endif

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "app_x-cube-ai.h"
#include "main.h"
#include "ai_datatypes_defines.h"
#include "neural_network.h"
#include "neural_network_data.h"

/* USER CODE BEGIN includes */
extern UART_HandleTypeDef huart2;
/* USER CODE END includes */

/* IO buffers ----------------------------------------------------------------*/

#if !defined(AI_NEURAL_NETWORK_INPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) ai_i8 data_in_1[AI_NEURAL_NETWORK_IN_1_SIZE_BYTES];
ai_i8* data_ins[AI_NEURAL_NETWORK_IN_NUM] = {
data_in_1
};
#else
ai_i8* data_ins[AI_NEURAL_NETWORK_IN_NUM] = {
NULL
};
#endif

#if !defined(AI_NEURAL_NETWORK_OUTPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) ai_i8 data_out_1[AI_NEURAL_NETWORK_OUT_1_SIZE_BYTES];
ai_i8* data_outs[AI_NEURAL_NETWORK_OUT_NUM] = {
data_out_1
};
#else
ai_i8* data_outs[AI_NEURAL_NETWORK_OUT_NUM] = {
NULL
};
#endif

/* Activations buffers -------------------------------------------------------*/

AI_ALIGNED(32)
static uint8_t pool0[AI_NEURAL_NETWORK_DATA_ACTIVATION_1_SIZE];

ai_handle data_activations0[] = {pool0};

/* AI objects ----------------------------------------------------------------*/

static ai_handle neural_network = AI_HANDLE_NULL;

static ai_buffer* ai_input;
static ai_buffer* ai_output;

static void ai_log_err(const ai_error err, const char *fct)
{
  /* USER CODE BEGIN log */
  printf("AI error - type=%d code=%d\r\n", err.type, err.code);
  printf("AI error - in function %s\r\n", fct);
  /* USER CODE END log */
}

static int ai_boostrap(ai_handle *act_addr)
{
  ai_error err;

  /* Create and initialize an instance of the model */
  err = ai_neural_network_create_and_init(&neural_network, act_addr, NULL);
  if (err.type != AI_ERROR_NONE) {
    ai_log_err(err, "ai_neural_network_create_and_init");
    return -1;
  }

  ai_input = ai_neural_network_inputs_get(neural_network, NULL);
  ai_output = ai_neural_network_outputs_get(neural_network, NULL);

#if defined(AI_NEURAL_NETWORK_INPUTS_IN_ACTIVATIONS)
  /*  In the case where "--allocate-inputs" option is used, memory buffer can be
   *  used from the activations buffer. This is not mandatory.
   */
  for (int idx=0; idx < AI_NEURAL_NETWORK_IN_NUM; idx++) {
	data_ins[idx] = ai_input[idx].data;
  }
#else
  for (int idx=0; idx < AI_NEURAL_NETWORK_IN_NUM; idx++) {
	  ai_input[idx].data = data_ins[idx];
  }
#endif

#if defined(AI_NEURAL_NETWORK_OUTPUTS_IN_ACTIVATIONS)
  /*  In the case where "--allocate-outputs" option is used, memory buffer can be
   *  used from the activations buffer. This is no mandatory.
   */
  for (int idx=0; idx < AI_NEURAL_NETWORK_OUT_NUM; idx++) {
	data_outs[idx] = ai_output[idx].data;
  }
#else
  for (int idx=0; idx < AI_NEURAL_NETWORK_OUT_NUM; idx++) {
	ai_output[idx].data = data_outs[idx];
  }
#endif

  return 0;
}

static int ai_run(void)
{
  ai_i32 batch;

  batch = ai_neural_network_run(neural_network, ai_input, ai_output);
  if (batch != 1) {
    ai_log_err(ai_neural_network_get_error(neural_network),
        "ai_neural_network_run");
    return -1;
  }

  return 0;
}

/* USER CODE BEGIN 2 */
/* Function to receive input data from UART and preprocess it for the model */
int acquire_and_process_data(ai_i8* data[], ai_u16 *n_batch)
{
  // Ensure we're processing a single sample at a time
  *n_batch = 1;

  // Buffer to hold incoming data - adjust size based on your input features
  float input_data[AI_NETWORK_IN_1_SIZE];
  uint32_t bytes_received = 0;
  uint8_t buffer[4];  // For receiving float values byte by byte
  int i;

  // Wait for start indicator (e.g., 'S') from Python script
  uint8_t start_byte = 0;
  while (start_byte != 'S') {
    HAL_UART_Receive(&huart2, &start_byte, 1, HAL_MAX_DELAY);
  }

  // Receive each feature as a float (4 bytes)
  for (i = 0; i < AI_NETWORK_IN_1_SIZE; i++) {
    // Receive 4 bytes (float size)
    HAL_UART_Receive(&huart2, buffer, 4, HAL_MAX_DELAY);

    // Convert 4 bytes back to float
    float value;
    memcpy(&value, buffer, sizeof(float));
    input_data[i] = value;
  }

  // Send an acknowledgment byte
  uint8_t ack = 'A';
  HAL_UART_Transmit(&huart2, &ack, 1, HAL_MAX_DELAY);

  // Copy to AI input buffer (with potential scaling if needed)
  float *ai_input = (float *)data[0];
  for (i = 0; i < AI_NETWORK_IN_1_SIZE; i++) {
    ai_input[i] = input_data[i];
  }

  return 1;  // Return 1 on success
}

/* Function to send the model output back through UART */
int post_process(ai_i8* data[], ai_u16 n_batch)
{
  // Extract the model output
  float *output = (float *)data[0];
  int output_size = AI_NETWORK_OUT_1_SIZE;
  int i;
  uint8_t buffer[4];  // For sending float values byte by byte

  // Send start indicator for output data
  uint8_t start_byte = 'R';  // 'R' for Result
  HAL_UART_Transmit(&huart2, &start_byte, 1, HAL_MAX_DELAY);

  // Send each output value as bytes
  for (i = 0; i < output_size; i++) {
    // Convert float to bytes
    memcpy(buffer, &output[i], sizeof(float));

    // Send the 4 bytes
    HAL_UART_Transmit(&huart2, buffer, 4, HAL_MAX_DELAY);
  }

  // Wait for acknowledgment from Python script
  uint8_t ack_byte = 0;
  while (ack_byte != 'A') {
    HAL_UART_Receive(&huart2, &ack_byte, 1, HAL_MAX_DELAY);
  }

  return 1;  // Return 1 on success
}
/* USER CODE END 2 */

/* Entry points --------------------------------------------------------------*/

void MX_X_CUBE_AI_Init(void)
{
    /* USER CODE BEGIN 5 */
    ai_error err;

    // Initialize the activation buffers
    if (ai_boostrap(data_activations0) != 0) {
        printf("Error: AI bootstrap failed\r\n");
        return;
    }

    // Get model info
    ai_network_report report;
    err = ai_neural_network_get_info(neural_network, &report);
    if (err.type != AI_ERROR_NONE) {
        ai_log_err(err, "ai_neural_network_get_info");
        return;
    }

    // Print some model info
    printf("Neural network initialized successfully\r\n");
    printf("Model name: %s\r\n", report.model_name);
    printf("Model signature: %s\r\n", report.model_signature);
    printf("Input size: %d\r\n", AI_NETWORK_IN_1_SIZE);
    printf("Output size: %d\r\n", AI_NETWORK_OUT_1_SIZE);
    /* USER CODE END 5 */
}

void MX_X_CUBE_AI_Process(void)
{
    /* USER CODE BEGIN 6 */
    ai_i32 batch_size = 1;
    ai_error err;
    ai_u16 n_batch;

    // Get data from UART and prepare inputs
    if (acquire_and_process_data(data_ins, &n_batch) != 0) {
        printf("Error: Failed to acquire and process input data\r\n");
        return;
    }

    // Run model inference
    if (ai_run() != 0) {
        printf("Error: Model inference failed\r\n");
        return;
    }

    // Post-process and send results via UART
    if (post_process(data_outs, batch_size) != 0) {
        printf("Error: Failed to post-process and transmit results\r\n");
        return;
    }
    /* USER CODE END 6 */
}
#ifdef __cplusplus
}
#endif
