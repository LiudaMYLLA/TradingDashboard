#ifndef METRICSLOGGER_H
#define METRICSLOGGER_H

#include <QString>
class metricsLogger
{
    QString filename;
public:
    metricsLogger(const QString& filename = "metrics.txt");
    void logVolatility();
    void logAverageReturn();
};

#endif // METRICSLOGGER_H
