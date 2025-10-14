#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "dataloader.h"
#include "Candle.h"
#include "orderbook.h"
#include "dataloader.h"

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
#include <QTableWidget>
#include <QLabel>

void MainWindow::LoaderDataToCharts(){
    DataLoader loader("/home/liudmyla-rybak/TradingDashboard/data.csv");
    this->candles = loader.loadFromCSV();

    if(this->candles.empty()){
        return;
    }

    qDebug() << "Total candles loaded:" << candles.size();
    this->MA.init(this->candles);
}

void MainWindow::nextCharts(){
    if(this->current_index + this->amount_per_one < candles.size()){
        this->current_index = this->current_index + this->amount_per_one;
        ChartsReflection();
    }
}

void MainWindow::prevCharts(){
    if(this->current_index - this->amount_per_one < candles.size()){
        this->current_index = this->current_index - this->amount_per_one;
        ChartsReflection();
    }
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
    QVBoxLayout *chartsButtonsLayout = new QVBoxLayout();

    QPushButton *nextButton = new QPushButton("Next");
    QPushButton *prevButton = new QPushButton("Previous");
    QHBoxLayout *buttonLauout = new QHBoxLayout();
    buttonLauout->addWidget(prevButton);
    buttonLauout->addWidget(nextButton);

    chartsButtonsLayout->addLayout(buttonLauout);
    chartsButtonsLayout->addWidget(chartView);

    QTableWidget *bidsTable = new QTableWidget();
    QTableWidget *asksTable = new QTableWidget();

    QVBoxLayout *bidsTitleTableHolder = new QVBoxLayout();
    QVBoxLayout *asksTitleTableHolder = new QVBoxLayout();
    QLabel *titelBids = new QLabel();
    QLabel *titleAsks = new QLabel();
    titelBids->setText("Bids");
    titleAsks->setText("Asks");
    bidsTitleTableHolder->addWidget(titelBids);
    bidsTitleTableHolder->addWidget(bidsTable);
    asksTitleTableHolder->addWidget(titleAsks);
    asksTitleTableHolder->addWidget(asksTable);

    Orderbook loaderJSON("/home/liudmyla-rybak/TradingDashboard/orderbook.json");
    loaderJSON.loadFromJSON();

    bidsTable->setColumnCount(2);
    bidsTable->setRowCount(loaderJSON.bids.size());
    asksTable->setColumnCount(2);
    asksTable->setRowCount(loaderJSON.asks.size());
    // Syntax: QTableWidget::setItem(int row, int column, QTableWidgetItem *item)
    for(int i = 0; i < loaderJSON.bids.size(); ++i){
        QString PriceString = QString::number(loaderJSON.bids[i].price);
        QString VolumeString = QString::number(loaderJSON.bids[i].volume);
        bidsTable->setItem(i, 0, new QTableWidgetItem(PriceString));
        bidsTable->setItem(i, 1, new QTableWidgetItem(VolumeString));
    }
    for(int j = 0; j < loaderJSON.asks.size(); ++j){
        asksTable->setItem(j, 0, new QTableWidgetItem(QString::number(loaderJSON.asks[j].price)));
        asksTable->setItem(j, 1, new QTableWidgetItem(QString::number(loaderJSON.asks[j].volume)));
    }

    QHBoxLayout *tables = new QHBoxLayout();
    tables->addLayout(bidsTitleTableHolder);
    tables->addLayout(asksTitleTableHolder);


    QHBoxLayout *chartsTablesLauout = new QHBoxLayout();
    chartsTablesLauout->addLayout(chartsButtonsLayout);
    chartsTablesLauout->addLayout(tables);

    container->setLayout(chartsTablesLauout);
    setCentralWidget(container);

    // Connect signals to slots
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextCharts);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::prevCharts);

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

//                  SIGNALS AND SLOTS
// connect() is a Qt function that links a signal (like a button click) to a slot (a function that runs when that signal happens).
// The general syntax (modern Qt5/Qt6 form)
// connect(sender, &SenderType::signal, receiver, &ReceiverType::slot);




