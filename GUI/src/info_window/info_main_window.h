#include <iostream>
#include <QWidget>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>

class info_main_window : public QMainWindow
{
    Q_OBJECT

public:
    info_main_window(QWidget *parent = nullptr);

private:
    QTextEdit *information;
    QVBoxLayout *vbox;
};