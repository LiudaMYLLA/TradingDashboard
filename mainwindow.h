#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Candle.h"
#include "strategyma.h"

#include <QMainWindow>
#include <vector>

#include <QChartView>
#include <QLabel>

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
    int amount_per_one = 6;

    QLabel *signalsMA = nullptr;
};
#endif // MAINWINDOW_H
