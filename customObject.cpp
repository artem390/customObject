#include "stdafx.h"
#include "customObject.h"

ACRX_DXF_DEFINE_MEMBERS(customObject, AcDbEntity, AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 0, CUSTOM, "CustomObject");

//dwgOutFields
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
// dwgInFields
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
    Acad::ErrorStatus es = Acad::eOk;
    /*AcGeMatrix3d xMat = get_Matrix(xMat);
    //Отрезки
    AcDbLine* line1 = new AcDbLine(getPt1().transformBy(xMat), getPt6().transformBy(xMat));
    entitySet.append(line1);
    AcDbLine* line2 = new AcDbLine(getPt2().transformBy(xMat), getPt5().transformBy(xMat));
    entitySet.append(line2);
    AcDbLine* line3 = new AcDbLine(getPt1().transformBy(xMat), getPt2().transformBy(xMat));
    entitySet.append(line3);
    AcDbLine* line4 = new AcDbLine(getPt2().transformBy(xMat), getPt5().transformBy(xMat));
    entitySet.append(line4);
    AcDbLine* line5 = new AcDbLine(get_PT2().transformBy(xMat), get_PT8().transformBy(xMat));
    entitySet.append(line5);
    AcDbLine* line6 = new AcDbLine(get_PT4().transformBy(xMat), get_PT10().transformBy(xMat));
    entitySet.append(line6);*/
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

    
    return Acad::eOk;
}

// Интерсекты (Точки пересечения)
Acad::ErrorStatus customObject::subIntersectWith(const AcDbEntity* ent, AcDb::Intersect intType, AcGePoint3dArray& points, Adesk::GsMarker thisGsMarker, Adesk::GsMarker otherGsMarker) const
{
    assertReadEnabled();
    Acad::ErrorStatus es = Acad::eOk;
    
    return es;
}

// Отрисовка объекта
Adesk::Boolean customObject::subWorldDraw(AcGiWorldDraw* draw)
{
    assertReadEnabled();
    AcGePoint3d  point17 = getPt17();
    AcGePoint3d  point20 = getPt20();
    AcGeMatrix3d xMat = get_Matrix(xMat);
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

// Грипы (Более старые функции)
Acad::ErrorStatus customObject::subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds) const
{
    assertReadEnabled();
    Acad::ErrorStatus es = Acad::eOk;
    AcGeMatrix3d xMat= get_Matrix(xMat);
    
    // Изменение внешнего радиуса 
    gripPoints.append(getPt9().transformBy(xMat));  //1
    // Изменение среднего радиуса 
    gripPoints.append(getPt10().transformBy(xMat));  //2
    // Изменение младшего радиуса 
    gripPoints.append(getPt15().transformBy(xMat));   //3
    // Изменение перемещение
    gripPoints.append(getCenter());   //4

    
    /*gripPoints.append(get_PT11().transformBy(xMat));  //6
    // Изменение длины заклепки
    gripPoints.append(get_PT3().transformBy(xMat));   //7
    gripPoints.append(get_PT9().transformBy(xMat));   //8
    // Точка для поворота (немного правее объекта)
    AcGePoint3d rotate_point(get_PT6().x + 50, get_PT6().y, get_PT6().z);
    gripPoints.append(rotate_point.transformBy(xMat)); //9*/
    return es;
}

Acad::ErrorStatus customObject::subMoveGripPointsAt(const AcDbIntArray& indices, const AcGeVector3d& offset)
{
    assertWriteEnabled();
    Acad::ErrorStatus es = Acad::eOk;




 // Накладываемые ограничения
 // Мин. разница R и r
    double RminusrMin = 2000;
 // Мин. разница r и r1  
    double rminusr1Min = 6000;
 // Мин. r1  
    double r1Min = 1500;
    


    // Вектор ОX для объекта
    AcGeVector3d vector_OX(directionV);
    double len_OX = vector_OX.dotProduct(offset);
    // Вектор ОУ для объекта
    AcGeVector3d vector_OY(directionV.perpVector());
    double len_OY = vector_OY.dotProduct(offset);


    for (int i = 0; i < indices.length(); i++)
    {

        switch (indices[i])
        {
            
        case 0:
            if ((r + len_OY - r1) < rminusr1Min)
            {
                r = r1 + rminusr1Min;
                R = r + RminusrMin;
            }
            else
            {
                R += len_OY;
                r += len_OY;
            }        
            return Acad::eOk;

        case 1:
            if (r + len_OY >= (R- RminusrMin))
            {
                r = R - RminusrMin;
            }
            else
            if((r+ len_OY) < r1+rminusr1Min)
            {
                r= r1+ rminusr1Min;
            }
            else
            {
                r += len_OY;
            }
            return Acad::eOk;
        case 2:
            if (r1 + len_OY >= r- rminusr1Min)
            {
                r1 = r - rminusr1Min;
            }
            else
            if ((r1 + len_OY) < r1Min)
            {
                r1 = r1Min;
            }
            else
            {
                r1 += len_OY;
            }
            return Acad::eOk;
        case 3:
            center += offset;
            return Acad::eOk;
        }
    }
    return es;
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