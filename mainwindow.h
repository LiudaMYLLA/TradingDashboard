#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "Candle.h"

#include <QChartView>

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
    void ChartsReflection();
    void LoaderDataToCharts();
    void nextCharts();
    void prevCharts();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::vector<Candle> candles;
    int current_index = 0;
    int amount_per_one = 6;
};
#endif // MAINWINDOW_H
