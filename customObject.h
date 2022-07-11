#pragma once
#include <database.h>
class customObject : public AcDbEntity
{
public:

    customObject() {};
    customObject(const AcGePoint3d& center) :center(center){};
    virtual ~customObject() {};

    // Для сохр. данных при загрузке/выгрузке
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const override;
    virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*) override;
    // Перемещение, поворот, зеркало
    virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform) override;
    // Разбиение примитива на простые объекты 
    virtual Acad::ErrorStatus subExplode(AcDbVoidPtrArray& entitySet) const override;

    virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const override;
    virtual Acad::ErrorStatus subGetTransformedCopy(const AcGeMatrix3d& xform, AcDbEntity*& ent) const override;

    // Получение точек привязки для объекта
    virtual Acad::ErrorStatus subGetOsnapPoints(
        AcDb::OsnapMode       osnapMode,
        Adesk::GsMarker       gsSelectionMark,
        const AcGePoint3d& pickPoint,
        const AcGePoint3d& lastPoint,
        const AcGeMatrix3d& viewXform,
        AcGePoint3dArray& snapPoints,
        AcDbIntArray& geomIds) const;

    // Функция получения точек пересечения
    virtual Acad::ErrorStatus
        subIntersectWith(
            const AcDbEntity* ent,
            AcDb::Intersect intType,
            AcGePoint3dArray& points,
            Adesk::GsMarker /*thisGsMarker*/,
            Adesk::GsMarker otherGsMarker) const;

    // Для отображения примититва
    virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* draw) override;

    virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds) const override;
    virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, const AcGeVector3d& offset) override;

    ACRX_DECLARE_MEMBERS(customObject);
    AcGePoint3d  getCenter() const { return center; };
    AcGeVector3d  getDirection() const { return directionV; };
    AcGeVector3d  getNormal() const { return normV; };
    double getR() const { return R; };
    double getr() const { return r; };
    double getr1()const { return r1; };
    double getH() const { return h; };

    AcGeMatrix3d get_Matrix(AcGeMatrix3d& xMat) const
    {
        //Задаем коорд. сист матрице ЛСК через центральную точку и 3 вектора (х,y,z)
        return xMat.setCoordSystem(center, directionV, (-1) * directionV.crossProduct(normV), normV);
    }

    AcGePoint3d  getPt1() const { return{ -R, -(R-r), 0 }; };
    AcGePoint3d  getPt2() const { return{ -R , 0, 0 }; };
    AcGePoint3d  getPt3() const { return{ -r, 0, 0 }; };
    AcGePoint3d  getPt4() const { return{ r , 0, 0 }; };
    AcGePoint3d  getPt5() const { return{ R, 0, 0 }; };
    AcGePoint3d  getPt6() const { return{ R, -(R - r), 0 }; };
    AcGePoint3d  getPt7() const { return{ -r1, 0, 0 }; };
    AcGePoint3d  getPt8() const { return{ r1 , 0, 0 }; };
    AcGePoint3d  getPt9() const { return{ 0, R, 0 }; };
    AcGePoint3d  getPt10() const { return{ 0 , r, 0 }; };
    AcGePoint3d  getPt11() const { return{ -h/2, sqrt(pow(r1,2)-pow(h/2,2)), 0 }; };
    AcGePoint3d  getPt12() const { return{ h / 2, sqrt(pow(r1,2) - pow(h / 2,2)), 0 }; };
    AcGePoint3d  getPt13() const { return{ -h / 2, sqrt(pow(r,2) - pow(h / 2,2)), 0 }; };
    AcGePoint3d  getPt14() const { return{ h / 2, sqrt(pow(r,2) - pow(h / 2,2)), 0 }; };
    AcGePoint3d  getPt15() const { return{ 0, r1, 0 }; };
    AcGePoint3d  getPt17() const ;
    AcGePoint3d  getPt20() const;

    void  setCenter(const AcGePoint3d& center) { this->center = center; };
    void  setNormal(const AcGeVector3d& normV) { this->normV=normV; };
    void  setDirection(const AcGeVector3d& directionV) { this->normV = directionV; };
    void setR( double R) { this->R=R; };
    void setr( double r) { this->r=r; };
    void setr1( double r1) {this->r1=r1; };
    void setH( double h) { this->h=h; };


protected:
    static const Adesk::Int16 VERSION = 1;


    AcGePoint3d center{};
    AcGeVector3d normV{ 0, 0, 1 };
    AcGeVector3d directionV{ 1,0,0 };

    
    double R = 10000;
    double r = 8000;
    double r1 = 2000;
    double h = 1000;
};

