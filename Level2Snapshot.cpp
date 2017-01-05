/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Level2Snapshot.cpp
 * Author: Niels Rothermel
 * 
 * Created on 2. März 2016, 15:53
 */

#include "Level2Snapshot.h"

Level2Snapshot::Level2Snapshot(){
    ask = "Ask";
    bid = "Bid";
    for(int i = 0; i <= 4; i++){
        book[qMakePair(ask,i)].first = 0;
        book[qMakePair(ask,i)].second = 0;
        book[qMakePair(bid,i)].first = 0;
        book[qMakePair(bid,i)].second = 0;
    }
}

Level2Snapshot::Level2Snapshot(QTime timestamp) : ask("Ask"), bid("Bid"){
    time = timestamp;

    for(int i = 0; i <= 4; i++){
        book[qMakePair(ask,i)].first = 0;
        book[qMakePair(ask,i)].second = 0;
        book[qMakePair(bid,i)].first = 0;
        book[qMakePair(bid,i)].second = 0;
    }
}

Level2Snapshot::Level2Snapshot(const Level2Snapshot& orig) {
}

Level2Snapshot::~Level2Snapshot() {
}


Level2Snapshot Level2Snapshot::clone(){
    Level2Snapshot snaptoClone;
    snaptoClone.time = this->time;
    snaptoClone.book = this->book;
    return snaptoClone;
}

bool Level2Snapshot::bookentry_complete(){
    is_complete = true;
    for(int i = 0; i <= 4; i++){
       if(book[qMakePair(ask,i)].second == 0) is_complete = false;
       if(book[qMakePair(bid,i)].second == 0) is_complete = false;
       if(is_complete == false) break;
    }
    return is_complete;
}
