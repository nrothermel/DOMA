#ifndef MYSQL_FUNCTIONS_H
#define MYSQL_FUNCTIONS_H

#include <QtSql>
#include <Level2Snapshot.h>

inline void create_price_chart(QString table, QVector<double> &price_vec, QVector<QTime> &time){
    QString use_databse = "SELECT Time,LastVolume,LastPrice FROM "+ table +" WHERE LastVolume >0 OR LastPrice >0 ORDER BY Time ASC";
    QSqlQuery query;
    QVector<QString> time_string; // because query cant get QDatetime type with subseconds
    query.exec(use_databse);

    while (query.next()){
            price_vec.push_back(query.value(2).toDouble());
    }
    use_databse = "SELECT DATE_FORMAT(time, '%Y-%m-%d %H:%i:%s.%f') AS time FROM " + table + " WHERE LastVolume >0 OR LastPrice >0 ORDER BY Time ASC";
    query.exec(use_databse);
    while (query.next()){
       time_string.push_back(query.value(0).toString());
       time_string.last().remove(0,11);//remove Date so that QTime can be used
       time_string.last().remove(12,14);//remove values <msec
       time.push_back(QTime::fromString(time_string.last(),"hh:mm:ss.zzz"));
    }
    //repairing zero price values
    while(price_vec.first() == 0){
       price_vec.remove(price_vec.first());
       time.remove(price_vec.first());
    }

    for(int i = 0; i < price_vec.size(); i++){
        if(price_vec.at(i) == 0){
            price_vec.replace(i,price_vec.at((i-1)));
        }
//        qDebug() << price_vec.at(i);
    }
}

inline QVector<Level2Snapshot> getBookentries(QString table){
    QString use_databse = "SELECT Time,Side,BookPosition,Size,Price FROM " + table + " WHERE Side = 'Bid' OR side = 'Ask' ORDER BY Time ASC";
    QSqlQuery query;
    query.exec(use_databse);
    //Vectors for intermediate saving
    QVector<QString> time_string; // because query cant get QDatetime type with subseconds
    QVector<QTime> time;
    QVector<QString> side;
    QVector<int> position;
    QVector<int> size;
    QVector<double> price;

    while (query.next()){
        side.push_back(query.value(1).toString());
        position.push_back(query.value(2).toInt());
        size.push_back(query.value(3).toInt());
        price.push_back(query.value(4).toDouble());
    }

    use_databse = "SELECT DATE_FORMAT(time, '%Y-%m-%d %H:%i:%s.%f') AS time FROM " + table + " WHERE Side = 'Bid' OR Side = 'Ask' ORDER BY Time ASC";
    query.exec(use_databse);
    while (query.next()){
       time_string.push_back(query.value(0).toString());
       time_string.last().remove(0,11);//remove Date so that QTime can be used
       time_string.last().remove(12,14);//remove values <msec
       time.push_back(QTime::fromString(time_string.last(),"hh:mm:ss.zzz"));
    }
    QVector<Level2Snapshot> bookentries(time.size());
    //filling the bookentries Vector
    bookentries[0].time = time.at(0);
    bookentries[0].book.insert(qMakePair(side.at(0).toStdString(),position.at(0)),qMakePair(size.at(0),price.at(0)));
    for(int i = 1; i < time.size(); i++){
        qDebug();
        bookentries[i] = bookentries[i-1];
        bookentries[i].time = time.at(i);
        //adding time in double format
        bookentries[i].time_double = time.at(i).msecsSinceStartOfDay();
        bookentries[i].time_double = bookentries[i].time_double/1000;
        bookentries[i].book.insert(qMakePair(side.at(i).toStdString(),position.at(i)),qMakePair(size.at(i),price.at(i)));
        bookentries[i].bookentry_complete();
    }
    return bookentries;
}

#endif // MYSQL_FUNCTIONS_H
