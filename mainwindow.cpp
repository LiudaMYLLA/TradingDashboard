#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "dataloader.h"
#include "candle.h"
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
}

void MainWindow::reflectReturn(){
    // Good works with state massive with new Candles realize func update!
    std::vector<double> data = this->resultR;
    double last = data.back();

    double prevLastIndex = data.size() - 1;
    double prevLast = data[prevLastIndex];

    double prevPrevLastIndex = data.size() - 2;
    double prevPrevLast = data[prevPrevLastIndex];

    this->reflectR->setText("data for 3 last candles: " + QString::number(last)+ ", "+
        QString::number(prevLast)+ ", "+
        QString::number(prevPrevLast));
}

void MainWindow::reflectVolatility(){
    // Good works with state massive with new Candles realize func update!
    double data = this->resultV;
    this->reflectV->setText(QString::number(data));
    // Logic for adding the data to the metrix.txt
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
    this->CandlesSeries->clear();
    this->fastMASeries->clear();
    this->slowMASeries->clear();
    this->timeAxis->clear();

    for (int i = current_index;
        i < current_index + amount_per_one&&
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
        this->CandlesSeries->append(set);
    }

    for(int i = current_index; i < current_index + amount_per_one && i < candles.size(); ++i){
        QString candleTime = QString::fromStdString(candles[i].data);

        for(const auto& entry: this->MA.MAPeriodFastHistory){
            if(QString::fromStdString(entry.first) == candleTime){
                int candlePos = i - current_index;
                //QLineSeries object    2p: x-pos on chart, y-MAMeaning
                this->fastMASeries->append(candlePos, entry.second);
                break;
            }
        }
        for(const auto& entry: this->MA.MAPeriodSlowHistory){
            if(QString::fromStdString(entry.first) == candleTime){
                int candlePos = i - current_index;
                this->slowMASeries->append(candlePos, entry.second);
                break;
            }
        }
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
    }

    QStringList categoriesC;
    for (int j = current_index;
        j < current_index + amount_per_one&& j < candles.size();
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
    this->CandlesSeries = new QtCharts::QCandlestickSeries();
    this->CandlesSeries ->setName("Candles");
    this->CandlesSeries ->setDecreasingColor(Qt::red);
    this->CandlesSeries ->setIncreasingColor(Qt::green);

    // Building chart from series
    this->chart = new QtCharts::QChart();
    this->chart->addSeries(CandlesSeries);
    this->chart->setTitle("CHART");
    this->chart->setBackgroundBrush(QBrush(Qt::lightGray));

    // Working with 2 Axes
    this->priceAxis = new QtCharts::QValueAxis();
    this->timeAxis = new QtCharts::QBarCategoryAxis();

    double cLowMin = 0;
    double cHighMax = 0;
    for(int i = 0; i < candles.size(); ++i){
        const Candle& c = candles[i];
        double cHigh = c.high;
        double cLow = c.low;
        if(i == 0){
            cHighMax = cHigh;
            cLowMin = cLow;
        }else{
            if(cHigh > cHighMax) cHighMax = cHigh;
            if(cLow < cLowMin) cLowMin = cLow;
        }
    }
    this->priceAxis->setMin(cLowMin);
    this->priceAxis->setMax(cHighMax);
    this->priceAxis->setTitleText("PRICE");
    this->priceAxis->setTickCount(7);
    this->timeAxis->setTitleText("TIME");

    this->chart->addAxis(priceAxis, Qt::AlignLeft);
    this->chart->addAxis(timeAxis, Qt::AlignBottom);
    this->CandlesSeries->attachAxis(priceAxis);
    this->CandlesSeries->attachAxis(timeAxis);

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

    chartsButtonsLayout->addWidget(chartView);
    chartsButtonsLayout->addLayout(buttonLauout);

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

    QVBoxLayout *returnAndVolatilityLayout = new QVBoxLayout();

    QLabel *returnLabel = new QLabel();
    QLabel *volatilityLabel = new QLabel();
    returnLabel->setText("Return: ");
    volatilityLabel->setText("Volatility: ");
    this->reflectR = new QLabel();
    this->reflectV = new QLabel();

    QHBoxLayout* volLayout = new QHBoxLayout();
    volLayout->addWidget(volatilityLabel);
    volLayout->addWidget(this->reflectV);
    volLayout->setAlignment(Qt::AlignLeft);

    QHBoxLayout* retLayout = new QHBoxLayout();
    retLayout->addWidget(returnLabel);
    retLayout->addWidget(this->reflectR);
    retLayout->setAlignment(Qt::AlignLeft);

    QPushButton *calculateReturn = new QPushButton("Calculate Return");
    calculateReturn->setFixedWidth(400);
    QPushButton *calculateVolatility = new QPushButton("Calculate Volatility");
    calculateVolatility->setFixedWidth(400);

    QHBoxLayout *buttonsRV = new QHBoxLayout();

    buttonsRV->addWidget(calculateReturn);
    buttonsRV->addWidget(calculateVolatility);
    buttonsRV->setAlignment(Qt::AlignLeft);
    buttonsRV->setContentsMargins(0, 0, 0, 0);

    QWidget *buttonsRVContainer = new QWidget();
    buttonsRVContainer->setLayout(buttonsRV);
    buttonsRVContainer->setFixedWidth(900);

    returnAndVolatilityLayout->addLayout(retLayout);
    returnAndVolatilityLayout->addLayout(volLayout);
    returnAndVolatilityLayout->addWidget(buttonsRVContainer, 0, Qt::AlignLeft);

    mainContainer->addLayout(chartsTablesLauout);
    mainContainer->addLayout(movingAverageStrategy);
    mainContainer->addLayout(returnAndVolatilityLayout);

    container->setLayout(mainContainer);
    setCentralWidget(container);

    // Connect signals to slots
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextCharts);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::prevCharts);
    connect(startAnalyse, &QPushButton::clicked, this, &MainWindow::signalMA);
    connect(calculateReturn, &QPushButton::clicked, this, &MainWindow::reflectReturn);
    connect(calculateVolatility, &QPushButton::clicked, this, &MainWindow::reflectVolatility);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LoaderDataToCharts();
    InitUI();
    this->MA.init(this->candles);
    UpdateChart();

    this->resultR = this->vol.returnCalculation(this->candles);
    this->resultV = this->vol.calculateVolatility();
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




