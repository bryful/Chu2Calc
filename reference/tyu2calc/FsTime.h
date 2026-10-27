#ifndef FSTIME_H
#define FSTIME_H

#include <QtMath>

#include "FsSpace.h"

class FsTime
{
public:
    FsTime(double d=0) {
        m_value = d;
        splitTime();
    }

    double SEC() {return m_value;}
    double MIN() {return m_value / 60.0;}
    double HOUR() {return m_value / 3600.0;}
    double DAY() {return m_value / (3600.0*24.0);}
    double YEAR() {return m_value / (3600.0*24.0*365.0);}

    double microSec(){
        return (double)m_microSec/MICRO_S;
    }
    int sec(){ return m_sec;}
    int min(){ return m_min;}
    int hour(){ return m_hour;}
    int day(){ return m_day;}
    int year(){ return m_year;}

    bool setSEC(double d){
        bool ret = false;
        if (d<0) d = 0;
        if (m_value != d){
            m_value = d;
            ret = true;
        }
        splitTime();
        return ret;
    }
    bool setMIN(double d) { return setSEC(d*60.0);}
    bool setHOUR(double d) { return setSEC(d*60.0*60.0);}
    bool setDAY(double d) { return setSEC(d*60.0*60.0*24.0);}
    bool setYEAR(double d) { return setSEC(d*60.0*60.0*24.0*365.0);}

    bool setSecP(int v){
        bool ret = false;
        if (v<0) v =0;
        if (m_sec != v){
            m_sec = v;
            combineTime();
            ret = true;
        }
        return ret;
    }
    bool setMicroSecP(double v){
        bool ret = false;
        if (v<0) v =0;
        int v2 = (int)(v * MICRO_S);
        if (m_microSec != v2){
            m_microSec = v2;
            combineTime();
            ret = true;
        }
        return ret;
    }
    bool setMinP(int v){
        bool ret = false;
        if (v<0) v =0;
        if (m_min != v){
            m_min = v;
            combineTime();
            ret = true;
        }
        return ret;
    }
    bool setHourP(int v){
        bool ret = false;
        if (v<0) v =0;
        if (m_hour != v){
            m_hour = v;
            combineTime();
            ret = true;
        }
        return ret;
    }
    bool setDayP(int v){
        bool ret = false;
        if (v<0) v =0;
        if (m_day != v){
            m_day = v;
            combineTime();
            ret = true;
        }
        return ret;
    }
    bool setYearP(int v){
        bool ret = false;
        if (v<0) v =0;
        if (m_year != v){
            m_year = v;
            combineTime();
            ret = true;
        }
        return ret;
    }

private:
    double m_value;

    //*********************************
    void splitTime()
    {
        if (m_value<0) m_value = 0;
        if (m_value<=0){
            m_microSec = 0;
            m_sec = 0;
            m_min = 0;
            m_hour = 0;
            m_day = 0;
            m_year = 0;
            return;
        }
        int dd = (int)qFloor(m_value);

        m_microSec = (int)((m_value - (double)dd )*MICRO_S);

        m_sec = (dd % 60);
        dd /= 60;
        m_min = (dd % 60);
        dd /= 60;
        m_hour = (dd % 24);
        dd /= 24;
        m_day = (dd % 365);
        dd /= 365;
        m_year = dd;
    }
    //*********************************
    void combineTime()
    {
        double b = 0;
        b += (double)m_microSec/MICRO_S;
        b += (double)m_sec;
        b += (double)m_min * 60.0;
        b += (double)m_hour * 60.0 * 60.0;
        b += (double)m_day * 60.0 * 60.0 *24.0;
        b += (double)m_year * 60.0 * 60.0 *24.0 *365.0;
        m_value = b;

    }
    //*********************************
    int m_sec;
    int m_microSec;
    int m_min;
    int m_hour;
    int m_day;
    int m_year;
};

#endif // FSTIME_H
