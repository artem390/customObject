#pragma once

#include <dbjig.h>
#include "customObject.h"

class CustomJig : public AcEdJig
{
public:
    CustomJig(); 
    ~CustomJig();
    void startJig();
    virtual DragStatus sampler();
    virtual Adesk::Boolean update();
    virtual AcDbDimDataPtrArray* dimData(const double dimScale);
    virtual Acad::ErrorStatus setDimValue(const AcDbDimData* dimData,const double dimValue);
    virtual AcDbEntity* entity() const { return m_obj; };
    void updateDimensions();
    

private:
    customObject* m_obj;
    AcGePoint3d m_center, m_Pt;  // Центральная точка и точка по оси х, y
    int count; // Счетчик для установления нескольких драгов
    AcDbDimDataPtrArray* m_dimData;
};


