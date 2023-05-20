#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QListWidgetItem>
#include <QVector>
#include <QScrollBar>
#include "eventlistitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadConfig();


    backgroundsWidget = new DictionaryView();
    backgroundsWidget->dict = &backgrounds;
    connect(backgroundsWidget, &DictionaryView::editingFinished, this, [this]() {
        if (!this->ui->backgrounds->text().isEmpty())
            this->saveDictionary(this->ui->backgrounds->text(), backgrounds, ui->res_base->text());
    });

    soundsWidget = new DictionaryView();
    soundsWidget->dict = &sounds;

    connect(soundsWidget, &DictionaryView::editingFinished, this,  [this]() {
        if (!this->ui->sounds->text().isEmpty())
            this->saveDictionary(this->ui->sounds->text(), sounds, ui->res_base->text());
    });

    videosWidget = new DictionaryView();
    videosWidget->dict = &videos;
    connect(soundsWidget, &DictionaryView::editingFinished, this,  [this]() {
        if (!this->ui->videos->text().isEmpty())
            this->saveDictionary(this->ui->videos->text(), videos, ui->res_base->text());
    });

    charactersWidget = new CharactersView();
    charactersWidget->dict = &characters;
    connect(charactersWidget, &CharactersView::editingFinished, this, [this]() {
        if (!this->ui->characters->text().isEmpty()) {
            auto obj = characters.toJson(ui->res_base->text());
            QJsonDocument doc;
            doc.setObject(obj);
            QFile f(this->ui->characters->text());
            if (f.open(QFile::WriteOnly)) {
                f.write(doc.toJson());
                f.flush();
                f.close();
            }
        }
    });

    auto model = ui->timeline_events->model();
    connect(model, &QAbstractItemModel::rowsMoved, this, [this](QModelIndex, int start, int, QModelIndex, int end) {
        if (start != end) {
            if (std::abs(start-end) > 1) {
                auto scroll = ui->timeline_events->verticalScrollBar()->value();
                auto item = timeline.events[start];
                timeline.events.removeAt(start);
                timeline.events.insert(end > 1 ? end - 1 : end, item);
                represent();
                ui->timeline_events->verticalScrollBar()->setValue(scroll);
            } else {
                auto scroll = ui->timeline_events->verticalScrollBar()->value();
                timeline.events.swapItemsAt(start, end);
                represent();
                ui->timeline_events->verticalScrollBar()->setValue(scroll);
            }
        }
    });
}

void MainWindow::loadConfig()
{
    auto obj = readJson("config.json");
    ui->res_base->setText(obj["res_base"].toString());
    ui->timeline->setText(obj["timeline"].toString());

    ui->backgrounds->setText(obj["backgrounds"].toString());
    if (!ui->backgrounds->text().isEmpty())
        on_backgrounds_load_clicked();
    ui->sounds->setText(obj["sounds"].toString());
    if (!ui->sounds->text().isEmpty())
        on_sounds_load_clicked();
    ui->characters->setText(obj["characters"].toString());
    if (!ui->characters->text().isEmpty())
        on_characters_load_clicked();
    ui->videos->setText(obj["videos"].toString());
    if (!ui->videos->text().isEmpty())
        on_videos_load_clicked();

    if (!ui->timeline->text().isEmpty())
        on_timeline_load_clicked();

    firstLoad = false;
}

void MainWindow::saveConfig()
{
    QJsonObject obj;
    obj["res_base"] = ui->res_base->text();
    obj["timeline"] = ui->timeline->text();
    obj["backgrounds"] = ui->backgrounds->text();
    obj["sounds"] = ui->sounds->text();
    obj["videos"] = ui->videos->text();
    obj["characters"] = ui->characters->text();
    QJsonDocument doc;
    doc.setObject(obj);
    auto data = doc.toJson();

    QFile f("config.json");
    if (f.open(QFile::WriteOnly)) {
        f.write(data);
        f.flush();
        f.close();
    }
}

