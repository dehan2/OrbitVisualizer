#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OrbitVisualizer.h"

class OrbitVisualizer : public QMainWindow
{
    Q_OBJECT

public:
    OrbitVisualizer(QWidget *parent = Q_NULLPTR);

private:
    Ui::OrbitVisualizerClass ui;
};
