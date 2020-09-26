#ifndef CTOPCONTROL_H
#define CTOPCONTROL_H

#include <QObject>

class CTopControl : public QObject
{
    Q_OBJECT
public:
    explicit CTopControl(QObject *parent = nullptr);

    Q_PROPERTY(QStringList arrWindows READ arrWindowsName)
public:


signals:
    void arrWindowsChanged();


public slots:
    QStringList arrWindowsName();
    QStringList RefreshProgressList();
    QStringList refreshHWNDList();
    void setWindowOnTop(int index, bool bOnTop);
    void setWindowVisible(bool bVisible);

    void printStringList(QStringList arrTexts);

public:
    //QStringList _arrWindows;
    QList<QPair<QString, int64_t>> _arrWindows;
private:

};



#endif // CTOPCONTROL_H
