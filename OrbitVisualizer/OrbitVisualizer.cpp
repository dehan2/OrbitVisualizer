#include "OrbitVisualizer.h"
#include "OrbitalBall.h"
#include <fstream>
#include "cJulian.h"
#include <QDateTime>
#include <sstream>
#include <iomanip>
#include <iostream>

OrbitVisualizer::OrbitVisualizer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	pView = ui.orbitViewWidget;
	pView->pManager = &m_manager;

	bPlay = false;
	simulationTimer = new QTimer(this);
	connect(simulationTimer, SIGNAL(timeout()), this, SLOT(increase_simulation_time()));

	simulation_option_changed();
	visualization_option_changed();

	showMaximized();
}



void OrbitVisualizer::read_prediction_command_file(const string& filePath, list<PredictionCommand>& commands)
{
	ifstream fin;
	fin.open(filePath);

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			char lineData[256];
			fin.getline(lineData, 256);

			if (lineData[0] != '#')
			{
				PredictionCommand command;

				char* context;
				string delimiter = " \t";

				string token = strtok_s(lineData, delimiter.c_str(), &context);
				command.directory = token;

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.tleFile = token;

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.numObject = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.numLineSegments = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.predictionTimeWindow = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.year = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.month = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.day = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.hour = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.min = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				command.sec = stoi(token);

				commands.push_back(command);
			}
		}
	}

	fin.close();
}




void OrbitVisualizer::read_close_approach_report(const string& filePath, list<PredictedTCA>& TCAInfos)
{
	ifstream fin;
	fin.open(filePath);

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			char lineData[256];
			fin.getline(lineData, 256);

			if (lineData[0] != '#')
			{
				PredictedTCA TCAInfo;

				char* context;
				string delimiter = "\t";

				string token = strtok_s(lineData, delimiter.c_str(), &context);
				TCAInfo.primaryRSO = token;

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.primaryID = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.secondaryRSO = token;

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.secondaryID = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.minDistance = stof(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.year = stoi(token)+1900;

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.month = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.day = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.hour = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.min = stoi(token);

				token = strtok_s(NULL, delimiter.c_str(), &context);
				TCAInfo.sec = stoi(token);

				TCAInfos.push_back(TCAInfo);
			}
		}
	}

	fin.close();
}



void OrbitVisualizer::update_RSO_coord(double time)
{
	list<rg_Point3D>& RSOCenters = pView->m_RSOCenters;
	RSOCenters.clear();

	for (auto& RSO : m_manager.get_RSOs())
	{
		RSOCenters.push_back(RSO.calculate_point_on_Kepler_orbit_at_time(time));
	}

	update_active_TCA(time);

	update_time_string(time);
	pView->update();
}



void OrbitVisualizer::update_time_string(double time)
{
	string timeStr = make_time_string(time);
	ui.lineEdit_timeString->setText(QString::fromStdString(timeStr));
}



tm OrbitVisualizer::make_time_on_UTC(double time)
{
	cJulian newTime = m_manager.get_epoch();
	newTime.AddSec(time);
	time_t rawTime = newTime.ToTime();
	tm timeOnUTC;
	gmtime_s(&timeOnUTC, &rawTime);
	return timeOnUTC;
}



string OrbitVisualizer::make_time_string(double time)
{
	tm timeOnUTC = make_time_on_UTC(time);

	int year = timeOnUTC.tm_year + 1900;
	int mon = timeOnUTC.tm_mon;
	int day = timeOnUTC.tm_mday;
	int hour = timeOnUTC.tm_hour;
	int min = timeOnUTC.tm_min;
	int sec = timeOnUTC.tm_sec;

	string monStr;
	switch (mon)
	{
	case 1:
		monStr = "Jan.";
		break;
	case 2:
		monStr = "Feb.";
		break;
	case 3:
		monStr = "Mar.";
		break;
	case 4:
		monStr = "Apr.";
		break;
	case 5:
		monStr = "May.";
		break;
	case 6:
		monStr = "Jun.";
		break;
	case 7:
		monStr = "Jul.";
		break;
	case 8:
		monStr = "Aug.";
		break;
	case 9:
		monStr = "Sep.";
		break;
	case 10:
		monStr = "Oct.";
		break;
	case 11:
		monStr = "Nov.";
		break;
	case 12:
		monStr = "Dec.";
		break;
	default:
		break;
	}


	stringstream timeStr;
	timeStr << setw(2) << setfill('0') << to_string(hour)<<":";
	timeStr << setw(2) << setfill('0') << to_string(min) << ":";
	timeStr << setw(2) << setfill('0') << to_string(sec) << ", ";
	timeStr<<monStr<<" "<< setw(2) << setfill('0') << to_string(day)<<", "<< to_string(year) << " UTC";
	return timeStr.str();
}



