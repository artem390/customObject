#pragma once

#include <dbjig.h>
#include "customObject.h"

class CustomJig : public AcEdJig
{
public:
    CustomJig() { count = 0; };
    virtual ~CustomJig();
    void startJig();
    virtual DragStatus sampler();
    virtual Adesk::Boolean update();
    virtual AcDbDimDataPtrArray* dimData(const double dimScale);
    virtual Acad::ErrorStatus setDimValue(const AcDbDimData* dimData,const double dimValue);
    virtual AcDbEntity* entity() const { return obj; };
    void updateDimensions();
    AcDbDimDataPtrArray m_dimData;

private:
    customObject* obj;
    AcGePoint3d center, Pt;  // Центральная точка и точка по оси х, y
    int count; // Счетчик для установления нескольких драгов
};


