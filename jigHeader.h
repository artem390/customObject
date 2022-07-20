#pragma once

#include <dbjig.h>
#include "customObject.h"

class CustomJig : public AcEdJig
{
public:
    CustomJig() { count = 0; };
    void startJig();
    virtual DragStatus sampler();
    virtual Adesk::Boolean update();
    virtual AcDbEntity* entity() const { return obj; };

private:
    customObject* obj;
    AcGePoint3d center, Pt;  // Центральная точка и точка по оси х, y
    int count; // Счетчик для установления нескольких драгов
};


