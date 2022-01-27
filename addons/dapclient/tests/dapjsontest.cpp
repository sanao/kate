#include "../schema.h"
#include <QObject>
#include <QTest>

class DAPJSONTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testJSON()
    {
        QJsonObject req = {
            {QLatin1String("seq"), 727},
            {QLatin1String("type"), QLatin1String("request")},
            {QLatin1String("command"), QLatin1String("foobar")},
        };

        const auto unmarshalled = unmarshal<Request>(QJsonDocument(req));

        QCOMPARE(unmarshalled.messageID, 727);
    };
};

QTEST_MAIN(DAPJSONTest)

#include "dapjsontest.moc"
