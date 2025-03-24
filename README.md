# Embedded Artificial Intelligence Lab

## Project Overview

This project focuses on implementing a deep learning model for predictive maintenance. We use the *AI4I 2020 Predictive Maintenance Dataset*, which contains 10,000 instances of industrial sensor data.

## Dataset

The dataset used is the *AI4I 2020 Predictive Maintenance Dataset*, which contains 10,000 instances of industrial sensor data. Each instance represents the operating condition of a machine and is associated with a label indicating whether a failure has occurred and, if so, what type of failure it is.

## Objective

The goal is to create a deep learning model capable of predicting whether a machine will fail and, if so, identifying the type of failure among 5 categories:
- TWF: Tool Wear Failure
- HDF: Heat Dissipation Failure
- PWF: Power Failure
- OSF: Overstrain Failure
- RNF: Random Failure

## Project Structure

The project is structured around a Jupyter notebook that guides through the following steps:
1. Dataset analysis
2. Training a model without balancing the dataset
3. Training a model with balanced dataset
4. Comparison of model performances

Once the model is done, we upload it and analyze it on STM32CubeIDE thanks to X-Cube AI. Then we validate it on desktop and on device.

## Installation and Usage

1. Clone this repository
2. Ensure the necessary dependencies are installed
3. Open the TP_IA_EMBARQUEE.ipynb notebook with Jupyter Notebook or JupyterLab
4. Execute the notebook cells sequentially to follow the complete process
5. Upload the model to STM32CubeIDE
6. Execute Communication_STM32_NN.py to test the model

## Model Training

### Without Balancing the Dataset

In this section, we build and train a model without rebalancing the dataset. We analyze the performance and identify the limitations due to the imbalanced nature of the data.

### With Balancing the Dataset

In this section, we apply techniques to balance the dataset, such as oversampling using SMOTE. We then train the model again and compare the performance with the previous model.

## Model Evaluation

We evaluate the model using various metrics, including accuracy, confusion matrix, and classification report. We also visualize the training and validation accuracy and loss to check for overfitting.

## Deployment

The trained model is converted to TensorFlow Lite format and deployed on an STM32 microcontroller using STM32CubeIDE and X-Cube AI. We validate the model on both desktop and device.

## Additional Information

For more details on the implementation and analysis, refer to the TP_IA_EMBARQUEE.ipynb notebook.

## Technologies Used

- Numpy
- TensorFlow/Keras
- Pandas
- Matplotlib
- Scikit-learn
- Imbalanced-learn (for data balancing)

## Authors
- BRIERE Yann (yann.briere@etu.emse.fr)
- MAURE Mathieu (mathieu.maure@etu.emse.fr)