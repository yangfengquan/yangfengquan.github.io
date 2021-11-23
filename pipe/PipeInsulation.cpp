#include "PipeInsulation.h"
double PipeInsulation::f_x(double a, double y)
{
    double x0 = 0, x1 = 10;
    do
    {
        double mid = (x0 + x1) / 2;
        if (mid * log(mid / a) - y > 0)
        {
            x1 = mid;
        }
        else
        {
            x0 = mid;
        }
    } while (abs(x0 - x1) > 1e-6);
    return x0;
}
const string PipeInsulation::wall[10] = {
        "铝合金薄板",
        "不锈钢薄板",
        "有光泽的镀锌薄钢板",
        "已氧化的镀锌薄钢板",
        "纤维织物",
        "水泥砂浆",
        "铝粉漆",
        "黑漆（有光泽）",
        "黑漆（无光泽）",
        "油漆"
    };
const double PipeInsulation::epsilon_low[10] = {0.15, 0.2, 0.23, 0.28, 0.7, 0.69, 0.41, 0.88, 0.96, 0.8};//表5.8.9
const double PipeInsulation::epsilon_up[10] = {0.3, 0.4, 0.27, 0.32, 0.8, 0.69, 0.41, 0.88, 0.96, 0.9};//表5.8.9
const string PipeInsulation::coolMK[6] = {
        "聚苯乙烯",
        "聚氨酯",
        "聚异氰脲酸酯",
        "泡沫玻璃",
        "泡沫橡塑",
        "酚醛"
    };
const double PipeInsulation::k_low[6] = {1.2, 1.2, 1.2, 1.1, 1.2, 1.2};//表5.9.8
const double PipeInsulation::k_up[6] = {1.4, 1.4, 1.35, 1.2, 1.4, 1.4};//表5.9.8


PipeInsulation::PipeInsulation(/* args */)
{
}

PipeInsulation::PipeInsulation(double D, double p, double t, double iCity, size_t iInsul1, size_t iInsul2, size_t iEpsilon, size_t iK)
    : Pipe(D, p, t)
{
    mT0 = t;
    mIInsul1 = iInsul1;
    mIInsul2 = iInsul2;
    reader("ctyw.dat", ctyw, iCity);
    epsilon(iEpsilon);
    k(iK);
}

PipeInsulation::~PipeInsulation()
{
}

/**
 * @brief 5.3.1-1/5.3.1-8
 *
 * @param d1
 * @param d2
 * @param k
 * @return double
 */
double PipeInsulation::delta(double d1, double k = 1)
{
    return k * 0.5 * (d1 - mDo);
}
double PipeInsulation::delta(double d1, double d2, double k = 1)
{
    return k * 0.5 * (d2 - d1);
}

/**
 * @brief 5.3.3-1 圆筒单层，热冷损失与绝热层外径关系式
 *
 * @param Q
 * @param ta
 * @return double 绝热层外径，mm
 */
double PipeInsulation::d1(double Q, double ta)
{
    double y = 2 * lambda(mIInsul1, mTm1) * ((mT0 - ta) / Q - 1 / alphas());
    return f_x(mDo, y);
}

/**
 * @brief 5.3.4-1 圆筒双层，热冷损失与外层绝热层外径关系式
 *
 * @param Q
 * @param t1
 * @return double
 */
double PipeInsulation::d2(double Q, double t1, double ta)
{
    double y = 2 * ((lambda(mIInsul1, mTm1) * (mT0 - t1) + lambda(mIInsul2, mTm2) * (t1 - ta)) / Q - lambda(mIInsul2, mTm2) / alphas());
    return f_x(mDo, y);
}

/**
 * @brief 5.3.4-2 圆筒双层，热冷损失与内层绝热层外径关系式
 *
 * @param Q
 * @param d2
 * @return double
 */
double PipeInsulation::d1_2(double Q, double d2, double t1)
{
    double y = 2 * lambda(mIInsul1, mTm1) / d2 * (mT0 - t1) / Q;
    return pow(M_E, y) * mDo;
}

/**
 * @brief 5.3.7 5.3.11 圆筒单层，表面温度与绝热层外径的关系式
 *        限于防烫、防结露
 * @param ts
 * @param ta
 * @return double
 */
