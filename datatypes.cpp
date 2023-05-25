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
    if (obj.contains("transition")) {
        auto t = obj["transition"].toObject();
        transition = true;

        swipe_mode_h = t["swipe_mode_h"].toBool();
        swipe_mode_v = t["swipe_mode_v"].toBool();
        swipe_speed_h = t["swipe_speed_h"].toDouble();
        swipe_speed_v = t["swipe_speed_v"].toDouble();
        swipe_min_h = t["swipe_min_h"].toDouble();
        swipe_max_h = t["swipe_max_h"].toDouble();
        swipe_min_v = t["swipe_min_v"].toDouble();
        swipe_max_v = t["swipe_max_v"].toDouble();
        swipe_shift_h = t["swipe_shift_h"].toDouble();
        swipe_shift_v = t["swipe_shift_v"].toDouble();

        scale_mode = t["scale_mode"].toBool();
        scale_speed = t["scale_speed"].toDouble();
        scale_min = t["scale_min"].toDouble();
        scale_max = t["scale_max"].toDouble();
        scale_shift = t["scale_shift"].toDouble();

        shake_mode = t["shake_mode"].toBool();
        shake_h  = t["shake_h"].toBool();
        shake_v = t["shake_v"].toBool();
        shake_speed = t["shake_speed"].toDouble();
        shake_height = t["shake_height"].toDouble();
        shake_time = t["shake_time"].toDouble();

        blend_mode = t["blend_mode"].toBool();
        blend_speed = t["blend_speed"].toDouble();

        fade_color = t["fade_color"].toString();
        fade_to = t["fade_to"].toBool();
        fade_from = t["fade_from"].toBool();
        fade_speed = t["fade_speed"].toDouble();

        slide_h = t["slide_h"].toBool();
        slide_v = t["slide_v"].toBool();
        slide_reverse = t["slide_reverse"].toBool();
        slide_speed = t["slide_speed"].toDouble();

    }
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

    if (transition) {
        QJsonObject transition;
        transition["swipe_mode_h"] = swipe_mode_h;
        transition["swipe_mode_v"] = swipe_mode_v;
        transition["swipe_speed_h"] = swipe_speed_h;
        transition["swipe_speed_v"] = swipe_speed_v;
        transition["swipe_min_h"] = swipe_min_h;
        transition["swipe_max_h"] = swipe_max_h;
        transition["swipe_min_v"] = swipe_min_v;
        transition["swipe_max_v"] = swipe_max_v;
        transition["swipe_shift_h"] = swipe_shift_h;
        transition["swipe_shift_v"] = swipe_shift_v;

        transition["scale_mode"] = scale_mode;
        transition["scale_speed"] = scale_speed;
        transition["scale_min"] = scale_min;
        transition["scale_max"] = scale_max;
        transition["scale_shift"] = scale_shift;

        transition["shake_mode"] = shake_mode;
        transition["shake_h"] = shake_h;
        transition["shake_v"] = shake_v;
        transition["shake_speed"] = shake_speed;
        transition["shake_height"] = shake_height;
        transition["shake_time"] = shake_time;

        transition["blend_mode"] = blend_mode;
        transition["blend_speed"] = blend_speed;

        transition["fade_color"] = fade_color;
        transition["fade_to"] = fade_to;
        transition["fade_from"] = fade_from;
        transition["fade_speed"] = fade_speed;

        transition["slide_h"] = slide_h;
        transition["slide_v"] = slide_v;
        transition["slide_reverse"] = slide_reverse;
        transition["slide_speed"] = slide_speed;
        result["transition"] = transition;
    }
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
        condition = obj["condition"].toString();
    }
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

    if (!condition.isEmpty()) {
        result["condition"] = condition;
    }

    return result;
}


Event::Event(QJsonObject obj)
{
    if (obj.contains("id"))
        id = obj["id"].toString();
    if (obj.contains("script"))
        script = obj["script"].toString();

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

    if (obj.contains("persons")) {
        auto p = new Event::Persons(obj["persons"].toObject());
        persons = p;
        personsActive = true;
    } else
        persons = new Event::Persons();

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
    if (persons->isActive())
        result["persons"] = persons->toJson();
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
    if (!script.isEmpty())
        result["script"] = script;
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
    foreach (auto k, obj["persons"].toObject().keys()) {
        QJsonObject person = obj["persons"].toObject()[k].toObject();
        QHash<QString, QString> data;
        foreach (auto mood, person.keys()) {
            QString s = person[mood].toString();
            data[mood] = s.replace("res:/", res);
        }
        result.persons[k] = data;
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

    QJsonObject pers;
    foreach (auto k, persons.keys()) {
        QJsonObject current;
        foreach (auto mood, persons[k].keys()) {
            QString s = persons[k][mood];
            current[mood] = s.replace(res, "res:/");
        }
        pers[k] = current;
    }
    result["persons"] = pers;
    return result;
}

Event::Persons::Persons(QJsonObject obj)
{
    foreach (auto s, QStringList() << "left" << "middle-left" << "middle" << "middle-right" << "right") {
        if (obj.contains(s)) {
            auto p = new Person(obj[s].toObject());
            data[s] = p;
        }
    }
}

QJsonObject Event::Persons::toJson() const
{
    QJsonObject result;
    foreach (auto k, data.keys()) {
        if (data[k]->isActive())
            result[k] = data[k]->toJson();
    }
    return result;
}

bool Event::Persons::isActive() const
{
    bool result = false;
    foreach (auto k, data.keys()) {
        result = data[k]->isActive();
    }
    return result;
}

Event::Persons::Person::Person(QJsonObject obj)
{
    if (obj.contains("op"))
        op = obj["op"].toString();
    if (obj.contains("person"))
        person = obj["person"].toString();
    if (obj.contains("mood"))
        mood = obj["mood"].toString();
    if (obj.contains("appear")) {
        appear = true;
        if (obj.contains("appearAnimation"))
            appearAnimation = obj["appearAnimation"].toString();
        if (obj.contains("appearTime"))
            appearTime = obj["appearTime"].toDouble();
        if (obj.contains("appearBackwards"))
            appearBackwards = obj["appearBackwards"].toBool();
    }
    if (obj.contains("fade")) {
        fade = true;
        if (obj.contains("fadeTime"))
            fadeTime = obj["fadeTime"].toDouble();
        if (obj.contains("fadeBackwards"))
            fadeBackwards = obj["fadeBackwards"].toBool();
    }
}

QJsonObject Event::Persons::Person::toJson() const
{
    QJsonObject result;
    result["op"] = op;
    result["person"] = person;
    result["mood"] = mood;
    if (appear) {
        result["appear"] = true;
        result["appearAnimation"] = appearAnimation;
        result["appearTime"] = appearTime;
        result["appearBackwards"] = appearBackwards;
    } else
        result["appear"] = false;
    if (fade) {
        result["fade"] = true;
        result["fadeTime"] = fadeTime;
        result["fadeBackwards"] = fadeBackwards;
    } else
        result["fade"] = false;
    return result;
}
