#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

// typedef struct date_t {
//     int year, month, day, hour, min, sec;
// } date_t;

// void printdate(date_t date) {
//     printf("%d-%02d-%02d %02d:%02d:%02d\n", date.year, date.month, date.day, date.hour, date.min, date.sec);
// }

// bool isvalid_date(date_t date) {
//     bool year_check = date.year >= 1900 || date.year <= 9999;
//     bool month_check = date.month >= 1 || date.month <= 12;
//     bool day_check = date.day >= 1 || date.day <= 31;

//     return year_check && month_check && day_check;
// }

// date_t getgmdate(time_t tm) {
//     date_t date;

//     struct tm* ltm_ptr = gmtime(&tm);
//     struct tm ltm = *ltm_ptr;

//     date.year = ltm.tm_year + 1900;
//     date.month = ltm.tm_mon + 1;
//     date.day = ltm.tm_mday;
//     date.hour = ltm.tm_hour;
//     date.min = ltm.tm_min;
//     date.sec = ltm.tm_sec;

//     free(ltm_ptr);
//     return date;
// }

// date_t getlocaldate(time_t tm) {
//     date_t date;

//     struct tm* ltm_ptr = localtime(&tm);
//     struct tm ltm = *ltm_ptr;

//     date.year = ltm.tm_year + 1900;
//     date.month = ltm.tm_mon + 1;
//     date.day = ltm.tm_mday;
//     date.hour = ltm.tm_hour;
//     date.min = ltm.tm_min;
//     date.sec = ltm.tm_sec;

//     free(ltm_ptr);
//     return date;
// }

int main() {
    time_t a = time(NULL) - ((60 * 60) * 24);
    printf("%lld\n", a);
    return 0;
}

// int main() {
//     // time_t tm = 0;
//     // struct tm* gtm = gmtime(&tm);
//     // printf("%s\n", asctime(gtm));

//     time_t new = time(NULL);
//     printf("%lld\n", new);

//     // 1 Jan 1970
    
//     struct tm* local = gmtime(&new);
//     printf("%s\n", asctime(local));

//     // time_t a = 1892736480;

//     // time_t day = 86400;
//     // time_t year = (((day * 7) * 4) * 12);

//     // printf("%lld\n", (a - new) / year);

//     // printf("Double: %lld\n", sizeof(double));
//     // printf("Float: %lld\n", sizeof(float));
//     // printf("Long long: %lld\n", sizeof(long long));
//     // printf("Long: %lld\n", sizeof(long));
//     // printf("Int: %lld\n", sizeof(int));
//     // printf("Short: %lld\n", sizeof(short));

//     return 0;
// }

// int main() {
//     // time_t tm = time(NULL);
//     // date_t date = getlocaldate(tm);

//     // printf("Current Date: ");
//     // printdate(date);

//     printf("Timezone: %s\n", tzname[0]);
//     printf("Timezone: %s\n", tzname[1]);
    
//     printf("%ld\n", timezone);

//     time_t tm;
//     time(&tm);
//     struct tm* gtm = gmtime(&tm);

//     printf("Date: %s", asctime(gtm));
//     printf("%ld\n", timezone);

//     // time_t parsed = mktime(ltm);
//     // printf("Reparsed Time: %lld\n", parsed);
//     return 0;
// }