QJsonObject MainWindow::readJson(QString filename)
{
    QJsonObject result;
    QFile f(filename);
    if (f.open(QFile::ReadOnly)) {
        auto data = f.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        result = doc.object();
        f.close();
    }
    return result;
}

QHash<QString, QString> MainWindow::loadDictionary(QString filename, QString res)
{
    QJsonObject data = readJson(filename);
    QHash<QString, QString> result;
    foreach (auto k, data.keys()) {
        QString s = data[k].toString();
        result[k] = s.replace("res:/", res);
    }
    return result;
}

void MainWindow::saveDictionary(QString filename, QHash<QString, QString> &dict, QString res)
{
    QJsonObject result;
    foreach (auto k, dict.keys()) {
        QString s = dict[k];
        result[k] = s.replace(res, "res:/");
    }
    QJsonDocument doc;
    doc.setObject(result);
    QFile f(filename);
    if (f.open(QFile::WriteOnly)) {
        f.write(doc.toJson());
        f.flush();
        f.close();
    }
}

void MainWindow::getIndexes(QVector<Event*> events)
{
    foreach (auto ev, events) {
        if (!ev->id.isEmpty())
            idList.append(ev->id);
        if (!ev->choices.empty()) {
            foreach (auto choice, ev->choices)
                getIndexes(choice->events);
        }
    }
}

