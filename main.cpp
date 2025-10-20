#include "mainwindow.h"
#include "dataloader.h"
#include "orderbook.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DataLoader loader("/home/liudmyla-rybak/TradingDashboard/data.csv");
    Orderbook loaderJSON("/home/liudmyla-rybak/TradingDashboard/orderbook.json");


    std::vector<Candle> result = loader.loadFromCSV();
    loaderJSON.loadFromJSON();

    return a.exec();
}
