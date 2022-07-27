#include "stdafx.h"
#include "jigHeader.h"

CustomJig::CustomJig()
{
    count = 0; 
    m_dimData = new AcDbDimDataPtrArray();
    obj = new customObject();
}

AcEdJig::DragStatus CustomJig::sampler()
{
    setUserInputControls((UserInputControls)
        (AcEdJig::kAccept3dCoordinates
            | AcEdJig::kNoNegativeResponseAccepted
            | AcEdJig::kNoZeroResponseAccepted
            | AcEdJig::kAcceptOtherInputString));
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
    updateDimensions();
    return Adesk::kTrue;
}

void CustomJig::startJig()
{
    setDispPrompt(_T("\nEnter object center "));
    AcEdJig::DragStatus stat = drag();
    if (stat == AcEdJig::kNormal)
    {
        count++;
        center = obj->getCenter();
        setDispPrompt(_T("\nEnter object direction "));
        stat = drag();
        if (stat == AcEdJig::kNormal)
        {
            count++;
            setDispPrompt(_T("\nEnter object radius: "));
            stat = drag();
            if (stat == AcEdJig::kNormal)
            {
                append();
            }
        }
    }    
}

Acad::ErrorStatus CustomJig::setDimValue(const AcDbDimData* dimData, const double dimValue)
{
    if (count == 2)
    {
        if (dimValue - obj->getminFrameThickness() < obj->getminWindowThickness())
        {
            obj->setr(obj->getr());
            obj->setR(obj->getR());
        }
        else
        {
            obj->setr(dimValue - obj->getminFrameThickness());
            obj->setR(dimValue);
        }
    }

    return Acad::eOk;
}

AcDbDimDataPtrArray* CustomJig::dimData(const double dimScale)
{
    AcGeMatrix3d xMat;
    obj->get_Matrix(xMat);
    for (int i = 0; i < m_dimData->length(); i++) {
        AcDbDimData* pData = (*m_dimData)[i];
        delete pData;
    }
    m_dimData->setLogicalLength(0);
    
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
        pDimension->setDimLinePoint(Pt);
        pDimension->setDimtad(1);
        AcDbDimData* pDimData = new AcDbDimData(pDimension);
        pDimData->setDimFocal(true);
        pDimData->setDimHideIfValueIsZero(true);
        pDimData->setDimEditable(true);
        m_dimData->append(pDimData);
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
        pDimension->setDimtad(1);
        AcDbDimData* pDimData = new AcDbDimData(pDimension);
        pDimData->setDimFocal(true);
        pDimData->setDimHideIfValueIsZero(true);
        pDimData->setDimEditable(true);
        m_dimData->append(pDimData);
    }

    return m_dimData;
}

AcDbDimDataPtrArray* CustomJig::dimData() const
{
    return m_dimData;
}


void CustomJig::updateDimensions()
{ 

    if (count == 1)
    {
        if (m_dimData->size() >0)
        {
            AcDbDimData* dimDataNC = m_dimData->getAt(0);
            AcDbDimension* pDim = (AcDbDimension*)dimDataNC->dimension();
            AcDbAlignedDimension* pAlnDim = AcDbAlignedDimension::cast(pDim);
            pAlnDim->setXLine1Point(obj->getCenter());
            pAlnDim->setXLine2Point(Pt);
            pAlnDim->setDimLinePoint(Pt);

        }     
    }
    else
    if (count == 2)
    {       
        if (m_dimData->size() > 0)
        {
            AcGeMatrix3d xMat;
            obj->get_Matrix(xMat);
            AcDbDimData* dimDataNC = m_dimData->getAt(0);
            AcDbDimension* pDim = (AcDbDimension*)dimDataNC->dimension();
            AcDbAlignedDimension* pAlnDim = AcDbAlignedDimension::cast(pDim);
            pAlnDim->setXLine1Point(obj->getCenter());
            pAlnDim->setXLine2Point(obj->getPt9().transformBy(xMat));
            pAlnDim->setDimLinePoint(AcGePoint3d(0, obj->getR() / 2, 0).transformBy(xMat));
        }
            
    }
}

CustomJig::~CustomJig() 
{
    delete[] m_dimData;
    delete obj;
}
