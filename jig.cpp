#include "stdafx.h"
#include "jigHeader.h"

AcEdJig::DragStatus CustomJig::sampler()
{
    DragStatus stat = AcEdJig::kNormal;
    AcGePoint3d tempPt;
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
        obj->setDirection(AcGeVector3d(Pt - center).normalize());
    }
    else if (count == 1)
    {

        AcGeVector3d vector_OY(obj->getDirection().perpVector());
        AcGeVector3d offset{ center.x - Pt.x,center.y - Pt.y,center.z - Pt.z };
        double R = abs(vector_OY.dotProduct(offset) * 2);
        if (R - obj->getminFrameThickness() < obj->getminWindowThickness())
        {
            obj->setr(obj->getr());
            obj->setR(obj->getR());
        }
        else
        {
            obj->setr(R - obj->getminFrameThickness());
            obj->setR(R);
        }

    }

    return Adesk::kTrue;
}

void CustomJig::startJig()
{
    acedGetPoint(NULL, L"\ncenter: ", asDblArray(center));
    obj = new customObject(center);
    setDispPrompt(_T("\nEnter object direction "));
    AcEdJig::DragStatus stat = drag();
    if (stat == AcEdJig::kNormal)
    {
        count++;
        setDispPrompt(_T("\nEnter object radius: "));
        stat = drag();
        if (stat == AcEdJig::kNormal)
        {
            append();
        }
        else
        {
            delete obj;
        }

    }
    else
    {
        delete obj;
    }

}

