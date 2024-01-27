# IoT-Based-Patient-Health-Monitoring-System-using-ESP32-and-AWS-IOT-core

Keeping track of the health status of a patient at home is a difficult task because
of the busy schedules and our lifestyle. Especially the elderly need regular
checkups and monitoring the vital parameters can be life saving. So we propose
an innovative system where by this task can be handled in an automated manner.
Our device put forward a smart patient health tracking system , so that the
Patient health parameters like Heart Rate and Blood Oxygen level along with
body temperature can be monitored. We are using the MAX30100/102 Pulse
Oxi-meter sensor to measure the Heart Rate/Pulse(BPM) as well as the Blood
Oxygen Level(SpO2) and using a DS18B20 Temperature Sensor to measure the
temperature of the body.
ESP32 is serving as the main controlling unit in the proposed system. The
sensors are connected to ESP32, ESP32 gathers data sensed by the sensors and
continuously transmits it to the cloud over the internet . The cloud platform
we have used is AWS IoT core. The connection of ESP 32 and AWS IOT
Core is done by MQTT protocol .forstoring the data from the sensors to the
cloud ,amazon DynamoDB is used .Amazon DynamoDB is a fully managed
proprietary No SQL database offered by Amazon.com as part of the Amazon
Web Services. 
