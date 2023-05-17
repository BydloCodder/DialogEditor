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
        persons = new Persons();
    }

    Event(QJsonObject obj);

    ~Event() {
        delete background;
        delete playSound;
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

    struct Persons {
        Persons() {}
        ~Persons() {}
        Persons(QJsonObject obj);

        struct Person {
            Person() {}
            Person(QJsonObject obj);
            ~Person() {}
            QString op = "Ignore";
            QString person, mood;
            bool appear = false, fade = false;
            QString appearAnimation;
            double appearTime = 1.0, fadeTime = 1.0;
            bool appearBackwards = false, fadeBackwards = false;

            inline bool isActive() const {
                if (op == "Ignore")
                    return false;
                if (op == "Show")
                    return !person.isEmpty() && !mood.isEmpty();
                return true;
            }

            QJsonObject toJson() const;
        };
        QHash<QString, Person*> data;

        QJsonObject toJson() const;
        bool isActive() const;
    };

    Persons *persons = 0;

    struct Choice {
        Choice() {}
        ~Choice() {}
        Choice(QJsonObject obj);

        QString text;
        QVector<Event*> events;
        QString condition;

        QJsonObject toJson() const;
    };


    bool backgroundActive = false, playSoundActive = false, personsActive = false;

    double timer = 0.0;

    QVector<Choice*> choices;
    QHash<QString, QString> state;
    QString script;

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

    QHash<QString, QHash<QString, QString> > persons;

    static Characters fromJson(QJsonObject obj, QString res);
    QJsonObject toJson(QString res);
};



#endif // DATATYPES_H
