#pragma once
#include <database.h>
class OWNGripAppData
{
private:
    int ID; //идентификатор для Грипа
    int gripSize;
    AcDbDimDataPtrArray d;
public:
    OWNGripAppData(int id) :ID(id){}
    virtual ~OWNGripAppData() {};
    AcDbDimDataPtrArray getDimArray() const { return d; };
    void setDimArray(const AcDbDimDataPtrArray dim) { this->d = dim; };
    int index() const { return ID; }
    void setIndex(const int id) { ID = id; }
    void setGripSize(const int size) { gripSize = size; }
    int getGripSize() const { return gripSize; }
};
class customObject : public AcDbEntity
{
public:
    customObject() {};
    customObject(const AcGePoint3d& center) :center(center){};
    virtual ~customObject() {};
    // Функция перевода из AcGeCircArc3d в_AcDbArc
    void AcGeCircArc3dToAcDbArc(const AcGeCircArc3d& pGeArc, AcDbArc* pDbArc) const;

    // Функции рисования грипов
    static void centerGripPointDraw(AcDbGripData* pThis, AcGiViewportDraw* pVd, const AcDbObjectId& entId, AcDbGripOperations::DrawType type, AcGePoint3d* cursor, int gripSize);
    static void radiusGripPointDraw(AcDbGripData* pThis, AcGiViewportDraw* pVd, const AcDbObjectId& entId, AcDbGripOperations::DrawType type, AcGePoint3d* cursor, int gripSize);
    static void stretchGripPointDraw(AcDbGripData* pThis, AcGiViewportDraw* pVd, const AcDbObjectId& entId, AcDbGripOperations::DrawType type, AcGePoint3d* cursor, int gripSize);
    // Для грипов радиусов
    static  void  MyGripHotGripRadiuspoints(AcDbGripData* pGripData, const  AcDbObjectId& entId, double  dimScale, AcDbDimDataPtrArray& dimDataArr);
    static AcGeVector3d setDimValueForRadius(AcDbDimData* pDimData, AcDbEntity* pEnt, double  newValue, const  AcGeVector3d& offset);
    // Для грипов ширины
    static  void  MyGripHotGripStretchpoints(AcDbGripData* pGripData, const  AcDbObjectId& entId, double  dimScale, AcDbDimDataPtrArray& dimDataArr);
    static AcGeVector3d setDimValueForH(AcDbDimData* pDimData, AcDbEntity* pEnt, double  newValue, const  AcGeVector3d& offset);
    // Для обновления динамического размера
    static  bool  updateDimensions(customObject* customObject, const AcGePoint3d& xline1Pt, const AcGePoint3d& xline2Pt);
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
            Adesk::GsMarker thisGsMarker,
            Adesk::GsMarker otherGsMarker) const;

    // Для отображения примититва
    virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* draw) override;

    // Функция добавления грипа
    AcDbGripData* addGrip(const AcGePoint3d& PT, const int& gripIdx, const double curViewUnitSize) const;

    // Используется для проверки статусов грип
    virtual void subGripStatus(const AcDb::GripStat status) override;
    // Получение грип поинтов
    virtual Acad::ErrorStatus subGetGripPoints(
        AcDbGripDataPtrArray& grips, const double curViewUnitSize, const int gripSize,
        const AcGeVector3d& curViewDir, const int bitflags) const override;

    // Обеспечение движения грип поинтов
    virtual Acad::ErrorStatus subMoveGripPointsAt(
        const AcDbVoidPtrArray& gripAppData,
        const AcGeVector3d& offset,
        const int bitflags) override;


    ACRX_DECLARE_MEMBERS(customObject);
    AcGePoint3d  getCenter() const { return center; };
    AcGeVector3d  getDirection() const { return directionV; };
    AcGeVector3d  getNormal() const { return normV; };
    double getR() const { return R; };
    double getr() const { return r; };
    double getr1()const { return r1; };
    double getH() const { return h; };

    void get_Matrix(AcGeMatrix3d& xMat) const
    {
        xMat.setCoordSystem(center, directionV, (-1) * directionV.crossProduct(normV), normV);
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
    double getCoeff() const { return 500; };
    double getminFrameThickness() const { return getCoeff()*4; };
    double getminWindowThickness() const { return getCoeff()*10; };
    double getHMin() const { return getCoeff(); };
    double getHmax() const { return 2 * r1 * sin(PI/8); };

    void  setCenter(const AcGePoint3d& center) { this->center = center; };
    void  setNormal(const AcGeVector3d& normV) { this->normV=normV; };
    void  setDirection(const AcGeVector3d& directionV) { this->directionV = directionV; };
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

    // Вектор указателей  (может изменяться константными методами класса)
    mutable std::vector<OWNGripAppData*> gripDataPtrArray;

    // Функция очистки вектора указателей OWNGripAppData*
    void clear_array_ptr(std::vector<OWNGripAppData*>& m_GripDataPtrArray);
};