double PipeInsulation::d1_ts(double ts, double ta)
{
    double y = 2 * lambda(mIInsul1, mTm1) / alphas() * (mT0 - ts) / (ts - ta);
    return f_x(mDo, y);
}

/**
 * @brief 5.3.8-1 圆筒双层，表面温度外层绝热层外径关系式
 *        限于防烫、防结露
 * @param t1
 * @param ta
 * @param ts
 * @return double
 */
double PipeInsulation::d2_ts(double t1, double ta, double ts)
{
    double y = 2 / alphas() * ((lambda(mIInsul1, mTm1) * (t1 - mT0) + lambda(mIInsul2, mTm2) * (ts - t1)) / (ta - ts));
    return f_x(mDo, y);
}


/**
 * @brief 5.3.8-2 圆筒双层，表面温度内层绝热层外径关系式
 *        限于防烫、防结露
 * @param d2
 * @param t1
 * @param ts
 * @param ta
 * @return double
 */
double PipeInsulation::d1_ts_2(double d2, double t1, double ts, double ta)
{
    double y = 2 * lambda(mIInsul1, mTm1) / (d2 * alphas()) * (t1 - mT0) / (ta - ts);
    return pow(M_E, y) * mDo;
}

/**
 * @brief 5.3.13 延迟管道内介质冻结、凝固、结晶与绝热层外径关系式
 *
 * @param tfr 介质凝固点 C
 * @param timefr 停留时间 h
 * @param rho 介质密度
 * @param c 介质热容
 * @param rhoP 关闭密度
 * @param cP 管壁热容
 * @return double
 */
double PipeInsulation::d1_timefr(double tfr, double timefr, double rho, double c, double _alphas, double rhoP, double cP)
{
    double kr = 1.1 * mDo * 0.2;
    if (kr > 1.2)
        kr = 1.2;
    double ta = ctyw->ta_lowest;
    double _lambda = lambda(mIInsul1, mTm1);
    //double _alphas = alphas();
    double v = M_PI * mDi * mDi / 4;
    double vP = M_PI * mDo * mDo / 4 -v;

    double y = 7200 * kr * M_PI * _lambda * timefr / ((v * rho * c + vP * rhoP * cP) * log((mT0 - ta) / (tfr - ta)));
    double x0 = 0, x1 = 10;
    do
    {
        double mid = (x0 + x1) / 2;
        if (log(mid / mDo) + 2 * _lambda / (mid * _alphas) > y)
        {
            x1 = mid;
        }
        else
        {
            x0 = mid;
        }

    } while (abs(x0 - x1) > 1e-6);
    return x0;
}

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
double PipeInsulation::d1_tAB_liquid(double lAB, double w, double rho, double c, double tA, double tB, double _alphas)
{
    double kr = 1.1 * mDo * 0.2;
    if (kr > 1.2)
        kr = 1.2;
    double ta = ctyw->ta_lowest;
    double _lambda = lambda(mIInsul1, mTm1);
    //double _alphas = alphas();
    double y = 8 * _lambda * lAB * kr / (mDi * w * rho * c * log((tA - ta) / (tB - ta)));
    double x0 = 0, x1 = 10;
    do
    {
        double mid = (x0 + x1) / 2;
        if (log(mid / mDo) + 2 * _lambda / (mid * _alphas) > y)
        {
            x1 = mid;
        }
        else
        {
            x0 = mid;
        }

    } while (abs(x0 - x1) > 1e-6);
    return x0;
}

/**
 * @brief 附录B 最大允许热损失
 *
 * @return double
 */
double PipeInsulation::Q_max(bool isYearly = true)
{
    if (isYearly)
    {
        return 3.8636 * pow(mT0, 0.6619);
    }
    else if(mT0 <= 300)
    {
        return 12.098 *pow(mT0, 0.5461);
    }
    else
    {
        return -1;
    }
}

/**
 * @brief 5.4.2-1、5.4.2-2 最大允许冷损失
 *
 * @return double
 */
