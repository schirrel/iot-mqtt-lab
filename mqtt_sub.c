#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include "constants.h"
#include <time.h>

int currentTemperature = 25;
int currentHumidity = 50;
int airConditionerState = 1; //1 - on | 0 - off

void handleTemperature(char *value)
{
    currentTemperature = atoi(value);
}

void handleHumidity(char *value)
{
    currentHumidity = atoi(value);
}

void validateSensors()
{
    int isTempIdeal = currentTemperature > SENSOR_TEMP_MIN && currentTemperature < SENSOR_TEMP_MAX;

    int isHumIdeal = currentHumidity > SENSOR_HUM_MIN && currentHumidity < SENSOR_HUM_MAX;

    int bothIdeal = isTempIdeal && isHumIdeal;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if (bothIdeal && !airConditionerState)
    {
        printf("Values of Temperature(%d) and Humidity(%d) are inside the threshold, turning Air conditioning on at %d-%02d-%02d %02d:%02d:%02d\n", currentTemperature, currentHumidity, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
    else if (!bothIdeal && airConditionerState)
    {

        printf("Values of Temperature(%d) and Humidity(%d) exceeds the threshold, turning Air conditioning off at %d-%02d-%02d %02d:%02d:%02d \n", currentTemperature, currentHumidity, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
    airConditionerState = bothIdeal;
}

void on_connect(struct mosquitto *msqt, void *obj, int errorCode)
{
    printf("ID: %d\n", *(int *)obj);

    if (errorCode)
    {
        printf("Error with result code: %d\n", errorCode);
        exit(-1);
    }

    mosquitto_subscribe(msqt, NULL, SENSOR_TOPIC_TEMP, 0);
    mosquitto_subscribe(msqt, NULL, SENSOR_TOPIC_HUM, 0);
}

void on_message(struct mosquitto *msqt, void *obj, const struct mosquitto_message *msg)
{

    if (strcmp(msg->topic, SENSOR_TOPIC_TEMP) == 0)
    {
        handleTemperature((char *)msg->payload);
    }
    if (strcmp(msg->topic, SENSOR_TOPIC_HUM) == 0)
    {
        handleHumidity((char *)msg->payload);
    }

    validateSensors();
}

int main()
{
    int errorCode, id = 12;

    mosquitto_lib_init();

    struct mosquitto *msqt;

    msqt = mosquitto_new("substribe-test", true, &id);
    mosquitto_connect_callback_set(msqt, on_connect);
    mosquitto_message_callback_set(msqt, on_message);

    errorCode = mosquitto_connect(msqt, "localhost", 1883, 10);

    if (errorCode)
    {
        printf("Could not connect to broker, error code: %d\n", errorCode);
        return -1;
    }

    mosquitto_loop_start(msqt);
    printf("Press enter to quit... \n");
    getchar();
    mosquitto_loop_stop(msqt, true);

    mosquitto_disconnect(msqt);

    mosquitto_destroy(msqt);

    mosquitto_lib_cleanup();

    return 0;
}
