#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStringListModel>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnInitList_clicked();

    void on_btnClearList_clicked();

    void on_btnAdd_clicked();

    void on_btnInsert_clicked();

    void on_btnDelete_clicked();

    void on_btnUp_clicked();

    void on_btnDown_clicked();

    void on_btnSort_clicked(bool checked);

    void on_btnClearText_clicked();

    void on_btnShow_clicked();

    void on_chkEditable_clicked(bool checked);

    void on_listView_clicked(const QModelIndex &index);

    void updatePlainText();

private:
    void applyQSS();
    QStringList m_strList;
    QStringListModel *m_model;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
