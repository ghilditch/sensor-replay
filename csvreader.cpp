#include "csvreader.h"
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

const QString CR("\r");
const QString LF("\n");

CsvReader::CsvReader()
{

}

QList<QStringList> CsvReader::readToList(const QString& filePath,
                                      const QString& separator,
                                      const QString& textDelimeter,
                                      QTextCodec* codec)
{
    QList<QStringList> data;
    CsvReader::read(filePath, data, separator, textDelimeter, codec);

    return data;
}

// Function that really reads csv-file and save it's data as strings to
// QList<QStringList>
// @input:
// - filePath - string with absolute path to csv-file
// - list - refernce to list container where read data will be saved
// - separator - string or character that separate values in a row
// - textDelimeter - string or character that enclose each element in a row
// - codec - pointer to codec object that would be used for file reading
// @output:
// - QList<QStringList> - list of values (as strings) from csv-file. If case of
// error will return empty QList<QStringList>.
bool CsvReader::read(const QString& filePath,
                            QList<QStringList>& list,
                            const QString& separator,
                            const QString& textDelimeter,
                            QTextCodec* codec)
{
    QFile csvFile(filePath);
    if ( false == csvFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        qDebug() << __FUNCTION__  << "Error - can't open file:" << filePath;
        return false;
    }

    QTextStream stream(&csvFile);
    stream.setCodec(codec);

    // This list will contain elements of the row if elements of this row
    // are located on several lines
    QStringList row;

    ElementInfo elemInfo;
    while ( false == stream.atEnd() )
    {
        QString line = stream.readLine();
        QStringList elements = CsvReader::splitElements(
                  line, separator, textDelimeter, elemInfo);
        if (elemInfo.isEnded)
        {
            // This elements ending in the current line.
            // Check if this elements are end elements of the long row
            if (row.isEmpty())
            {
                // No, this elements constitute the entire row
                list << elements;
            }
            else
            {
                // Yes, this elements should be added to the row
                if (false == elements.isEmpty())
                {
                    row.last().append(elements.takeFirst());
                    row << elements;
                }

                list << row;
                row.clear();
            }
        }
        else
        {
            // This elements constitute long row that on several lines
            if (false == elements.isEmpty())
            {
                if (false == row.isEmpty())
                {
                    row.last().append(elements.takeFirst());
                }

                row << elements;
            }
        }
    }

    csvFile.close();

    if (false == elemInfo.isEnded && false == row.isEmpty())
    {
        list << row;
    }

    return true;
}

// Split string line to elements by separators
// @input:
// - line - string with data
// - separator - string or character that separate values in a row
// - textDelimeter - string that used as text delimeter
// @output:
// - QStringList - list of elements
QStringList CsvReader::splitElements(const QString& line,
                                            const QString& separator,
                                            const QString& textDelimeter,
                                            ElementInfo& elemInfo)
{
    // If separator is empty, return whole line. Can't work in this
    // conditions!
    if (separator.isEmpty())
    {
        elemInfo.isEnded = true;
        return (QStringList() << line);
    }

    if (line.isEmpty())
    {
        // If previous row was ended, then return empty QStringList.
        // Otherwise return list that contains one element - new line symbols
        if (elemInfo.isEnded)
        {
            return QStringList();
        }
        else
        {
            return (QStringList() << LF);
        }
    }

    QStringList result;
    int pos = 0;
    while(pos < line.size())
    {
        if (elemInfo.isEnded)
        {
            // This line is a new line, not a continuation of the previous
            // line.
            // Check if element starts with the delimeter symbol
            int delimeterPos = line.indexOf(textDelimeter, pos);
            if (delimeterPos == pos)
            {
                pos = delimeterPos + textDelimeter.size();

                // Element starts with the delimeter symbol. It means that
                // this element could contain any number of double
                // delimeters and separator symbols. This element could:
                // 1. Be the first or the middle element. Then it should end
                // with delimeter and the seprator symbols standing next to each
                // other.
                int midElemEndPos = FindMiddleElementPositioin(
                                        line, pos, separator, textDelimeter);
                if (midElemEndPos > 0)
                {
                    int length = midElemEndPos - pos;
                    result << line.mid(pos, length);
                    pos = midElemEndPos +
                          textDelimeter.size() + separator.size();
                    continue;
                }

                // 2. Be The last element on the line. Then it should end with
                // delimeter symbol.
                if (IsElementLast(line, pos, separator, textDelimeter))
                {
                    int length = line.size() - textDelimeter.size() - pos;
                    result << line.mid(pos, length);
                    break;
                }

                // 3. Not ends on this line
                int length = line.size() - pos;
                result << line.mid(pos, length);
                elemInfo.isEnded = false;
                break;
            }
            else
            {
                // Element do not starts with the delimeter symbol. It means
                // that this element do not contain double delimeters and it
                // ends at the next separator symbol.
                // Check if line contains separator symbol.
                int separatorPos = line.indexOf(separator, pos);
                if (separatorPos >= 0 )
                {
                    // If line contains separator symbol, then our element
                    // located between current position and separator
                    // position. Copy it into result list and move
                    // current position over the separator position.
                    result << line.mid(pos, separatorPos - pos);
                    pos = separatorPos + separator.size();
                }
                else
                {
                    // If line do not contains separator symbol, then
                    // this element ends at the end of the string.
                    // Copy it into result list and exit the loop.
                    result << line.mid(pos);
                    break;
                }
            }
        }
        else
        {
            // This line is a continuation of the previous. Last element of the
            // previous line did not end. It started with delimeter symbol.
            // It means that this element could contain any number of double
            // delimeters and separator symbols. This element could:
            // 1. End somewhere in the middle of the line. Then it should end
            // with delimeter and the seprator symbols standing next to each
            // other.
            int midElemEndPos = FindMiddleElementPositioin(
                                line, pos, separator, textDelimeter);
            if (midElemEndPos > 0)
            {
                result << (LF + line.mid(pos, midElemEndPos - pos));
                pos = midElemEndPos + textDelimeter.size() + separator.size();
                elemInfo.isEnded = true;
                continue;
            }

            // 2. End at the end of the line. Then it should end with
            // delimeter symbol.
            if (IsElementLast(line, pos, separator, textDelimeter))
            {
                int length = line.size() - textDelimeter.size() - pos;
                result << (LF + line.mid(pos, length));
                elemInfo.isEnded = true;
                break;
            }

            // 3. Not ends on this line
            result << (LF + line);
            break;
        }
    }

    return removeTextDelimeters(result, textDelimeter);
}

