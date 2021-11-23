#ifndef COMMON_H
#define COMMON_H

struct CTYW      // 附录C
{
    double id;
    char province[20];
    char city[20];
    double ta_year;     //保温常年运行
    double ta_day_5;    //采暖季运行
    double ta_day_8;    //采暖季运行
    double ta_month_h;  //防烫
    double ta_ac;       //防结露
    double ta_lowest;   //防冻
    double ta_highest;
    double phi;         //防结露
    double w_winter_m;
    double w_winter;
    double w_summer;
    char styear[10];
};

struct Insul
{
    unsigned int id;
    char name[40];
    char remark[20];
    unsigned int rho_min;
    unsigned int rho_max;
    int allowTemperature_min;
    int allowTemperature_max;
    int reTemperature_min;
    int reTemperature_max;
    double lambda0;
    int tm_min;
    int tm_max;
    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
    double g;
    double strength;
    double strength_196;
};

struct Material
{
    unsigned int id;
    char stressCode[20];
    char ShapeCode[20];
    char Class[50];
    char ClassCode[20];
    char pipeName[40];
    char weldingCode[20];
    char pipeCode[20];
    char Code[20];
    char process[20];
    unsigned int thickness_min;
    unsigned int thickness_max;
    char temperature_min[20];
    unsigned int rm;
    unsigned int rel;
    unsigned int temperature_max;
    double stress[36];
    double stress_tmax;
    char remark[20];
};

int reader(const char* path, struct CTYW* ctyw, int i);
int reader(const char* path, struct Insul* insul, int i);
int reader(const char* path, struct Material* material, int i);
#endif
