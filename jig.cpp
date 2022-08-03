#include "stdafx.h"
#include "jig.h"

CustomJig::CustomJig()
{
    count = 0; 
    m_dimData = new AcDbDimDataPtrArray();
}

CustomJig::~CustomJig()
{
    for (int i = 0; i < m_dimData->length(); i++) {
        AcDbDimData* pData = (*m_dimData)[i];
        delete pData;
    }
    delete m_dimData;
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
    stat = acquirePoint(m_Pt, m_center);

    if (!tempPt.isEqualTo(m_Pt))
    {
        tempPt = m_Pt;
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
        m_obj->setCenter(m_Pt);
    }
    else
    if (count == 1)
    {
        if (m_Pt.isEqualTo(m_center))
        {
            m_obj->setDirection(AcGeVector3d::kXAxis);
        }
        else
        {
            m_obj->setDirection(AcGeVector3d(m_Pt - m_center).normalize());
        }        
    }
    else if (count == 2)
    {
        AcGeVector3d offset{ m_center - m_Pt };
        double R = offset.length();
        if (R - m_obj->getminFrameThickness() >= m_obj->getminWindowThickness())
        {
            m_obj->setr(R - m_obj->getminFrameThickness());
            m_obj->setR(R);
        }

    }
    updateDimensions();
    return Adesk::kTrue;
}

void CustomJig::startJig()
{
    m_obj = new customObject();
    setDispPrompt(_T("\nEnter object center "));
    AcEdJig::DragStatus stat = drag();
    if (stat == AcEdJig::kNormal)
    {
        count++;
        m_center = m_obj->getCenter();
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
                return;
            }
        }
    }
    delete m_obj;
}

Acad::ErrorStatus CustomJig::setDimValue(const AcDbDimData* dimData, const double dimValue)
{
    if (count == 1)
    {
        AcGeVector3d direction{ AcGeVector3d::kXAxis };
        double cs = cos(dimValue);
        double sn = sin(dimValue);
        double rx = direction.x * cs - direction.y * sn;
        double ry = direction.x * sn + direction.y * cs;
        m_obj->setDirection(AcGeVector3d{rx,ry,0});
    }
    else
    if (count == 2)
    {
        if (dimValue - m_obj->getminFrameThickness() < m_obj->getminWindowThickness())
        {
            m_obj->setr(m_obj->getr1()+ m_obj->getminWindowThickness());
            m_obj->setR(m_obj->getr()+ m_obj->getminFrameThickness());
        }
        else
        {
            m_obj->setr(dimValue - m_obj->getminFrameThickness());
            m_obj->setR(dimValue);
        }
    }

    return Acad::eOk;
}

AcDbDimDataPtrArray* CustomJig::dimData(const double dimScale)
{
    AcGeMatrix3d xMat;
    m_obj->get_Matrix(xMat);
    
    if (count == 1)
    {
        AcDb3PointAngularDimension* pDimension = new AcDb3PointAngularDimension;
        pDimension->setDatabaseDefaults();
        pDimension->setNormal(AcGeVector3d::kZAxis);
        pDimension->setElevation(0.0);
        pDimension->setDimscale(dimScale);
        pDimension->setHorizontalRotation(0.0);
        pDimension->setDynamicDimension(true);
        pDimension->setXLine1Point(m_obj->getPt5().transformBy(xMat));
        pDimension->setXLine2Point(m_obj->getPt5().transformBy(xMat));
        pDimension->setCenterPoint(m_obj->getCenter());
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
        pDimension->setXLine1Point(m_obj->getCenter());
        pDimension->setXLine2Point(m_obj->getPt9().transformBy(xMat));
        pDimension->setDimLinePoint(AcGePoint3d(0, m_obj->getR() / 2, 0).transformBy(xMat));
        pDimension->setDimtad(1);
        AcDbDimData* pDimData = new AcDbDimData(pDimension);
        pDimData->setDimFocal(true);
        pDimData->setDimHideIfValueIsZero(true);
        pDimData->setDimEditable(true);
        m_dimData->append(pDimData);
    }

    return m_dimData;
}

