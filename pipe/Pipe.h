#ifndef PIPE_H
#define PIPE_H

class Pipe
{
protected:
    /* data */
    int mDN;        //公称直径
    double mDo, mDi;  //外径，内径；unit: mm
    double mP, mT;   //压力 unit: MPa，温度 unit: C
public:
    Pipe();
    /**
     * @brief Construct a new Pipe object
     *
     * @param D 外径 mm
     * @param p 压力 MPa
     * @param t 温度 C
     */
    Pipe(double D, double p, double t);
    ~Pipe();

};


#endif
