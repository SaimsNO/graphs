#include "mainwindow.h"
#include <cmath>
#include <QtWidgets>
#include "qcustomplot.h"
#include <QtConcurrent>

QMap<int, GraphData> graphDataMap;

double Phi(double z) {
    return 0.5 * (1.0 + std::erf(z / std::sqrt(2.0)));
}

double calculateFunction(double x, double mean, double sigma) {
    const double factor = 1 / (sigma * std::sqrt(2 * M_PI));
    double exponent = -std::pow(x - mean, 2) / (2 * std::pow(sigma, 2));
    return factor * std::exp(exponent);
}

void MainWindow::initWindow(){
    resize(1500, 800);

    // Создаем виджеты
    auto *mainWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(mainWidget);
    controlLayout = new QVBoxLayout();
    plot = new QCustomPlot(this);

    // Создаем виджет для размещения в scrollArea
    QWidget *controlWidget = new QWidget(this);
    controlWidget->setLayout(controlLayout);

    // Добавляем scrollArea
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(controlWidget);
    scrollArea->setWidgetResizable(true);  // Автоматическое изменение размера содержимого
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  // Включаем вертикальную полосу прокрутки
    controlWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->addWidget(scrollArea, 1);  // Лейаут с элементами управления с левой стороны
    mainLayout->addWidget(plot, 3);  // График с правой стороны
    // Поля для ввода
    meanLabel = new QLabel("Mean (μ):", this);
    meanInput = new QDoubleSpinBox(this);
    meanInput->setDecimals(5);
    meanInput->setRange(-10.0, 10.0);
    meanInput->setValue(0.026);

    sigmaLabel = new QLabel("Sigma (σ):", this);
    sigmaInput = new QDoubleSpinBox(this);
    sigmaInput->setDecimals(5);
    sigmaInput->setRange(0.001, 10.0);
    sigmaInput->setValue(0.012);
    
    xMinLabel = new QLabel("ei:", this);
    xMinInput = new QDoubleSpinBox(this);
    xMinInput->setDecimals(5);
    xMinInput->setRange(-10.0, 10.0);
    xMinInput->setValue(0.006);
    
    xMaxLabel = new QLabel("es:", this);
    xMaxInput = new QDoubleSpinBox(this);
    xMaxInput->setDecimals(5);
    xMaxInput->setRange(-10.0, 10.0);
    xMaxInput->setValue(0.055);
    
    plotButton = new QPushButton("Добавить график", this);
    plotButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 5px;");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(meanLabel, meanInput);
    formLayout->addRow(sigmaLabel, sigmaInput);
    formLayout->addRow(xMinLabel, xMinInput);
    formLayout->addRow(xMaxLabel, xMaxInput);
    formLayout->addRow(plotButton);
    controlLayout->addLayout(formLayout);
    // Настройка графика
    plot->xAxis->setLabel("X");
    plot->yAxis->setLabel("f(X)");
    plot->xAxis->setRange(-0.1, 0.1);
    plot->yAxis->setRange(0, 35);
    plot->setInteraction(QCP::iRangeZoom);
    plot->setInteraction(QCP::iRangeDrag);
    plot->setInteraction(QCP::iSelectPlottables, true);
    plot->setBackground(QBrush(QColor(245, 245, 245)));    // Светлый фон
    plot->xAxis->setBasePen(QPen(Qt::black, 1));
    plot->yAxis->setBasePen(QPen(Qt::black, 1));
    plot->xAxis->setTickPen(QPen(Qt::black, 1));
    plot->yAxis->setTickPen(QPen(Qt::black, 1));
    plot->xAxis->setSubTickPen(QPen(Qt::black, 1));
    plot->yAxis->setSubTickPen(QPen(Qt::black, 1));
    plot->xAxis->setTickLabelColor(Qt::black);
    plot->yAxis->setTickLabelColor(Qt::black);

    mainWidget->setStyleSheet("QLabel { font-size: 12pt; font-family: Arial; }"
                              "QLineEdit { font-size: 12pt; font-family: Arial; }"
                              "QDoubleSpinBox { font-size: 12pt; font-family: Arial; }");

    connect(plotButton, &QPushButton::clicked, this, &MainWindow::plotButtonClicked);


    //connect(plot, &QCustomPlot::selectionChangedByUser, this, &MainWindow::onSelectionChanged);//есть возможность по нажатию на график открывать его свойства

    setCentralWidget(mainWidget);
}

