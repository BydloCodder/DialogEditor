#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

#include "datatypes.h"

Event::Background Event::Background::fromJson(QJsonObject obj)
{
    Event::Background result;
    if (obj.contains("video"))
        result.video = obj["video"].toString();
    if (obj.contains("name"))
        result.name = obj["name"].toString();
    if (obj.contains("fade"))
        result.fade = obj["fade"].toDouble();
    if (obj.contains("clickable"))
        result.clickable = obj["clickable"].toBool() ? 1 : 0;
    return result;
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


Event::PlaySound Event::PlaySound::fromJson(QJsonObject obj)
{
    Event::PlaySound result;
    result.name = obj["name"].toString();
    result.channel = obj["channel"].toString();
    if (obj.contains("bus"))
        result.bus = obj["bus"].toString();
    if (obj.contains("fade"))
        result.fade = obj["fade"].toDouble();
    if (obj.contains("volume"))
        result.volume = obj["volume"].toDouble();
    if (obj.contains("loop"))
        result.loop = obj["loop"].toBool();
    return result;
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



Event::Choice Event::Choice::fromJson(QJsonObject obj)
{
    Event::Choice result;
    result.text = obj["text"].toString();
    QJsonArray events = obj["events"].toArray();
    foreach(auto v, events) {
        result.events.append(Event::fromJson(v.toObject()));
    }
    return result;
}

QJsonObject Event::Choice::toJson() const
{
    QJsonObject result;
    result["text"] = text;
    QJsonArray arr;
    foreach (auto e, events) {
        arr.append(e.toJson());
    }
    result["events"] = arr;
    return result;
}


Event Event::fromJson(QJsonObject obj)
{
    Event result;
    if (obj.contains("id"))
        result.id = obj["id"].toString();

    if (obj.contains("text"))
        result.text = obj["text"].toString();
    if (obj.contains("character"))
        result.character = obj["character"].toString();
    else
        result.character = ":ignore:";

    if (obj.contains("jump_to"))
        result.jump = obj["jump_to"].toString();
    if (obj.contains("stop_sound"))
        result.stopSound = obj["stop_sound"].toString();

    if (obj.contains("background")) {
        result.background = Event::Background::fromJson(obj["background"].toObject());
        result.backgroundActive = true;
    }
    if (obj.contains("play_sound")) {
        result.playSound = Event::PlaySound::fromJson(obj["play_sound"].toObject());
        result.playSoundActive = true;
    }

    if (obj.contains("choices")) {
        QJsonArray array = obj["choices"].toArray();
        foreach (auto v, array) {
            result.choices.append(Event::Choice::fromJson(v.toObject()));
        }
    }

    if (obj.contains("state")) {
        QJsonObject stateObject = obj["state"].toObject();
        foreach (auto k, stateObject.keys()) {
            result.state[k] = stateObject[k].toString();
        }
    }

    if (obj.contains("condition")) {
        result.condition = Event::Condition::fromJson(obj["condition"].toObject());
        result.conditionActive = true;
    }

    if (obj.contains("timer"))
        result.timer = obj["timer"].toDouble();
    return result;
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
        result["background"] = background.toJson();
    if (playSoundActive)
        result["play_sound"] = playSound.toJson();
    if (conditionActive)
        result["condition"] = condition.toJson();
    if (choices.count() > 0) {
        QJsonArray array;
        foreach (auto c, choices) {
            array.append(c.toJson());
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
        result.events.append(Event::fromJson(e.toObject()));
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
        arr.append(e.toJson());
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


Event::Condition Event::Condition::fromJson(QJsonObject obj)
{
    Event::Condition result;
    result.op = obj["op"].toString();
    if (obj.contains("var"))
        result.var = obj["var"].toString();
    if (obj.contains("value"))
        result.value = obj["value"].toString();
    if (obj.contains("data")) {
        auto array = obj["data"].toArray();
        foreach (auto i, array) {
            result.data.append(Event::Condition::fromJson(i.toObject()));
        }
    }
    if (obj.contains("cast"))
        result.cast = obj["cast"].toString();
    return result;
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
        array.append(c.toJson());
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
            result += data[i].toString();
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














