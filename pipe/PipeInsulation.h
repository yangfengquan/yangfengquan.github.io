#ifndef PIPEINSULATION_H
#define PIPEINSULATION_H
#include "common.h"
#include "Pipe.h"
#include <math.h>
#include "Water.h"
using namespace std;

class PipeInsulation : Pipe
{
private:
    /* data */
    double mT0;
    double mTm1;
    double mTm2;
    size_t mIInsul1;
    size_t mIInsul2;
    size_t mEpsilon;
    size_t mK;
    double f_x(double a, double y);  //y = xln(x/a)
public:
    enum Mode{HOT, PREFREEZE, COOL, PREDEW};
    static const string wall[10]; //表5.8.9
    static const double epsilon_low[10]; //表5.8.9
    static const double epsilon_up[10]; //表5.8.9
    static const string coolMK[6]; //表5.9.8
    static const double k_low[6];  //表5.9.8
    static const double k_up[6];   //表5.9.8
    struct CTYW* ctyw; //附录C 环境温度、相对湿度
    double raise = 0.5;
    size_t iCity;

    PipeInsulation(/* args */);
    PipeInsulation(double D, double p, double t, double iCity, size_t iInsul1, size_t iInsul, size_t iEpsilon, size_t iK);
    ~PipeInsulation();

    /**
     * @brief 5.3.1-1/5.3.1-8
     *
     * @param d1
     * @param d2
     * @param k
     * @return double
     */
    double delta(double d1, double k);
    double delta(double d1, double d2, double k);

    /**
     * @brief 5.3.3-1 圆筒单层，热冷损失与绝热层外径关系式
     *
     * @param Q
     * @param ta
     * @return double 绝热层外径，mm
     */
    double d1(double Q, double ta);

    /**
     * @brief 5.3.4-1 圆筒双层，热冷损失与外层绝热层外径关系式
     *
     * @param Q
     * @param t1
     * @param ta
     * @return double
     */
    double d2(double Q, double t1, double ta);

    /**
     * @brief 5.3.4-2 圆筒双层，热冷损失与内层绝热层外径关系式
     *
     * @param Q
     * @param d2
     * @param t1
     * @return double
     */
    double d1_2(double Q, double d2, double t1);

    /**
     * @brief 5.3.7 5.3.11 圆筒单层，表面温度与绝热层外径的关系式
     *        限于防烫、防结露
     * @param ts
     * @param ta
     * @return double
     */
    double d1_ts(double ts, double ta);

    /**
     * @brief 5.3.8-1 圆筒双层，表面温度外层绝热层外径关系式
     *                限于防烫、防结露
     * @param t1
     * @param ta
     * @param ts
     * @return double
     */
    double d2_ts(double t1, double ta, double ts);

    /**
     * @brief 5.3.8-2 圆筒双层，表面温度内层绝热层外径关系式
     *        限于防烫、防结露
     * @param d2
     * @param t1
     * @param ts
     * @param ta
     * @return double
     */
    double d1_ts_2(double d2, double t1, double ts, double ta);

    /**
     * @brief 5.3.8-3 圆筒双层，表面温度与外层绝热层外径关系式
     *
     * @param d1
     * @param t1
     * @param ts
     * @param ta
     * @return double
     */
    double d2_d1_ts(double d1, double t1, double ts, double ta);

    /**
     * @brief 5.3.13 延迟管道内介质冻结、凝固、结晶与绝热层外径关系式
     *
     * @param tfr 介质凝固点 C
     * @param timefr 停留时间 h
     * @param rho 介质密度 kg/m3
     * @param c 介质热容 J/(kg-K)
     * @param rhoP 关闭密度 kg/m3
     * @param cP 管壁热容 J/(kg-K)
     * @return double
     */
    double d1_timefr(double tfr, double timefr, double rho, double c, double _alphas = 8.141, double rhoP = 7850, double cP = 460);

    /**
     * @brief 5.3.14-1 液体管道温度降与绝热层外径的关系式
     *
     * @param lAB A、B之间管道实际长度 m
     * @param w 介质流速 m/s
     * @param rho 介质密度 kg/m3
     * @param c 介质热容 J/(kg-K)
     * @param tA 介质在上游A点处的温度 C
     * @param tB 介质在下游B点出的温度 C
     * @return double
     */
    double d1_tAB_liquid(double lAB, double w, double rho, double c, double tA, double tB, double _alphas = 8.141);

