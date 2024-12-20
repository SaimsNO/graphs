#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QtWidgets>
#include <qstring.h>
#define RU(text) QString::fromUtf8(reinterpret_cast<const char*>(u8##text))

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
double calculateFunction(double x, double mean, double sigma);
double Phi(double z);
struct ShadingGraph {
    QCPGraph* graph;
    double x0{};
    double x1{};
};
struct GraphLines {
    std::vector<QCPItemLine*>lines;
    std::vector<QCPItemText*>texts;
    QCPItemLine* lineEs;
    QCPItemText* labelEs;
    QCPItemLine* lineMean;
    QCPItemText* labelMean;
    QCPItemLine* lineEi;
    QCPItemText* labelEi;
    QCPItemLine* lineMinus3Sigma;
    QCPItemText* labelminus3Sigma;
    QCPItemLine* linePlus3Sigma;
    QCPItemText* labelplus3Sigma;
public:
    bool check() {
        if ((lines.size() != texts.size()) || lines.size() == 0)
            return false;
        return true;
    }
    void clear() {
        for (auto& graph : lines) {
            delete graph;
        }
        for (auto& graph : texts) {
            delete graph;
        }
        lines.clear();
        texts.clear();
    }

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
    GraphLines lines;
    
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
    void addVerticalLines(GraphData& data);
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
