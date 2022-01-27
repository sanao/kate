#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <algorithm>

template<std::size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N])
    {
        std::copy_n(str, N, value);
    }

    char value[N];
};

template<StringLiteral Tag, typename T>
struct TaggedMember {
    T val;

    TaggedMember() = default;
    TaggedMember(const T &v)
        : val(v)
    {
    }
    TaggedMember(const TaggedMember &v) = default;
    TaggedMember &operator=(const TaggedMember &rhs) = default;
    TaggedMember &operator=(const T &rhs)
    {
        val = rhs;
        return *this;
    }
    operator const T &() const
    {
        return val;
    }
    bool operator==(const TaggedMember &rhs) const
    {
        return val == rhs.val;
    }
    bool operator==(const T &rhs) const
    {
        return val == rhs;
    }
    bool operator<(const TaggedMember &rhs) const
    {
        return val < rhs.val;
    }

    static constexpr auto tag = Tag.value;
    using kind = T;
};

struct UniversalType {
    template<typename T>
    operator T()
    {
    }
};

template<typename T>
consteval auto MemberCounter(auto... Members)
{
    if constexpr (requires { T{Members...}; } == false)
        return sizeof...(Members) - 1;
    else
        return MemberCounter<T>(Members..., UniversalType{});
}

template<typename T>
T unmarshalObject(const QJsonObject &obj)
{
    T t;
    constexpr auto count = MemberCounter<T>();

    if constexpr (count == 1) {
        auto &[first] = t;
        unmarshalMember(first, obj[QLatin1String(first.tag)]);
    } else if constexpr (count == 2) {
        auto &[first, second] = t;
        unmarshalMember(first, obj[QLatin1String(first.tag)]);
        unmarshalMember(second, obj[QLatin1String(second.tag)]);
    } else if constexpr (count == 3) {
        auto &[first, second, third] = t;
        unmarshalMember(first, obj[QLatin1String(first.tag)]);
        unmarshalMember(second, obj[QLatin1String(second.tag)]);
        unmarshalMember(third, obj[QLatin1String(third.tag)]);
    } else if constexpr (count == 4) {
        auto &[first, second, third, fourth] = t;
        unmarshalMember(first, obj[QLatin1String(first.tag)]);
        unmarshalMember(second, obj[QLatin1String(second.tag)]);
        unmarshalMember(third, obj[QLatin1String(third.tag)]);
        unmarshalMember(fourth, obj[QLatin1String(fourth.tag)]);
    } else {
        Q_UNREACHABLE();
    }

    return t;
}

template<typename T>
QJsonObject marshalObject(const T &t)
{
    QJsonObject ret;
    constexpr auto count = MemberCounter<T>();

    if constexpr (count == 1) {
        const auto &[first] = t;
        ret[QLatin1String(first.tag)] = marshalMember(first);
    } else if constexpr (count == 2) {
        const auto &[first, second] = t;
        ret[QLatin1String(first.tag)] = marshalMember(first);
        ret[QLatin1String(second.tag)] = marshalMember(second);
    } else if constexpr (count == 3) {
        const auto &[first, second, third] = t;
        ret[QLatin1String(first.tag)] = marshalMember(first);
        ret[QLatin1String(second.tag)] = marshalMember(second);
        ret[QLatin1String(third.tag)] = marshalMember(third);
    } else if constexpr (count == 4) {
        const auto &[first, second, third, fourth] = t;
        ret[QLatin1String(first.tag)] = marshalMember(first);
        ret[QLatin1String(second.tag)] = marshalMember(second);
        ret[QLatin1String(third.tag)] = marshalMember(third);
        ret[QLatin1String(fourth.tag)] = marshalMember(fourth);
    } else {
        Q_UNREACHABLE();
    }

    return ret;
}

template<typename T>
QJsonValue marshalMember(const T &t)
{
    using tagKind = typename T::kind;
    const auto fieldName = T::tag;

    constexpr bool isString = std::is_convertible<tagKind, QString>::value;
    constexpr bool isBool = std::is_convertible<tagKind, bool>::value;
    constexpr bool isDouble = std::is_convertible<tagKind, double>::value;
    constexpr bool isInt = std::is_convertible<tagKind, int>::value;

    if constexpr (isString || isBool || isDouble || isInt) {
        return QJsonValue(t);
    } else {
        constexpr bool isArray = requires(tagKind x)
        {
            x.detach();
            x.detachShared();
            x.isEmpty();
            x.clear();
            x.takeFirst();
        };

        if constexpr (isArray)
        {
            QJsonArray array;

            for (const auto &item : t) {
                array << marshalMember(t);
            }

            return array;
        }
        else
        {
            return marshalObject(t);
        }
    }
}

template<typename T>
void unmarshalMember(T &member, const QJsonValue &val)
{
    using tagKind = typename T::kind;

    constexpr bool isString = std::is_convertible<tagKind, QString>::value;
    constexpr bool isBool = std::is_convertible<tagKind, bool>::value;
    constexpr bool isDouble = std::is_convertible<tagKind, double>::value;
    constexpr bool isInt = std::is_convertible<tagKind, int>::value;

    if constexpr (isString) {
        if (!val.isString()) {
            qWarning() << "Expected string, got" << val.type();
        }
        member = val.toString();
    } else if constexpr (isDouble) {
        if (!val.isDouble()) {
            qWarning() << "Expected number, got" << val.type();
        }
        member = val.toDouble();
    } else if constexpr (isInt) {
        if (!val.isDouble()) {
            qWarning() << "Expected number, got" << val.type();
        }
        member = val.toInt();
    } else if constexpr (isBool) {
        if (!val.isBool()) {
            qWarning() << "Expected bool, got" << val.type();
        }
        member = val.toBool();
    } else {
        constexpr bool isArray = requires(tagKind x)
        {
            x.detach();
            x.detachShared();
            x.isEmpty();
            x.clear();
            x.takeFirst();
        };

        if constexpr (isArray)
        {
            if (!val.isArray()) {
                qWarning() << "Expected number, got array" << val.type();
            }

            auto array = val.toArray();
            tagKind t;
            using arrayType = decltype(t.takeFirst());

            for (auto item : array) {
                constexpr bool isMember = requires
                {
                    arrayType::kind;
                    arrayType::tag;
                };

                if constexpr (isMember)
                {
                    arrayType yoot;
                    unmarshalMember<arrayType>(yoot, item);
                    t << yoot;
                }
                else
                {
                    t << unmarshalObject<arrayType>(item.toObject());
                }
            }

            member = t;
        }
        else
        {
            member = unmarshalObject<tagKind>(val.toObject());
        }
    }
}

template<typename T>
auto unmarshal(const QJsonDocument &doc) -> T
{
    T t;

    if (doc.isObject()) {
        return unmarshalObject<T>(doc.object());
    }

    return t;
}
