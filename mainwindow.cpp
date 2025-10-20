#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "dataloader.h"
#include "Candle.h"
#include "orderbook.h"
#include "dataloader.h"

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

    this->MA.init(this->candles);
}

void MainWindow::nextCharts(){
    if(this->current_index + this->amount_per_one < candles.size()){
        this->current_index = this->current_index + this->amount_per_one;
        UpdateChart();
    }
}

void MainWindow::prevCharts(){
    if(this->current_index >= this->amount_per_one){
        this->current_index = this->current_index - this->amount_per_one;
        UpdateChart();
    }
}

void MainWindow::signalMA(){
    if(candles.size() >= 50){
        if(this->signalsMA != nullptr){
            this->MA.getLastSignal();
            if(this->MA.lastSygnal == NONE){
                this->signalsMA->setText("No Signal!");
            }else if(this->MA.lastSygnal == BUY){
                this->signalsMA->setText("BUY");
            }else if(this->MA.lastSygnal == SELL){
                this->signalsMA->setText("SELL");
            }else{
                std::cout << "Here some problems" << "\n";
            }
        }
    }else{
        std::cout << "Not enough candles for analyses" << "\n";
        if(this->signalsMA != nullptr){
            this->signalsMA->setText("Candles not enough for analyse");
        }
    }
}

void MainWindow::UpdateChart(){
    this->series->clear();
    this->fastMASeries->clear();
    this->slowMASeries->clear();
    this->timeAxis->clear();

    for (int i = current_index;
        i < current_index + amount_per_one &&
        i < candles.size(); ++i){

        const Candle &candle = candles[i];
            // transform every candle to normall objectscandles[i];
        auto *set = new QtCharts::QCandlestickSet(
            candle.open,
            candle.high,
            candle.low,
            candle.close
            );
        //series saved all candle objects
        //QCandlestickSeries object
        this->series->append(set);

        QString candleTime = QString::fromStdString(candle.data);
        int candlePos = i - current_index;
        //              0 - 0   while 6 charts per page
        //              1 - 0
        //              2 - 0
        //               ...
        //              5 - 0
        //              6 - 0

        //              7 - 6 = 1
        //              8 - 6 = 2
        //              9 - 6 = 3
        //              10- 6 = 4
        //              11 -6 = 5
        //              12 -6 = 6

        for(const auto& entry: this->MA.MAPeriodFastHistory){
            if(QString::fromStdString(entry.first) == candleTime){
                //QLineSeries object    2p: x-pos on chart, y-MAMeaning
                this->fastMASeries->append(candlePos, entry.second);
                break;
            }
        }

        for (const auto &entry : this->MA.MAPeriodSlowHistory) {
            if (QString::fromStdString(entry.first) == candleTime) {
                this->slowMASeries->append(candlePos, entry.second);
                break;
            }
        }
    }

    QStringList categoriesC;
    for (int j = current_index;
        j < current_index + amount_per_one && j < candles.size();
        ++j){
        const Candle &c = candles[j];
        categoriesC << QString::fromStdString(c.data);
    }

    this->timeAxis->append(categoriesC);
    this->timeAxis->setRange(categoriesC.first(), categoriesC.last());

}

void MainWindow::InitUI(){
    // HERE IS EVERYTHING FOR CHARTS: START
    // Group of candles objects
    this->series = new QtCharts::QCandlestickSeries();
    this->series->setName("SERIES");
    this->series->setDecreasingColor(Qt::red);
    this->series->setIncreasingColor(Qt::green);

    // Building chart from series
    this->chart = new QtCharts::QChart();
    this->chart->addSeries(series);
    this->chart->setTitle("CHART");
    this->chart->setBackgroundBrush(QBrush(Qt::lightGray));

    // Working with 2 Axes
    this->priceAxis = new QtCharts::QValueAxis();
    this->timeAxis = new QtCharts::QBarCategoryAxis();
    this->priceAxis->setMin(80.0);
    this->priceAxis->setMax(200.0);
    this->priceAxis->setTitleText("PRICE");
    this->priceAxis->setTickCount(7);
    this->timeAxis->setTitleText("TIME");

    this->chart->addAxis(priceAxis, Qt::AlignLeft);
    this->chart->addAxis(timeAxis, Qt::AlignBottom);
    this->series->attachAxis(priceAxis);
    this->series->attachAxis(timeAxis);

    // MA strategy lines
    this->fastMASeries = new QtCharts::QLineSeries();
    this->slowMASeries = new QtCharts::QLineSeries();
    this->fastMASeries->setName("Fast MA");
    this->slowMASeries->setName("Slow MA");
    QPen fastPen(Qt::blue);
    fastPen.setWidth(3);
    this->fastMASeries->setPen(fastPen);
    QPen slowPen(Qt::darkYellow);
    slowPen.setWidth(3);
    this->slowMASeries->setPen(slowPen);

    this->chart->addSeries(this->fastMASeries);
    this->chart->addSeries(this->slowMASeries);

    this->fastMASeries->attachAxis(this->priceAxis);
    this->slowMASeries->attachAxis(this->priceAxis);
    this->fastMASeries->attachAxis(this->timeAxis);
    this->slowMASeries->attachAxis(this->timeAxis);

    // Building final chart
    this->chartView = new QtCharts::QChartView(chart);
    this->chartView->setDragMode(QGraphicsView::ScrollHandDrag);
    //chartView->setFixedSize(800,800);
    // HERE IS EVERYTHING FOR CHARTS: END

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

    QLabel *textMA = new QLabel();
    textMA->setText("Crossover Strategy");
    QPushButton *startAnalyse = new QPushButton("Start Analyse");
    startAnalyse->setFixedWidth(400);

    this->signalsMA = new QLabel();
    this->signalsMA->setText("Signals will appear here");
    this->signalsMA->setStyleSheet("font-weight: bold; color: blue");

    QVBoxLayout *mainContainer = new QVBoxLayout();
    QVBoxLayout *movingAverageStrategy = new QVBoxLayout();

    movingAverageStrategy->addWidget(textMA);
    movingAverageStrategy->addWidget(startAnalyse);
    movingAverageStrategy->addWidget(this->signalsMA);

    mainContainer->addLayout(chartsTablesLauout);
    mainContainer->addLayout(movingAverageStrategy);

    container->setLayout(mainContainer);
    setCentralWidget(container);

    // Connect signals to slots
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextCharts);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::prevCharts);
    connect(startAnalyse, &QPushButton::clicked, this, &MainWindow::signalMA);

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();
    this->MA.init(this->candles);
    this->MA.getLastSignal();

    LoaderDataToCharts();
    UpdateChart();

    // Testing Update
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




