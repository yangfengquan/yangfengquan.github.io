#include <stdio.h>

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
void read()
{
    /*
    struct Insul insul[71];
    FILE *fp = fopen("insulation.dat","rb");
    fread(insul, sizeof(insul[0]),71,fp);
    for (size_t i = 0; i < 71; i++)
    {
        printf("%d %s %lf\n",insul[i].id,insul[i].name,insul[i].strength_196);
    }
    fclose(fp);
    */
   struct Material material[226];
    FILE *fp = fopen("material.dat","rb");
    fread(material, sizeof(material[0]),226,fp);
    for (size_t i = 0; i < 226; i++)
    {
        printf("%d %s %s\n",material[i].id,material[i].pipeName,material[i].remark);
    }
    fclose(fp);
}
void txtToDat()
{
    struct CTYW ctyw;    
    FILE *ctywcsv = fopen("ctyw.txt","r");
    FILE *ctywdat = fopen("ctyw.dat","wb");
    rewind(ctywcsv);
    while (!feof(ctywcsv))
    {
        fscanf(ctywcsv,"%lf %s %s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %s\n",
            &ctyw.id,ctyw.province,ctyw.city,&ctyw.ta_year,&ctyw.ta_day_5,&ctyw.ta_day_8,
            &ctyw.ta_month_h,&ctyw.ta_ac,&ctyw.phi,&ctyw.w_winter_m,&ctyw.w_winter,
            &ctyw.w_summer,&ctyw.ta_lowest,&ctyw.ta_highest,ctyw.styear);
        fwrite(&ctyw,sizeof(ctyw),1,ctywdat);
    }
    fclose(ctywcsv);
    fclose(ctywdat);
    
    struct  Insul insul;  
    FILE *insulcsv = fopen("insulation.txt","r");
    FILE *insuldat = fopen("insulation.dat","wb");
    rewind(insulcsv);
    while (!feof(insulcsv))
    {
        fscanf(insulcsv,"%u %s %s %u %u %d %d %d %d %lf %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
            &insul.id,insul.name,insul.remark,&insul.rho_min,&insul.rho_max,&insul.allowTemperature_min,
            &insul.allowTemperature_max,&insul.reTemperature_min,&insul.reTemperature_max,&insul.lambda0,
            &insul.tm_min,&insul.tm_max,&insul.a,&insul.b,&insul.c,&insul.d,&insul.e,&insul.f,&insul.g,
            &insul.strength,&insul.strength_196);
        fwrite(&insul,sizeof(insul),1,insuldat);
    }
    fclose(insulcsv);
    fclose(insuldat);

    struct  Material material;  
    FILE *materialcsv = fopen("material.txt","r");
    FILE *materialdat = fopen("material.dat","wb");
    rewind(materialcsv);
    while (!feof(materialcsv))
    {
        fscanf(materialcsv,"%u %s %s %s %s %s %s %s %s %s %u %u %s %u %u %u",
            &material.id,material.stressCode,material.ShapeCode,material.Class,material.ClassCode,
            material.pipeName,material.weldingCode,material.pipeCode,material.Code,material.process,
            &material.thickness_min,&material.thickness_max,material.temperature_min,&material.rm,
            &material.rel,&material.temperature_max);
        for (size_t i = 0; i < 36; i++)
        {
            fscanf(materialcsv, " %lf", &material.stress[i]);
        }
        fscanf(materialcsv, " %lf %s", &material.stress_tmax, material.remark);
        fwrite(&material,sizeof(material),1,materialdat);
    }
    fclose(materialcsv);
    fclose(materialdat);
}
int main()
{
    txtToDat();
    return 0;
}