double PipeInsulation::Q_max(double ta)
{
    if ((ta - td()) <= 4.5)
    {
        return -(ta - td()) * alphas();
    }
    else
    {
        return -4.5 * alphas();
    }
}

/**
 * @brief 5.4.3-1 圆筒单层， 热冷损失关系式
 *
 * @param d1
 * @return double W/m2
 */
double PipeInsulation::Q(double d1, double _alphas)
{
    return (mT0 - ctyw->ta_year) / (d1 / (2 * lambda(mIInsul1, mTm1)) * log(d1 / mDo) + 1 / _alphas);
}

/**
 * @brief 5.4.3-2 圆筒单层， 热冷损失关系式
 *
 * @param d1
 * @param Q
 * @return double W/m
 */
double PipeInsulation::q(double d1, double Q)
{
    return M_PI * d1 * Q;
}

/**
 * @brief 5.4.3-3 圆筒双层， 热冷损失关系式
 *
 * @param d1
 * @param d2
 * @return double W/m2
 */
double PipeInsulation::Q(double d1, double d2, double _alphas)
{
    return (mT0 - ctyw->ta_year) / (d2 / (2 * lambda(mIInsul1, mTm1)) * log(d1 / mDo) + d2 / (2 * lambda(mIInsul2, mTm2)) * log(d2 / mDo) + 1 / _alphas);
}

/**
 * @brief 5.5.1 绝热表面温度
 *
 * @param Q
 * @param ta
 * @return double
 */
double PipeInsulation::ts(double Q, double ta, double _alphas)
{
    return Q / _alphas + ta;
}

/**
 * @brief 5.5.2 绝热表面温度
 *
 * @param d2
 * @param q
 * @param ta
 * @return double
 */
double PipeInsulation::ts(double d2, double q, double ta, double _alphas)
{
    return q / (M_PI * d2 *  _alphas) + ta;
}

/**
 * @brief 5.6.1 圆筒双层，绝热层间温度关系式
 *
 * @param d1
 * @param d2
 * @param ts
 * @return double
 */
double PipeInsulation::t1(double d1, double d2, double ts)
{
    double m = lambda(mIInsul1, mTm1) * mT0 * log(d2/d1) + lambda(mIInsul2, mTm2) * ts * log(d1/mDo);
    double n = lambda(mIInsul1, mTm1) * log(d2/d1) + lambda(mIInsul2, mTm2) * log(d1/mDo);
    return m / n;
}

/**
 * @brief 5.8.4-1、5.8.4-2、5.8.4-3、5.8.4-4 表面换热系数，包含辐射换热系数和对流换热系数
 *
 * @param d1
 * @param ts
 * @param ta
 * @param w
 * @return double
 */
double PipeInsulation::alphas(double d1, double ts, double ta, double w)
{
    double alphar = 5.669 * mEpsilon / (ts - ta) * (pow((273 + ts) / 100, 4) - pow((273 + ta) / 100, 4));//[5.8.4-1]
    double alphac;
    if (w == 0)
    {
        alphac = 26.4 / sqrt(297 + 0.5 * (ts + ta)) * pow((ts - ta) / d1, 0.25);//[5.8.4-2]
    }
    else if (w * d1 <= 0.8)
    {
        alphac = 0.08 / d1 + 4.2 * pow(w, 0.618) / pow(d1, 0.382);//[5.8.4-3]
    }
    else
    {
        alphac = 4.53 * pow(w, 0.805) / pow(d1, 0.195);//[5.8.4-4]
    }

    return alphar + alphac;
}

/**
 * @brief 5.8.4.2、5.9.4 仅限防烫、防结露、允许冷损失计算
 *
 * @return double
 */
double PipeInsulation::alphas()
{
    return 8.141;
}

/**
 * @brief 5.3.4 双层，绝热层间最大允许温度，绝热材料推荐温度*0.9
 *
 * @return double
 */
double PipeInsulation::t1_max()
{
    struct Insul* insul;
    reader("insulation.dat", insul, mIInsul2);
    return 0.9 * insul->reTemperature_max; //保冷未考虑
}

/**
 * @brief 判断是保温还是保冷
 *
 * @return true
 * @return false
 */
