#pragma once
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QPixmap>
#include <QStackedWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMenu>
#include <iostream>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>

class Board : public QWidget {
    Q_OBJECT

public:
    ~Board();
    Board(QWidget *parent = nullptr);
    int SaveFile();
    int OpenFile();

protected:
    void keyPressEvent(QKeyEvent *event) override;

    QTextEdit *user_input = new QTextEdit(this);
    QMenuBar *menuBar = new QMenuBar(this);

private slots:
    void showShortcuts();
    void showAbout();
};
