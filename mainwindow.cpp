#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_strList<<"北京"<<"上海"<<"天津"<<"河北"<<"山东"<<"四川"<<"重庆"<<"广东"<<"河南";
    m_model=new QStringListModel(this);
    m_model->setStringList(m_strList);
    ui->listView->setModel(m_model);
    //支持shift连续选择、ctrl跳选
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //同步更新plainText
    connect(m_model, &QStringListModel::dataChanged, this, &MainWindow::updatePlainText);
    connect(m_model, &QStringListModel::rowsInserted, this, &MainWindow::updatePlainText);
    connect(m_model, &QStringListModel::rowsRemoved, this, &MainWindow::updatePlainText);
    connect(m_model, &QStringListModel::rowsMoved, this, &MainWindow::updatePlainText);
    applyQSS();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnInitList_clicked()
{
    m_model->setStringList(m_strList);
    ui->statusbar->showMessage("初始化");
}


void MainWindow::on_btnClearList_clicked()
{
    m_model->removeRows(0,m_model->rowCount());
    ui->statusbar->showMessage("清空");
}


void MainWindow::on_btnAdd_clicked()
{
    m_model->insertRow(m_model->rowCount());
    QModelIndex index=m_model->index(m_model->rowCount()-1,0);
    m_model->setData(index,"new item",Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
    ui->statusbar->showMessage("添加成功");
}


void MainWindow::on_btnInsert_clicked()
{
    QModelIndex curIndex = ui->listView->currentIndex();
    int row = curIndex.isValid() ? curIndex.row() : 0;
    m_model->insertRow(row);
    //设置新行内容
    QModelIndex newIndex = m_model->index(row, 0);
    m_model->setData(newIndex, "inserted item");
    ui->listView->setCurrentIndex(newIndex);
    ui->statusbar->showMessage("插入成功");
}



void MainWindow::on_btnDelete_clicked()
{
    int row;
    QModelIndex curIndex = ui->listView->currentIndex();

    if (curIndex.isValid())
    {
        // 如果有选中行，删除当前行
        row = curIndex.row();
    } else
    {
        // 没有选中
        ui->statusbar->showMessage("请先选中一行");
        return;
    }

    if (row < 0)
    {
        ui->statusbar->showMessage("没有可删除的行");
        return;
    }

    m_model->removeRow(row);

    // 删除后设置新选中行，优先选中同位置或前一行
    int newRow = qMin(row, m_model->rowCount() - 1);
    if (newRow >= 0)
    {
        QModelIndex newIndex = m_model->index(newRow, 0);
        ui->listView->setCurrentIndex(newIndex);
    }

    ui->statusbar->showMessage("已删除");
}



void MainWindow::on_btnUp_clicked()
{
    QModelIndex currentIndex = ui->listView->currentIndex();
    if (!currentIndex.isValid())
    {
        ui->statusbar->showMessage("请先选择一个项目");
        return;
    }
    int curRow = currentIndex.row();
    if (curRow>0)
    {
        if (!m_model->moveRow(QModelIndex(), curRow, QModelIndex(), curRow - 1))
            ui->statusbar->showMessage("移动失败");
    }
    else
    {
        ui->statusbar->showMessage("已经在最顶部");
    }
}


void MainWindow::on_btnDown_clicked()
{
    QModelIndex currentIndex = ui->listView->currentIndex();
    if (!currentIndex.isValid())
    {
        ui->statusbar->showMessage("请先选择一个项目");
        return;
    }
    int curRow = currentIndex.row();
    if (curRow < m_model->rowCount() - 1)
    {
        // 正确的移动：从curRow移动到curRow+2的位置，因为是“插入到哪一行之前”，而不是“要放到哪一行上”
        if (!m_model->moveRow(QModelIndex(), curRow, QModelIndex(), curRow + 2))
            ui->statusbar->showMessage("移动失败");
    }
    else
    {
        ui->statusbar->showMessage("已经在最底部");
    }
}


void MainWindow::on_btnSort_clicked(bool checked)
{
    if(checked)
    {
        m_model->sort(0,Qt::AscendingOrder);
        ui->statusbar->showMessage("升序排序");
    }
    else
    {
        m_model->sort(0,Qt::DescendingOrder);
        ui->statusbar->showMessage("降序排序");
    }
}


void MainWindow::on_btnClearText_clicked()
{
    ui->plainTextEdit->clear();
    ui->statusbar->showMessage("清空");
}


void MainWindow::on_btnShow_clicked()
{
    ui->plainTextEdit->clear();
    for (const auto &item : m_model->stringList())
        ui->plainTextEdit->appendPlainText(item);
}


void MainWindow::on_chkEditable_clicked(bool checked)
{
    if(checked)
    {
        ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
        ui->statusbar->showMessage("可编辑");
    }
    else
    {
        ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->statusbar->showMessage("不可编辑");
    }
}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QString str1=QString::asprintf("模型索引:row=%d, column=%d;",index.row(),index.column());
    QVariant var=m_model->data(index,Qt::DisplayRole);
    QString str2=var.toString();
    ui->statusbar->showMessage(str1+str2);
}

void MainWindow::updatePlainText()
{
    ui->plainTextEdit->clear();
    for(const QString &item:m_model->stringList())
        ui->plainTextEdit->appendPlainText(item);
}

void MainWindow::applyQSS()
{
    // ===== QSS 样式 =====
    QString style = R"(
        /* 主窗口背景 */
        QMainWindow {
            background-color: #f4f6f9;
        }

        /* 按钮样式 */
        QPushButton {
            background-color: #3498db;
            color: white;
            border-radius: 6px;
            padding: 6px 12px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #2980b9;
        }

        QPushButton:pressed {
            background-color: #1f5f8b;
        }

        /* 状态栏 */
        QStatusBar {
            background-color: #ecf0f1;
            color: #2c3e50;
            font-weight: bold;
        }

        /* 列表样式 */
        QListView {
            background: white;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            selection-background-color: #5dade2;
            selection-color: white;
        }

        /* 文本框 */
        QPlainTextEdit {
            background: #ffffff;
            border: 1px solid #cccccc;
            border-radius: 4px;
        }
    )";

    qApp->setStyleSheet(style); // 应用全局样式
}

