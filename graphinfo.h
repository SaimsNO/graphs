#pragma once
#include <QtWidgets>
#include "mainwindow.h"
#include "qstring.h"
#include <QtWidgets/qlineedit.h>

class GraphInfoDialog : public QDialog {
public:
    explicit GraphInfoDialog(GraphData& data, QWidget* parent = nullptr);
};

class GraphEditDialog : public QDialog {
public:
    explicit GraphEditDialog(GraphData& data, QWidget* parent = nullptr);
private:
    QDoubleSpinBox* meanInput;
    QDoubleSpinBox* sigmaInput;
    QDoubleSpinBox* xMinInput;
    QDoubleSpinBox* xMaxInput;
private slots:
    void onOkClicked(GraphData& data);
};


