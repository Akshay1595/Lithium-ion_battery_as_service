#ifndef CONFIG_H
#define CONFIG_H

/* Debug Lines */
#define DEBUG

#undef CREDS_IN_RS

static inline void debug_log(char *buff) {
#ifdef DEBUG
    Serial.println(buff);
#endif
}

/* Formula for conversion of watt hours to unit of currency */
#define WHR_PER_RS                      (10)

/* Battery Capacity */
#define BATTERY_VOLTAGE                 (48)
#define BATTERY_AHR_CAP                 (10)
#define BATTERY_CAP_IN_WHR              (BATTERY_VOLTAGE * BATTERY_AHR_CAP)

#endif