void CustomJig::updateDimensions()
{ 
    if (count == 1)
    {
        if (m_dimData->size() >0)
        {
            AcGeMatrix3d xMat;
            m_obj->get_Matrix(xMat);
            AcDbDimData* dimDataNC = m_dimData->getAt(0);
            AcDbDimension* pDim = (AcDbDimension*)dimDataNC->dimension();
            AcDb3PointAngularDimension* pAlnDim = AcDb3PointAngularDimension::cast(pDim);

            
            AcGeVector3d vec1(m_obj->getPt5().transformBy(xMat)- m_center);
            AcGeVector3d vec2(AcGePoint3d(m_center.x+m_obj->getR(), m_center.y,0) - m_center); 
            if (abs(vec1.angleTo(vec2)-PI)<0.000001)
            {
                pAlnDim->setArcPoint(AcGePoint3d(0,-m_obj->getR()*1.5,0).transformBy(xMat));
            }
            else
            {
                AcGeVector3d bisVector(vec1 + vec2);
                if (m_center.y > m_Pt.y)
                {
                    bisVector.negate();
                }
                AcGePoint3d arcPt = m_center + (bisVector.normalize() * m_obj->getR() * 1.5);
                pAlnDim->setArcPoint(arcPt);
            }
                
            AcGePoint3d xLine1Pt;
            AcGeLineSeg3d line1(m_center, AcGePoint3d(m_center.x + m_obj->getR(), m_center.y, 0));
            AcGeLineSeg3d line2(m_obj->getPt1().transformBy(xMat), m_obj->getPt6().transformBy(xMat));
            if (line1.intersectWith(line2, xLine1Pt))
            {
                pAlnDim->setXLine1Point(xLine1Pt);
            }
            else
            {
                AcGeLineSeg3d line3(m_obj->getPt5().transformBy(xMat), m_obj->getPt6().transformBy(xMat));
                if (line1.intersectWith(line3, xLine1Pt))
                {
                    pAlnDim->setXLine1Point(xLine1Pt);
                }
                else
                {
                    AcGeLineSeg3d line4(m_obj->getPt1().transformBy(xMat), m_obj->getPt2().transformBy(xMat));
                    if (line1.intersectWith(line4, xLine1Pt))
                    {
                        pAlnDim->setXLine1Point(xLine1Pt);
                    }
                    else
                    {
                        AcGeCircArc3d circArc(m_obj->getPt2().transformBy(xMat), m_obj->getPt9().transformBy(xMat), m_obj->getPt5().transformBy(xMat));
                        int intersectsCount;
                        AcGePoint3d Pt2;
                        if (circArc.intersectWith(line1, intersectsCount, xLine1Pt, Pt2))
                        {
                            pAlnDim->setXLine1Point(xLine1Pt);
                        }
                    }
                }
            }

            pAlnDim->setXLine2Point(m_obj->getPt5().transformBy(xMat));
        }     
    }
    else
    if (count == 2)
    {   
        AcDbDimData* dimDataNC = m_dimData->getAt(0);
        dimDataNC->setDimEditable(false);
        dimDataNC->setDimInvisible(true);
        dimDataNC->setDimFocal(false);
        if (m_dimData->size() > 1)
        {
            AcGeMatrix3d xMat;
            m_obj->get_Matrix(xMat);
            AcDbDimData* dimDataNC = m_dimData->getAt(1);
            AcDbDimension* pDim = (AcDbDimension*)dimDataNC->dimension();
            AcDbAlignedDimension* pAlnDim = AcDbAlignedDimension::cast(pDim);
            pAlnDim->setXLine1Point(m_obj->getCenter());
            pAlnDim->setXLine2Point(m_obj->getPt9().transformBy(xMat));
            pAlnDim->setDimLinePoint(AcGePoint3d(0, m_obj->getR() / 2, 0).transformBy(xMat));
        }
            
    }
}