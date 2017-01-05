#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <mysql_functions.h>
#include <algorithm>
#include <qcustomplot.h>
#include <popup_window.h>
#include <analysis_functions.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    void setPlot();
    QSqlDatabase db;
    double y_coord;
    double x_coord;
    double x_coord_blank;
    QCPItemLine *vLine;
    QCPItemLine *hLine;
    QTime cursortime;
    QTime cursortime_dclicked;
    QString statusmsg;
    QString statusmsg_2;
    QVector<Level2Snapshot> book_entries;
    void lookup_bookentry();
    QBrush price_column_color;
    Popup_window database_settings;
    void writeSettings();
    void readSettings();

private slots:
    void updatePlot();
    void moveCrosshair(QMouseEvent*);
    void click_chart(QMouseEvent*);
    void get_bid_ask_ratio();
    void on_show_database_settings();

public slots:
    void establish_db_connection();
};

#endif // MAINWINDOW_H
