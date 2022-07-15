#pragma once

#include <dbjig.h>
#include "customObject.h"

class CustomJig : public AcEdJig
{
public:
    CustomJig(const AcGePoint3d&);
    void startJig();
    virtual DragStatus sampler();
    virtual Adesk::Boolean update();
    virtual AcDbEntity* entity() const;

private:
    customObject* obj;
    AcGePoint3d center, Pt;  // Центральная точка и точка по оси х, y
    int count; // Счетчик для установления нескольких драгов
};

AcEdJig::DragStatus CustomJig::sampler()
{
    DragStatus stat = AcEdJig::kNormal;
    static AcGePoint3d tempPt;
    stat = acquirePoint(Pt, center);
    if (tempPt != Pt)
    {
        tempPt = Pt;
    }
    else
    if (stat == AcEdJig::kNormal)
    {
        return AcEdJig::kNoChange;
    }
    return stat;
}

Adesk::Boolean CustomJig::update()
{
    
    if (count == 0)
    {
        obj->setDirection(AcGeVector3d(Pt-center).normalize());
    }
    else if (count == 1)
    {

        AcGeVector3d vector_OY(obj->getDirection().perpVector());
        AcGeVector3d offset{ center.x - Pt.x,center.y - Pt.y,center.z - Pt.z };
        double R = abs(vector_OY.dotProduct(offset) * 2);
        if ((R - obj->getr1()) < 8000)
        {
            obj->setr(obj->getr1() + 6000);
            obj->setR(obj->getr() + 2000);
        } 
        else
        {
            obj->setR(R);
            obj->setr(R - 2000);
        }
        
    }

    return Adesk::kTrue;
}

void CustomJig::startJig()
{
    obj = new customObject(center);
    setDispPrompt(_T("\nEnter object direction "));
    AcEdJig::DragStatus stat = drag();

    count++;   
    setDispPrompt(_T("\nEnter object radius: "));
    stat = drag();
    append();
}

CustomJig::CustomJig(const AcGePoint3d& pt) : center(pt)
{
    count = 0;
}

AcDbEntity* CustomJig::entity() const
{
    return obj;
}