bool PipeInsulation::isHeatInsulation()
{
    if (mT0 > ctyw->ta_month_h)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief 表A，获取传热系数
 *
 * @param i
 * @param tm
 * @return double
 */

double PipeInsulation::lambda(size_t i, double tm)
{
    struct Insul* insul;
    reader("insulation.dat", insul, i);
    if(tm > insul->tm_max)
        reader("insulation.dat", insul, i + 1);
    return insul->a + insul->b * (tm + insul->c) + insul->d * pow((tm + insul->e), 2) + insul->f * pow((tm + insul->g), 3);

}
double PipeInsulation::epsilon(size_t i, double raise)
{
   mEpsilon = raise * (epsilon_up[i] - epsilon_low[i]) + epsilon_low[i];
   return mEpsilon;
}

double PipeInsulation::k(size_t i, double raise)
{
    mK = raise * (k_up[i] - k_low[i]) + k_low[i];
    return mK;
}

/**
 * @brief 露点温度，调用Water.h
 *        相对湿度，指空气中水汽压与相同温度下饱和水汽压的百分比。
 * @return double
 */
double PipeInsulation::td()
{
    Water w;
    double psat = w.Psat(ctyw->ta_ac + 273.15);
    double pw = ctyw->phi / 100 * psat;
    return w.Tsat(pw) - 273.15;
}

/**
 * @brief 圆筒单层，绝热层外径与热冷损失量、绝热层外表面温度关系式，传热学2-30
 *
 * @param Q
 * @param ts
 * @return double
 */
double PipeInsulation::d1_i(double Q, double ts)
{
    double tm = 0.5 * (mT0 + ts);
    double _lambda = lambda(mIInsul1, tm);
    double y = 2 * _lambda * (mT0 - ts) / Q;
    return f_x(mDo, y);
}

/**
 * @brief 圆筒双层，绝热层外径与热冷损失量、绝热层外表面温度关系式，传热学2-30
 *
 * @param Q
 * @param t1
 * @param ts
 * @return double
 */
double PipeInsulation::d2_i(double Q, double t1, double ts)
{
    double tm1 = 0.5 * (mT0 + t1);
    double tm2 = 0.5 * (t1 + ts);
    double lambda1 = lambda(mIInsul1, tm1);
    double lambda2 = lambda(mIInsul2, tm2);

    double y = 2 * (lambda1 * (mT0, - t1) + lambda2 * (t1 - ts)) / Q;
    return f_x(mDo, y);
}

/**
 * @brief 传热学 2-30
 *
 * @param d1
 * @param ts
 * @return double
 */
double PipeInsulation::Q_i(double d1, double ts)
{
    double tm = 0.5 * (mT0 + ts);
    double _lambda = lambda(mIInsul1, tm);
    return 2 * _lambda / d1 * (mT0 - ts) / log(d1 / mDo);
}

/**
 * @brief 圆筒双层，热冷损失量与绝热层外径、外表面温度关系式，绝热层传热学 2-30
 *
 * @param d1
 * @param d2
 * @param ts
 * @return double
 */
double PipeInsulation::Q_i(double d1, double d2, double ts)
{
    double _t1 = t1(d1, d2, ts);
    double tm1 = 0.5 * (mT0 + _t1);
    double tm2 = 0.5 * (_t1 + ts);
    double lambda1 = lambda(mIInsul1, tm1);
    double lambda2 = lambda(mIInsul2, tm2);

    return 2 * (mT0 - ts) / (log(d1/mDo) / lambda1 + log(d2/d1) / lambda2);
}


double PipeInsulation::tm1(double ta, double w)
{
    double Q = Q_max();
    double x0 = mT0, x1 = ta;
    do
    {
        double _ts = 0.5 * (x0 + x1);

        double _d1 = d1_i(Q, _ts);
        double _alphas = alphas(_d1, _ts, ta, w);
        double _tsc = ts(Q, ta, _alphas);
        if (abs(_ts - ta) > abs(_tsc - ta))
        {
            x0 = _ts;
        }
        else
        {
            x1 = _ts;
        }
    } while (abs(x0 - x1) < 1e-6);
    return x0;
}

void PipeInsulation::solve(double Q)
{

}
