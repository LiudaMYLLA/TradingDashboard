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

    for(int i = 0; i < result.size(); ++i){
        Candle c = result[i];
        std::cout << c.data << "," <<
            c.open << "," <<
            c.high << "," <<
            c.low << "," <<
            c.close << "," <<
            c.volume << "\n";
    }

    for(int j = 0; j < loaderJSON.bids.size(); ++j){
        Order o = loaderJSON.bids[j];
        std::cout << "price: " << o.price <<
            " volume: " << o.volume << "\n";
    }

    for(int k = 0; k < loaderJSON.asks.size(); ++k){
        Order o = loaderJSON.asks[k];
        std::cout << "price: " << o.price <<
            " volume: " << o.volume << "\n";
    }

    return a.exec();
}
