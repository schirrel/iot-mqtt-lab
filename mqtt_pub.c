#include <stdio.h>
#include <mosquitto.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constants.h"

int offset = 5;

char *itoa(int val, int base)
{

    static char buf[32] = {0};

    int i = 30;

    for (; val && i; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i + 1];
}

char *generateRandom(const int min, const int max)
{
    int generated = rand() % ((max + 1) - min) + min;

    char *str = itoa(generated, 10);
    return str;
}

char *randomTemperature()
{
    int tempIdealMin = SENSOR_TEMP_MIN - offset;
    int tempIdealMax = SENSOR_TEMP_MAX + offset;
    return generateRandom(tempIdealMin, tempIdealMax);
    ;
}

char *randomHumidity()
{
    int humIdealMin = SENSOR_HUM_MIN - offset;
    int humIdealMax = SENSOR_HUM_MAX + offset;
    return generateRandom(humIdealMin, humIdealMax);
}

int main()
{
    int errorCode;
    struct mosquitto *msqt;

    mosquitto_lib_init();

    msqt = mosquitto_new("publisher-test", true, NULL);

    errorCode = mosquitto_connect(msqt, "localhost", 1883, 60);

    if (errorCode != 0)
    {
        printf("Failed to connect to broker! error %d\n", errorCode);
        mosquitto_destroy(msqt);
        return -1;
    }

    printf("Succesfully connected to broker\n");

    printf("Starting sensor monitoring\n");

    for (int count = 0; count < 100; count++)
    {

        sleep(4);
        char *temp = randomTemperature();

        mosquitto_publish(msqt, NULL, SENSOR_TOPIC_TEMP, 6, temp, 0, false);

        sleep(1);
        char *hum = randomHumidity();
        mosquitto_publish(msqt, NULL, SENSOR_TOPIC_HUM, 6, hum, 0, false);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("Data sent at %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    }

    mosquitto_disconnect(msqt);
    mosquitto_destroy(msqt);

    mosquitto_lib_cleanup();

    printf("Finished\n");

    return 0;
}
