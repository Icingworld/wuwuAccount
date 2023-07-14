#ifndef CHECKGROUP_H
#define CHECKGROUP_H
#include <QCheckBox>
#include <QVector>

class Checkgroup
{
private:
    QVector <QCheckBox *> list;
    int lastIndex{-1};
public:
    Checkgroup();
    ~Checkgroup();
    void add(QCheckBox * checkbox);
    void select(const int index);
    bool isEmpty() const;
    int currentSelectIndex() const;
};

#endif // CHECKGROUP_H
