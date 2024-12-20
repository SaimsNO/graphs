#include "graphinfo.h"

GraphInfoDialog::GraphInfoDialog(GraphData& data, QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(RU("Информация о графике"));

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* suitparts = new QLabel(RU("Процент годных деталей"), this);
    layout->addWidget(suitparts);
    QLineEdit* suitpartsedit = new QLineEdit();
    layout->addWidget(suitpartsedit);
    double suitpartvalue = (Phi((data.es - data.mean) / data.sigma) - Phi((data.ei - data.mean) / data.sigma)) * 100;
    suitpartsedit->setText(QString::number(suitpartvalue, 'f', 4));
    suitpartsedit->setReadOnly(true);

    QLabel* incormar = new QLabel(RU("Процент неисправимого брака"), this);
    layout->addWidget(incormar);
    QLineEdit* incormaredit = new QLineEdit();
    layout->addWidget(incormaredit);
    double incormarvalue = (Phi((data.ei - data.mean) / data.sigma) - Phi((data.mean - 3 * data.sigma - data.mean) / data.sigma)) * 100;
    incormaredit->setText(QString::number(incormarvalue, 'f', 4));
    incormaredit->setReadOnly(true);

    QLabel* cormar = new QLabel(RU("Процент исправимого брака"), this);
    layout->addWidget(cormar);
    QLineEdit* cormaredit = new QLineEdit();
    layout->addWidget(cormaredit);
    double cormarvalue = (Phi((data.mean + 3 * data.sigma - data.mean) / data.sigma) - Phi((data.es - data.mean) / data.sigma)) * 100;
    cormaredit->setText(QString::number(cormarvalue, 'f', 4));
    cormaredit->setReadOnly(true);

    QPushButton* closeButton = new QPushButton(RU("Закрыть"), this);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    setLayout(layout);
}

GraphEditDialog::GraphEditDialog(GraphData& data, QWidget* parent) 
    : QDialog(parent) 
{
    QLabel* meanLabel = new QLabel("Mean (x'):", this);
    meanInput = new QDoubleSpinBox(this);
    meanInput->setDecimals(5);
    meanInput->setRange(-10.0, 10.0);
    meanInput->setValue(data.mean);

    QLabel* sigmaLabel = new QLabel(RU("Sigma (σ):"), this);
    sigmaInput = new QDoubleSpinBox(this);
    sigmaInput->setDecimals(5);
    sigmaInput->setRange(0.001, 10.0);
    sigmaInput->setValue(data.sigma);

    QLabel*  xMinLabel = new QLabel("ei:", this);
    xMinInput = new QDoubleSpinBox(this);
    xMinInput->setDecimals(5);
    xMinInput->setRange(-10.0, 10.0);
    xMinInput->setValue(data.ei);

    QLabel* xMaxLabel = new QLabel("es:", this);
    xMaxInput = new QDoubleSpinBox(this);
    xMaxInput->setDecimals(5);
    xMaxInput->setRange(-10.0, 10.0);
    xMaxInput->setValue(data.es);

    QPushButton* okBtn = new QPushButton("OK");
    QPushButton* cancelBtn = new QPushButton(RU("Отмена"));
    QFormLayout* formLayout = new QFormLayout(this);
    formLayout->addRow(meanLabel, meanInput);
    formLayout->addRow(sigmaLabel, sigmaInput);
    formLayout->addRow(xMinLabel, xMinInput);
    formLayout->addRow(xMaxLabel, xMaxInput);
    formLayout->addRow(okBtn, cancelBtn);
    connect(okBtn, &QPushButton::clicked, this, [this, data]() mutable{
        onOkClicked(data);
        });
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void GraphEditDialog::onOkClicked(GraphData& data) {
    data.sigma = sigmaInput->value();
    data.mean = meanInput->value();
    data.ei = xMinInput->value();
    data.es = xMaxInput->value();
    accept();
}