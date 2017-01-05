#ifndef ANALYSIS_FUNCTIONS_H
#define ANALYSIS_FUNCTIONS_H

#include <Level2Snapshot.h>
#include <QVector>
#include <QDebug>

inline void calc_bid_ask_ratio(QVector<Level2Snapshot> &bookentries ){

    //checking for incomplete Level2Snaptshots and ignoring them
    int i = 0; //incrementer
    while(!bookentries[i].is_complete){
        bookentries[i].bid_ask_ratio = 0;
        i++;
    }

    for(; i < bookentries.size(); i++){
        double bidsum = 0;
        bidsum += bookentries[i].book[qMakePair(bookentries[i].bid,0)].first;
        bidsum += bookentries[i].book[qMakePair(bookentries[i].bid,1)].first;
        bidsum += bookentries[i].book[qMakePair(bookentries[i].bid,2)].first;
        bidsum += bookentries[i].book[qMakePair(bookentries[i].bid,3)].first;
        bidsum += bookentries[i].book[qMakePair(bookentries[i].bid,4)].first;
        double asksum = 0;
        asksum += bookentries[i].book[qMakePair(bookentries[i].ask,0)].first;
        asksum += bookentries[i].book[qMakePair(bookentries[i].ask,1)].first;
        asksum += bookentries[i].book[qMakePair(bookentries[i].ask,2)].first;
        asksum += bookentries[i].book[qMakePair(bookentries[i].ask,3)].first;
        asksum += bookentries[i].book[qMakePair(bookentries[i].ask,4)].first;
        bookentries[i].bid_ask_ratio = bidsum/asksum;
        qDebug() << "Bid Ask ratio was calculated " << bookentries[i].bid_ask_ratio;
    }
}

#endif // ANALYSIS_FUNCTIONS_H