// Try to find end position of first or middle element
// @input:
// - str - string with data
// - startPos - start position of the current element in the string
// - separator - string or character that separate values in a row
// - textDelimeter - string that used as text delimeter
// @output:
// - int - end position of the element or -1 if this element is not first
// or middle
int CsvReader::FindMiddleElementPositioin(const QString& str,
                                              const int& startPos,
                                              const QString& separator,
                                              const QString& txtDelim)
{
    const int ERROR = -1;
    if (str.isEmpty() ||
            startPos < 0 ||
            separator.isEmpty() ||
            txtDelim.isEmpty())
    {
        return ERROR;
    }

    const QString elemEndSymbols = txtDelim + separator;
    int elemEndPos = startPos;
    while(elemEndPos < str.size())
    {
        // Find position of element end symbol
        elemEndPos = str.indexOf(elemEndSymbols, elemEndPos);
        if (elemEndPos < 0)
        {
            // This element could not be the middle element, becaise string
            // do not contains any end symbols
            return ERROR;
        }

        // Check that this is really the end symbols of the
        // element and we don't mix up it with double delimeter
        // and separator. Calc number of delimeter symbols from elemEndPos
        // to startPos that stands together.
        int numOfDelimeters = 0;
        for (int pos = elemEndPos; startPos <= pos; --pos, ++numOfDelimeters)
        {
            QStringRef strRef = str.midRef(pos, txtDelim.size());
            if (QStringRef::compare(strRef, txtDelim) != 0)
            {
                break;
            }
        }

        // If we have odd number of delimeter symbols that stand together,
        // then this is the even number of double delimeter symbols + last
        // delimeter symbol. That means that we have found end position of
        // the middle element.
        if (numOfDelimeters % 2 == 1)
        {
            return elemEndPos;
        }
        else
        {
            // Otherwise this is not the end of the middle element and we
            // should try again
            elemEndPos += elemEndSymbols.size();
        }
    }

    return ERROR;
}

// Is current element is the last element
// @input:
// - str - string with data
// - startPos - start position of the current element in the string
// - separator - string or character that separate values in a row
// - textDelimeter - string that used as text delimeter
// @output:
// - bool - True if the current element is the last element of the string,
// False otherwise
bool CsvReader::IsElementLast(const QString& str,
                                  const int& startPos,
                                  const QString& separator,
                                  const QString& txtDelim)
{
    if (str.isEmpty() ||
            startPos < 0 ||
            separator.isEmpty() ||
            txtDelim.isEmpty())
    {
        return false;
    }

    // Check if string ends with text delimeter. If not, then this element
    // do not ends on this line
    if (false == str.endsWith(txtDelim))
    {
        return false;
    }

    // Check that this is really the end symbols of the
    // element and we don't mix up it with double delimeter.
    // Calc number of delimeter symbols from end
    // to startPos that stands together.
    int numOfDelimeters = 0;
    for (int pos = str.size() - 1; startPos <= pos; --pos, ++numOfDelimeters)
    {
        QStringRef strRef = str.midRef(pos, txtDelim.size());
        if (QStringRef::compare(strRef, txtDelim) != 0)
        {
            break;
        }
    }

    // If we have odd number of delimeter symbols that stand together,
    // then this is the even number of double delimeter symbols + last
    // delimeter symbol. That means that this element is the last on the line.
    if (numOfDelimeters % 2 == 1)
    {
        return true;
    }

    return false;
}

// Remove text delimeter symbols
// @input:
// - elements - list of string elements
// - textDelimeter - string that delimeter text parts from each other
// @output:
// - QStringList - list of elements
QStringList CsvReader::removeTextDelimeters(const QStringList& elements,
                                                const QString& textDelimeter)
{
    if (elements.isEmpty() || textDelimeter.isEmpty())
    {
        return elements;
    }

    QStringList result;
    const QString doubleTextDelim = textDelimeter + textDelimeter;
    for (int i = 0; i < elements.size(); ++i)
    {
        QString str = elements.at(i);
        if (str.startsWith(textDelimeter))
        {
            str.remove(0, textDelimeter.size());
        }

        if (str.endsWith(textDelimeter))
        {
            str.chop(textDelimeter.size());
        }

        // Also replace double text delimiter with one text delimiter symbol
        str.replace(doubleTextDelim, textDelimeter);
        result << str;
    }

    return result;
}
