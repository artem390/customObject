#include "stdafx.h"
#include "customObject.h"

ACRX_DXF_DEFINE_MEMBERS(customObject, AcDbEntity, AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 0, CUSTOM, "CustomObject");

//dwgOutFields
Acad::ErrorStatus customObject::dwgOutFields(AcDbDwgFiler* fill) const
{
    assertReadEnabled();
   

    return fill->filerStatus();
}
// dwgInFields
Acad::ErrorStatus customObject::dwgInFields(AcDbDwgFiler* fill)
{
    assertWriteEnabled();
    

    return fill->filerStatus();
}

Acad::ErrorStatus customObject::subTransformBy(const AcGeMatrix3d& xform)
{
    assertWriteEnabled();

    return Acad::eOk;
}
// Разбиение объекта
Acad::ErrorStatus customObject::subExplode(AcDbVoidPtrArray& entitySet) const
{
    assertReadEnabled();
    Acad::ErrorStatus es = Acad::eOk;
    

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
