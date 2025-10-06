#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "dataloader.h"
#include "Candle.h"

#include <QtCharts/QCandlestickSet>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QBarCategoryAxis>

#include <QDateTime>
#include <QDebug>
#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

void MainWindow::LoaderDataToCharts(){
    DataLoader loader("/home/liudmyla-rybak/TradingDashboard/data.csv");
    this->candles = loader.loadFromCSV();

    if(this->candles.empty()){
        return;
    }
    qDebug() << "Total candles loaded:" << candles.size();
}

void MainWindow::nextCharts(){
    this->current_index = this->current_index + this->amount_per_one;
}

void MainWindow::prevCharts(){
    this->current_index = this->current_index - this->amount_per_one;
}

void MainWindow::ChartsReflection(){

    // Group of candles objects
    auto *series = new QtCharts::QCandlestickSeries();
    series->setName("here is a title for series");
    series->setDecreasingColor(Qt::red);
    series->setIncreasingColor(Qt::green);


    for (int i = current_index; i < current_index + amount_per_one && i < candles.size(); ++i){
        const Candle &candle = candles[i];
            // transform every candle to normall objectscandles[i];
        auto *set = new QtCharts::QCandlestickSet(
            candle.open,
            candle.high,
            candle.low,
            candle.close
            );

        //series saved all candle objects
        series->append(set);
    }

    // Building chart from series
    auto *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Here is a title for chart");
    chart->setBackgroundBrush(QBrush(Qt::lightGray));
    chart->setTitleBrush(QBrush(Qt::darkMagenta));

    // Working with 2 Axes
    auto *priceAxis = new QtCharts::QValueAxis();
    auto *timeAxis = new QtCharts::QBarCategoryAxis();

    priceAxis->setMin(0.0);
    priceAxis->setMax(300.0);
    priceAxis->setTitleText("Here is a title for price");
    priceAxis->setTickCount(7);

    timeAxis->setTitleText("Here is a title for time");
    QStringList categories;
    for (int j = current_index; j < current_index + amount_per_one && j < candles.size(); ++j){
        const Candle &c = candles[j];
        categories << QString::fromStdString(c.data);
    }

    timeAxis->append(categories);
    timeAxis->setRange(categories.first(), categories.last());

    chart->addAxis(priceAxis, Qt::AlignLeft);
    chart->addAxis(timeAxis, Qt::AlignBottom);
    series->attachAxis(priceAxis);
    series->attachAxis(timeAxis);
    //series->clear();

    // Building final chart
    auto *chartView = new QtCharts::QChartView(chart);
    //setCentralWidget(chartView);
    chartView->setDragMode(QGraphicsView::ScrollHandDrag);
    //chartView->setFixedSize(800,800);

    // Work with docks
    //auto *dock = new QDockWidget("Charts", this);
    //dock->setWidget(chartView);
    //addDockWidget(Qt::RightDockWidgetArea, dock);

    // Working with main window looking
    QWidget *container = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QPushButton *nextButton = new QPushButton("Next");
    QPushButton *prevButton = new QPushButton("Previous");
    QHBoxLayout *buttonLauout = new QHBoxLayout();
    buttonLauout->addWidget(prevButton);
    buttonLauout->addWidget(nextButton);

    mainLayout->addLayout(buttonLauout);
    mainLayout->addWidget(chartView);

    container->setLayout(mainLayout);
    setCentralWidget(container);

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LoaderDataToCharts();
    ChartsReflection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Working with module: QtCharts
// Candle → QCandlestickSet → QCandlestickSeries → QChart → QChartView

// Structure of app
// MainWindow
//  └── QWidget container
//       ├── Buttons (layout)
//       └── Chart

// | Layout type   | Arranges widgets              |
// | ------------- | ----------------------------- |
// | `QVBoxLayout` | Vertically (top → bottom)     |
// | `QHBoxLayout` | Horizontally (left → right)   |
// | `QGridLayout` | Like a table (rows & columns) |
// | `QFormLayout` | Label–input pairs (for forms) |



