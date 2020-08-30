#pragma once

#include <QWidget>
#include "ui_OrbitView.h"

#include "VDRCOpenGLWidget.h"
#include "RSOManager.h"
#include "rg_Point3D.h"

class OrbitView : public VDRCOpenGLWidget	
{
	Q_OBJECT

public:
	bool bFileOpen = false;
	RSOManager* pManager = nullptr;
	list<rg_Point3D> m_RSOCenters;
	list<ActiveTCA> m_activeTCAPairs;

	//View Options
	int RSOSize = 1;
	int pairLineSize = 5;

public:
	OrbitView(QWidget *parent = Q_NULLPTR);
	~OrbitView();

	virtual void draw();

private:
	Ui::OrbitView ui;
};
