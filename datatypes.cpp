#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

#include "datatypes.h"

Event::Background::Background(QJsonObject obj)
{
    if (obj.contains("video"))
        video = obj["video"].toString();
    if (obj.contains("name"))
        name = obj["name"].toString();
    if (obj.contains("fade"))
        fade = obj["fade"].toDouble();
    if (obj.contains("clickable"))
        clickable = obj["clickable"].toBool() ? 1 : 0;

}

QJsonObject Event::Background::toJson() const
{
    QJsonObject result;
    if (!name.isEmpty())
        result["name"] = name;
    if (!video.isEmpty())
        result["video"] = video;
    if (fade > 0.0)
        result["fade"] = fade;
    if (clickable >= 0)
        result["clickable"] = clickable == 0 ? false : true;
    return result;
}


Event::PlaySound::PlaySound(QJsonObject obj)
{
    name = obj["name"].toString();
    channel = obj["channel"].toString();
    if (obj.contains("bus"))
        bus = obj["bus"].toString();
    if (obj.contains("fade"))
        fade = obj["fade"].toDouble();
    if (obj.contains("volume"))
        volume = obj["volume"].toDouble();
    if (obj.contains("loop"))
        loop = obj["loop"].toBool();
}

QJsonObject Event::PlaySound::toJson() const
{
    QJsonObject result;
    result["name"] = name;
    result["channel"] = channel;
    if (!bus.isEmpty())
        result["bus"] = bus;
    if (fade > 0.0)
        result["fade"] = fade;
    if (volume != 1.0)
        result["volume"] = volume;
    if (loop)
        result["loop"] = loop;
    return result;
}

Event::Choice::Choice(QJsonObject obj)
{
    text = obj["text"].toString();
    QJsonArray events = obj["events"].toArray();
    foreach(auto v, events) {
        auto e = new Event(v.toObject());
        this->events.append(e);
    }
    if (obj.contains("condition")) {
        conditionActive = true;
        condition = new Event::Condition(obj["condition"].toObject());
    } else
        condition = new Event::Condition();
}

QJsonObject Event::Choice::toJson() const
{
    QJsonObject result;
    result["text"] = text;
    QJsonArray arr;
    foreach (auto e, events) {
        arr.append(e->toJson());
    }
    result["events"] = arr;

    if (conditionActive) {
        result["condition"] = condition->toJson();
    }

    return result;
}


Event::Event(QJsonObject obj)
{
    if (obj.contains("id"))
        id = obj["id"].toString();

    if (obj.contains("text"))
        text = obj["text"].toString();
    if (obj.contains("character"))
        character = obj["character"].toString();
    else
        character = ":ignore:";

    if (obj.contains("jump_to"))
        jump = obj["jump_to"].toString();
    if (obj.contains("stop_sound"))
        stopSound = obj["stop_sound"].toString();

    if (obj.contains("background")) {
        auto b = new Event::Background(obj["background"].toObject());
        background = b;
        backgroundActive = true;
    } else
        background = new Event::Background();

    if (obj.contains("play_sound")) {
        auto p = new Event::PlaySound(obj["play_sound"].toObject());
        playSound = p;
        playSoundActive = true;
    } else
        playSound = new Event::PlaySound();

    if (obj.contains("choices")) {
        QJsonArray array = obj["choices"].toArray();
        foreach (auto v, array) {
            auto c = new Event::Choice(v.toObject());
            choices.append(c);
        }
    }

    if (obj.contains("state")) {
        QJsonObject stateObject = obj["state"].toObject();
        foreach (auto k, stateObject.keys()) {
            state[k] = stateObject[k].toString();
        }
    }

    if (obj.contains("condition")) {
        auto c = new Event::Condition(obj["condition"].toObject());
        condition = c;
        conditionActive = true;
    } else
        condition = new Event::Condition();

    if (obj.contains("timer"))
        timer = obj["timer"].toDouble();
}

