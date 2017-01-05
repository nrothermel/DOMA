/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Level2Snapshot.h
 * Author: Niels Rothermel
 *
 * Created on 2. März 2016, 15:53
 */

#ifndef LEVEL2SNAPSHOT_H
#define LEVEL2SNAPSHOT_H

#include <QMap>
#include <utility>
#include <string>
#include <QDateTime>


class Level2Snapshot{
public:
    Level2Snapshot();
    Level2Snapshot(QTime timestamp);// time is set in contstructor and is also unique identifier
    Level2Snapshot(const Level2Snapshot& orig);
    virtual ~Level2Snapshot();
    QTime time;
    double time_double;
    //char should be Bid or Ask and int the Bookposition (0-4). double should be price and int size

    QMap<QPair<std::string,int>,QPair<int,double> > book;
    Level2Snapshot clone();
    std::string ask;
    std::string bid;
    bool bookentry_complete();
    double bid_ask_ratio;
    bool is_complete;
private:
    
};

#endif /* LEVEL2SNAPSHOT_H */