void MainWindow::renamebuttons()
{
    
}


void MainWindow::calcShading(int graphIndex) {
    // Основной график (предполагаем, что он уже добавлен)
    QCPGraph* mainGraph = graphDataMap[graphIndex].graph;
    for (int i = 0; i < graphDataMap[graphIndex].shadingGraphs.size(); ++i) {
        ShadingGraph graph = graphDataMap[graphIndex].shadingGraphs[i];
        // Создаем новый график для заполнения
        QCPGraph* fillGraph = graph.graph;
        QVector<double> xFill, yFill;

        // Сканируем основной график, чтобы найти точки в диапазоне [xMin, xMax]
        auto data = mainGraph->data();
        for (auto it = data->findBegin(graph.x0); it != data->findEnd(graph.x1); ++it) {
            xFill.append(it->key);
            yFill.append(it->value); // Значения из основного графика
        }

        // Добавляем крайние точки для корректной заливки
        xFill.prepend(graph.x0);
        yFill.prepend(0); // Базовая линия (y = 0)
        xFill.append(graph.x1);
        yFill.append(0); // Базовая линия (y = 0)

        // Устанавливаем данные
        fillGraph->setData(xFill, yFill);

        // Создаём кисть с текстурой (штриховка 45 градусов)
        QPixmap texture(10, 10); // Размер узора
        texture.fill(Qt::transparent); // Прозрачный фон

        QPainter painter(&texture);
        painter.setPen(QPen(QColor(100, 100, 250, 150), 2)); // Цвет и ширина линий
        painter.drawLine(0, 10, 10, 0); // Диагональная линия
        painter.end();

        // Настраиваем заливку с текстурой
        QBrush hatchBrush(texture);
        fillGraph->setBrush(hatchBrush);
        fillGraph->setPen(Qt::NoPen); // Без линии, только заливка
    }
}

void MainWindow::buildGraphic(int graphIndex){
    GraphData graphData = graphDataMap[graphIndex];
    QVector<double> x, y;
    // Целевое количество точек
    const int targetPoints = 1000 / graphDataMap.size();

    // Вычисляем шаг
    double step = (graphData.xMax - graphData.xMin) / targetPoints;

    for (double i = graphData.xMin; i <= graphData.xMax; i += step) {
        x.append(i);
        y.append(calculateFunction(i, graphData.mean, graphData.sigma));
    }
    graphData.graph->setData(x, y);
    calcShading(graphIndex);
}

void MainWindow::onXAxisRangeChanged(const QCPRange &newRange) {
    if(!plot->graph()) return;
    QList<QFuture<void>> futures;
    for (auto& it : graphDataMap) {
        GraphData& data = it;
        data.xMax = newRange.upper;
        data.xMin = newRange.lower;
        futures.append(QtConcurrent::run([&, data]() {
            buildGraphic(data.id);  // buildGraphic возвращает вычисленные данные
            }));
    }
    for (auto& future : futures) {
        future.waitForFinished();
    }
    plot->replot();
}

void MainWindow::onYAxisRangeChanged(const QCPRange &newRange) {
    if(plot->graph()->data()->isEmpty()) return;
}

