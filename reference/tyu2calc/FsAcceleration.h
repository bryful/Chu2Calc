#ifndef FSACCELERATION_H
#define FSACCELERATION_H

#include "FsSpace.h"

class FsAcceleration
{
public:
    FsAcceleration(double d = 0){ m_value = d;}

    double G(){ return m_value / GACL;}

    double MPS2(){ return m_value;}
    double KMPS2(){ return m_value / 1000.0;}
    double TKMPS2(){ return m_value / (1000.0*100000.0);}

    double MPM2() { return m_value * BYMIN;}
    double KMPM2(){ return m_value * (BYMIN/1000.0);}
    double TKMPM2(){ return m_value * (BYMIN/(1000.0*100000.0));}

    double MPH2() { return m_value * BYHOUR;}
    double KMPH2(){ return m_value * (BYHOUR/1000.0);}
    double TKMPH2(){ return m_value * (BYHOUR/(1000.0*100000.0));}

    bool setMPS2(double d) {
        bool ret = false;
        if (m_value != d) {
            m_value = d;
            ret = true;
        }
        return ret;
    }
    bool setKMPS2(double d) { return setMPS2(d*1000);}
    bool setTKMPS2(double d) { return setMPS2(d*1000*100000.0);}

    bool setMPM2(double d) { return setMPS2(d/BYMIN);}
    bool setKMPM2(double d) { return setMPS2(d*1000.0/BYMIN);}
    bool setTKMPM2(double d) { return setMPS2(d*1000.0*100000.0/BYMIN);}

    bool setMPH2(double d) { return setMPS2(d/BYHOUR);}
    bool setKMPH2(double d) { return setMPS2(d*1000.0/BYHOUR);}
    bool setTKMPH2(double d) { return setMPS2(d*1000.0*100000.0/BYHOUR);}

    bool setG(double d) { return setMPS2(d*GACL);}
private:
    double m_value;
};

#endif // FSACCELERATION_H
