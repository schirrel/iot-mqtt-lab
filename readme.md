
## Deps
sudo apt-get install gcc mosquitto mosquitto-clients libmosquitto-dev 


## Exec

### Subscriber
- `gcc -c constants.c`                     
- `gcc -c mqtt_sub.c `                  
- `gcc constants.o mqtt_sub.o -o mqtt_sub -lmosquitto`
- `./mqtt_sub`

O simple use the **.sh** file as
- `./sub.sh`

### Publisher
- `gcc -c constants.c`                     
- `gcc -c mqtt_sub.c `                  
- `gcc constants.o mqtt_pub.o -o mqtt_pub -lmosquitto`
- `./mqtt_pub`


O simple use the **.sh** file as
- `./pub.sh`