    /**
     * @brief 附录B 最大允许热损失
     *
     * @return double
     */
    double Q_max(bool isYearly);

    /**
     * @brief 5.4.2-1、5.4.2-2 最大允许冷损失
     *
     * @return double
     */
    double Q_max(double ta);

    /**
     * @brief 5.4.3-1 圆筒单层， 热冷损失关系式
     *
     * @param d1
     * @return double W/m2
     */
    double Q(double d1, double _alphas);

    /**
     * @brief 5.4.3-2 圆筒单层， 热冷损失关系式
     *
     * @param d1
     * @param Q
     * @return double W/m
     */
    double q(double d1, double Q);

    /**
     * @brief 5.4.3-3 圆筒双层， 热冷损失关系式
     *
     * @param d1
     * @param d2
     * @return double W/m2
     */
    double Q(double d1, double d2, double _alphas);

    /**
     * @brief 5.5.1 绝热表面温度
     *
     * @param Q
     * @param ta
     * @return double
     */
    double ts(double Q, double ta, double _alphas);

    /**
     * @brief 5.5.2 绝热表面温度
     *
     * @param d2
     * @param q
     * @param ta
     * @return double
     */
    double ts(double d2, double q, double ta, double _alphas);

    /**
     * @brief 5.6.1 圆筒双层，绝热层间温度关系式
     *
     * @param d1
     * @param d2
     * @param ts
     * @return double
     */
    double t1(double d1, double d2, double ts);

    /**
     * @brief 5.8.4-1、5.8.4-2、5.8.4-3、5.8.4-4 表面换热系数，包含辐射换热系数和对流换热系数
     *
     * @param d1
     * @param ts
     * @param ta
     * @param w 防冻计算中，取冬季最多风向平均风速
     * @return double
     */
    double alphas(double d1, double ts, double ta, double w);

    /**
     * @brief 5.8.4.2、5.9.4 仅限防烫、防结露、允许冷损失计算
     *
     * @return double
     */
    double alphas();

    /**
     * @brief 5.3.4 双层，绝热层间最大允许温度，绝热材料推荐温度*0.9
     *
     * @return double
     */
    double t1_max();

    /**
     * @brief 判断是保温还是保冷
     *
     * @return true
     * @return false
     */
    bool isHeatInsulation();

    /**
     * @brief 表A，获取传热系数
     *
     * @param i
     * @param tm
     * @return double
     */
    double lambda(size_t i, double tm);

    /**
     * @brief 表5.8.9 黑度
     *
     * @param i
     * @param raise
     * @return double raise * (up - low) + low
     */

    double epsilon(size_t i, double raise = 0.5);


    /**
     * @brief 表5.9.8 保冷厚度修正系数
     *
     * @param i
     * @param raise
     * @return double  raise * (up - low) + low
     */
    double k(size_t i, double raise = 0.5);

    /**
     * @brief 露点温度，调用Water.h
     *        相对湿度，指空气中水汽压与相同温度下饱和水汽压的百分比。
     * @return double
     */
    double td();

    /**
     * @brief 圆筒单层，绝热层外径与热冷损失量、绝热层外表面温度关系式，传热学2-30
     *
     * @param Q
     * @param ts
     * @return double
     */
    double d1_i(double Q, double ts);

    /**
     * @brief 圆筒双层，绝热层外径与热冷损失量、绝热层外表面温度关系式，传热学2-30
     *
     * @param Q
     * @param t1
     * @param ts
     * @return double
     */
    double d2_i(double Q, double t1, double ts);

    /**
     * @brief 圆筒单层，热冷损失量与绝热层外径、外表面温度关系式，绝热层传热学 2-30
     *
     * @param d1
     * @param ts
     * @return double
     */
    double Q_i(double d1, double ts);

    /**
     * @brief 圆筒双层，热冷损失量与绝热层外径、外表面温度关系式，绝热层传热学 2-30
     *
     * @param d1
     * @param d2
     * @param ts
     * @return double
     */
    double Q_i(double d1, double d2, double ts);

    double tm1(double ta, double w);

    void solve(double Q);
};
#endif
