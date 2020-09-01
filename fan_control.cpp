#include <iostream>

#include <wiringPi.h>

#define MIN_TEMP 35
#define MAX_TEMP 65

#define OFF_FAN 0
#define MIN_FAN 20
#define MAX_FAN 100

#define PWM_Range 76
#define PWM_Clock 28 // 10 for Pi3, 28 for Pi4
#define PWM_Fan_Pin 24 // WPI24 corresponding to GPIO19

#define DELAY 2000 // delay between temperature checks

void read_temp(float & temp) {
    static FILE *temp_file;
    temp_file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    fscanf(temp_file, "%f", &temp);
    fclose(temp_file);
    temp /= 1000.0f;
    #ifdef DEBUG
        std::cout << temp << "°C" << std::endl;
    #endif    
}

int get_fan_value(const float & temp) {
    if(temp > MAX_TEMP) return MAX_FAN;
    if(temp <= MIN_TEMP) return OFF_FAN;
    int value = MIN_FAN + (temp-MIN_TEMP) / (MAX_TEMP-MIN_TEMP) * (MAX_FAN-MIN_FAN);
    #ifdef DEBUG
        std::cout << value << "%" << std::endl;
    #endif  
    return value;
}

int main(void) {
    if(wiringPiSetup()==-1)
        exit(1);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(PWM_Range);
    pwmSetClock(PWM_Clock);
    pinMode(PWM_Fan_Pin, PWM_OUTPUT); // set PWM_Fan_Pin as output

    float temp;
    for(;;) {
        read_temp(temp);
        pwmWrite(PWM_Fan_Pin, get_fan_value(temp));
        delay(DELAY);
    }
}
        