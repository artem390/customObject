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
        obj->setCenter(Pt);
    }
    else
    if (count == 1)
    {
        obj->setDirection(AcGeVector3d(Pt - center).normalize());
    }
    else if (count == 2)
    {
        AcGeVector3d offset{ center - Pt };
        double R = offset.length();
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
    obj = new customObject();
    setDispPrompt(_T("\nEnter object center "));
    AcEdJig::DragStatus stat = drag();
    count++;
    center = obj->getCenter();
    setDispPrompt(_T("\nEnter object direction "));
    stat = drag();
    count++;
    setDispPrompt(_T("\nEnter object radius: "));
    stat = drag();
    if (stat == AcEdJig::kNormal)
    {
        append();
    }
}

Acad::ErrorStatus CustomJig::setDimValue(const AcDbDimData* dimData, const double dimValue)
{
    if (count == 1)
    {
        AcDbDimData* dimDataNC = const_cast<AcDbDimData*>(dimData);
        int inputNumber = -1;
        if (m_dimData.find(dimDataNC, inputNumber)) {
            if (inputNumber == 0)
            {
                AcDbDimension* pDim = (AcDbDimension*)dimDataNC->dimension();
                //- Check it's the type of dimension we want
                AcDbAlignedDimension* pAlnDim = AcDbAlignedDimension::cast(pDim);
                //- If ok
                if (pAlnDim) {
                    obj->setDirection(AcGeVector3d(Pt - center).normalize());
                    //AcGePoint3d rotatePt = Pt;
                    //rotatePt += offset;
                    //AcGeVector3d vec(rotatePt - center);
                    //obj->setDirection(vec.normalize());
                    //pAlnDim->setXLine1Point(center);
                    //pAlnDim->setXLine2Point(AcGePoint3d(dimValue,0,0));
                    //pAlnDim->setDimLinePoint(AcGePoint3d(dimValue, 0, 0));
                    //acutPrintf(L"\setdimvalue pEnd %.0f;%.0f;%.0f\n",pEnd.x,pEnd.y,pEnd.z);
                    /*AcEdJig::DragStatus stat = AcEdJig::drag();*/
                    /*pointTmp=dimEndNew;*/
                }
            }
        }
    }
    else
    if (count == 2)
    {
        AcDbDimData* dimDataNC = const_cast<AcDbDimData*>(dimData);
        int inputNumber = -1;
        if (m_dimData.find(dimDataNC, inputNumber)) {
            if (inputNumber == 1)
            {
                AcDbDimension* pDim = (AcDbDimension*)dimDataNC->dimension();
                //- Check it's the type of dimension we want
                AcDbAlignedDimension* pAlnDim = AcDbAlignedDimension::cast(pDim);
                //- If ok
                if (pAlnDim) {
                    obj->setR(dimValue);
                    //pAlnDim->setXLine1Point(center);
                    //pAlnDim->setXLine2Point(AcGePoint3d(0, dimValue, 0));
                    //pAlnDim->setDimLinePoint(AcGePoint3d(0, dimValue/2, 0));
                    //acutPrintf(L"\setdimvalue pEnd %.0f;%.0f;%.0f\n",pEnd.x,pEnd.y,pEnd.z);
                    /*AcEdJig::DragStatus stat = AcEdJig::drag();*/
                    /*pointTmp=dimEndNew;*/
                }
            }
        }
    }

    return Acad::eOk;
}

AcDbDimDataPtrArray* CustomJig::dimData(const double dimScale)
{
    AcGeMatrix3d xMat;
    obj->get_Matrix(xMat);
    if (count == 1)
    {
        AcDbAlignedDimension* pDimension = new AcDbAlignedDimension;
        pDimension->setDatabaseDefaults();
        pDimension->setNormal(AcGeVector3d::kZAxis);
        pDimension->setElevation(0.0);
        pDimension->setDimscale(dimScale);
        pDimension->setHorizontalRotation(0.0);
        pDimension->setDynamicDimension(true);
        pDimension->setXLine1Point(obj->getCenter());
        pDimension->setXLine2Point(Pt);
        pDimension->setDimLinePoint(AcGePoint3d(obj->getR() * 0.75, 0, 0).transformBy(xMat));
        AcDbDimData* pDimData = new AcDbDimData(pDimension);
        pDimData->setDimFocal(true);
        pDimData->setDimHideIfValueIsZero(true);
        pDimData->setDimEditable(true);
        m_dimData.append(pDimData);
    }
    else
    if (count == 2)
    {
        AcDbAlignedDimension* pDimension = new AcDbAlignedDimension;
        pDimension->setDatabaseDefaults();
        pDimension->setNormal(AcGeVector3d::kZAxis);
        pDimension->setElevation(0.0);
        pDimension->setDimscale(dimScale);
        pDimension->setHorizontalRotation(0.0);
        pDimension->setDynamicDimension(true);
        pDimension->setXLine1Point(obj->getCenter());
        pDimension->setXLine2Point(obj->getPt9().transformBy(xMat));
        pDimension->setDimLinePoint(AcGePoint3d(0, obj->getR() / 2, 0).transformBy(xMat));
        AcDbDimData* pDimData = new AcDbDimData(pDimension);
        pDimData->setDimFocal(true);
        pDimData->setDimHideIfValueIsZero(true);
        pDimData->setDimEditable(true);
        m_dimData.append(pDimData);
    }

    return &m_dimData;
}

CustomJig::~CustomJig() 
{
    for (auto p : m_dimData)
    {
        delete p;
    }
    m_dimData.clear();
    delete obj;
}
