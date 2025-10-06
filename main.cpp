#include "mainwindow.h"
#include "dataloader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DataLoader loader("/home/liudmyla-rybak/TradingDashboard/data.csv");

    std::vector<Candle> result = loader.loadFromCSV();

    for(int i = 0; i < result.size(); i++){
        Candle c = result[i];
        std::cout << c.data << "," <<
            c.open << "," <<
            c.high << "," <<
            c.low << "," <<
            c.close << "," <<
            c.volume << "\n";
    }

    return a.exec();
}