void OrbitVisualizer::update_active_TCA(double time)
{
	list<ActiveTCA>& activeTCAPairs = pView->m_activeTCAPairs;
	activeTCAPairs.clear();

	list<PredictedTCA*> activeTCAInfos = find_active_TCA(time);
	for (auto& activeTCAInfo : activeTCAInfos)
	{
		ActiveTCA activeTCA;
		OrbitalBall* primary = m_manager.find_RSO_from_ID(activeTCAInfo->primaryID);
		OrbitalBall* secondary = m_manager.find_RSO_from_ID(activeTCAInfo->secondaryID);
		rg_Point3D primaryCoord = primary->calculate_point_on_Kepler_orbit_at_time(time);
		rg_Point3D secondaryCoord = secondary->calculate_point_on_Kepler_orbit_at_time(time);
		double interRSODistance = primaryCoord.distance(secondaryCoord);

		Color3f lineColor = VIOLET;
		if(interRSODistance < 800)
			lineColor = BLUE;
		if (interRSODistance < 600)
			lineColor = GREEN;
		if (interRSODistance < 400)
			lineColor = ORANGE;
		if (interRSODistance < 200)
			lineColor = RED;

		activeTCA.pt1 = primaryCoord;
		activeTCA.pt2 = secondaryCoord;
		activeTCA.color = lineColor;
		activeTCAPairs.push_back(activeTCA);
	}
}



list<PredictedTCA*> OrbitVisualizer::find_active_TCA(double time)
{
	tm timeOnUTC = make_time_on_UTC(time);

	list<PredictedTCA*> activeTCAs;

	for (auto& TCAInfo : m_TCAInfos)
	{
		int hourDiff = timeOnUTC.tm_hour - TCAInfo.hour;
		int minDiff = timeOnUTC.tm_min - TCAInfo.min;
		int secDiff = timeOnUTC.tm_sec - TCAInfo.sec;
		int diffInSec = hourDiff * 3600 + minDiff * 60 + secDiff;

		if (abs(diffInSec) < TCA_ACTIVATION_THRESHOLD)
		{
			activeTCAs.push_back(&TCAInfo);
		}
	}
	return activeTCAs;
}



void OrbitVisualizer::open_prediction_command_file()
{
	QString QfilePath = QFileDialog::getOpenFileName(this, tr("Open Prediction Command"), NULL, tr("Prediction Command (*.txt)"));
	string filePath = translate_to_window_path(QfilePath);

	read_prediction_command_file(filePath, m_commands);

	m_manager.initialize_RSO_manager(m_commands.front());
	bFileOpen = true;

	string filePathForCAReport = m_commands.front().directory + "CAReport.txt";
	read_close_approach_report(filePathForCAReport, m_TCAInfos);

	update_RSO_coord(m_time);
	update();
}



void OrbitVisualizer::play_simulation()
{
	if (simulationTimer->isActive() == false)
	{
		simulationTimer->start(100);
	}
	else
	{
		simulationTimer->stop();
	}
}



void OrbitVisualizer::increase_simulation_time()
{
	m_time += m_playSpeed * standardTimeIncrement;

	update_RSO_coord(m_time);
}




void OrbitVisualizer::simulation_option_changed()
{
	m_playSpeed = ui.doubleSpinBox_playSpeed->value();
}



void OrbitVisualizer::visualization_option_changed()
{
	pView->RSOSize = ui.spinBox_point_size->value();
	pView->update();
}



void OrbitVisualizer::move_to_global_TCA()
{
	double globalMinDistance = DBL_MAX;
	PredictedTCA* globalTCA = nullptr;

	for (auto& TCAInfo : m_TCAInfos)
	{
		if (TCAInfo.minDistance < globalMinDistance)
		{
			globalMinDistance = TCAInfo.minDistance;
			globalTCA = &TCAInfo;
		}
	}

	tm epochOnUTC = make_time_on_UTC(0.0);
	int hourDiff = globalTCA->hour - epochOnUTC.tm_hour;
	int minDiff = globalTCA->min - epochOnUTC.tm_min;
	int secDiff = globalTCA->sec - epochOnUTC.tm_sec;
	int diffInSec = hourDiff * 3600 + minDiff * 60 + secDiff;

	cout << "Predicted Global TCA: [" << globalTCA->primaryRSO << " (" << globalTCA->primaryID << ")] and ";
	cout << "[" << globalTCA->secondaryRSO << " (" << globalTCA->secondaryID << ")]";
	cout << " with min. distance " << globalTCA->minDistance << " at " << make_time_string(diffInSec) << endl;

	OrbitalBall* primary = m_manager.find_RSO_from_ID(globalTCA->primaryID);
	OrbitalBall* secondary = m_manager.find_RSO_from_ID(globalTCA->secondaryID);
	rg_Point3D primaryAtGlobalTCA = primary->calculate_point_on_Kepler_orbit_at_time(diffInSec);
	rg_Point3D secondaryAtGlobalTCA = secondary->calculate_point_on_Kepler_orbit_at_time(diffInSec);

	pView->set_eye_direction((primaryAtGlobalTCA + secondaryAtGlobalTCA) / 2);
	m_time = diffInSec - 300;
	update_RSO_coord(m_time);
	update_time_string(m_time);

	pView->update();
}



