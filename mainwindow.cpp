#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL");
    readSettings();
    db.open();
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Bid"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Price"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Ask"));
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,80);

    connect(ui->Plot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(moveCrosshair(QMouseEvent*)));
    connect(ui->Plot, SIGNAL(mouseDoubleClick(QMouseEvent*)), SLOT(click_chart(QMouseEvent*)));
    connect(ui->menuAnalysis, SIGNAL(triggered(QAction*)), SLOT(get_bid_ask_ratio()));
    connect(ui->menuSettings, SIGNAL(triggered(QAction*)), SLOT(on_show_database_settings()));
    connect(&database_settings, &Popup_window::onOKpressed, this, &MainWindow::establish_db_connection);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::setPlot(){
    //check if database is acessible
    if (db.isOpen() != true){
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText("Database connection is not established");
        msgBox->exec();
        delete msgBox;
        return;
    }
    QVector<double> price_vec;
    QVector<QTime> time;
    QVector<double> time_double;
    book_entries = getBookentries("test");
    create_price_chart("test", price_vec, time);
    const double min_y = *std::min_element(price_vec.constBegin(),price_vec.constEnd());
    const double max_y = *std::max_element(price_vec.constBegin(),price_vec.constEnd());
    //converting DateTime to double
    for (int i = 0; i < time.size(); i++){
        time_double.push_back(time.at(i).msecsSinceStartOfDay());//to get seconds with subseconds
        time_double.last() = time_double.last()/1000;
    }
    const double min_x = *std::min_element(time_double.constBegin(),time_double.constEnd());
    const double max_x = *std::max_element(time_double.constBegin(),time_double.constEnd());
    ui->Plot->addGraph();

    ui->Plot->setInteraction(QCP::iRangeDrag, true);
//    ui->Plot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->Plot->setInteraction(QCP::iRangeZoom);
    ui->Plot->setInteraction(QCP::iSelectPlottables, false);
    ui->Plot->graph(0)->setData(time_double,price_vec);
    ui->Plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->Plot->xAxis->setDateTimeFormat("hh:mm:ss:zzz");
    ui->Plot->xAxis->setDateTimeSpec(Qt::UTC);
    ui->Plot->xAxis->setRange(min_x,max_x);
    ui->Plot->yAxis->setRange(min_y,max_y);
    ui->Plot->graph()->setLineStyle((QCPGraph::LineStyle)1);
    ui->Plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,5));
    ui->Plot->replot();
}

void MainWindow::updatePlot(){
    setPlot();
}

void MainWindow::moveCrosshair(QMouseEvent *event){
    y_coord = ui->Plot->yAxis->pixelToCoord(event->pos().y());
    x_coord = ui->Plot->xAxis->pixelToCoord(event->pos().x())*1000;
    x_coord_blank = ui->Plot->xAxis->pixelToCoord(event->pos().x());
    cursortime = QTime::fromMSecsSinceStartOfDay(x_coord);
    statusmsg = "Cursor Position:  "+ cursortime.toString("hh:mm:ss.zzz") + " ; " + QString::number(y_coord);
    qDebug() << statusmsg;
    ui->label->setText(statusmsg);

    if(hLine) ui->Plot->removeItem(hLine);
    if(vLine) ui->Plot->removeItem(vLine);
    vLine = new QCPItemLine(ui->Plot);
    hLine = new QCPItemLine(ui->Plot);
    ui->Plot->addItem(vLine);
    ui->Plot->addItem(hLine);
    hLine->start->setCoords(QCPRange::minRange, y_coord);
    hLine->end->setCoords(QCPRange::maxRange, y_coord);
    vLine->start->setCoords(x_coord_blank, QCPRange::minRange);
    vLine->end->setCoords(x_coord_blank, QCPRange::maxRange);
    ui->Plot->replot();
}

void MainWindow::click_chart(QMouseEvent *event){
    qDebug() << "doubleclick";
    y_coord = ui->Plot->yAxis->pixelToCoord(event->pos().y());
    x_coord = ui->Plot->xAxis->pixelToCoord(event->pos().x())*1000;
    cursortime_dclicked = QTime::fromMSecsSinceStartOfDay(x_coord);
    //lookup the bookentry at doubleklicked cursor time
    lookup_bookentry();
}

