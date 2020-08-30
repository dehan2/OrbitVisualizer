#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OrbitVisualizer.h"

#include "RSOManager.h";
#include "OrbitView.h"
#include "constForDVDCOOP.h"
#include <list>
#include <QTimer>
#include <string>

using namespace std;

class OrbitVisualizer : public QMainWindow
{
    Q_OBJECT

private:
    RSOManager m_manager;
    OrbitView* pView;

    bool bFileOpen = false;
    bool bPlay = false;
    QTimer* simulationTimer = nullptr;

    double m_time = 0.0;
    double m_playSpeed = 1.0;

    list<PredictionCommand> m_commands;
    list<PredictedTCA> m_TCAInfos;

public:
    OrbitVisualizer(QWidget *parent = Q_NULLPTR);

	void read_prediction_command_file(const string& filePath, list<PredictionCommand>& commands);
    void read_close_approach_report(const string& filePath, list<PredictedTCA>& TCAInfos);
    void update_RSO_coord(double time);
    void update_time_string(double time);
    tm make_time_on_UTC(double time);
    string make_time_string(double time);
    
    void update_active_TCA(double time);
    list<PredictedTCA*> find_active_TCA(double time);

private:
    Ui::OrbitVisualizerClass ui;

public slots:
	void open_prediction_command_file();

    void play_simulation();
    void increase_simulation_time();

    void simulation_option_changed();
    void visualization_option_changed();
};
