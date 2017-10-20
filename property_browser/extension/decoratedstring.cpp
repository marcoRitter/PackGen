#include "decoratedstring.h"

DecoratedStringPropertyManager::DecoratedStringPropertyManager(QObject *parent)
    : QtStringPropertyManager(parent)
{
}

DecoratedStringPropertyManager::~DecoratedStringPropertyManager()
{
}

QString DecoratedStringPropertyManager::prefix(const QtProperty *property) const
{
    if (!propertyToData.contains(property))
        return QString();
    return propertyToData[property].prefix;
}

QString DecoratedStringPropertyManager::suffix(const QtProperty *property) const
{
    if (!propertyToData.contains(property))
        return QString();
    return propertyToData[property].suffix;
}

void DecoratedStringPropertyManager::setPrefix(QtProperty *property, const QString &prefix)
{
    if (!propertyToData.contains(property))
        return;

    DecoratedStringPropertyManager::Data data = propertyToData[property];
    if (data.prefix == prefix)
        return;

    data.prefix = prefix;
    propertyToData[property] = data;

    emit propertyChanged(property);
    emit prefixChanged(property, prefix);
}

void DecoratedStringPropertyManager::setSuffix(QtProperty *property, const QString &suffix)
{
    if (!propertyToData.contains(property))
        return;

    DecoratedStringPropertyManager::Data data = propertyToData[property];
    if (data.suffix == suffix)
        return;

    data.suffix = suffix;
    propertyToData[property] = data;

    emit propertyChanged(property);
    emit suffixChanged(property, suffix);
}

QString DecoratedStringPropertyManager::valueText(const QtProperty *property) const
{
    QString text = QtStringPropertyManager::valueText(property);
    if (!propertyToData.contains(property))
        return text;

    DecoratedStringPropertyManager::Data data = propertyToData[property];
    text = data.prefix + text + data.suffix;

    return text;
}

void DecoratedStringPropertyManager::initializeProperty(QtProperty *property)
{
    propertyToData[property] = DecoratedStringPropertyManager::Data();
    DecoratedStringPropertyManager::initializeProperty(property);
}

void DecoratedStringPropertyManager::uninitializeProperty(QtProperty *property)
{
    propertyToData.remove(property);
    QtStringPropertyManager::uninitializeProperty(property);
}

