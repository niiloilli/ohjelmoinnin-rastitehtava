#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidgetCars->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished,
            this, &MainWindow::onNetworkReply);

    getCars();

    connect(ui->btnRefresh, &QPushButton::clicked, this, &MainWindow::getCars);
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::addCar);
    connect(ui->btnUpdate, &QPushButton::clicked, this, &MainWindow::updateCar);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::deleteCar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getCars() {
    QNetworkRequest request(QUrl("http://localhost:3000/car"));
    manager->get(request);
}

void MainWindow::onNetworkReply(QNetworkReply *reply) {
    QByteArray response = reply->readAll();
    QString path = reply->url().path();
    auto op = reply->operation();

    if (path == "/car" && op == QNetworkAccessManager::GetOperation) {
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonArray arr = doc.array();

        ui->tableWidgetCars->setRowCount(arr.size());

        for (int i = 0; i < arr.size(); i++) {
            QJsonObject obj = arr[i].toObject();
            ui->tableWidgetCars->setItem(i, 0, new QTableWidgetItem(QString::number(obj["id"].toInt())));
            ui->tableWidgetCars->setItem(i, 1, new QTableWidgetItem(obj["branch"].toString()));
            ui->tableWidgetCars->setItem(i, 2, new QTableWidgetItem(obj["model"].toString()));
        }
    }

    reply->deleteLater();
}

void MainWindow::addCar() {
    QUrl url("http://localhost:3000/car");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["branch"] = ui->lineEditBranch->text();
    obj["model"] = ui->lineEditModel->text();

    manager->post(request, QJsonDocument(obj).toJson());
}

void MainWindow::updateCar() {
    int row = ui->tableWidgetCars->currentRow();
    if (row < 0) return;

    QString id = ui->tableWidgetCars->item(row, 0)->text();

    QUrl url("http://localhost:3000/car/" + id);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["branch"] = ui->lineEditBranch->text();
    obj["model"] = ui->lineEditModel->text();

    manager->put(request, QJsonDocument(obj).toJson());
}

void MainWindow::deleteCar() {
    int row = ui->tableWidgetCars->currentRow();
    if (row < 0) return;

    QString id = ui->tableWidgetCars->item(row, 0)->text();

    QUrl url("http://localhost:3000/car/" + id);
    QNetworkRequest request(url);

    manager->deleteResource(request);
}
