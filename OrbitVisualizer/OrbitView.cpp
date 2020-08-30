#include "OrbitView.h"

OrbitView::OrbitView(QWidget *parent)
	: VDRCOpenGLWidget(parent)
{
	ui.setupUi(this);

	set_eye_distance(250);
	//show_option_changed();
}



OrbitView::~OrbitView()
{
}



void OrbitView::draw()
{
	draw_sphere(rg_Point3D(), 64, SKYBLUE);

	if (!pManager->get_RSOs().empty())
	{
		for (auto& center : m_RSOCenters)
		{
			draw_point(center / 100, RSOSize, BLACK);
		}

		for (auto& activeTCAPair : m_activeTCAPairs)
		{
			draw_line(activeTCAPair.pt1/100, activeTCAPair.pt2/100, pairLineSize, activeTCAPair.color);
		}
	}
}