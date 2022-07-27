#pragma once

#include <dbjig.h>
#include "customObject.h"

class CustomJig : public AcEdJig
{
public:
    CustomJig(); 
    virtual ~CustomJig();
    void startJig();
    virtual DragStatus sampler();
    virtual Adesk::Boolean update();
    virtual AcDbDimDataPtrArray* dimData(const double dimScale);
    virtual AcDbDimDataPtrArray* dimData() const;
    virtual Acad::ErrorStatus setDimValue(const AcDbDimData* dimData,const double dimValue);
    virtual AcDbEntity* entity() const { return obj; };
    void updateDimensions();
    AcDbDimDataPtrArray *m_dimData;

private:
    customObject* obj;
    AcGePoint3d center, Pt;  // Центральная точка и точка по оси х, y
    int count; // Счетчик для установления нескольких драгов
};


