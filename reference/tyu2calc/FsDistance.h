#ifndef FSDISTANCE_H
#define FSDISTANCE_H

#include "FsSpace.h"

class FsDistance
{
public:
    FsDistance(double d = 0){ m_value = d;}

    double M() { return m_value;}
    double Km() { return m_value/1000.0;}
    double TKm() { return m_value/(1000.0*100000.0);}
    double SS() { return m_value/SND_SPEED;}
    double SEAMILE() { return m_value/SEA_MILE;}
    double FEET() { return m_value/FEET_M;}
    double INCH() { return m_value/INCH_M;}
    double YARD() { return m_value/YARD_M;}
    double MILE() { return m_value/MILE_M;}
    double LS() { return m_value/LIGHT_SPEED;}
    double LM() { return m_value/(BYMIN*LIGHT_SPEED);}
    double LH() { return m_value/(BYHOUR*LIGHT_SPEED);}
    double LD() { return m_value/(BYDAY*LIGHT_SPEED);}
    double LY() { return m_value/(BYYEAR*LIGHT_SPEED);}
    double AU() { return m_value/AU_M;}
    double PARSEC() { return m_value/PARSEC_M;}


    bool setM(double d) {
        bool ret = false;
        if  (m_value != d){
            m_value = d;
            ret = true;
        }
        return ret;
    }
    bool setKm(double d) { return setM(d * 1000.0);}
    bool setTKm(double d) { return setM(d * 1000.0 *100000.0);}
    bool setSS(double d) { return setM(d * SND_SPEED);}
    bool setSEAMILE(double d) { return setM(d * SEA_MILE);}
    bool setFEET(double d) { return setM(d * FEET_M);}
    bool setINCH(double d) { return setM(d * INCH_M);}
    bool setYARD(double d) { return setM(d * YARD_M);}
    bool setMILE(double d) { return setM(d * MILE_M);}
    bool setLS(double d) { return setM(d * LIGHT_SPEED);}
    bool setLM(double d) { return setM(d *(LIGHT_SPEED * BYMIN));}
    bool setLH(double d) { return setM(d *(LIGHT_SPEED * BYHOUR));}
    bool setLD(double d) { return setM(d *(LIGHT_SPEED * BYDAY));}
    bool setLY(double d) { return setM(d *(LIGHT_SPEED * BYYEAR));}
    bool setAU(double d) { return setM(d * AU_M);}
    bool setPARSEC(double d) {return setM(d * PARSEC_M);}

private:
        double m_value;
};
#endif // FSDISTANCE_H
