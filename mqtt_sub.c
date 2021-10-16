#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include "constants.h"


void handleTemperature(char *value)
{
    printf("new Temperature %s\n", value);
}

void handleHumidity(char *value)
{
    printf("new Humidity %s\n", value);
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