void MainWindow::lookup_bookentry(){
    Level2Snapshot *lowest_match;
    Level2Snapshot empty_dummy;
    ;
    //do nothing if no data is loaded yet
    if(book_entries.isEmpty()) return;

    for(int i = 0; i < book_entries.size(); i++){
//        qDebug() << book_entries[i].time.toString("hh:mm:ss.zzz");
        //compare times of curcor double clicked with bookentries
        if(book_entries[i].time > cursortime_dclicked && cursortime_dclicked >= book_entries[1].time){
            lowest_match = &book_entries[i-1];
            qDebug() << lowest_match->time.toString("hh:mm:ss.zzz");
            break;
        }
        else if(book_entries[i].time == cursortime_dclicked){
            lowest_match = &book_entries[i];
            qDebug() << lowest_match->time.toString("hh:mm:ss.zzz");
            break;
        }
        else if(cursortime_dclicked < book_entries[0].time){
            lowest_match = &empty_dummy;
            break;
        }
    }
    statusmsg_2 = "Book Snapshot:  "+ lowest_match->time.toString("hh:mm:ss.zzz");
    ui->label_2->setText(statusmsg_2);
//    QPair<QString,int> summy2 = qMakePair(QString("Ask"),1);
//      int summy = lowest_match->book[qMakePair(lowest_match->ask,4)].first;
      ui->tableWidget->setItem(0,2,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,4)].first)));
      ui->tableWidget->setItem(1,2,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,3)].first)));
      ui->tableWidget->setItem(2,2,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,2)].first)));
      ui->tableWidget->setItem(3,2,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,1)].first)));
      ui->tableWidget->setItem(4,2,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,0)].first)));
      ui->tableWidget->setItem(5,0,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,0)].first)));
      ui->tableWidget->setItem(6,0,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,1)].first)));
      ui->tableWidget->setItem(7,0,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,2)].first)));
      ui->tableWidget->setItem(8,0,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,3)].first)));
      ui->tableWidget->setItem(9,0,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,4)].first)));
      //now Price
      ui->tableWidget->setItem(0,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,4)].second)));
      ui->tableWidget->setItem(1,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,3)].second)));
      ui->tableWidget->setItem(2,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,2)].second)));
      ui->tableWidget->setItem(3,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,1)].second)));
      ui->tableWidget->setItem(4,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->ask,0)].second)));
      ui->tableWidget->setItem(5,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,0)].second)));
      ui->tableWidget->setItem(6,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,1)].second)));
      ui->tableWidget->setItem(7,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,2)].second)));
      ui->tableWidget->setItem(8,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,3)].second)));
      ui->tableWidget->setItem(9,1,new QTableWidgetItem(QString::number(lowest_match->book[qMakePair(lowest_match->bid,4)].second)));

}

void MainWindow::get_bid_ask_ratio(){
    //check if database is acessible
    if (book_entries.isEmpty() == true){
        QMessageBox *msgBox = new QMessageBox;
        msgBox->setText("No data loaded yet");
        msgBox->exec();
        delete msgBox;
        return;
    }
    calc_bid_ask_ratio(book_entries);
    QVector<double> time;
    QVector<double> bid_ask_ratio;
    for(int i = 0; i < book_entries.size(); i++){
        time.push_back(book_entries[i].time_double);
        bid_ask_ratio.push_back(book_entries[i].bid_ask_ratio);
    }
    ui->Plot->addGraph(ui->Plot->xAxis,ui->Plot->yAxis2);
    ui->Plot->yAxis2->setRange(*std::min_element(bid_ask_ratio.constBegin(),bid_ask_ratio.constEnd()),*std::max_element(bid_ask_ratio.constBegin(),bid_ask_ratio.constEnd()));
    ui->Plot->yAxis2->setVisible(true);
    ui->Plot->graph(1)->setData(time,bid_ask_ratio);
    ui->Plot->graph(1)->setPen(QPen(Qt::red));
    ui->Plot->replot();
}

void MainWindow::on_show_database_settings(){
    database_settings.show();
}

void MainWindow::establish_db_connection(){
    try{
        db.setHostName("localhost");
        db.setDatabaseName(database_settings.database);
        db.setUserName(database_settings.mysql_user);
        db.setPassword(database_settings.mysql_pass);
        db.open();
        writeSettings();
    }
    catch(...){
        qDebug() << "Something went wrong!";
    }
}

void MainWindow::writeSettings(){
    QSettings settings;
    settings.setValue("mysql_user",database_settings.mysql_user);
    settings.setValue("mysql_pass",database_settings.mysql_pass);
    settings.setValue("database",database_settings.database);
}

void MainWindow::readSettings(){
    QSettings settings;
    db.setDatabaseName(settings.value("database").toString());
    db.setUserName(settings.value("mysql_user").toString());
    db.setPassword(settings.value("mysql_pass").toString());
    database_settings.setLineEdit(settings.value("mysql_user").toString(),settings.value("mysql_pass").toString(),settings.value("database").toString());
}
