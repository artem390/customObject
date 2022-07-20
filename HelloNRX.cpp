#pragma once
#include "stdafx.h"
#include <aced.h>
#include <geassign.h>
#include <rxregsvc.h>
#include <dbents.h>
#include <dbgroup.h>
#include "customObject.h"
#include "jigHeader.h"
void initApp();
void unloadApp();
void test();
void addObject(AcDbEntity* pEnt, AcDbObjectId &id);
void add();
void addJig();
void initApp()
{
    acedRegCmds->addCommand(L"TEST_COMMANDS",
        L"TestARX",
        L"Test",
        ACRX_CMD_TRANSPARENT,
        test);
	acedRegCmds->addCommand(L"TEST_COMMANDS",
		L"AddObject",
		L"AddObj",
		ACRX_CMD_TRANSPARENT,
		add);
	acedRegCmds->addCommand(L"TEST_COMMANDS",
		L"AddJigObject",
		L"AddJig",
		ACRX_CMD_TRANSPARENT,
		addJig);
	customObject::rxInit();
	acrxBuildClassHierarchy();
}

void unloadApp()
{
    acedRegCmds->removeGroup(L"TEST_COMMANDS");
	deleteAcRxClass(customObject::desc());
}

void test()
{
	AcDbObjectId circleId, line1Id, line2Id;
	AcGePoint3d Ptc, Pt1, Pt2, Pt3;
	double R = 0;
	AcGeVector3d vector(0.0, 0.0, 1.0);
	TCHAR str[100]{};

	acedGetPoint(NULL, L"\ncenter: ", asDblArray(Ptc));
	acedGetDist(asDblArray(Ptc), L"\nR: ", &R);
	AcDbCircle* circle = new AcDbCircle(Ptc, vector, R);
	addObject(circle, circleId);
	circle->close();


	acedGetPoint(NULL, L"\nPt1: ", asDblArray(Pt1));
	acedGetPoint(asDblArray(Pt1), L"\nPt2: ", asDblArray(Pt2));
	acedGetPoint(asDblArray(Pt2), L"\nPt3: ", asDblArray(Pt3));
	AcDbLine* line1 = new AcDbLine(Pt1, Pt2);
	AcDbLine* line2 = new AcDbLine(Pt2, Pt3);
	addObject(line1, line1Id);
	line1->close();
	addObject(line2, line2Id);
	line2->close();

	acedGetKword(_T("\nPress enter:"), str);

	acdbOpenObject(circle, circleId, AcDb::kForWrite);

	AcGeVector3d vector1 = Pt1 - Pt2;
	AcGeVector3d vector2 = Pt3 - Pt2;

	double angle = vector1.angleTo(vector2);
	double l = R / sin(angle * 0.5);

	AcGeVector3d bisVector = vector1.normalize() + vector2.normalize();
	Ptc = Pt2 + (bisVector.normalize() * l);
	circle->setCenter(Ptc);

	circle->close();
}

void addObject(AcDbEntity* pEnt, AcDbObjectId &id)
{
	AcDbBlockTable* pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

	pBlockTable->close();

	pBlockTableRecord->appendAcDbEntity(id, pEnt);
	pBlockTableRecord->close();
}

void add()
{
	AcGePoint3d Ptc;
	AcDbObjectId objId;
	acedGetPoint(NULL, L"\ncenter: ", asDblArray(Ptc));
	customObject* obj = new customObject(Ptc);
	addObject(obj, objId);
	obj->close();
}

void addJig()
{
	CustomJig* jig = new CustomJig();
	jig->startJig();
	delete jig;
}

extern "C" __declspec(dllexport) AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* appId) 
{
  switch (msg) 
  {
  case AcRx::kInitAppMsg:
    acrxDynamicLinker->unlockApplication(appId);
    acrxDynamicLinker->registerAppMDIAware(appId);
    initApp();

    break;

  case AcRx::kUnloadAppMsg:
    unloadApp();
    break;
  }

  return AcRx::kRetOK;
}
