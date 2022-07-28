#include "stdafx.h"
#include "customObject.h"

ACRX_DXF_DEFINE_MEMBERS(customObject, AcDbEntity, AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 0, CUSTOM, "CustomObject");
static  AcDbDimData* mpDimData;

// Функция перевода из AcGeCircArc3d в_AcDbArc
void customObject::AcGeCircArc3dToAcDbArc(const AcGeCircArc3d& pGeArc, AcDbArc* pDbArc) const
{
    AcGePoint3d center = pGeArc.center();
    AcGeVector3d normal = pGeArc.normal();
    AcGeVector3d refVec = pGeArc.refVec();
    AcGePlane plane(center, normal);
    double ang = refVec.angleOnPlane(plane);
    pDbArc->setCenter(center);
    pDbArc->setNormal(normal);
    pDbArc->setRadius(pGeArc.radius());
    pDbArc->setStartAngle(pGeArc.startAng() + ang);
    pDbArc->setEndAngle(pGeArc.endAng() + ang);
    return;
}

Acad::ErrorStatus customObject::dwgOutFields(AcDbDwgFiler* fill) const
{
    assertReadEnabled();
    const Acad::ErrorStatus err = AcDbEntity::dwgOutFields(fill);
    if (err != Acad::eOk)
        return err;
    // Запоминаем версию, контрольные точки
    fill->writeItem(VERSION);
    fill->writePoint3d(center);
    fill->writeVector3d(normV);
    fill->writeVector3d(directionV);
    fill->writeDouble(R);
    fill->writeDouble(r);
    fill->writeDouble(r1);
    fill->writeDouble(h);

    return fill->filerStatus();
}

Acad::ErrorStatus customObject::dwgInFields(AcDbDwgFiler* fill)
{
    assertWriteEnabled();
    const Acad::ErrorStatus err = AcDbEntity::dwgInFields(fill);
    if (err != Acad::eOk)
        return err;

    Adesk::Int16 version;
    fill->readItem(&version);
    if (version > VERSION)
        return Acad::eMakeMeProxy;
       // Читаем версию, контрольные точки
        fill->readPoint3d(&center);
        fill->readVector3d(&normV);
        fill->readVector3d(&directionV);
        fill->readDouble(&R);
        fill->readDouble(&r);
        fill->readDouble(&r1);
        fill->readDouble(&h);
    return fill->filerStatus();
}

Acad::ErrorStatus customObject::subTransformBy(const AcGeMatrix3d& xform)
{
    assertWriteEnabled();
    center.transformBy(xform);
    directionV.transformBy(xform);
    return Acad::eOk;
}
// Разбиение объекта
Acad::ErrorStatus customObject::subExplode(AcDbVoidPtrArray& entitySet) const
{
    assertReadEnabled();
    AcGePoint3d  point17 = getPt17();
    AcGePoint3d  point20 = getPt20();
    AcGePoint3d  point16 = AcGePoint3d(point17.y, point17.x, 0);
    AcGePoint3d  point21 = AcGePoint3d(point20.y, point20.x, 0);
    AcGePoint3d  point18 = AcGePoint3d(-point17.x, point17.y, 0);
    AcGePoint3d  point23 = AcGePoint3d(-point20.x, point20.y, 0);
    AcGePoint3d  point19 = AcGePoint3d(-point17.y, point17.x, 0);
    AcGePoint3d  point22 = AcGePoint3d(-point20.y, point20.x, 0);
    Acad::ErrorStatus es = Acad::eOk;
    AcGeMatrix3d xMat;
    get_Matrix(xMat);
    //Отрезки
    AcDbLine* line1 = new AcDbLine(getPt1().transformBy(xMat), getPt6().transformBy(xMat));
    entitySet.append(line1);
    AcDbLine* line2 = new AcDbLine(getPt1().transformBy(xMat), getPt2().transformBy(xMat));
    entitySet.append(line2);
    AcDbLine* line3 = new AcDbLine(getPt5().transformBy(xMat), getPt6().transformBy(xMat));
    entitySet.append(line3);
    AcDbLine* line4 = new AcDbLine(getPt3().transformBy(xMat), getPt7().transformBy(xMat));
    entitySet.append(line4);
    AcDbLine* line5 = new AcDbLine(getPt8().transformBy(xMat), getPt4().transformBy(xMat));
    entitySet.append(line5);
    AcDbLine* line6 = new AcDbLine(getPt11().transformBy(xMat), getPt13().transformBy(xMat));
    entitySet.append(line6);
    AcDbLine* line7 = new AcDbLine(getPt12().transformBy(xMat), getPt14().transformBy(xMat));
    entitySet.append(line7);
    AcDbLine* line8 = new AcDbLine(point17.transformBy(xMat), point20.transformBy(xMat));
    entitySet.append(line8);
    AcDbLine* line9 = new AcDbLine(point16.transformBy(xMat), point21.transformBy(xMat));
    entitySet.append(line9);
    AcDbLine* line10 = new AcDbLine(point19.transformBy(xMat), point22.transformBy(xMat));
    entitySet.append(line10);
    AcDbLine* line11 = new AcDbLine(point18.transformBy(xMat), point23.transformBy(xMat));
    entitySet.append(line11);
    //Дуги
    AcGeCircArc3d circ_geometry1(getPt2(), getPt9(), getPt5());
    circ_geometry1.transformBy(xMat);
    AcDbArc* circArc1 = new AcDbArc;
    AcGeCircArc3dToAcDbArc(circ_geometry1, circArc1);
    entitySet.append(circArc1);

    AcGeCircArc3d circ_geometry2(getPt3(), getPt10(), getPt4());
    circ_geometry2.transformBy(xMat);
    AcDbArc* circArc2 = new AcDbArc;
    AcGeCircArc3dToAcDbArc(circ_geometry2, circArc2);
    entitySet.append(circArc2);

    AcGeCircArc3d circ_geometry3(getPt7(), getPt15(), getPt8());
    circ_geometry3.transformBy(xMat);
    AcDbArc* circArc3 = new AcDbArc;
    AcGeCircArc3dToAcDbArc(circ_geometry3, circArc3);
    entitySet.append(circArc3);
    return es;
}

