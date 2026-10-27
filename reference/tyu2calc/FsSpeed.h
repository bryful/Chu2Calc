#ifndef FSSPEED_H
#define FSSPEED_H

#include "FsSpace.h"

class FsSpeed
{
public:
    FsSpeed(double d =0){ m_value = d;}

    double MPS(){return m_value;}
    double KMPS(){return m_value / 1000.0;}

    double MPM() { return m_value * BYMIN;}
    double KMPM(){ return m_value * (BYMIN/1000.0);}

    double MPH() { return m_value * BYHOUR;}
    double KMPH(){ return m_value * (BYHOUR/1000.0);}
    double TKMPH(){ return m_value * (BYHOUR/(1000.0*100000.0));}

    double MACH() { return m_value / SND_SPEED;}
    double KNOT(){ return m_value / KNOT_M;}
    double LS() { return m_value / LIGHT_SPEED;}
    double LM() { return m_value * (BYMIN / LIGHT_SPEED);}
    double LH() { return m_value * (BYHOUR/ LIGHT_SPEED);}
    double LD() { return m_value * (BYDAY/ LIGHT_SPEED);}
    double LY() { return m_value * (BYYEAR/ LIGHT_SPEED);}

    bool setMPS(double d) {
        bool ret = false;
        if (m_value != d) {
            m_value = d;
            ret = true;
        }
        return ret;
    }
    bool setKMPS(double d) { return setMPS(d*1000);}
    bool setMPM(double d) { return setMPS(d/BYMIN);}
    bool setKMPM(double d) { return setMPS(d*1000.0/BYMIN);}
    bool setTKMPM(double d) { return setMPS(d*1000.0*100000.0/BYMIN);}

    bool setMPH(double d) { return setMPS(d/BYHOUR);}
    bool setKMPH(double d) { return setMPS(d*1000.0/BYHOUR);}
    bool setTKMPH(double d) { return setMPS(d*1000.0*100000.0/BYHOUR);}

    bool setMACH(double d) { return setMPS(d*SND_SPEED);}
    bool setKNOT(double d) { return setMPS(d * KNOT_M);}
    bool setLS(double d) { return setMPS(d*LIGHT_SPEED);}
    bool setLM(double d) {
        return setMPS(d*((double)BYMIN*(double)LIGHT_SPEED));
    }
    bool setLH(double d) { return setMPS(d*((double)BYHOUR*(double)LIGHT_SPEED));}
    bool setLD(double d) { return setMPS(d*((double)BYDAY*(double)LIGHT_SPEED));}
    bool setLY(double d) { return setMPS(d*((double)BYYEAR*(double)LIGHT_SPEED));}

private:
    double m_value;
};

#endif // FSSPEED_H
