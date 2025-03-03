# Embedded Artificial Intelligence Lab

## Project Overview

This project focuses on implementing a deep learning model for predictive maintenance. We use the **AI4I 2020 Predictive Maintenance Dataset**, which contains 10,000 instances of industrial sensor data.

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

## Authors

- BRIERE Yann
- MAURE Mathieu

## Technologies Used

- Python
- TensorFlow/Keras
- Pandas
- Matplotlib
- Scikit-learn
- Imbalanced-learn (for data balancing)

## Installation and Usage

1. Clone this repository
2. Ensure the necessary dependencies are installed
3. Open the `TP_IA_EMBARQUEE.ipynb` notebook with Jupyter Notebook or JupyterLab
4. Execute the notebook cells sequentially to follow the complete process