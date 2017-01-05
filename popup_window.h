#ifndef POPUP_WINDOW_H
#define POPUP_WINDOW_H

#include <QWidget>
#include <analysis_functions.h>

namespace Ui {
class Popup_window;
}

class Popup_window : public QWidget
{
    Q_OBJECT

public:
    explicit Popup_window(QWidget *parent = 0);
    ~Popup_window();
    QString mysql_user;
    QString mysql_pass;
    QString database;
    void setLineEdit(QString user, QString pass, QString database);
private:
    Ui::Popup_window *ui;


private slots:
    void safe_content();

signals:
    void onOKpressed();

};

#endif // POPUP_WINDOW_H
