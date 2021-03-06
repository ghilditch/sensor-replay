#ifndef CSVREADER_H
#define CSVREADER_H

#include <QList>
#include <QTextCodec>

struct ElementInfo
{
    bool isEnded;

    ElementInfo() : isEnded(true) {}
};


class CsvReader
{
public:
    CsvReader();

public:
    // Read csv-file and save it's data as strings to QList<QStringList>
    static QList<QStringList> readToList(const QString& filePath);

private:
    // Split string line to elements by separators
    static QStringList splitElements(const QString& line,
                                     const QString& separator,
                                     const QString& textDelimeter,
                                     ElementInfo& elemInfo);

    // Try to find end position of first or middle element
    static int FindMiddleElementPositioin(const QString& str,
                                          const int& startPos,
                                          const QString& separator,
                                          const QString& txtDelim);

    // Is current element is the last element
    static bool IsElementLast(const QString& str,
                              const int& startPos,
                              const QString& separator,
                              const QString& txtDelim);

    // Remove text delimeter symbols
    static QStringList removeTextDelimeters(const QStringList& elements,
                                            const QString& textDelimeter);
};

#endif // CSVREADER_H