Acad::ErrorStatus customObject::subGetGeomExtents(AcDbExtents& extents) const
{
    assertReadEnabled();
    Acad::ErrorStatus es = Acad::eOk;
    return es;
}
Acad::ErrorStatus customObject::subGetTransformedCopy(const AcGeMatrix3d& mat, AcDbEntity*& ent) const
{
    assertReadEnabled();
    Acad::ErrorStatus es = Acad::eOk;
    return es;
}

// Оснапы Базовые точки объекта
Acad::ErrorStatus customObject::subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint, const AcGeMatrix3d& xform, AcGePoint3dArray& snapPoints, AcDbIntArray& geomIds) const
{
    assertReadEnabled();
 
    if (osnapMode != AcDb::kOsModeEnd
        && osnapMode != AcDb::kOsModeMid
        && osnapMode != AcDb::kOsModeNear
        && osnapMode != AcDb::kOsModePerp
        && osnapMode != AcDb::kOsModeCen
        && osnapMode != AcDb::kOsModeIns)
    { 
        return Acad::eOk;
    }
    
    AcGeMatrix3d xMat;
    get_Matrix(xMat);

    switch (osnapMode)
    {
    case AcDb::kOsModeCen:    // Центр объекта
        snapPoints.append(center);
        break;
    case AcDb::kOsModeNear:   // Ближайшие точки (для дуг)
        if (gsSelectionMark == 12)
        {
            AcGeCircArc3d circArc1(getPt2().transformBy(xMat), getPt9().transformBy(xMat), getPt5().transformBy(xMat));
            snapPoints.append(circArc1.closestPointTo(pickPoint));
        }
        break;
    case AcDb::kOsModeEnd:    // Вершина 
        if (gsSelectionMark == 10)//Нижняя дуга
        {
            snapPoints.append(getPt7().transformBy(xMat));
            snapPoints.append(getPt15().transformBy(xMat));
            snapPoints.append(getPt9().transformBy(xMat));
        }
        if (gsSelectionMark == 11)//Средняя дуга
        {
            snapPoints.append(getPt3().transformBy(xMat));
            snapPoints.append(getPt10().transformBy(xMat));
            snapPoints.append(getPt4().transformBy(xMat));
        }
        if (gsSelectionMark == 12)//Верхняя дуга
        {
            snapPoints.append(getPt2().transformBy(xMat));
            snapPoints.append(getPt9().transformBy(xMat));
            snapPoints.append(getPt5().transformBy(xMat));
        }
        if (gsSelectionMark == 13)//Нижняя часть основания
        {
            snapPoints.append(getPt1().transformBy(xMat));
            snapPoints.append(getPt6().transformBy(xMat));
        }
        if (gsSelectionMark == 14)//Левая верхняя часть основания
        {
            snapPoints.append(getPt3().transformBy(xMat));
            snapPoints.append(getPt7().transformBy(xMat));
        }
        if (gsSelectionMark == 15)//Правой верхняя часть основания
        {
            snapPoints.append(getPt8().transformBy(xMat));
            snapPoints.append(getPt4().transformBy(xMat));
        }
        if (gsSelectionMark == 16)//Левый бок основания
        {
            snapPoints.append(getPt1().transformBy(xMat));
            snapPoints.append(getPt2().transformBy(xMat));
        }
        if (gsSelectionMark == 17)//Правый бок основания
        {
            snapPoints.append(getPt5().transformBy(xMat));
            snapPoints.append(getPt6().transformBy(xMat));
        }
        break;
    }
    return Acad::eOk;
}