QJsonObject Event::toJson() const
{
    QJsonObject result;
    result["text"] = text;
    if (!id.isEmpty())
        result["id"] = id;
    if (character != ":ignore:")
        result["character"] = character;
    if (!jump.isEmpty())
        result["jump_to"] = jump;
    if (!stopSound.isEmpty())
        result["stop_sound"] = stopSound;
    if (backgroundActive)
        result["background"] = background->toJson();
    if (playSoundActive)
        result["play_sound"] = playSound->toJson();
    if (conditionActive)
        result["condition"] = condition->toJson();
    if (choices.count() > 0) {
        QJsonArray array;
        foreach (auto c, choices) {
            array.append(c->toJson());
        }
        result["choices"] = array;
    }
    if (state.count() > 0) {
        QJsonObject obj;
        foreach (auto k, state.keys()) {
            obj[k] = state[k];
        }
        result["state"] = obj;
    }
    if (timer > 0.0)
        result["timer"] = timer;
    return result;
}


Timeline Timeline::fromJson(QJsonObject obj)
{
    Timeline result;
    result.uniqueName = obj["unique_name"].toString();
    result.name = obj["display_name"].toString();

    QJsonArray eventArray = obj["events"].toArray();
    foreach (auto e, eventArray) {
        auto o = e.toObject();
        auto ev = new Event(o);
        result.events.append(ev);
    }
    return result;
}

QJsonObject Timeline::toJson() const
{
    QJsonObject result;
    result["unique_name"] = uniqueName;
    result["display_name"] = name;
    QJsonArray arr;
    foreach (auto e, events) {
        arr.append(e->toJson());
    }
    result["events"] = arr;
    return result;
}

Characters Characters::fromJson(QJsonObject obj, QString res)
{
    Characters result;
    foreach (auto k, obj["characters"].toObject().keys()) {
        QJsonObject c = obj["characters"].toObject()[k].toObject();
        Character chr;
        chr.name = c["name"].toString();
        QString s = c["portrait"].toString();
        chr.portrait = s.replace("res:/", res);
        result.characters[k] = chr;
    }
    return result;
}

QJsonObject Characters::toJson(QString res)
{
    QJsonObject result;
    QJsonObject chars;
    foreach (auto k, characters.keys()) {
        QJsonObject current;
        current["name"] = characters[k].name;
        QString s = characters[k].portrait;
        current["portrait"] = s.replace(res, "res:/");
        chars[k] = current;
    }
    result["characters"] = chars;
    return result;
}


Event::Condition::Condition(QJsonObject obj)
{
    op = obj["op"].toString();
    if (obj.contains("var"))
        var = obj["var"].toString();
    if (obj.contains("value"))
        value = obj["value"].toString();
    if (obj.contains("data")) {
        auto array = obj["data"].toArray();
        foreach (auto i, array) {
            auto c = new Event::Condition(i.toObject());
            data.append(c);
        }
    }
    if (obj.contains("cast"))
        cast = obj["cast"].toString();
}

QJsonObject Event::Condition::toJson() const
{
    QJsonObject result;
    result["op"] = op;
    if (!logical()) {
        result["var"] = var;
        result["value"] = value;
    }

    if (!cast.isEmpty())
        result["cast"] = cast;

    QJsonArray array;
    foreach (auto c, data) {
        array.append(c->toJson());
    }
    if (array.count() > 0)
        result["data"] = array;
    return result;
}

QString Event::Condition::toString() const
{
    if (!logical()) {
        if (var.isEmpty() && value.isEmpty())
            return "true";
        return var + " " + op + " " + (value.isEmpty() ? QString("\"\"") : value);
    }
    else {
        QString result;
        for (int i = 0; i < data.count(); i++) {
            result += data[i]->toString();
            if (i != data.count() - 1)
                result += " " + op + " ";
        }
        return result;
    }
}

bool Event::Condition::logical() const
{
    QString s = "nandnor";
    return s.contains(op);

}

