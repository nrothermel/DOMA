#include "popup_window.h"
#include "ui_popup_window.h"

Popup_window::Popup_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Popup_window)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted,this, &Popup_window::safe_content);
    connect(ui->buttonBox, &QDialogButtonBox::accepted,this, &Popup_window::onOKpressed);
    connect(ui->buttonBox, &QDialogButtonBox::accepted,this, &Popup_window::close);
}

Popup_window::~Popup_window()
{
    delete ui;
}

void Popup_window::safe_content(){
    mysql_user = ui->mysql_user_edit->text();
    mysql_pass = ui->password_edit->text();
    database = ui->database_name_edit->text();
    qDebug() << mysql_user;
}

void Popup_window::setLineEdit(QString user, QString pass, QString database){
    ui->mysql_user_edit->setText(user);
    ui->password_edit->setText(pass);
    ui->database_name_edit->setText(database);
}