// Интерсекты (Точки пересечения)
Acad::ErrorStatus customObject::subIntersectWith(const AcDbEntity* ent, AcDb::Intersect intType, AcGePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const
{
    assertReadEnabled();
    AcGePoint3d  point17 = getPt17();
    AcGePoint3d  point20 = getPt20();
    AcGePoint3d  point16 = AcGePoint3d(point17.y, point17.x, 0);
    AcGePoint3d  point21 = AcGePoint3d(point20.y, point20.x, 0);
    AcGePoint3d  point18 = AcGePoint3d(-point17.x, point17.y, 0);
    AcGePoint3d  point23 = AcGePoint3d(-point20.x, point20.y, 0);
    AcGePoint3d  point19 = AcGePoint3d(-point17.y, point17.x, 0);
    AcGePoint3d  point22 = AcGePoint3d(-point20.y, point20.x, 0);
    Acad::ErrorStatus es = Acad::eOk;
    AcGePoint3d intersecPt1, intersecPt2;
    int count;
    AcGeMatrix3d xMat;
    get_Matrix(xMat);
    if (ent == NULL)
        return Acad::eNullEntityPointer;
    AcDbLine* pLine = AcDbLine::cast(ent);
    if (pLine != nullptr)
    {
        AcGePoint3d startPt(pLine->startPoint());
        AcGePoint3d endPt(pLine->endPoint());
        AcGeLine3d ent_line(startPt, endPt);
        
        if (thisGsMarker == 10)
        {
            AcGeCircArc3d circArc(getPt7().transformBy(xMat), getPt15().transformBy(xMat), getPt8().transformBy(xMat));
            if (circArc.intersectWith(ent_line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {
                    
                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 11)
        {
            AcGeCircArc3d circArc(getPt3().transformBy(xMat), getPt10().transformBy(xMat), getPt4().transformBy(xMat));
            if (circArc.intersectWith(ent_line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 12)
        {
            AcGeCircArc3d circArc(getPt2().transformBy(xMat), getPt9().transformBy(xMat), getPt5().transformBy(xMat));
            if (circArc.intersectWith(ent_line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 13)
        {
            AcGeLine3d line(getPt1().transformBy(xMat), getPt6().transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }           
        }
        if (thisGsMarker == 14)
        {
            AcGeLine3d line(getPt3().transformBy(xMat), getPt7().transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
        if (thisGsMarker == 15)
        {
            AcGeLine3d line(getPt8().transformBy(xMat), getPt4().transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
        if (thisGsMarker == 16)
        {
            AcGeLine3d line(getPt1().transformBy(xMat), getPt2().transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
        if (thisGsMarker == 17)
        {
            AcGeLine3d line(getPt5().transformBy(xMat), getPt6().transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
        if (thisGsMarker == 18)
        {
            AcGeLine3d line(getPt11().transformBy(xMat), getPt13().transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
        if (thisGsMarker == 19)
        {
            AcGeLine3d line(getPt12().transformBy(xMat), getPt14().transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
        if (thisGsMarker == 20)
        {
            AcGeLine3d line(point17.transformBy(xMat), point20.transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
        if (thisGsMarker == 21)
        {
            AcGeLine3d line(point16.transformBy(xMat), point21.transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
        if (thisGsMarker == 22)
        {
            AcGeLine3d line(point19.transformBy(xMat), point22.transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
        if (thisGsMarker == 23)
        {
            AcGeLine3d line(point18.transformBy(xMat), point23.transformBy(xMat));
            if (line.intersectWith(ent_line, intersecPt1))
            {
                points.append(intersecPt1);
            }
        }
    }
    
    AcDbCircle* circle = AcDbCircle::cast(ent);
    if (circle != nullptr)
    {
        AcGeCircArc3d circArcEnt(circle->center(), circle->normal(), circle->radius());
        if (thisGsMarker == 10)
        {
            AcGeCircArc3d circArc(getPt7().transformBy(xMat), getPt15().transformBy(xMat), getPt8().transformBy(xMat));
            if (circArcEnt.intersectWith(circArc, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 11)
        {
            AcGeCircArc3d circArc(getPt3().transformBy(xMat), getPt10().transformBy(xMat), getPt4().transformBy(xMat));
            if (circArcEnt.intersectWith(circArc, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 12)
        {
            AcGeCircArc3d circArc(getPt2().transformBy(xMat), getPt9().transformBy(xMat), getPt5().transformBy(xMat));
            if (circArcEnt.intersectWith(circArc, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 13)
        {
            AcGeLine3d line(getPt1().transformBy(xMat), getPt6().transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 14)
        {
            AcGeLine3d line(getPt3().transformBy(xMat), getPt7().transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 15)
        {
            AcGeLine3d line(getPt8().transformBy(xMat), getPt4().transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 16)
        {
            AcGeLine3d line(getPt1().transformBy(xMat), getPt2().transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 17)
        {
            AcGeLine3d line(getPt5().transformBy(xMat), getPt6().transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 18)
        {
            AcGeLine3d line(getPt11().transformBy(xMat), getPt13().transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 19)
        {
            AcGeLine3d line(getPt12().transformBy(xMat), getPt14().transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 20)
        {
            AcGeLine3d line(point17.transformBy(xMat), point20.transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 21)
        {
            AcGeLine3d line(point16.transformBy(xMat), point21.transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 22)
        {
            AcGeLine3d line(point19.transformBy(xMat), point22.transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
        if (thisGsMarker == 23)
        {
            AcGeLine3d line(point18.transformBy(xMat), point23.transformBy(xMat));
            if (circArcEnt.intersectWith(line, count, intersecPt1, intersecPt2))
            {
                points.append(intersecPt1);
                if (count > 1) {

                    points.append(intersecPt2);
                }
            }
        }
    }
    return es;
}

// Отрисовка объекта
Adesk::Boolean customObject::subWorldDraw(AcGiWorldDraw* draw)
{
    assertReadEnabled();
    AcGePoint3d  point17 = getPt17();
    AcGePoint3d  point20 = getPt20();
    AcGeMatrix3d xMat;
    get_Matrix(xMat);
    draw->geometry().pushModelTransform(xMat);
    
    draw->subEntityTraits().setSelectionMarker(10);
    draw->geometry().circularArc(getPt7(), getPt15(), getPt8());//Малая дуга (10)
    draw->subEntityTraits().setSelectionMarker(11);
    draw->geometry().circularArc(getPt3(), getPt10(), getPt4());//Средняя дуга (11)
    draw->subEntityTraits().setSelectionMarker(12);
    draw->geometry().circularArc(getPt2(), getPt9(), getPt5());//Большая дуга (12)

    //Линия 1 (13)
    AcGePoint3d* pVertexListPt1_Pt6 = new AcGePoint3d[2];
    pVertexListPt1_Pt6[0] = getPt1();
    pVertexListPt1_Pt6[1] = getPt6();
    draw->geometry().polyline(2, pVertexListPt1_Pt6, NULL, 13);
    delete[] pVertexListPt1_Pt6;

    //Линия 2 (14)
    AcGePoint3d* pVertexListPt3_Pt7 = new AcGePoint3d[2];
    pVertexListPt3_Pt7[0] = getPt3();
    pVertexListPt3_Pt7[1] = getPt7();
    draw->geometry().polyline(2, pVertexListPt3_Pt7, NULL, 14);
    delete[] pVertexListPt3_Pt7;

    //Линия 3 (15)
    AcGePoint3d* pVertexListPt8_Pt4 = new AcGePoint3d[2];
    pVertexListPt8_Pt4[0] = getPt8();
    pVertexListPt8_Pt4[1] = getPt4();
    draw->geometry().polyline(2, pVertexListPt8_Pt4, NULL, 15);
    delete[] pVertexListPt8_Pt4;

    //Линия 4 (16)
    AcGePoint3d* pVertexListPt1_Pt2 = new AcGePoint3d[2];
    pVertexListPt1_Pt2[0] = getPt1();
    pVertexListPt1_Pt2[1] = getPt2();
    draw->geometry().polyline(2, pVertexListPt1_Pt2, NULL, 16);
    delete[] pVertexListPt1_Pt2;

    //Линия 5 (17)
    AcGePoint3d* pVertexListPt5_Pt6 = new AcGePoint3d[2];
    pVertexListPt5_Pt6[0] = getPt5();
    pVertexListPt5_Pt6[1] = getPt6();
    draw->geometry().polyline(2, pVertexListPt5_Pt6, NULL, 17);
    delete[] pVertexListPt5_Pt6;

    //Линия 6 (18)
    AcGePoint3d* pVertexListPt11_Pt13 = new AcGePoint3d[2];
    pVertexListPt11_Pt13[0] = getPt11();
    pVertexListPt11_Pt13[1] = getPt13();
    draw->geometry().polyline(2, pVertexListPt11_Pt13, NULL, 18);
    delete[] pVertexListPt11_Pt13;

    //Линия 7 (19)
    AcGePoint3d* pVertexListPt12_Pt14 = new AcGePoint3d[2];
    pVertexListPt12_Pt14[0] = getPt12();
    pVertexListPt12_Pt14[1] = getPt14();
    draw->geometry().polyline(2, pVertexListPt12_Pt14, NULL, 19);
    delete[] pVertexListPt12_Pt14;

    //Линия 8 (20)
    AcGePoint3d* pVertexListPt17_Pt20 = new AcGePoint3d[2];
    pVertexListPt17_Pt20[0] = point20;
    pVertexListPt17_Pt20[1] = point17;
    draw->geometry().polyline(2, pVertexListPt17_Pt20, NULL, 20);
    delete[] pVertexListPt17_Pt20;
    
    //Линия 9 (21)
    AcGePoint3d* pVertexListPt16_Pt21 = new AcGePoint3d[2];
    pVertexListPt16_Pt21[0] = AcGePoint3d(point20.y,point20.x,0);
    pVertexListPt16_Pt21[1] = AcGePoint3d(point17.y, point17.x, 0);
    draw->geometry().polyline(2, pVertexListPt16_Pt21, NULL, 21);
    delete[] pVertexListPt16_Pt21;
    
    //Линия 10 (22)
    AcGePoint3d* pVertexListPt19_Pt22 = new AcGePoint3d[2];
    pVertexListPt19_Pt22[0] = AcGePoint3d(-point20.x,point20.y,0);
    pVertexListPt19_Pt22[1] = AcGePoint3d(-point17.x, point17.y, 0);;
    draw->geometry().polyline(2, pVertexListPt19_Pt22, NULL, 22);
    delete[] pVertexListPt19_Pt22;

    //Линия 11 (23)
    AcGePoint3d* pVertexListPt18_Pt23 = new AcGePoint3d[2];
    pVertexListPt18_Pt23[0] = AcGePoint3d(-point20.y, point20.x, 0);;
    pVertexListPt18_Pt23[1] = AcGePoint3d(-point17.y, point17.x, 0);
    draw->geometry().polyline(2, pVertexListPt18_Pt23, NULL, 23);
    delete[] pVertexListPt18_Pt23;

    draw->geometry().popModelTransform();
            
    return Adesk::kTrue;
}

// Грипы 
Acad::ErrorStatus customObject::subGetGripPoints(
    AcDbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize,
    const AcGeVector3d& curViewDir, const int bitflags) const
{
    assertReadEnabled();
    Acad::ErrorStatus es = Acad::eOk;
    AcGeMatrix3d xMat;
    get_Matrix(xMat);
    AcGeVector3d vector = getPt14() - getPt12();
    double yPoint = (sqrt(pow(vector.x, 2) + pow(vector.y, 2)) / 2) + (sqrt(pow(r1, 2) - pow(h / 2, 2)));
    // Изменение внешнего радиуса 
    grips.append(addGrip(getPt9().transformBy(xMat),1, curViewUnitSize));  //1
    // Изменение среднего радиуса 
    grips.append(addGrip(getPt10().transformBy(xMat), 2, curViewUnitSize));  //2
    // Изменение младшего радиуса 
    grips.append(addGrip(getPt15().transformBy(xMat), 3, curViewUnitSize));   //3
    // Изменение перемещение
    grips.append(addGrip(getCenter(), 4, curViewUnitSize));   //4
    // Изменение ширины столбцов
    grips.append(addGrip(AcGePoint3d(h / 2, yPoint, 0).transformBy(xMat), 5, curViewUnitSize));   //5
    grips.append(addGrip(AcGePoint3d(-h / 2, yPoint, 0).transformBy(xMat), 6, curViewUnitSize)); //6
    // Точка поворта
    grips.append(addGrip(getPt6().transformBy(xMat), 7, curViewUnitSize)); //7
    return es;
}

Acad::ErrorStatus customObject::subMoveGripPointsAt(
    const AcDbVoidPtrArray& gripAppData,
    const AcGeVector3d& offset,
    const int bitflags)
{
    assertWriteEnabled();
    if (offset.isZeroLength())
        return Acad::eInvalidInput;


    // Вектор ОX для объекта
    AcGeVector3d vector_OX(directionV);
    double len_OX = vector_OX.dotProduct(offset);
    // Вектор ОУ для объекта
    AcGeVector3d vector_OY(directionV.perpVector());
    double len_OY = vector_OY.dotProduct(offset);


    for (int i = 0; i < gripAppData.length(); i++)
    {
        OWNGripAppData* pAppData = (OWNGripAppData*)gripAppData[i];
        int iIndex = pAppData->index();
        switch (iIndex)
        {
            
        case 1:
            if ((r + len_OY) < (r1+ getminWindowThickness()))
            {
                r = r1 + getminWindowThickness();
                if ((R+ len_OY)<(r + getminFrameThickness()))
                {
                    R = r + getminFrameThickness();
                }
                else
                {
                    R += len_OY;
                }
            }
            else
            {
                R += len_OY;
                r += len_OY;
            }
            updateDimensions(this, AcGePoint3d(0,0,0), getPt9());
            return Acad::eOk;

        case 2:
            if ((r + len_OY )> (R- getminFrameThickness()))
            {
                r = R - getminFrameThickness();
            }
            else
            if((r+ len_OY) < (r1+ getminWindowThickness()))
            {
                r= r1+ getminWindowThickness();
            }
            else
            {
                r += len_OY;
            }
            updateDimensions(this, AcGePoint3d(0, 0, 0), getPt10());
            return Acad::eOk;
        case 3:
            if ((r1 + len_OY )> (r- getminWindowThickness()))
            {
                r1 = r - getminWindowThickness();
            }
            else
            if (h > 2 * (r1 + len_OY) * sin(PI / 8))
            {
                r1 = h / (2 * sin(PI / 8));
            }
            else
            {
                r1 += len_OY;
            }
            updateDimensions(this, AcGePoint3d(0, 0, 0), getPt15());
            return Acad::eOk;
        case 4:
            center += offset;
            return Acad::eOk;
        case 5:
        case 6:
            if (iIndex == 6)
            {
                len_OX = len_OX * (-1);
            }   
            if ((h + (2*len_OX) ) > getHmax())
            {
                h = getHmax();
            }
            else
            if ((h + (len_OX * 2)) < getHMin())
            {
                h = getHMin();
            }
            else
            {
                h += (len_OX * 2);
            }
            updateDimensions1(this, AcGePoint3d(-h / 2, -R+r, 0), AcGePoint3d(h / 2, -R+r, 0));
            return Acad::eOk;
        case 7:
            AcGeMatrix3d xMat;
            get_Matrix(xMat);
            AcGePoint3d rotatePt = getPt6().transformBy(xMat);
            rotatePt += offset;
            AcGeVector3d vec(rotatePt - center);
            directionV = vec.normalize();
            return Acad::eOk;
        }
    }
    return Acad::eOk;
}


AcGePoint3d  customObject::getPt17() const {

    const double cosinus{ cos(PI/4) };
    const AcGeCircArc3d arc(getPt7(), getPt15(), getPt8());

    AcGePoint3d point1(r1*cosinus + h / 2 * cosinus, r1 * cosinus - h / 2 * cosinus, 0);
    AcGePoint3d point2(h / 2 * cosinus, -h / 2 * cosinus, 0);
    AcGeLineSeg3d line(point1, point2);
    int count;

    arc.intersectWith(line, count, point1, point2);
    if (count == 1)
    {
        return point1;
    }
    return AcGePoint3d::kOrigin;
}

AcGePoint3d  customObject::getPt20() const {

    const double cosinus{ cos(PI / 4) };
    const AcGeCircArc3d arc(getPt3(), getPt10(), getPt4());

    AcGePoint3d point1(r * cosinus + h / 2 * cosinus, r * cosinus - h / 2 * cosinus, 0);
    AcGePoint3d point2(h / 2 * cosinus, -h / 2 * cosinus, 0);
    AcGeLineSeg3d line(point1, point2);
    int count;

    arc.intersectWith(line, count, point1, point2);
    if (count == 1)
    {
        return point1;
    }
    return AcGePoint3d::kOrigin;
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Функция обновления динамического размера 
bool  customObject::updateDimensions(customObject* obj, const AcGePoint3d& xline1Pt, const AcGePoint3d& xline2Pt)
{
    if (!obj || !mpDimData)
        return  false;

    Acad::ErrorStatus es;

    AcGeMatrix3d xMat;
    obj->get_Matrix(xMat);
    // Получаем точки, на которых будет указываться размерная линия
    AcGePoint3d pLine1Point = xline1Pt;
    pLine1Point.transformBy(xMat);
    AcGePoint3d pLine2Point = xline2Pt;
    pLine2Point.transformBy(xMat);

    AcGePoint3d center = { (xline1Pt.x + xline2Pt.x) / 2,(xline1Pt.y + xline2Pt.y) / 2,0 };
    center.transformBy(xMat);

    AcDbDimData* pDimData = mpDimData;
    AcDbAlignedDimension* pAlignedDim = AcDbAlignedDimension::cast(pDimData->dimension());

    if (pAlignedDim != NULL)
    {
        es = pAlignedDim->setXLine1Point(pLine1Point);
        es = pAlignedDim->setXLine2Point(pLine2Point);
        es = pAlignedDim->setDimLinePoint(center);

        es = pAlignedDim->setDimscale(12);
    }
    return  true;
}

bool  customObject::updateDimensions1(customObject* obj, const AcGePoint3d& xline1Pt, const AcGePoint3d& xline2Pt)
{
    if (!obj || !mpDimData)
        return  false;

    Acad::ErrorStatus es;

    AcGeMatrix3d xMat;
    obj->get_Matrix(xMat);
    // Получаем точки, на которых будет указываться размерная линия
    AcGePoint3d pLine1Point = xline1Pt;
    pLine1Point.transformBy(xMat);
    AcGePoint3d pLine2Point = xline2Pt;
    pLine2Point.transformBy(xMat);

    AcGePoint3d center = { 0,-obj->getR(),0 };
    center.transformBy(xMat);

    AcDbDimData* pDimData = mpDimData;
    AcDbAlignedDimension* pAlignedDim = AcDbAlignedDimension::cast(pDimData->dimension());

    if (pAlignedDim != NULL)
    {
        es = pAlignedDim->setXLine1Point(pLine1Point);
        es = pAlignedDim->setXLine2Point(pLine2Point);
        es = pAlignedDim->setDimLinePoint(center);

        es = pAlignedDim->setDimscale(12);
    }
    return  true;
}

// Грип для центральной точки 
void customObject::centerGripPointDraw(AcDbGripData* pThis, AcGiViewportDraw* pVd, const AcDbObjectId& entId, AcDbGripOperations::DrawType type, AcGePoint3d* cursor, int gripSize)
{
    if (pThis == NULL)
        return;
    AcGePoint2d scale2d;
    AcGePoint3d pntGrip = pThis->gripPoint();
    pVd->viewport().getNumPixelsInUnitSquare(pntGrip, scale2d);	// в пикселе единиц 
    double gripUnits =  gripSize / scale2d.x;
    // Получаем в точку наш гриппоинт
    AcGePoint3d gripPt = pThis->gripPoint();
    AcGeVector3d Normal{ 0,0,1 };
    pVd->subEntityTraits().setFillType(kAcGiFillAlways);
    pVd->geometry().circle(gripPt, gripUnits, Normal);

    return;
}

// Грип для радиусов
void customObject::radiusGripPointDraw(AcDbGripData* pThis, AcGiViewportDraw* pVd, const AcDbObjectId& entId, AcDbGripOperations::DrawType type, AcGePoint3d* cursor, int gripSize)
{
    AcDbObjectPointer<AcDbObject> object(entId, kForRead);
    if (object.openStatus() != Acad::ErrorStatus::eOk)
    {
        return;
    }
       
    customObject* obj = customObject::cast(object);
    if (!obj)
    {
        return;
    }

    AcGePoint3d pntGrip = pThis->gripPoint();
    AcGeVector3d  vecXDir = obj->getDirection();
    AcGeVector3d  vecNormal = obj->getNormal();

    AcGePoint2d scale2d;
    pVd->viewport().getNumPixelsInUnitSquare(pntGrip, scale2d);	// в пикселе единиц 
    double gripUnits = gripSize / scale2d.x;

    AcGeMatrix3d xMat;
    xMat.setCoordSystem(pntGrip, vecXDir, (-1) * vecXDir.crossProduct(vecNormal), vecNormal);
    pVd->geometry().pushModelTransform(xMat);

    AcGePoint3d* pts = new AcGePoint3d[3];
    pts[0] = { gripUnits,-gripUnits ,0 };
    pts[1] = { 0,gripUnits,0 };
    pts[2] = { -gripUnits,-gripUnits,0 };

    pVd->subEntityTraits().setFillType(kAcGiFillAlways);
    pVd->geometry().polygon(3, pts);
    delete[] pts;
    pVd->geometry().popModelTransform();
    return;
};

// Грип для изменения ширины столбца
void customObject::stretchGripPointDraw(AcDbGripData* pThis, AcGiViewportDraw* pVd, const AcDbObjectId& entId, AcDbGripOperations::DrawType type, AcGePoint3d* cursor, int gripSize)
{
    AcDbObjectPointer<AcDbObject> object(entId, kForRead);
    if (object.openStatus() != Acad::ErrorStatus::eOk)
    {
        return;
    }

    customObject* obj = customObject::cast(object);
    if (!obj)
    {
        return;
    }

    AcGePoint3d pntGrip = pThis->gripPoint();
    AcGeVector3d  vecXDir = obj->getDirection();
    AcGeVector3d  vecNormal = obj->getNormal();

    AcGePoint2d scale2d;
    pVd->viewport().getNumPixelsInUnitSquare(pntGrip, scale2d);	// в пикселе единиц 
    double gripUnits = gripSize / scale2d.x;

    AcGeMatrix3d xMat;
    xMat.setCoordSystem(pntGrip, vecXDir, (-1) * vecXDir.crossProduct(vecNormal), vecNormal);
    pVd->geometry().pushModelTransform(xMat);

    AcGePoint3d* pts = new AcGePoint3d[4];
    pts[0] = { gripUnits ,gripUnits ,0 };
    pts[1] = { gripUnits ,-gripUnits ,0 };
    pts[2] = { -gripUnits ,-gripUnits ,0 };
    pts[3] = { -gripUnits ,gripUnits ,0 };

    pVd->subEntityTraits().setFillType(kAcGiFillAlways);
    pVd->geometry().polygon(4, pts);
    delete[] pts;
    pVd->geometry().popModelTransform();

    return;
};

// Грип поворота
void customObject::rotateGripPointDraw(AcDbGripData* pThis, AcGiViewportDraw* pVd, const AcDbObjectId& entId, AcDbGripOperations::DrawType type, AcGePoint3d* cursor, int gripSize)
{
    AcDbObjectPointer<AcDbObject> object(entId, kForRead);
    if (object.openStatus() != Acad::ErrorStatus::eOk)
    {
        return;
    }

    customObject* obj = customObject::cast(object);
    if (!obj)
    {
        return;
    }

    AcGePoint3d pntGrip = pThis->gripPoint();
    AcGeVector3d  vecXDir = obj->getDirection();
    AcGeVector3d  vecNormal = obj->getNormal();

    AcGePoint2d scale2d;
    pVd->viewport().getNumPixelsInUnitSquare(pntGrip, scale2d);	// в пикселе единиц 
    double gripUnits = gripSize / scale2d.x;

    AcGeMatrix3d xMat;
    xMat.setCoordSystem(pntGrip, vecXDir, (-1) * vecXDir.crossProduct(vecNormal), vecNormal);
    pVd->geometry().pushModelTransform(xMat);

    AcGePoint3d* pts = new AcGePoint3d[4];
    pts[0] = { 0 ,gripUnits ,0 };
    pts[1] = { gripUnits ,0 ,0 };
    pts[2] = { 0 ,-gripUnits ,0 };
    pts[3] = { -gripUnits ,0 ,0 };

    pVd->subEntityTraits().setFillType(kAcGiFillAlways);
    pVd->geometry().polygon(4, pts);
    delete[] pts;
    pVd->geometry().popModelTransform();

    return;
};

void  customObject::MyGripHotGripStretchpoints(AcDbGripData* pGripData, const  AcDbObjectId& entId, double  dimScale, AcDbDimDataPtrArray& dimDataArr)
{
    AcDbAlignedDimension* pAlignedDim = new  AcDbAlignedDimension();
    pAlignedDim->setDatabaseDefaults();
    pAlignedDim->setDimsah(true);
    pAlignedDim->setDimse1(true);
    pAlignedDim->setDynamicDimension(true);
    AcDbDimData* pDimData = new  AcDbDimData(pAlignedDim);
    pDimData->setOwnerId(entId);
    pDimData->setDimFocal(true);
    pDimData->setDimEditable(true);
    pDimData->setDimRadius(true);
    pDimData->setDimInvisible(false);
    pDimData->setDimResultantLength(true);
    pDimData->setDimHideIfValueIsZero(true);
    pDimData->setDimValueFunc(setDimValueForH);
    dimDataArr.append(pDimData);
    mpDimData = pDimData;
}

void  customObject::MyGripHotGripRadiuspoints1(AcDbGripData* pGripData, const  AcDbObjectId& entId, double  dimScale, AcDbDimDataPtrArray& dimDataArr)
{
    AcDbAlignedDimension* pAlignedDim = new  AcDbAlignedDimension();
    pAlignedDim->setDatabaseDefaults();
    pAlignedDim->setDynamicDimension(true);
    AcDbDimData* pDimData = new  AcDbDimData(pAlignedDim);
    pDimData->setOwnerId(entId);
    pDimData->setDimFocal(true);
    pDimData->setDimEditable(true);
    pDimData->setDimRadius(false);
    pDimData->setDimInvisible(false);
    pDimData->setDimResultantLength(true);
    pDimData->setDimHideIfValueIsZero(true);
    pDimData->setDimValueFunc(setDimValueForRadius1);
    dimDataArr.append(pDimData);
    mpDimData = pDimData;
}
void  customObject::MyGripHotGripRadiuspoints2(AcDbGripData* pGripData, const  AcDbObjectId& entId, double  dimScale, AcDbDimDataPtrArray& dimDataArr)
{
    AcDbAlignedDimension* pAlignedDim = new  AcDbAlignedDimension();
    pAlignedDim->setDatabaseDefaults();
    pAlignedDim->setDynamicDimension(true);
    AcDbDimData* pDimData = new  AcDbDimData(pAlignedDim);
    pDimData->setOwnerId(entId);
    pDimData->setDimFocal(true);
    pDimData->setDimEditable(true);
    pDimData->setDimRadius(true);
    pDimData->setDimInvisible(false);
    pDimData->setDimResultantLength(true);
    pDimData->setDimHideIfValueIsZero(true);
    pDimData->setDimValueFunc(setDimValueForRadius2);
    dimDataArr.append(pDimData);
    mpDimData = pDimData;
}

void  customObject::MyGripHotGripRadiuspoints3(AcDbGripData* pGripData, const  AcDbObjectId& entId, double  dimScale, AcDbDimDataPtrArray& dimDataArr)
{
    AcDbAlignedDimension* pAlignedDim = new  AcDbAlignedDimension();
    pAlignedDim->setDatabaseDefaults();
    pAlignedDim->setDynamicDimension(true);
    AcDbDimData* pDimData = new  AcDbDimData(pAlignedDim);
    pDimData->setOwnerId(entId);
    pDimData->setDimFocal(true);
    pDimData->setDimEditable(true);
    pDimData->setDimRadius(true);
    pDimData->setDimInvisible(false);
    pDimData->setDimResultantLength(true);
    pDimData->setDimHideIfValueIsZero(true);
    pDimData->setDimValueFunc(setDimValueForRadius3);
    dimDataArr.append(pDimData);
    mpDimData = pDimData;
}

// Устанавливаем новое значение для внешнего радиуса
AcGeVector3d customObject::setDimValueForRadius1(AcDbDimData* pDimData, AcDbEntity* pEnt, double  newValue, const  AcGeVector3d& offset)
{

    AcGeVector3d newOffset(offset);
    if ((pDimData == NULL) || (pEnt == NULL))
        return newOffset;

    customObject* obj = customObject::cast(pEnt);
    //customObject* obj = dynamic_cast<customObject*>(pEnt);
    if (obj == NULL)
    {
        return  newOffset;
    }
    double diff = obj->getR() - obj->getr();
    if ((newValue-diff) < (obj->getr1() + obj->getminWindowThickness()))
    {
        obj->setr(obj->getr1() + obj->getminWindowThickness()); 
        if ((newValue) < (obj->getr() + obj->getminFrameThickness()))
        {
            obj->setR(obj->getr() + obj->getminFrameThickness());
        }
        else
        {
            obj->setR(newValue);
        }
    }
    else
    {
        obj->setR(newValue);
        obj->setr(newValue-diff);
    }
    return  newOffset;
}

// Устанавливаем новое значение для среднего радиуса
AcGeVector3d customObject::setDimValueForRadius2(AcDbDimData* pDimData, AcDbEntity* pEnt, double  newValue, const  AcGeVector3d& offset)
{
    AcGeVector3d newOffset(offset);
    if ((pDimData == NULL) || (pEnt == NULL))
        return newOffset;

    customObject* obj = customObject::cast(pEnt);

    if (obj == NULL)
    {
        return  newOffset;
    }
    if (newValue > (obj->getR() - obj->getminFrameThickness()))
    {
        obj->setr(obj->getR() - obj->getminFrameThickness());
    }
    else
    if (newValue < (obj->getr1() + obj->getminWindowThickness()))
    {
        obj->setr(obj->getr1() + obj->getminWindowThickness());
    }
    else
    {
        obj->setr(newValue);
    }
    return  newOffset;
}

// Устанавливаем новое значение для меньшего радиуса
AcGeVector3d customObject::setDimValueForRadius3(AcDbDimData* pDimData, AcDbEntity* pEnt, double  newValue, const  AcGeVector3d& offset)
{

    AcGeVector3d newOffset(offset);
    if ((pDimData == NULL) || (pEnt == NULL))
        return newOffset;

    customObject* obj = customObject::cast(pEnt);

    if (obj == NULL)
    {
        return  newOffset;
    }


    if (newValue > (obj->getr() - obj->getminWindowThickness()))
    {
        obj->setr1(obj->getr() - obj->getminWindowThickness());
    }
    else
    if (obj->getH() > 2 * newValue * sin(PI / 8))
    {
        obj->setr1(obj->getH() / (2 * sin(PI / 8)));
    }
    else
    {
        obj->setr1(newValue);
    }
    return  newOffset;
}

AcGeVector3d customObject::setDimValueForH(AcDbDimData* pDimData, AcDbEntity* pEnt, double  newValue, const  AcGeVector3d& offset)
{
    AcGeVector3d newOffset(offset);
    if ((pDimData == NULL) || (pEnt == NULL))
        return newOffset;

    customObject* obj = customObject::cast(pEnt);

    if (obj == NULL)
        return  newOffset;
    if (newValue > obj->getHmax())
    {
        obj->setH(obj->getHmax());
    }
    else
    if (newValue < obj->getHMin())
    {
        obj->setH(obj->getHMin());
    }
    else
    {
         obj->setH(newValue);
    }
    return  newOffset;
}
// Функция добавления грипа ========
AcDbGripData* customObject::addGrip(const AcGePoint3d& PT, const int& gripIdx, const double curViewUnitSize) const
{
    AcDbGripData* pGripData = new AcDbGripData();
    pGripData->setGripPoint(PT);
    OWNGripAppData* pAppData = new OWNGripAppData(gripIdx);
    gripDataPtrArray.push_back(pAppData);  //Добавляем собственную грип дату в массив для дальнейшего удаления
    pGripData->setAppData(pAppData);

    switch (gripIdx)
    {        
    case 1:
        pGripData->setViewportDraw(radiusGripPointDraw);
        pGripData->setHotGripDimensionFunc(MyGripHotGripRadiuspoints1);
        break;
    case 2:
        pGripData->setViewportDraw(radiusGripPointDraw);
        pGripData->setHotGripDimensionFunc(MyGripHotGripRadiuspoints2);
        break;
    case 3:   
        pGripData->setViewportDraw(radiusGripPointDraw);
        pGripData->setHotGripDimensionFunc(MyGripHotGripRadiuspoints3);
        break;
    case 4:
        pGripData->setViewportDraw(centerGripPointDraw);
        break;
    case 5:
    case 6:
        pGripData->setViewportDraw(stretchGripPointDraw);
        pGripData->setHotGripDimensionFunc(MyGripHotGripStretchpoints);
        break;
    case 7:
        pGripData->setViewportDraw(rotateGripPointDraw);
        break;
    }

    return pGripData;
};
void customObject::clear_array_ptr(std::vector<OWNGripAppData*>& gripDataPtrArray)
{
    for (auto p : gripDataPtrArray)
    {
        delete p;
    }
    gripDataPtrArray.clear();
    return;
};
// Используется для проверки статусов грип  
void customObject::subGripStatus(const AcDb::GripStat status)
{
    switch (status)
    {
    case AcDb::kGripsDone:
        clear_array_ptr(gripDataPtrArray);
        break;

    case AcDb::kGripsToBeDeleted:
        clear_array_ptr(gripDataPtrArray);
        break;

    case AcDb::kDimDataToBeDeleted:
        break;
    }
}

