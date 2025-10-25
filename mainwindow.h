#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "candle.h"
#include "strategyma.h"
#include "volatilityCalc.h"
#include "metricslogger.h"

#include <QMainWindow>
#include <vector>

#include <QtCharts/QCandlestickSet>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>

#include <QLabel>

#include <QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void InitUI();
    void UpdateChart();
    void LoaderDataToCharts();
    void nextCharts();
    void prevCharts();
    void signalMA();
    void reflectReturn();
    void reflectVolatility();
    ~MainWindow();
private:
    Ui::MainWindow *ui;

    std::vector<Candle> candles;
    strategyMA MA;
    volatilityCalc vol;
    metricsLogger metricsL;

    int current_index = 0;
    int amount_per_one = 7;

    double resultV = 0;
    std::vector<double> resultR;
private:
    QtCharts::QCandlestickSeries* CandlesSeries = nullptr;
    QtCharts::QChart* chart = nullptr;
    QtCharts::QValueAxis* priceAxis = nullptr;
    QtCharts::QBarCategoryAxis* timeAxis = nullptr;
    QtCharts::QChartView* chartView = nullptr;

    QLabel* signalsMA = nullptr;
    QLabel* reflectR = nullptr;
    QLabel* reflectV = nullptr;

    QtCharts::QLineSeries* fastMASeries = nullptr;
    QtCharts::QLineSeries* slowMASeries = nullptr;
};
#endif // MAINWINDOW_H
