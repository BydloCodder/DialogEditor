#ifndef DATATYPES_H
#define DATATYPES_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QJsonObject>


struct Event {
    QString id, text, jump, stopSound;
    QString character = ":ignore:";

    Event() {
        background = new Background();
        playSound = new PlaySound();
        condition = new Condition();
    }

    Event(QJsonObject obj);

    ~Event() {
        delete background;
        delete playSound;
        delete condition;
    }

    struct Background {
        Background() {};
        Background(QJsonObject obj);
        ~Background() {}

        QString name, video;
        double fade = 0.0;
        int clickable = -1;

        QJsonObject toJson() const;

    };
    Background *background = 0;


    struct PlaySound {
        PlaySound() {}
        ~PlaySound() {}
        PlaySound(QJsonObject obj);
        QString name, channel, bus;
        double fade = 0.0, volume = 1.0;
        bool loop = false;

        QJsonObject toJson() const;
    };
    PlaySound *playSound = 0;

    struct Condition {
        Condition() {}
        ~Condition() {}
        Condition(QJsonObject obj);

        QString op = "==", var, value, cast;
        QVector<Condition*> data;

        QJsonObject toJson() const;
        QString toString() const;
        bool logical() const;
    };
    Condition *condition = 0;

    struct Choice {
        Choice() {condition = new Event::Condition(); }
        ~Choice() {delete condition;}
        Choice(QJsonObject obj);

        QString text;
        QVector<Event*> events;
        bool conditionActive = false;
        Condition *condition = 0;

        QJsonObject toJson() const;
    };

    bool backgroundActive = false, playSoundActive = false;
    bool conditionActive = false;

    double timer = 0.0;

    QVector<Choice*> choices;
    QHash<QString, QString> state;

    QJsonObject toJson() const;
};

struct Timeline
{
    QString uniqueName, name;
    QVector<Event*> events;

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