void MainWindow::represent()
{
    ui->timeline_events->clear();
    getIndexes(timeline.events);

    for (int i = 0; i < timeline.events.count(); i++) {
        auto evPointer = timeline.events[i];
        auto itemWidget = new EventListItem();
        itemWidget->setBase(&backgrounds, &sounds, &videos, &characters, &idList);
        itemWidget->setEvent(evPointer);
        itemWidget->represent();

        QListWidgetItem * item = new QListWidgetItem();
        ui->timeline_events->addItem(item);
        ui->timeline_events->setItemWidget(item, itemWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_res_button_clicked()
{
    QString dir = QFileDialog::getExistingDirectory();
    if (!dir.isEmpty()) {
        ui->res_base->setText(dir);
        saveConfig();
    }
}

void MainWindow::on_timeline_load_clicked()
{
    QString filename = firstLoad ? ui->timeline->text() : QFileDialog::getOpenFileName(0, "", "", "*.json");
    if (!filename.isEmpty()) {
        ui->timeline->setText(filename);
        auto data = readJson(filename);
        timeline = Timeline::fromJson(data);
        saveConfig();
        ui->groupbox->setEnabled(true);
        ui->timeline_id->setText(timeline.uniqueName);
        ui->timeline_name->setText(timeline.name);

        represent();
    }
}

void MainWindow::on_backgrounds_load_clicked()
{
    QString filename = firstLoad ? ui->backgrounds->text() :  QFileDialog::getOpenFileName(0, "", "", "*.json");
    if (!filename.isEmpty()) {
        ui->backgrounds->setText(filename);
        backgrounds = loadDictionary(filename, ui->res_base->text());
        saveConfig();
    }
    if (backgroundsWidget)
        backgroundsWidget->mapDict();
}



void MainWindow::on_sounds_load_clicked()
{
    QString filename = firstLoad ? ui->sounds->text() : QFileDialog::getOpenFileName(0, "", "", "*.json");
    if (!filename.isEmpty()) {
        ui->sounds->setText(filename);
        sounds = loadDictionary(filename, ui->res_base->text());
        saveConfig();
    }
    if (soundsWidget)
        soundsWidget->mapDict();
}


void MainWindow::on_videos_load_clicked()
{
    QString filename = firstLoad ? ui->videos->text() : QFileDialog::getOpenFileName(0, "", "", "*.json");
    if (!filename.isEmpty()) {
        ui->videos->setText(filename);
        videos = loadDictionary(filename, ui->res_base->text());
        saveConfig();
    }
    if (videosWidget)
        videosWidget->mapDict();
}


void MainWindow::on_characters_load_clicked()
{
    QString filename = firstLoad ? ui->characters->text() : QFileDialog::getOpenFileName(0, "", "", "*.json");
    if (!filename.isEmpty()) {
        ui->characters->setText(filename);
        characters = Characters::fromJson(readJson(filename), ui->res_base->text());
        saveConfig();
    }
    if (charactersWidget)
        charactersWidget->mapDict();
}

void MainWindow::on_backgrounds_edit_clicked()
{
    backgroundsWidget->mapDict();
    backgroundsWidget->setWindowTitle("Backgrounds");
    backgroundsWidget->exec();
}


void MainWindow::on_sounds_edit_clicked()
{
    soundsWidget->mapDict();
    soundsWidget->setWindowTitle("Sounds");
    soundsWidget->exec();
}


void MainWindow::on_videos_edit_clicked()
{
    videosWidget->mapDict();
    videosWidget->setWindowTitle("Videos");
    videosWidget->exec();
}


void MainWindow::on_characters_edit_clicked()
{
    auto dialog = new CharactersView();
    dialog->setWindowTitle("Characters");
    dialog->dict = &characters;
    dialog->mapDict();
    dialog->exec();

    if (!this->ui->characters->text().isEmpty()) {
        auto obj = characters.toJson(ui->res_base->text());
        QJsonDocument doc;
        doc.setObject(obj);
        QFile f(this->ui->characters->text());
        if (f.open(QFile::WriteOnly)) {
            f.write(doc.toJson());
            f.flush();
            f.close();
        }
    }
    dialog->deleteLater();
}

void MainWindow::on_groupBox_toggled(bool arg1)
{
    auto children = ui->groupBox->children();
    foreach (auto c, children) {
        if (c->metaObject()->className() != QString("QGridLayout")) {
            auto widget = qobject_cast<QWidget*>(c);
            if (widget)
                widget->setVisible(arg1);
        }
    }
    ui->groupBox->setMinimumHeight(100 * arg1);
    ui->groupBox->setMaximumHeight(arg1 ? 16777215 : 20);
    qDebug() << arg1;
}


void MainWindow::on_timeline_id_editingFinished()
{
    timeline.uniqueName = ui->timeline_id->text();
}


void MainWindow::on_timeline_name_editingFinished()
{
    timeline.name = ui->timeline_name->text();
}


void MainWindow::on_pushButton_16_clicked()
{
    auto ev = new Event();
    auto r = ui->timeline_events->currentRow();
    if (r > -1)
        timeline.events.insert(r, ev);
    else
        timeline.events.append(ev);

    auto itemWidget = new EventListItem();
    itemWidget->setBase(&backgrounds, &sounds, &videos, &characters, &idList);
    itemWidget->setEvent(ev);
    itemWidget->represent();

    QListWidgetItem * item = new QListWidgetItem();
    if (r > -1)
        ui->timeline_events->insertItem(r, item);
    else
        ui->timeline_events->addItem(item);
    ui->timeline_events->setItemWidget(item, itemWidget);
    ui->timeline_events->scrollToItem(item);
}


void MainWindow::on_pushButton_19_clicked()
{
    int row = ui->timeline_events->currentRow();
    if (row >= 0) {
        timeline.events.removeAt(row);
        delete ui->timeline_events->takeItem(row);
    }
}


void MainWindow::on_pushButton_20_clicked()
{
    QJsonObject obj = timeline.toJson();
    QJsonDocument doc;
    doc.setObject(obj);
    QFile f(ui->timeline->text());
    if (f.open(QFile::WriteOnly)) {
        f.write(doc.toJson());
        f.flush();
        f.close();
    }
}


void MainWindow::on_timeline_id_textChanged(const QString &arg1)
{
     timeline.uniqueName = arg1;
}


void MainWindow::on_timeline_id_returnPressed()
{

}


void MainWindow::on_timeline_name_textChanged(const QString &arg1)
{
    timeline.name = arg1;
}

