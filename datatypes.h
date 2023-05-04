#ifndef DATATYPES_H
#define DATATYPES_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QJsonObject>


struct Event {
    QString id, text, jump, stopSound;
    QString character = ":ignore:";

    struct Background {
        QString name, video;
        double fade = 0.0;
        int clickable = -1;

        static Background fromJson(QJsonObject obj);
        QJsonObject toJson() const;

    } background;


    struct PlaySound {
        QString name, channel, bus;
        double fade = 0.0, volume = 1.0;
        bool loop;

        static PlaySound fromJson(QJsonObject obj);
        QJsonObject toJson() const;

    } playSound;

    struct Choice {
        QString text;
        QVector<Event> events;

        static Choice fromJson(QJsonObject obj);
        QJsonObject toJson() const;
    };

    bool backgroundActive = false, playSoundActive = false;

    QVector<Choice> choices;
    QHash<QString, QString> state;

    static Event fromJson(QJsonObject obj);
    QJsonObject toJson() const;
};

struct Timeline
{
    QString uniqueName, name;
    QVector<Event> events;

    static Timeline fromJson(QJsonObject obj);
    QJsonObject toJson() const;
};

struct Characters
{
    struct Character
    {
        QString name, portrait;
    };
    QHash<QString, Character> characters;

    static Characters fromJson(QJsonObject obj, QString res);
    QJsonObject toJson(QString res);
};



#endif // DATATYPES_H
