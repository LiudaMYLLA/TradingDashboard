#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Candle.h"
#include "strategyma.h"

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
    ~MainWindow();
private:
    Ui::MainWindow *ui;

    std::vector<Candle> candles;
    strategyMA MA;

    int current_index = 0;
    int amount_per_one = 8;
private:
    QtCharts::QCandlestickSeries* series = nullptr;
    QtCharts::QChart* chart = nullptr;
    QtCharts::QValueAxis* priceAxis = nullptr;
    QtCharts::QBarCategoryAxis* timeAxis = nullptr;
    QtCharts::QChartView* chartView = nullptr;

    QLabel *signalsMA = nullptr;

    QtCharts::QLineSeries* fastMASeries = nullptr;
    QtCharts::QLineSeries* slowMASeries = nullptr;
};
#endif // MAINWINDOW_H
