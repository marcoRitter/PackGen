#ifndef DECORATEDSTRING_H
#define DECORATEDSTRING_H

#include <QMap>
#include "qtpropertybrowser.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"


class DecoratedStringPropertyManager : public QtStringPropertyManager
{
    Q_OBJECT
public:
    DecoratedStringPropertyManager(QObject *parent = 0);
    ~DecoratedStringPropertyManager();

    QString prefix(const QtProperty *property) const;
    QString suffix(const QtProperty *property) const;
public Q_SLOTS:
    void setPrefix(QtProperty *property, const QString &prefix);
    void setSuffix(QtProperty *property, const QString &suffix);
Q_SIGNALS:
    void prefixChanged(QtProperty *property, const QString &prefix);
    void suffixChanged(QtProperty *property, const QString &suffix);
protected:
    QString valueText(const QtProperty *property) const;
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private:
    struct Data {
        QString prefix;
        QString suffix;
    };
    QMap<const QtProperty *, Data> propertyToData;
};

#endif // DECORATEDSTRING_H