void MainWindow::onSelectionChanged() {
}
void MainWindow::plotButtonClicked(){
    int graphIndex = plot->graphCount(); // Индекс нового графика
    plot->addGraph();

    plot->graph(graphIndex)->setPen(QPen(QColor(40, 110, 255), 2));
    QWidget *buttonContainer = new QWidget(this);
    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonContainer);

    QPushButton *button = new QPushButton(QString("График %1").arg(graphDataMap.size() + 1), this);
    renamebuttons();
    QPushButton* deleteGraph = new QPushButton("Удалить график", this);
    QPushButton* editGraph = new QPushButton("Изменить график", this);
    deleteGraph->setStyleSheet(
        "background-color: #DC143C; "
        "color: white; "
        "font-weight: bold; "
        "padding: 5px; "
        "border: none; "
        "border-radius: 5px; "
        "transition: background-color 0.3s ease;"
        "}"
        "QPushButton:hover {"
        "background-color: #FF6347; "
        "}"
        );

    editGraph->setStyleSheet(
        "background-color: #7B68EE; "
        "color: white; "
        "font-weight: bold; "
        "padding: 5px; "
        "border: none; "
        "border-radius: 5px; "
        "transition: background-color 0.3s ease;"
        "}"
        "QPushButton:hover {"
        "background-color: #8A2BE2; "
        "}"
        );
    // Создаем скрытый контейнер для дополнительной информации
    QWidget *infoContainer = new QWidget(this);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoContainer);
    infoLayout->addWidget(deleteGraph);
    infoLayout->addWidget(editGraph);
    infoContainer->setLayout(infoLayout);

    buttonLayout->addWidget(button);
    buttonLayout->addWidget(infoContainer);

    buttonContainer->setObjectName("buttonContainer");
    buttonContainer->setStyleSheet("QWidget#buttonContainer { border: 2px solid #4CAF50; border-radius: 5px; padding: 10px; background-color: #f9f9f9; }");
    // Добавляем контейнер в основной лейаут, но скрываем его
    controlLayout->addWidget(buttonContainer);
    infoContainer->hide();
    buttonContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    // Анимация для раскрытия/сокрытия контейнера
    QPropertyAnimation *animation = new QPropertyAnimation(infoContainer, "minimumHeight");
    animation->setDuration(300);
    animation->setStartValue(0);
    animation->setEndValue(100);
    // При клике на текст "График" показываем/скрываем контейнер
    connect(button, &QPushButton::clicked, [infoContainer, animation]() {
        if (infoContainer->isVisible()) {

            infoContainer->hide();
        } else {
            infoContainer->show();

        }
    });
    connect(deleteGraph, &QPushButton::clicked, [this, buttonContainer, graphIndex]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Подтверждение удаления",
                                      "Вы уверены, что хотите удалить график?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            auto it = std::find_if(graphDataMap.begin(), graphDataMap.end(), [&graphIndex](const GraphData& g) {
                return g.id == graphIndex;
            });

            if (it != graphDataMap.end()) {
                plot->removeGraph(it->graph);  // Удаляем график из plot
                for (int i = 0; i < graphDataMap[graphIndex].shadingGraphs.size(); ++i) {//удаление штриховки под графиком
                    plot->removeGraph(graphDataMap[graphIndex].shadingGraphs[i].graph);
                }
                graphDataMap.erase(it);  // Удаляем график из списка
            }

            controlLayout->removeWidget(buttonContainer); // Убираем кнопку для этого графика
            buttonContainer->deleteLater(); // Удаляем саму кнопку
            plot->replot(); // Обновляем график
        } else {
            return;
        }

    });
    connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(onXAxisRangeChanged(QCPRange)));
    ShadingGraph shad1{ plot->addGraph(), -3 * sigmaInput->value() + meanInput->value(),  xMinInput->value()};
    ShadingGraph shad2{ plot->addGraph(), xMaxInput->value(),  3 * sigmaInput->value() + meanInput->value() };
    std::vector<ShadingGraph>graphs{ shad1 ,shad2};
    graphDataMap[graphIndex] = {graphIndex,
                                plot->xAxis->range().lower, 
                                plot->xAxis->range().upper,
                                meanInput->value(), 
                                sigmaInput->value(),
                                xMinInput->value(),
                                xMaxInput->value(),
                                plot->graph(graphIndex), 
                                graphs
    };
    buildGraphic(graphIndex);
    plot->replot();
}
