# 🌞 Self Sustaining Cost Effective Wearable for Monitoring Sun Exposure
## 📌 Project Overview

This project presents a wearable, self-sustaining UV exposure monitoring system that leverages embedded hardware and machine learning to classify high UV exposure levels. The system integrates a solar panel as both a power source and UV sensing mechanism, enabling cost-effective and lightweight deployment.

Approximately 5,000 data samples were collected to study the correlation between solar panel power output and UV sensor intensity, demonstrating that solar power output reliably tracks UV exposure patterns.

## 🛠️ Hardware Components

ESP32 – Microcontroller for data acquisition and processing

INA219 – Power monitoring sensor for solar panel output

UV Sensor – Ground-truth UV intensity measurement

Solar Panel – Power source and indirect UV sensing element

## 📊 Dataset & Experimental Setup

Total samples: ~5,000

Dataset split:

70% Training

15% Validation

15% Testing

This split ensures that model performance is evaluated on previously unseen data, preventing overfitting.

## 🤖 Machine Learning Models

The following models were trained and evaluated for binary classification of high UV exposure:

Logistic Regression

Support Vector Machine (SVM)

Baseline Model (reference performance)

## 📈 Evaluation Metrics

Accuracy

Precision

Recall

F1 Score

## ✅ Results
Model	Accuracy
Logistic Regression	97%
SVM	96%
Baseline Model	93%

Logistic Regression achieved the best overall performance with 97% accuracy

SVM closely followed with 96% accuracy

The solar panel’s power output closely matched UV sensor readings, validating its use as a proxy UV detector

## 🌍 Key Contributions

Demonstrated a strong correlation between solar panel output and UV intensity

Replaced dedicated UV sensing with a dual-purpose solar panel, reducing cost and complexity

Designed a self-powered, lightweight wearable system

Achieved high classification accuracy using simple, efficient ML models

## 🔮 Future Work

Extend classification to multi-level UV exposure categories

Deploy real-time inference on embedded hardware

Explore deep learning models for temporal UV pattern analysis
