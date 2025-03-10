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
    Board(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        this->textEdit->setPlaceholderText(QStringLiteral("Start coding !"));
        layout->addWidget(this->textEdit);

        QMenuBar *menuBar = new QMenuBar(this);
        QMenu *helpMenu = menuBar->addMenu(QStringLiteral("Help"));
        QAction *shortcutsAction = helpMenu->addAction(QStringLiteral("Shortcuts"));
        QAction *shortcutsAbout = helpMenu->addAction(QStringLiteral("About"));
        connect(shortcutsAbout, &QAction::triggered, this, &Board::showAbout);
        connect(shortcutsAction, &QAction::triggered, this, &Board::showShortcuts);

        layout->setMenuBar(menuBar);
        setLayout(layout);

        this->setStyleSheet("background-color: #282A36; color: #F8F8F2;");
        menuBar->setStyleSheet("QMenuBar { background-color: #282A36; color: #F8F8F2; }"
                               "QMenuBar::item { background-color: #282A36; color: #F8F8F2; }"
                               "QMenuBar::item:selected { background-color: #44475a; }"
                               "QMenu { background-color: #282A36; color: #F8F8F2; }"
                               "QMenu::item { background-color: #282A36; color: #F8F8F2; }"
                               "QMenu::item:selected { background-color: #44475a; }");
    };
    int SaveFile() {
        QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("Save File"), QStringLiteral("./"), QStringLiteral("Text Files (*)"));
        if (fileName.isEmpty()) {
            QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("A file name is required"));
            return 1;
        }
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("Could not save file"));
            return 1;
        }
        QTextStream out(&file);
        out << this->textEdit->toPlainText();
        file.close();
        QMessageBox::information(this, QStringLiteral("Save"), QStringLiteral("File saved !"));
        return 0;
    };
    int OpenFile() {
        QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Open File"), QStringLiteral("./"), QStringLiteral("Any file (*)"));
        if (fileName.isEmpty()) {
            QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("Could not open file"));
            return 1;
        }
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("Could not open file"));
            return 1;
        }
        QTextStream in(&file);
        this->textEdit->setPlainText(in.readAll());
        file.close();
        QMessageBox::information(this, QStringLiteral("Open"), QStringLiteral("File opened !"));
        return 0;
    };

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier) {
            this->SaveFile();
        } else if (event->key() == Qt::Key_O && event->modifiers() == Qt::ControlModifier) {
            this->OpenFile();
        } else if (event->key() == Qt::Key_Z && event->modifiers() == Qt::ControlModifier) {
            QMessageBox::information(this, QStringLiteral("Undo"), QStringLiteral("Undo action !"));
        } else if (event->key() == Qt::Key_Y && event->modifiers() == Qt::ControlModifier) {
            QMessageBox::information(this, QStringLiteral("Redo"), QStringLiteral("Redo action !"));
        } else if (event->key() == Qt::Key_Escape) {
            exit(0);
        } else {
            QWidget::keyPressEvent(event);
        }
    };

    QTextEdit *textEdit = new QTextEdit(this);

private slots:
    void showShortcuts() {
        QMessageBox::information(this, QStringLiteral("Shortcuts"), QStringLiteral("List of shortcuts:\n\n"
            "Escape: Exit the application\n"
            "Ctrl+S: Save\n"
            "Ctrl+O: Open\n"
            "Ctrl+Z: Undo\n"
            "Ctrl+Y: Redo"));
    };
    void showAbout() {
        QMessageBox::information(this, QStringLiteral("About"), QStringLiteral("This is a simple text editor made with Qt\n Author: shookapic (github.com/shookapic)"));

    };
};

class LoadingScreen : public QWidget {
    Q_OBJECT

public:
    LoadingScreen(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *imageLabel = new QLabel(this);
        QPixmap pixmap(QStringLiteral("./assets/STE2.jpg"));
        if (pixmap.isNull()) {
            QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("Some assets are missing \n Exiting..."));
            exit(1);
        }
        imageLabel->setPixmap(pixmap);
        imageLabel->setScaledContents(true);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(imageLabel);

        setLayout(layout);

        // Set background color to black
        this->setStyleSheet("background-color: black;");
    }
};


class Application : public QStackedWidget {
    Q_OBJECT

public:
    Application(QWidget *parent = nullptr) : QStackedWidget(parent) {
        loadingScreen = new LoadingScreen(this);
        mainWindow = new Board(this);

        addWidget(loadingScreen);
        addWidget(mainWindow);

        QTimer::singleShot(3000, this, &Application::showMainWindow);
    }

private slots:
    void showMainWindow() {
        setCurrentWidget(mainWindow);
    }

private:
    LoadingScreen *loadingScreen;
    Board *mainWindow;
};
