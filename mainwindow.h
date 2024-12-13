#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct ShadingGraph {
    QCPGraph* graph;
    double x0{};
    double x1{};
};

struct GraphData{
    int id{};
    double xMin{};
    double xMax{};
    double mean{};
    double sigma{};
    double ei{};
    double es{};
    QCPGraph* graph;
    std::vector<ShadingGraph>shadingGraphs;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        initWindow();
    }
private:
    void buildGraphic(int graphIndex);
    void onSelectionChanged();
    void calcShading(int graphIndex);
private slots:
    void onXAxisRangeChanged(const QCPRange &newRange);
    void onYAxisRangeChanged(const QCPRange &newRange);
    void plotButtonClicked();
private:
    void initWindow();
    void renamebuttons();
    QCustomPlot* plot;
    QLabel* meanLabel;
    QDoubleSpinBox* meanInput;
    QLabel* sigmaLabel;
    QDoubleSpinBox* sigmaInput;
    QLabel* xMinLabel;
    QDoubleSpinBox* xMinInput;
    QLabel* xMaxLabel;
    QDoubleSpinBox* xMaxInput;
    QPushButton* plotButton;
    QVBoxLayout* controlLayout;

};

#endif // MAINWINDOW_H
